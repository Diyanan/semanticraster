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
    public class RasterLayerSettingsViewModel : ViewModelBase
    {
        public LayerManagerViewModel LayerManager { get; private set; }

        public LayerViewModel CurrentLayer { get; set; }

        public ObservableCollectionEx<RasterBandViewModel> RasterBands { get; private set; }

        public RasterBandViewModel RedBand { get; set; }
        public RasterBandViewModel GreenBand { get; set; }
        public RasterBandViewModel BlueBand { get; set; }

        public ICommand ApplyStyleCommand { get; set; }

        public RasterLayerSettingsViewModel(LayerManagerViewModel layerManagerVM)
        {
            this.LayerManager = layerManagerVM;
            this.RasterBands = new ObservableCollectionEx<RasterBandViewModel>();
            this.ApplyStyleCommand = new DelegatedCommand(this.OnApplyStyle, new Predicate(()=> this.CurrentLayer != null));
            this.RedBand = new RasterBandViewModel("红");
            this.GreenBand = new RasterBandViewModel("绿");
            this.BlueBand = new RasterBandViewModel("蓝");
            this.RasterBands.Add(this.RedBand);
            this.RasterBands.Add(this.GreenBand);
            this.RasterBands.Add(this.BlueBand);
        }

        private void OnApplyStyle()
        {
            var geoVis = this.CurrentLayer.GeoVisualization;
            geoVis.Vis.SetDispBandLabel(
                this.CurrentLayer.LayerDefinition, 
                this.RedBand.GetBandName(),
                this.GreenBand.GetBandName(),
                this.BlueBand.GetBandName());
        }

        public void SetLayer(LayerViewModel layer)
        {
            this.CurrentLayer = layer;
            var geoVis = layer.GeoVisualization;
            if (geoVis != null)
            {
                var labels = geoVis.Vis.GetDispBandLabel(layer.LayerDefinition);
                if (labels.Count() >= 3)
                {
                    var label = labels.ToList();
                    var prefix = label[0].Substring(0, label[0].IndexOf("_") + 1);
                    int bandCount = this.LayerManager.LayerTools.GetRasterLayerBandsCount(layer.LayerDefinition);
                    var bandNames = GetBandNames(prefix, bandCount).ToList();
                    SetDropItems(bandNames);
                    this.RedBand.BandIndex = bandNames.IndexOf(label[0]) + 1;
                    this.GreenBand.BandIndex = bandNames.IndexOf(label[1]) + 1;
                    this.BlueBand.BandIndex = bandNames.IndexOf(label[2]) + 1;
                }
            }
        }

        public IEnumerable<string> GetBandNames(string prefix, int bandCount)
        {
            return Enumerable.Range(1, bandCount).Select(i => prefix + i);
        }

        private void SetDropItems(IEnumerable<string> bandNames)
        {
            foreach (var item in this.RasterBands)
            {
                item.RestoreAllowedDropItems(bandNames);
            }
        }
    }
}
