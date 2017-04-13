using ESRI.ArcGIS.Carto;
using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class CheckViewModel: ViewModelBase
    {
        private int _SelectedFeatureID;
        private FeatureTableItemViewModel selectedItem;
        public ObservableCollectionEx<FeatureTableItemViewModel> FTables { get; private set; }
        public IFeatureSelect selection { get; set; }
        public LayerDefinition lyr { get; set; }
        
        public ICommand _DeleteCommand;

        public ICommand DeleteCommand
        {
            get
            {
                return this._DeleteCommand;
            }
            set
            {
                this.SetProperty<ICommand>("DeleteCommand",ref this._DeleteCommand,value,false);
            }
        }

        public int SelectedFeatureID
        {
            get
            {
                return this._SelectedFeatureID;
            }
            set
            {
                this.SetProperty<int>("SelectedFeatureID",ref this._SelectedFeatureID,value,false);
            }
        }

        public FeatureTableItemViewModel SelectedItem
        {
            get
            {
                return this.selectedItem;
            }
            set
            {
                this.SetProperty<FeatureTableItemViewModel>("SelectedItem",ref this.selectedItem,value,()=>
                {
                    this.OnSelectedItemChanged(value);
                });
            }
        }

        public void AddFTable(int fid, int cc)
        {
            FeatureTableItemViewModel ft = new FeatureTableItemViewModel();
            ft.CC = cc;
            ft.FID = fid;
            this.FTables.Add(ft);
        }

        public void RemoveFTable()
        {
            this.FTables.RemoveAll();
        }

        public void OnSelectedItemChanged(FeatureTableItemViewModel ft)
        {
            if (ft !=null)
            {
                this.SelectedFeatureID = ft.FID;
                this.selection.SelectFeature(this.lyr, this.SelectedFeatureID);
            }
            else
            {
                return;
            }
            
        }

        public CheckViewModel()
        {
            this.FTables = new ObservableCollectionEx<FeatureTableItemViewModel>();
        }

    }
}
