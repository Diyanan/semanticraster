using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class CalculatorFactory
    {
        public CalculatorFactory()
        { }

        public virtual FeatureCalculator Create(FeatureParam param)
        {
            return new FeatureCalculator(param);
        }

        public virtual ICalculator Create(SemanticTask task, IEnumerable<SampleEntry> samples)
        {
            switch(task.TaskType)
            {
                case TaskType.Feature:
                    var featureTask = task as FeatureTask;
                    return new FeatureCalculator(featureTask.Param);
                case TaskType.Classify:
                    var classifyTask = task as ClassifyTask;
                    return new ClassifyCalculator(classifyTask.Param, samples);
                case TaskType.Segment:
                    var segmentTask = task as SegTask;
                    return new SegmentCaculator(segmentTask.Param);
                case TaskType.ChangeDetection:
                    var changeTask = task as ChangeTask;
                    return new ChangeCalculator(changeTask.Param);
            }
            return null;
        }

    }
}
