using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class CharacterSettingsViewModel : ViewModelBase
    {
        private int _BandCount = 0;
        private bool _UseDefaultFields = true;
        private LayerDefinition _VectorLayer = null;
        private LayerDefinition _RasterLayer = null;

        public static string PropertyBandCount
        {
            get
            {
                return "BandCount";
            }
        }

        public int BandCount
        {
            get
            {
                return _BandCount;
            }
            set
            {
                if (value > 0 && this._BandCount != value)
                {
                    this.SetProperty<int>(PropertyBandCount, ref this._BandCount, value, new Action(this.SetDropItems));
                }
            }
        }

        public static string PropertyUseDefaultFields
        {
            get
            {
                return "UseDefaultFields";
            }
        }

        public bool UseDefaultFields
        {
            get
            {
                return this._UseDefaultFields;
            }
            set
            {
                this.SetProperty<bool>(PropertyUseDefaultFields, ref this._UseDefaultFields, value, false);
            }
        }


        public static string PropertyVectorLayer
        {
            get
            {
                return "VectorLayer";
            }
        }


        public LayerDefinition VectorLayer
        {
            get
            {
                return _VectorLayer;
            }

            set
            {
                this.SetProperty<LayerDefinition>(PropertyVectorLayer, ref this._VectorLayer, value, false);
            }
        }

        public static string PropertyRasterLayer
        {
            get
            {
                return "RasterLayer";
            }
        }

        public LayerDefinition RasterLayer
        {
            get
            {
                return _RasterLayer;
            }

            set
            {
                this.SetProperty<LayerDefinition>(PropertyRasterLayer, ref this._RasterLayer, value, false);
            }
        }

        public LayerChooserViewModel VectorLayerChooser { get; private set; }
        public LayerChooserViewModel RasterLayerChooser { get; private set; }

        public ObservableCollectionEx<RasterBandViewModel> RasterBands { get; private set; }

        public LayerManagerViewModel LayerManager { get; private set; }

        public CharacterSettingsViewModel(LayerManagerViewModel layerManagerVM)
        {
            this.LayerManager = layerManagerVM;
            this.RasterBands = new ObservableCollectionEx<RasterBandViewModel>();
            this.VectorLayerChooser = new LayerChooserViewModel(layerManagerVM, LayerType.Vector);
            this.VectorLayerChooser.PropertyChanged += SelectedVectorLayerChanged;
            this.RasterLayerChooser = new LayerChooserViewModel(layerManagerVM, LayerType.Raster);
            this.RasterLayerChooser.PropertyChanged += SelectedRasterLayerChanged;
        }

        private void SelectedVectorLayerChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == LayerChooserViewModel.PropertySelectedLayer)
            {
                if (this.VectorLayerChooser.SelectedLayer != null)
                {
                    this.VectorLayer = this.VectorLayerChooser.SelectedLayer.LayerDefinition;
                }
                else
                {
                    this.VectorLayer = null;
                }
            }
        }

        private void SelectedRasterLayerChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == LayerChooserViewModel.PropertySelectedLayer)
            {
                if (this.RasterLayerChooser.SelectedLayer != null)
                {
                    var rasterLayerDefn = this.RasterLayerChooser.SelectedLayer.LayerDefinition;
                    this.BandCount = this.LayerManager.LayerTools.GetRasterLayerBandsCount(rasterLayerDefn);
                    this.RasterLayer = rasterLayerDefn;
                }
            }
        }

        private void SetDropItems()
        {
            var bandNames = Enumerable.Range(1, this.BandCount).Select(i => "Layer " + i).ToList();
            foreach (var item in this.RasterBands)
            {
                item.RestoreAllowedDropItems(bandNames);
            }
        }

        public void InitBandMap(IEnumerable<string> bandNames)
        {
            this.RasterBands.Clear();
            foreach (var label in bandNames)
            {
                if (!string.IsNullOrEmpty(label))
                {
                    var rasterBand = new RasterBandViewModel(label);
                    this.RasterBands.Add(rasterBand);
                }
            }

            if (this.BandCount > 0)
            {
                SetDropItems();
            }
        }
    }
}
