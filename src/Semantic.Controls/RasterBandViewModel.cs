using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class RasterBandViewModel : FieldWellEntryViewModel
    {
        private List<string> allowedDropDownOptions = new List<string>();

        private string _BandLabel;
        private int _BandIndex = -1;

        public static string PropertyBandIndex
        {
            get
            {
                return "BandIndex";
            }
        }

        public int BandIndex
        {
            get
            {
                return this._BandIndex;
            }
            set
            {
                this.SetProperty<int>(PropertyBandIndex, ref this._BandIndex, value, false);
            }
        }

        public static string PropertyBandLabel
        {
            get
            {
                return "BandLabel";
            }
        }

        public string BandLabel
        {
            get
            {
                return this._BandLabel;
            }
            set
            {
                this.SetProperty<string>(PropertyBandLabel, ref this._BandLabel, value, false);
            }
        }

        public bool RasterBandsAreListed { get; private set; }

        public Action<RasterBandViewModel> RemoveCallback { get; set; }

        public List<string> BandNames { get; private set; }

        public RasterBandViewModel(string label)
        {
            this.BandLabel = label;
            this.DisplayString = label;
            this.PropertyChanged += new PropertyChangedEventHandler(this.OnPropertyChanged);
        }

        public void RemoveRasterBand(string bandName)
        {
            this.DropDownOptions.Remove(bandName);
        }

        public void RestoreAllowedDropItems(IEnumerable<string> dropItems)
        {
            this.BandNames = dropItems.ToList();
            this.RasterBandsAreListed = this.BandNames.Count > 1;
            if (this.RasterBandsAreListed)
            {
                this.allowedDropDownOptions = this.BandNames;
                this.PopulateDropDownOptions();
            }
        }

        private void PopulateDropDownOptions()
        {
            this.DropDownOptions.Clear();
            foreach (string item in this.allowedDropDownOptions)
            {
                this.DropDownOptions.Add(item);
            }
            // this.AddDropDownOptionControls();
        }

        private void OnPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName != PropertyBandIndex)
                return;
            if (this.RasterBandsAreListed)
            {
                var bandName = this.BandNames[this.BandIndex - 1];
                this.DisplayString = string.Format("{0} ({1})", this.BandLabel, bandName);
            }
            else
            {
                this.DisplayString = this.BandLabel;
            }
        }

        protected override void OnSelectedDropDownOptionValueChanged()
        {
            if (this.SelectedDropDownOption is string)
            {
                var item = (string)this.SelectedDropDownOption;
                this.BandIndex = this.BandNames.IndexOf(item) + 1;
            }
        }

        protected override void OnSelectedRemoveEntryOption()
        {
            if (this.RemoveCallback != null)
            {
                this.RemoveCallback(this);
            }
        }

        public string GetBandName()
        {
            return this.BandNames[this.BandIndex - 1];
        }
    }
}
