using Microsoft.Win32;
using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class HostControlViewModel : WindowViewModel
    {
        private ICommand _ShapefileBrowserCommand;
        private bool _TaskEditorVisible = true;
        private bool _IsScratchingSample = false;
        private bool _CanSelectFeature = false;
        private bool _OpenCheckView = false;

        private ICommand _CreateFeatureTaskCommand;
        private ICommand _CreateClassifyTaskCommand;
        private ICommand _CreateSegmentTaskCommand;
        private ICommand _CreateChangeTaskCommand;

        private ICommand _CustomClassifyCommand;

        private ICommand _LoadSamplesCommand;
        private ICommand _ExportSamplesCommand;
        private ICommand _OpenConfigurePanel;

        //public ICommand OpenSubMapCommand
        //{
        //    get
        //    {
        //        return this._OpenSubMapCommand;
        //    }
        //    set
        //    {
        //        this.SetProperty<ICommand>("OpenSubMapCommand",ref this._OpenSubMapCommand,value,false);
        //    }
        //}

        public ICommand LoadSamplesCommand
        {
            get
            {
                return this._LoadSamplesCommand;
            }
            set
            {
                this.SetProperty<ICommand>("LoadSamplesCommand", ref this._LoadSamplesCommand, value, false);
            }
        }

        public ICommand ExportSamplesCommand
        {
            get
            {
                return this._ExportSamplesCommand;
            }
            set
            {
                this.SetProperty<ICommand>("ExportSamplesCommand", ref this._ExportSamplesCommand, value, false);
            }
        }

        public ICommand ShapefileBrowserCommand
        {
            get
            {
                return this._ShapefileBrowserCommand;
            }
            set
            {
                this.SetProperty<ICommand>("ShapefileBrowserCommand", ref this._ShapefileBrowserCommand, value, false);
            }
        }

        public ICommand OpenConfigurePanel
        {
            get
            {
                return this._OpenConfigurePanel;
            }
            set
            {
                this.SetProperty<ICommand>("OpenConfigurePanel", ref this._OpenConfigurePanel, value, false);
            }
        }

        public static string PropertyCanSelectFeature
        {
            get
            {
                return "CanSelectFeature";
            }
        }

        public bool CanSelectFeature
        {
            get
            {
                return this._CanSelectFeature;
            }
            set
            {
                this.SetProperty<bool>(PropertyCanSelectFeature, ref this._CanSelectFeature, value, () =>
                {
                    this.Map.SwitchSelectedFeature(value);
                }); 
            }
        }

        public bool OpenCheckView
        {
            get
            {
                return this._OpenCheckView;
            }
            set
            {
                this.SetProperty<bool>("OpenCheckView", ref this._OpenCheckView, value, () =>
                {
                    this.OnOpenCheckView(value);
                });
            }
        }

        public static string PropertyIsScratchingSample
        {
            get
            {
                return "IsScratchingSample";
            }
        }

        public bool IsScratchingSample
        {
            get
            {
                return this._IsScratchingSample;
            }
            set
            {
                this.SetProperty<bool>(PropertyIsScratchingSample, ref this._IsScratchingSample, value, () =>
                {
                    SampleModeChanged(value);
                });
            }
        }

        public static string PropertyTaskEditorVisible
        {
            get
            {
                return "TaskEditorVisible";
            }
        }

        public bool TaskEditorVisible
        {
            get
            {
                return this._TaskEditorVisible;
            }
            set
            {
                this.SetProperty<bool>(PropertyTaskEditorVisible, ref this._TaskEditorVisible, value, false);
            }
        }

        public static string PropertyCreateFeatureTaskCommand
        {
            get
            {
                return "CreateFeatureTaskCommand";
            }
        }

        public ICommand CreateFeatureTaskCommand
        {
            get
            {
                return this._CreateFeatureTaskCommand;
            }
            set
            {
                this.SetProperty<ICommand>(PropertyCreateFeatureTaskCommand, ref this._CreateFeatureTaskCommand, value, false);
            }
        }

        public static string PropertyCreateSegmentTaskCommand
        {
            get
            {
                return "CreateSegmentTaskCommand";
            }
        }
        public ICommand CreateSegmentTaskCommand
        {
            get
            {
                return this._CreateSegmentTaskCommand;
            }
            set
            {
                this.SetProperty<ICommand>(PropertyCreateSegmentTaskCommand, ref this._CreateSegmentTaskCommand, value, false);
            }
        }

        public ICommand CreateChangeTaskCommand
        {
            get
            {
                return this._CreateChangeTaskCommand;
            }
            set
            {
                this.SetProperty<ICommand>("CreateChangeTaskCommand", ref this._CreateChangeTaskCommand, value, false);
            }
        }

        public static string PropertyCreateClassifyTaskCommand
        {
            get
            {
                return "CreateClassifyTaskCommand";
            }
        }

        public ICommand CreateClassifyTaskCommand
        {
            get
            {
                return this._CreateClassifyTaskCommand;
            }
            set
            {
                this.SetProperty<ICommand>(PropertyCreateClassifyTaskCommand, ref this._CreateClassifyTaskCommand, value, false);
            }
        }
        
        public static string PropertyCustomClassifyCommand
        {
            get
            {
                return "CustomClassifyCommand";
            }
        }

        public ICommand CustomClassifyCommand
        {
            get
            {
                return this._CustomClassifyCommand;
            }
            set
            {
                this.SetProperty<ICommand>(PropertyCustomClassifyCommand, ref this._CustomClassifyCommand, value, false);
            }
        }

        public CalculatorFactory Factory { get; set; }

        public MapViewModel Map { get; set; }

        public LayerManagerViewModel LayerManagerViewModel { get; private set; }

        public TaskPanelSettingsTabViewModel TaskPanelSettingTabVW { get; private set; }

        // 任务管理
        public TaskManagerViewModel TaskManager { get; set; }

        // 工程面板
        public TaskEditorViewModel TaskEditor { get; private set; }

        public TaskPanelViewModel TaskPanel { get; private set; }

        public StatusBarViewModel StatusBar { get; private set; }

        public VisualizationModel Model { get; private set; }

        //任务
        public TaskSettingsViewModel TaskSettings { get; private set; }

        public ProjectManagerViewModel ProjectManager { get; set; }

        public CheckViewModel CheckTable { get; set; }

        public IFeatureSelect selection { get; set; }

        public HostControlViewModel(VisualizationModel model)
        {
            // 设置窗体大小
            this.MinWidth = 320.0;
            this.MinHeight = 364.0;
            this.Width = 1024.0;
            this.Height = 720.0;
            
            // 显示应用图标
            this.AppIconVisible = true;
            this.UndoRedoVisible = true;

            this.Model = model;
            this.Factory = model.CalFactory;
            this.Map = model.Map;
            // MapViewModel mapCopy;
            // this.Map1 = mapCopy;
            this.LayerManagerViewModel = new LayerManagerViewModel(model.LayerManager, model.LayerTools, model.VisOperation, model.JoinTableManager, this);
            this.TaskSettings = new TaskSettingsViewModel(this.LayerManagerViewModel);
            this.TaskManager = new TaskManagerViewModel(this, this.TaskSettings);
            this.TaskEditor = new TaskEditorViewModel(this);
            this.TaskEditor.CloseCommand = new DelegatedCommand(new Action(this.OnTaskEditorClose));
            this.TaskPanel = new TaskPanelViewModel(this.Map, this.LayerManagerViewModel, this.TaskManager, this.TaskSettings);
            this.StatusBar = new StatusBarViewModel(this.LayerManagerViewModel, this);
            this.CheckTable = new CheckViewModel();

            this.CheckTable.DeleteCommand = new DelegatedCommand(new Action(this.OnCheckViewDeleteCommand));
            this.CreateFeatureTaskCommand = new DelegatedCommand(this.OnCreateFeatureTask);
            this.CreateClassifyTaskCommand = new DelegatedCommand<ClassificationAlgorithm>(this.OnCreateClassifyTask);
            this.CreateChangeTaskCommand = new DelegatedCommand(this.OnCreateChangeTask);
            this.LoadSamplesCommand = new DelegatedCommand(this.OnLoadSamples);
            this.ExportSamplesCommand = new DelegatedCommand(this.OnExportSamples);
            this.CreateSegmentTaskCommand = new DelegatedCommand<SegmentationAlgorithm>(this.OnCreateSegmentTask);

            this.CustomClassifyCommand = new DelegatedCommand(new Action(this.OnOpenFilterTab));
            this.OpenConfigurePanel = new DelegatedCommand(new Action(this.OnOpenOpenConfigurePanel));
            this.ProjectManager = new ProjectManagerViewModel(this);
        }

        private void OnTaskEditorClose()
        {
            this.TaskEditorVisible = false;
        }


        private void OnOpenFilterTab()
        {
            this.TaskPanel.SelectedIndex = TaskPanelViewModel.IndexOfFilterTab;
        }

        private void OnOpenOpenConfigurePanel()
        {
            if (this.TaskPanel.Visible == true)
            {
                this.TaskPanel.LayersTab.OnInitLayerManageTool();
            }
            else
            {
                this.TaskPanel.LayersTab.LayerList.Clear();
            }
            
        }

        private void OnExportSamples()
        {
            SaveFileDialog sf = new SaveFileDialog();
            sf.Filter = "文本文件|*.txt";
            if (sf.ShowDialog() == true)
            {
                string file = sf.FileName;
                try
                {
                    using (var fs = File.CreateText(file))
                    {
                        var samples = GetSamples();
                        if (samples != null)
                        {
                            foreach (var s in samples)
                            {
                                string line = string.Format("{0},{1},{2}",
                                s.GeoClass,
                                s.Layer.Name,
                                s.OID);

                                fs.WriteLine(line);
                            }
                        }
                    }
                }
                catch
                { }
            }
        }

        private void OnOpenCheckView(bool visibity)
        {
            // 检查路径文件是否存在，读取文件并显示核准列表 
            string path = this.ProjectManager.PrjPath + "\\PrjData\\changedetection\\ChangeDetection.shp"; 
            if (visibity == true)
            {
                if (File.Exists(path))
                {
                    LayerDefinition plyr = new LayerDefinition(path);
                    this.LayerManagerViewModel.Model.AddLayerDefinition(plyr);
                    this.LayerManagerViewModel.LayerManager.AddLayer(plyr);
                    this.CheckTable.lyr = plyr;
                    List<FeatureTable> fts = this.Map.GetFeatureTables(plyr);
                    for (int i = 0; i < fts.Count; i++)
                    {
                        this.CheckTable.AddFTable(fts[i].FID, fts[i].CC);
                    }
                }
                else
                {
                    System.Windows.MessageBox.Show("无法找到结果文件！");
                }
            }
            else
            {
                LayerDefinition plyr = new LayerDefinition(path);
                this.LayerManagerViewModel.Model.LayerDefinitions.Remove(plyr);
                this.CheckTable.RemoveFTable();
                // this.LayerManagerViewModel.LayerManager.RemoveLayer(plyr);
                // close and clears layers;
            }
        }

        public void OnCheckViewDeleteCommand()
        {
            this.OpenCheckView = false;
        }

        private void OnLoadSamples()
        {
            OpenFileDialog sf = new OpenFileDialog();
            sf.Filter = "文本文件|*.txt";
            if (sf.ShowDialog() == true)
            {
                string file = sf.FileName;
                try
                {
                    using (var fs = File.OpenRead(file))
                    {
                        using (var sr = new StreamReader(fs, Encoding.UTF8))
                        {
                            string line = null;
                            while ((line = sr.ReadLine()) != null)
                            {
                                var arr = line.Split(',');
                                Coordinates coords = new Coordinates
                                {
                                    Longitude = double.Parse(arr[1]),
                                    Latitude = double.Parse(arr[2])
                                };

                                // 获取id
                                string cls = arr[0];
                                // int id = this.Map.GetFeatureId();
                            }
                        }
                    }
                }
                catch
                { }
            }
        }

        public void InitTaskPanel()
        {
            this.OnCreateTaskTyped(false, TaskType.Feature, AlgorithmType.Feature1);
            this.OnCreateTaskTyped(false, TaskType.Classify, AlgorithmType.Class1);
            this.OnCreateTaskTyped(false, TaskType.Segment, AlgorithmType.Seg1);
        }

        private void OnCreateClassifyTask(ClassificationAlgorithm alg)
        {
            this.OnCreateTaskTyped(false, TaskType.Classify, AlgorithmType.Class1);
        }
        private void OnCreateSegmentTask(SegmentationAlgorithm alg)
        {
            this.OnCreateTaskTyped(false, TaskType.Segment, AlgorithmType.Seg1);
        }
        private void OnCreateFeatureTask()
        {
            this.OnCreateTaskTyped(false, TaskType.Feature, AlgorithmType.Feature1);
        }
        private void OnCreateChangeTask()
        {
            this.OnCreateTaskTyped(false, TaskType.ChangeDetection, AlgorithmType.Feature1);
        }

        private void OnCreateTaskTyped(bool isDuplicate, TaskType taskType, AlgorithmType algType)
        {
            this.TaskEditorVisible = true;
            this.TaskPanel.Visible = true;
            if (isDuplicate)
            {
                // this.TaskEditor.OnCaptureScene();
            }
            else
            {
                this.TaskManager.OnCreateNewTask(isDuplicate, taskType, algType);
                // this.TaskPanel.LayersTab.AddNewLayerCommand.Execute(this);
            }
        }

        public void SetProject(SemanticProject project)
        {
            this.LayerManagerViewModel.Model.LayerDefinitions.Clear();

            // 工程图层列表
            if (project.Layers != null)
            {
                foreach (var layer in project.Layers)
                {
                    this.LayerManagerViewModel.AddLayerDefinition(layer.DataSource);
                }
            }

            this.TaskManager.Project = project;
            // this.TaskEditorVisible = true;
            
            this.TaskPanel.SelectedIndex = TaskPanelViewModel.IndexOfLayersTab;
        }

        public void SampleModeChanged(bool enable)
        {
            // 选择图层，并添加分类字段

            // 打开样本面板，并设置样本工具
            this.Map.SetSampleTool(enable, this.OnAddSample);

            if (enable)
            {
                this.TaskPanel.SamplesTab.StartSample();
                this.TaskPanel.SelectedIndex = TaskPanelViewModel.IndexOfSamplesTab;
            }
            // 设置图层分类设色样式
        }

        public void OnAddSample(int id, string clsName, bool del)
        {
            if (del)
            {
                this.TaskPanel.SamplesTab.DeleteSample(id);
            }
            else
            {
                this.TaskPanel.SamplesTab.AddSample(id, clsName);
            }
        }

        public IEnumerable<SampleEntry> GetSamples()
        {
            if (this.TaskPanel != null && this.TaskPanel.SamplesTab != null)
            {
                return this.TaskPanel.SamplesTab.GetSamples();
            }

            return null;
        }
    }

}

