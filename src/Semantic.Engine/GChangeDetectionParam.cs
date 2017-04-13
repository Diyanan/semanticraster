using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    [Serializable]
    public class GChangeDetectionParam
    {
        public string segShppath { get; set; }

        public GChangeDetectionParam()
        {
            segShppath = "..\\PrjData\\calfeature\\CalFeature.shp";
        }
    }
}
