using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    [Serializable]
    public class GChangeDetection : SemanticTask
    {
        public GChangeDetectionParam Param { get; set; }

        public GChangeDetection()
        {
            this.TaskType = TaskType.Segment;
            this.Param = new GChangeDetectionParam();
        }
    }
}
