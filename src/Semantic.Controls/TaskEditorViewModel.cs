using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class TaskEditorViewModel : ViewModelBase
    {
        private ICommand _CloseCommand;
        private ICommand _DuplicateTaskCommand;

        private HostControlViewModel hostControlViewModel;

        public TaskManagerViewModel TaskManager { get; set; }

        public static string PropertyCloseCommand
        {
            get
            {
                return "CloseCommand";
            }
        }

        public ICommand CloseCommand
        {
            get
            {
                return this._CloseCommand;
            }
            set
            {
                this.SetProperty<ICommand>(PropertyCloseCommand, ref this._CloseCommand, value, false);
            }
        }

        public static string PropertyDuplicateTaskCommand
        {
            get
            {
                return "DuplicateTaskCommand";
            }
        }

        public ICommand DuplicateTaskCommand
        {
            get
            {
                return this._DuplicateTaskCommand;
            }
            set
            {
                this.SetProperty<ICommand>(PropertyDuplicateTaskCommand, ref this._DuplicateTaskCommand, value, false);
            }
        }


        public TaskEditorViewModel(HostControlViewModel hostControlViewModel)
        {
            this.hostControlViewModel = hostControlViewModel;
            this.TaskManager = this.hostControlViewModel.TaskManager;
        }
    }
}
