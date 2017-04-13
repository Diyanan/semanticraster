using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    /// <summary>
    /// 样本对象
    /// </summary>
    public class SampleEntry
    {
        public LayerDefinition Layer { get; set; }
        public int OID { get; set; }
        public string GeoClass { get; set; }
    }
}
