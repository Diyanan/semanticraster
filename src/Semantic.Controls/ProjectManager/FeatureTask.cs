using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    [Serializable]
    public class FeatureTask : SemanticTask
    {
        public FeatureParam Param { get; set; }

        public FeatureTask()
        {
            this.TaskType = TaskType.Feature;
            this.Param = new FeatureParam();
        }
    }
}
