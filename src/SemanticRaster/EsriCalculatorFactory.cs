using Semantic.Controls;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.MainApplication
{
    public class EsriCalculatorFactory : CalculatorFactory
    {
        private EsriMapViewModel MapVW { get; set; }

        public EsriCalculatorFactory(EsriMapViewModel mapVW)
        {
            this.MapVW = mapVW;
        }

        public override FeatureCalculator Create(FeatureParam param)
        {
            return new EsriFeatureCalculator(this.MapVW, param);
        }

        public override ICalculator Create(SemanticTask task, IEnumerable<SampleEntry> samples)
        {
            switch (task.TaskType)
            {
                case TaskType.Feature:
                    var featureTask = task as FeatureTask;
                    return new EsriFeatureCalculator(this.MapVW, featureTask.Param);
                case TaskType.Classify:
                    var classifyTask = task as ClassifyTask;
                    return new EsriClassifyCalculator(this.MapVW, classifyTask.Param, samples);
            }
            return base.Create(task, samples);
        }
    }
}
