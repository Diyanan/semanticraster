using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class LayerSettingsViewModel : ViewModelBase
    {
        private GeoVisualization _GeoVisualization;

        private LayerChooserViewModel _LayerChooser;

        private ViewModelBase _Settings = null;

        public string PropertyLayerChooser
        {
            get
            {
                return "LayerChooser";
            }
        }

        public LayerChooserViewModel LayerChooser
        {
            get
            {
                return this._LayerChooser;
            }
            set
            {
                this.SetProperty<LayerChooserViewModel>(this.PropertyLayerChooser, ref this._LayerChooser, value, false);
            }
        }

        public static string PropertySettings
        {
            get
            {
                return "Settings";
            }
        }

        public ViewModelBase Settings
        {
            get
            {
                return _Settings;
            }

            set
            {
                this.SetProperty<ViewModelBase>(PropertySettings, ref _Settings, value, false);
            }
        }
        // 图层列表
        public LayerManagerViewModel Model { get; private set; }

        public VectorSettingsViewModel VectorSettings { get; private set; }   // 矢量
        public RasterLayerSettingsViewModel RasterSettings { get; private set; } //影像

        public LayerSettingsViewModel(LayerManagerViewModel layerManagerVM, List<Color4F> customColors)
        {
            this.Model = layerManagerVM;
            this.LayerChooser = new LayerChooserViewModel(layerManagerVM, LayerType.None);
            this.LayerChooser.PropertyChanged += new PropertyChangedEventHandler(this.LayerChooserPropertyChanged);

            this.VectorSettings = new VectorSettingsViewModel(layerManagerVM.JoinTalbeManager, customColors);
            this.RasterSettings = new RasterLayerSettingsViewModel(layerManagerVM);
        }
        
        private void LayerChooserPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName.Equals(LayerChooserViewModel.PropertySelectedLayer) && 
                this.LayerChooser.SelectedLayer != null)
            {
                RefreshSettings(this.LayerChooser.SelectedLayer);
            }
        }

        public void SetParentLayer(LayerViewModel currLayer)
        {
            this.LayerChooser.SelectedLayer = currLayer;
        }

        public void RefreshSettings(LayerViewModel parentLayer)
        {
            if (parentLayer != null)
            {
                if (parentLayer.LayerDefinition != null)
                {
                    var layerType = parentLayer.LayerDefinition.LayerType;
                    if (layerType == LayerType.Vector)
                    {
                        this.VectorSettings.SetLayer(parentLayer);
                        this.Settings = this.VectorSettings;
                    }
                    else if (layerType == LayerType.Raster)
                    {
                        this.RasterSettings.SetLayer(parentLayer);
                        this.Settings = this.RasterSettings;
                    }
                }
            }
        }
    }
}
