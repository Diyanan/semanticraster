using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class TaskChooserViewModel : ViewModelBase
    {
        private TaskManagerViewModel _Model;
        private TaskViewModel _SelectedTask;

        private Predicate<SemanticTask> OnFilter = null;
        public ObservableCollectionEx<TaskViewModel> Tasks { get; private set; }

        public static string PropertySelectedTask
        {
            get
            {
                return "SelectedTask";
            }
        }

        public TaskViewModel SelectedTask
        {
            get
            {
                return this._SelectedTask;
            }
            set
            {
                base.SetProperty(PropertySelectedTask, ref this._SelectedTask, value);
            }
        }

        public string PropertyModel
        {
            get
            {
                return "Model";
            }
        }

        public TaskManagerViewModel Model
        {
            get
            {
                return this._Model;
            }
            set
            {
                base.SetProperty(this.PropertyModel, ref this._Model, value);
            }
        }


        public TaskChooserViewModel(TaskManagerViewModel visModel, Predicate<SemanticTask> filter)
        {
            this.Model = visModel;
            this.OnFilter = filter;
            this.Tasks = new ObservableCollectionEx<TaskViewModel>();
            foreach (var task in this.Model.TaskList)
            {
                this.AddTask(task);
            }

            this.Model.TaskList.ItemAdded += new ObservableCollectionExChangedHandler<TaskViewModel>(this.AddTask);
            this.Model.TaskList.ItemRemoved += new ObservableCollectionExChangedHandler<TaskViewModel>(this.RemoveTask);
        }

        private void AddTask(TaskViewModel task)
        {
            if (this.OnFilter == null || this.OnFilter(task.Model))
            {
                this.Tasks.Add(task);
            }
        }

        private void RemoveTask(ObservableCollectionEx<TaskViewModel> tasks, SemanticTask item)
        {
            TaskViewModel result = null;
            foreach (var task in tasks)
            {
                if (task.Model == item)
                {
                    result = task;
                    break;
                }
            }
            if (result != null)
            {
                tasks.Remove(result);
            }
        }


        private void RemoveTask(TaskViewModel taskVW)
        {
            var task = taskVW.Model;
            TaskViewModel result = null;
            foreach (TaskViewModel item in this.Tasks)
            {
                if (taskVW == item)
                {
                    result = taskVW;
                    break;
                }
            }
            if (result != null)
            {
                this.Tasks.Remove(result);
            }
        }
    }
}
