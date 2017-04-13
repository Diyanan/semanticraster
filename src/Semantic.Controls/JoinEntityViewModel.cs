using Semantic.WpfExtensions;
using System;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class JoinEntityViewModel : ViewModelBase
    {
        private string joinName;

        public static string PropertyJoinName
        {
            get
            {
                return "JoinName";
            }
        }

        public string DisplayString
        {
            get
            {
                return this.joinName;
            }
            set
            {
                this.SetProperty<string>(PropertyJoinName, ref this.joinName, value, false);
            }
        }

        public ICommand RemoveCommand { get; private set; }

        public Action<JoinEntityViewModel> RemoveCallback { get; set; }

        public JoinEntityViewModel(string joinName)
        {
            this.joinName = joinName;
            this.RemoveCommand = new DelegatedCommand(this.RemoveEntry);
        }

        public void RemoveEntry()
        {
            if (RemoveCallback != null)
            {
                this.RemoveCallback(this);
            }
        }
    }
}
