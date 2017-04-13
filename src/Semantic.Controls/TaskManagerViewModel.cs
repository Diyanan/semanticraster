using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class TaskManagerViewModel : ViewModelBase
    {
        private SemanticProject _Project;
        private TaskViewModel _SelectedItem;
        private ObservableCollectionEx<TaskViewModel> _TaskList;

        private TaskSettingsViewModel settings;

        private CompletionStats _stats;

        public static string PropertyProject
        {
            get
            {
                return "Project";
            }
        }

        public SemanticProject Project
        {
            get
            {
                return this._Project;
            }
            set
            {
                this.SetProperty<SemanticProject>(PropertyProject, ref this._Project, value, false);

                this.TaskList.CollectionChanged -= new NotifyCollectionChangedEventHandler(this.TaskList_CollectionChanged);
                this.TaskList.Clear();
                if (this._Project != null && this._Project.Tasks.Count > 0)
                {
                    for (int i = 0; i < this._Project.Tasks.Count; ++i)
                    {
                        this.AddTaskViewModel(this._Project.Tasks[i], false);
                    }
                    this.SetSelectedItemValue(0, false);
                }
                this.TaskList.CollectionChanged += new NotifyCollectionChangedEventHandler(this.TaskList_CollectionChanged);
            }
        }


        public static string PropertySelectedItem
        {
            get
            {
                return "SelectedItem";
            }
        }

        public TaskViewModel SelectedItem
        {
            get
            {
                return this._SelectedItem;
            }
            set
            {
                //if (this._SelectedItem != null)
                //{
                //    this.UpdateTaskViewModel(this._SelectedItem);
                //}
                this.SetSelectedItemValue(value, false);
            }
        }

        public static string PropertyTaskList
        {
            get
            {
                return "TaskList";
            }
        }

        public ObservableCollectionEx<TaskViewModel> TaskList
        {
            get
            {
                return this._TaskList;
            }
            private set
            {
                this.SetProperty<ObservableCollectionEx<TaskViewModel>>(PropertyTaskList, ref this._TaskList, value, false);
            }
        }

        public LayerManagerViewModel LayerManagerVM { get; set; }
        public HostControlViewModel hostControlViewModel { get; private set; }

        public TaskManagerViewModel(HostControlViewModel hostControlVW, TaskSettingsViewModel settings)
        {
            this._stats = new CompletionStats();
            this.hostControlViewModel = hostControlVW;
            this.settings = settings;
            this.TaskList = new ObservableCollectionEx<TaskViewModel>();
        }

        private void TaskList_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            this.SyncProjectTasks();
        }


        public void OnCopyTask(TaskViewModel task)
        {
            this.AddCapturedScene(task, task.Model.TaskType);
        }

        public void OnCreateNewTask(bool isDuplicate, TaskType taskType, AlgorithmType algType)
        {
            this.AddCapturedScene(null, taskType);
        }

        private void AddCapturedScene(TaskViewModel toDupleTask, TaskType taskType)
        {
            // 创建指定类型任务
            SemanticTask task = SemanticTask.Create(taskType);
            switch (taskType)
            {
                case TaskType.Segment:
                    task.Name = "分割";
                    break;
                case TaskType.Feature:
                    task.Name = "特征计算";
                    break;
                case TaskType.Classify:
                    task.Name = "分类";
                    break;
                case TaskType.ChangeDetection:
                    task.Name = "变化检测";
                    break;
                case TaskType.GChangeDetection:
                    task.Name = "国情检测";
                    break;
                default:
                    break;
            }
            if (toDupleTask != null)
            {
                // 参数配置
            }
            TaskViewModel taskVM = this.AddTaskViewModel(task, true);
            if (this.SelectedItem != null)
            {
                // update param
                // this.UpdateTaskViewModel(this.SelectedItem);
            }
            this.SetSelectedItemValue(taskVM, false);
            // save state
            // this.hostControlViewModel.SaveState();
        }

        private void OnTaskExecute(TaskViewModel taskVM)
        {
            this.hostControlViewModel.StatusBar.TaskProcessStarted(this._stats);
            // var task = taskVM.Model as FeatureTask;
            var samples = hostControlViewModel.GetSamples();
            var cal = hostControlViewModel.Factory.Create(taskVM.Model, samples);
            var ct = taskVM.Model as ClassifyTask;
            if (ct != null)
            {
                var layerDefn = ct.Param.VectorLayer;
                var fields = ct.Param.SelFields;
                hostControlViewModel.Map.SaveTrainFile(layerDefn, fields, samples);
            }

            // 报告进度
            cal.OnProgressChanged += ReportProgress;
            Task.Factory.StartNew(() => cal.Process()).ContinueWith(t =>
            {
                //加载分割后的shp
                if (taskVM.Model.TaskType == TaskType.Segment)
                {
                    var task = taskVM.Model as SegTask;
                    string imgPath = task.Param.RasterFileName;
                    string SegShpPath = Path.GetDirectoryName(imgPath) + "\\" + Path.GetFileNameWithoutExtension(imgPath) + "_Region.shp";
                    
                    //hostControlViewModel.Map.AddLayer(SegShpLayer);
                    foreach (var layerdefinition in this.hostControlViewModel.LayerManagerViewModel.Model.LayerDefinitions)
                    {
                        if (layerdefinition.DataSource == SegShpPath)
                        {
                            this.hostControlViewModel.LayerManagerViewModel.Model.RemoveLayerDefinition(layerdefinition);
                            break;
                        }
                    }
                    this.hostControlViewModel.Model.UIDispatcher.Invoke(() =>
                    {
                        this.hostControlViewModel.LayerManagerViewModel.Model.AddLayerDefinition(SegShpPath);
                        foreach (var layerdefinition in this.hostControlViewModel.LayerManagerViewModel.Model.LayerDefinitions)
                        {
                            if (layerdefinition.DataSource == SegShpPath)
                            {
                                this.LayerManagerVM.LayerManager.AddLayer(layerdefinition);
                                break;
                            }
                        }
                        
                    });
                }

                if (taskVM.Model.TaskType == TaskType.Feature)
                {
                    var task = taskVM.Model as FeatureTask;
                    this.hostControlViewModel.LayerManagerViewModel.RelateTable(task.Param.VectorLayer, task.Param.CsvFile);
                }

                if (taskVM.Model.TaskType == TaskType.Classify)
                {
                    var task = taskVM.Model as ClassifyTask;
                    var result = ClassifyCalculator.GetClassResult(samples);
                    this.hostControlViewModel.Map.UpdateClass(task.Param.VectorLayer, result);
                }
            });
        }

        private void ReportProgress(object sender, EventArgs e)
        {
            var args = e as ProgressEventArgs;
            this._stats.Pending = false;
            this._stats.Requested = args.Requested;
            this._stats.Completed = args.Completed;
        }

        private void OnDeleteTask(TaskViewModel taskVM)
        {
            if (this.TaskList.Count <= 1)
                return;
            this.TaskList.Remove(taskVM);
        }

        private void OnTaskSettings(TaskViewModel taskVM)
        {
            this.SelectedItem = taskVM;
            this.hostControlViewModel.TaskPanel.OpenSettings(TaskPanelSettingsSubhead.TaskSettings);
        }

        public TaskViewModel AddTaskViewModel(SemanticTask task, bool insertBelowCurrunt = false)
        {
            TaskViewModel taskVM = new TaskViewModel(task, this.TaskList.Count);
            taskVM.ExecuteCommand = new DelegatedCommand<TaskViewModel>(this.OnTaskExecute);
            taskVM.SettingsCommand = new DelegatedCommand<TaskViewModel>(this.OnTaskSettings);
            taskVM.DeleteCommand = new DelegatedCommand<TaskViewModel>(this.OnDeleteTask);

            if (insertBelowCurrunt && this.SelectedItem != null)
            {
                int num = this.TaskList.IndexOf(this.SelectedItem);
                try
                {
                    this.TaskList.Insert(num + 1, taskVM);
                }
                catch
                {
                    this.TaskList.Add(taskVM);
                }
            }
            else
            {
                this.TaskList.Add(taskVM);
            }
            return taskVM;
        }

        public void SetSelectedItemValue(int taskIndex, bool openSettings)
        {
            if (taskIndex < 0 || taskIndex >= this.TaskList.Count)
                return;
            this.SetSelectedItemValue(this.TaskList[taskIndex], openSettings);
        }


        public void SetSelectedItemValue(TaskViewModel taskVW, bool openSettings)
        {
            if (this._SelectedItem == null && taskVW == null ||
                this._SelectedItem != null && this._SelectedItem.Equals(taskVW))
                return;
            if (taskVW != null && taskVW.Model != null)
            {
                this.SetProperty<TaskViewModel>(PropertySelectedItem, ref this._SelectedItem, taskVW, false);
                this.settings.SetParentTask(taskVW);
                if (openSettings)
                    this.hostControlViewModel.TaskPanel.OpenSettings();
            }
            else
            {
                this.SetProperty<TaskViewModel>(PropertySelectedItem, ref this._SelectedItem, taskVW, false);
            }
        }


        private void SyncProjectTasks()
        {
            if (this.Project == null)
                return;
            this.Project.Tasks.Clear();
            for (int i = 0; i < this.TaskList.Count; i++)
            {
                this.TaskList[i].SetIndex(i);
                this.Project.Tasks.Add(this.TaskList[i].Model);
            }
            if (this.SelectedItem == null)
                return;
            // this.settings.ParentScene = this.SelectedItem;
        }


        private void UpdateTaskViewModel(TaskViewModel taskVM)
        {
            if (taskVM != null)
            {
                SemanticTask task = SemanticTask.Create(taskVM.Model.TaskType);
                if (this.SelectedItem != null)
                {
                    task.Name = this.SelectedItem.Model.Name;
                }
                taskVM.Model = task;
            }
            this.SyncProjectTasks();
            // 保存工程
            // this.hostControlViewModel.SaveState();
        }

    }
}
