using ESRI.ArcGIS.Geodatabase;
using Semantic.Engine;
using Semantic.EsriTools;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.MainApplication
{
    public class EsriClassifyCalculator : ClassifyCalculator
    {
        private EsriMapViewModel MapVW { get; set; }

        private EsriVectorLayer Layer = null;

        public EsriClassifyCalculator(EsriMapViewModel mapVW, ClassifyParam param, IEnumerable<SampleEntry> samples) :
            base(param, samples)
        {
            this.MapVW = mapVW;
            var shpLayer = mapVW.GetLayer(param.VectorLayer);
            this.Layer = new EsriVectorLayer(shpLayer);
        }

        protected override void AddClassField(string fieldName)
        {
            this.Layer.AddGeoClassField(fieldName);
        }
    }
}
