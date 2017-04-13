using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    [Serializable]
    public class SegTask : SemanticTask
    {
        public SegmentParam Param { get; set; }

        public SegTask()
        {
            this.TaskType = TaskType.Segment;
            this.Param = new SegmentParam();
        }
    }
}
