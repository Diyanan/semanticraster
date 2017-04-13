using ESRI.ArcGIS.Geodatabase;
using Semantic.EsriTools;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.MainApplication
{
    public class EsriFeatureCalculator : FeatureCalculator
    {
        private EsriMapViewModel MapVW { get; set; }

        private GdalRasterSet RasterSet { get; set; }
        private EsriVectorLayer Layer { get; set; }

        public EsriFeatureCalculator(EsriMapViewModel mapVW, FeatureParam param) :
            base(param)
        {
            this.MapVW = mapVW;
            // 分割图层
            var shpLayer = mapVW.GetLayer(param.VectorLayer);
            this.Layer = new EsriVectorLayer(shpLayer);
            // 影像集
            this.RasterSet = new GdalRasterSet(param.ImageLayer.DataSource);
        }

        public IEnumerable<ImagePoint> GetImagePoints(IFeature feature)
        {
            foreach (Coordinates coord in EsriHelper.GetPoints(feature))
            {
                yield return this.RasterSet.ConvertToImageCoords(coord);
            }
        }

        protected override IEnumerable<Tuple<int, IEnumerable<ImagePoint>>> GetFeatures()
        {
            foreach (var feature in this.Layer.GetAllFeatures())
            {
                int featureId = EsriHelper.GetFeatureId(feature);
                var imagePoints = GetImagePoints(feature);
                yield return new Tuple<int, IEnumerable<ImagePoint>>(featureId, imagePoints);
            }
        }

        //protected override void AddDoubleField(string fieldName)
        //{
        //    this.Layer.AddDoubleField(fieldName);
        //}

    }
}
