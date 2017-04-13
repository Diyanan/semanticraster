using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;

namespace Semantic.Controls
{
    public class VisualizationModel
    {
        public Dispatcher UIDispatcher { get; set; }
        public ILayerTools LayerTools { get; set; }
        public ILayerManager LayerManager { get; set; }
        public IJoinManager JoinTableManager { get; set; }
        public IVisualization VisOperation { get; set; }
        public FeatureSet FeatureSet { get; set; }
        public GeoClass GeoClass { get; set; }
        public ColorSelector ColorSelector { get; set; }
        public CalculatorFactory CalFactory { get; set; }
        public MapViewModel Map { get; set; }
        public MapViewModel Map1 { get; set; }

        public VisualizationModel(MapViewModel map, CalculatorFactory cal,
            ILayerTools tools, ILayerManager layerManager, IVisualization vis, IJoinManager joinTableManager)
        {
            this.UIDispatcher = Dispatcher.CurrentDispatcher;
            this.Map = map;
            this.ColorSelector = new ColorSelector();
            this.CalFactory = cal;
            this.LayerTools = tools;
            this.LayerManager = layerManager;
            this.VisOperation = vis;
            this.JoinTableManager = joinTableManager;
        }
    }
}
