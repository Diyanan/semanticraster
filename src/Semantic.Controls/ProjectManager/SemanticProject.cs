using Semantic.Engine;
using Semantic.WpfCommon;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    [XmlRoot("Project", IsNullable = false)]
    [Serializable]
    public class SemanticProject : CompositePropertyChangeNotificationBase
    {
        private string _Name;

        private List<SemanticTask> tasks;

        [XmlElement("Settings")]
        public ProjectSettings Model { get; set; }

        public static string PropertyName
        {
            get
            {
                return "Name";
            }
        }

        [XmlIgnore]
        public string Name
        {
            get
            {
                return this.Model.Name;
            }
            set
            {
                base.SetProperty<string>(PropertyName, ref this._Name, value, ()=>
                {
                    this.Model.Name = value;
                });
            }
        }

        public List<LayerDefinition> Layers { get; set; }

        [XmlArrayItem("seg", typeof(SegTask))]
        [XmlArrayItem("fea", typeof(FeatureTask))]
        [XmlArrayItem("cls", typeof(ClassifyTask))]
        [XmlArrayItem("cha", typeof(ChangeTask))]
        [XmlArray("TaskList")]
        public List<SemanticTask> Tasks
        {
            get
            {
                return this.tasks;
            }
        }

        public SemanticProject()
        {
            this.tasks = new List<SemanticTask>();
        }
    }
}
