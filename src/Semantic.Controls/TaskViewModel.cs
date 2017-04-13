using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class TaskViewModel : ViewModelBase
    {
        // 执行的任务
        private SemanticTask _Model;

        // 索引号
        private string _TaskIndex;

        public static string PropertyTaskIndex
        {
            get
            {
                return "TaskIndex";
            }
        }

        public string TaskIndex
        {
            get
            {
                return this._TaskIndex;
            }
            set
            {
                this.SetProperty<string>(PropertyTaskIndex, ref this._TaskIndex, value, false);
            }
        }

        public static string PropertyModel
        {
            get
            {
                return "Model";
            }
        }

        public SemanticTask Model
        {
            get
            {
                return this._Model;
            }
            set
            {
                if (value == null)
                    return;
                this.SetProperty<SemanticTask>(PropertyModel, ref this._Model, value, false);
            }
        }

        public ICommand ExecuteCommand { get; set; }

        public ICommand SettingsCommand { get; set; }

        public ICommand DeleteCommand { get; set; }


        public TaskViewModel(SemanticTask task, int index)
        {
            this.Model = task;
            this.SetIndex(index);
        }

        public void SetIndex(int i)
        {
            this.TaskIndex = (i + 1).ToString();
        }
    }
}
