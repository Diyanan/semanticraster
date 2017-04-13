using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;

namespace Semantic.Engine
{
    [Serializable]
    public class Meta
    {
        [XmlElement("name")]
        public string Name { get; set; }
        [XmlElement("desc")]
        public string Desc { get; set; }
    }

    public class ClassDefinition
    {
        [XmlAttribute("label")]
        public string Label { get; set; }
        [XmlAttribute("code")]
        public string Code { get; set; }

        [XmlElement("secondary")]
        public List<ClassDefinition> SubLevel { get; set; }
    }


    [XmlRoot("root")]
    [Serializable]
    public class GeoClassDefinition
    {
        [XmlElement("meta")]
        public Meta Meta;

        [XmlArrayItem("subject", typeof(ClassDefinition))]
        [XmlArray("category")]
        public List<ClassDefinition> Category { get; set; }

        [XmlIgnore]
        private static string ConfigureFile { get; set; }
        [XmlIgnore]
        private static GeoClassDefinition instance = null;

        static GeoClassDefinition()
        { }

        public static GeoClassDefinition LoadFile(string file)
        {
            if (instance == null || ConfigureFile != file)
            {
                ConfigureFile = file;
                return XmlSerialize.DeserializeXML<GeoClassDefinition>(File.ReadAllText(file));
            }
            return instance;
        }
    }
}
