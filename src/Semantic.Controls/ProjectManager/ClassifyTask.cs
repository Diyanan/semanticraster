using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    [Serializable]
    public class ClassifyTask : SemanticTask
    {
        public ClassifyParam Param { get; set; }

        public ClassifyTask()
        {
            this.TaskType = TaskType.Classify;
            this.Param = new ClassifyParam();
        }
    }
}
