using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class CharacterListEntryViewModel : ViewModelBase
    {
        private string displayString;
        private CharacterViewModel _Character;

        public ICommand RemoveCommand { get; private set; }


        public Action<CharacterListEntryViewModel> RemoveCallback { get; set; }

        public string PropertyCharacter
        {
            get
            {
                return "Character";
            }
        }

        public CharacterViewModel Character
        {
            get
            {
                return this._Character;
            }
            set
            {
                this.SetProperty<CharacterViewModel>(this.PropertyCharacter, ref this._Character, value, false);
            }
        }

        public static string PropertyDisplayString
        {
            get
            {
                return "DisplayString";
            }
        }

        public string DisplayString
        {
            get
            {
                return this.displayString;
            }
            set
            {
                this.SetProperty<string>(FieldWellEntryViewModel.PropertyDisplayString, ref this.displayString, value, false);
            }
        }
        public CharacterListEntryViewModel()
        {
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
