using Semantic.WpfCommon;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    public enum TaskType
    {
        Segment, Feature, Classify, ChangeDetection, GChangeDetection
    }

    public enum AlgorithmType
    {
        Seg1, Seg2, Seg3, Class1, Class2, Class3, Feature1, ChangeDetection1, GChangeDetection1
    }

    [Serializable]
    public class SemanticTask : CompositePropertyChangeNotificationBase
    {
        private string _Name;
        private TaskType _TaskType;

        public static string PropertyName
        {
            get
            {
                return "Name";
            }
        }

        [XmlAttribute("Name")]
        public string Name
        {
            get
            {
                return this._Name;
            }
            set
            {
                base.SetProperty<string>(PropertyName, ref this._Name, value);
            }
        }

        public static string PropertyTaskType
        {
            get
            {
                return "TaskType";
            }
        }

        [XmlAttribute("Type")]
        public TaskType TaskType
        {
            get
            {
                return this._TaskType;
            }
            set
            {
                base.SetProperty<TaskType>(PropertyTaskType, ref this._TaskType, value);
            }
        }

        public SemanticTask()
        {
            //chuli 
        }

        // public abstract void Process();

        public static SemanticTask Create(TaskType taskType)
        {
            switch (taskType)
            {
                case TaskType.Feature:
                    return new FeatureTask();
                case TaskType.Classify:
                    return new ClassifyTask();
                case TaskType.Segment:
                    return new SegTask();
                case TaskType.ChangeDetection:
                    return new ChangeTask();
                case TaskType.GChangeDetection:
                    return new GChangeDetection();
                default:
                    break;
            }
            return null;
        }
    }
}
