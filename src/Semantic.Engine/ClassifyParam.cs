using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    [Serializable]
    public class ClassifyParam
    {
        public LayerDefinition VectorLayer { get; set; }
        public List<int> SelSamples { get; set; }
        public int Algorithm { get; set; }
        public List<string> SelFields { get; set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public ClassifyParam()
        {
            this.SelFields = new List<string>();
        }
    }
}
