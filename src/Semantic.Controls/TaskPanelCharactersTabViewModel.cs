using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class TaskPanelCharactersTabViewModel : ViewModelBase
    {
        public TaskManagerViewModel TaskManager { get; set; }

        public TaskPanelCharactersTabViewModel(TaskManagerViewModel taskManagerVW)
        {
            this.TaskManager = taskManagerVW;
        }

        private bool IsFeatureTask(SemanticTask task)
        {
            return task.TaskType == TaskType.Feature;
        }
    }
}
