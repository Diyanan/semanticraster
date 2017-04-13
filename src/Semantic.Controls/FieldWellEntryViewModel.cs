using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public abstract class FieldWellEntryViewModel : ViewModelBase
    {
        private RemoveItemPlaceholder removeHeightOption = new RemoveItemPlaceholder("删除");

        private string displayString;
        private object _SelectedDropDownOption;

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
                this.SetProperty<string>(PropertyDisplayString, ref this.displayString, value, false);
            }
        }


        public string PropertySelectedDropDownOption
        {
            get
            {
                return "SelectedDropDownOption";
            }
        }

        public object SelectedDropDownOption
        {
            get
            {
                return this._SelectedDropDownOption;
            }
            set
            {
                base.SetProperty<object>(this.PropertySelectedDropDownOption, ref this._SelectedDropDownOption, value, new Action(this.OnSelectedDropDownOptionChanged));
            }
        }

        public ObservableCollectionEx<object> DropDownOptions { get; private set; }

        public FieldWellEntryViewModel()
        {
            this.DropDownOptions = new ObservableCollectionEx<object>();
        }

        public void RemoveEntry()
        {
            this.OnSelectedRemoveEntryOption();
        }

        protected void AddDropDownOptionControls(bool addSeparator = true)
        {
            if (addSeparator)
            {
                this.AddDropDownSeparator();
            }
            this.AddDropDownOptionRemove();
        }

        protected void AddDropDownSeparator()
        {
            this.DropDownOptions.Add(ListUtilities.Separator);
        }

        protected void AddDropDownOptionRemove()
        {
            this.DropDownOptions.Add(this.removeHeightOption);
        }

        protected abstract void OnSelectedRemoveEntryOption();

        protected abstract void OnSelectedDropDownOptionValueChanged();

        private void OnSelectedDropDownOptionChanged()
        {
            if (this.SelectedDropDownOption == this.removeHeightOption)
            {
                this.OnSelectedRemoveEntryOption();
            }
            else
            {
                this.OnSelectedDropDownOptionValueChanged();
            }
        }
    }
}
