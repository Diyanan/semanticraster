using Semantic.Configure;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Semantic.Engine
{
    [Serializable]
    public class FeatureParam
    {
        public LayerDefinition VectorLayer { get; set; }
        public bool UseDefaultField { get; set; }

        public LayerDefinition ImageLayer { get; set; }
        public int BandCount { get; set; }

        public List<ImageFeature> CalFeatures { get; set; }

        [XmlIgnore]
        public IEnumerable<string> NeedBands
        {
            get
            {
                return this.CalFeatures.SelectMany(c => c.NeedBandNames).Distinct();
            }
        }

        [XmlIgnore]
        public Dictionary<string, int> BandsIndex { get; set; }

        public string CsvFile { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public FeatureParam()
        {
            this.CalFeatures = new List<ImageFeature>();
            this.BandsIndex = new Dictionary<string, int>();
        }

    }
}
