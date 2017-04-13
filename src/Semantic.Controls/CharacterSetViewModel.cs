using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class CharacterSetViewModel : ViewModelBase
    {
        private string _Name;
        private bool _IsExpanded = true;
        private bool _IsEnable = true;

        public static string PropertyName
        {
            get
            {
                return "Name";
            }
        }

        public string Name
        {
            get
            {
                return _Name;
            }
            set
            {
                SetProperty(PropertyName, ref _Name, value, false);
            }
        }

        public string PropertyIsExpanded
        {
            get
            {
                return "IsExpanded";
            }
        }

        public bool IsExpanded
        {
            get
            {
                return this._IsExpanded;
            }
            set
            {
                this.SetProperty<bool>(this.PropertyIsExpanded, ref this._IsExpanded, value, false);
            }
        }

        public string PropertyIsEnable
        {
            get
            {
                return "IsEnable";
            }
        }

        public bool IsEnable
        {
            get
            {
                return _IsEnable;
            }

            set
            {
                this.SetProperty<bool>(this.PropertyIsEnable, ref this._IsEnable, value, false);
            }
        }

        public ObservableCollectionEx<CharacterViewModel> Characters { get; private set; }

        public CharacterSetViewModel(string typeName)
        {
            this.Name = typeName;
            this.Characters = new ObservableCollectionEx<CharacterViewModel>();
        }

        public void AddCharacter(CharacterViewModel character)
        {
            this.Characters.Add(character);
        }
    }
}
