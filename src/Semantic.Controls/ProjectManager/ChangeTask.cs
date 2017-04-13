using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    [Serializable]
    public class ChangeTask : SemanticTask
    {
        public ChangeParam Param { get; set; }

        public ChangeTask()
        {
            this.TaskType = TaskType.ChangeDetection;
            this.Param = new ChangeParam();
        }
    }
}
