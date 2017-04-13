using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.IO;
namespace Semantic.Engine
{
    [Serializable]
    public class VerifyFeatureSetProperties
    {
        public string Name;
        public string Version;
        public string ProductDate;
    }

    [Serializable]
    public class ImageFeatureExt
    {
        public string Name;
        [XmlElement("Type")]
        public string FeatureType;
        public string Formula;
        public string AlgorithmName;
        public string NeedBandNames;
        [XmlArrayItem("FieldDef", typeof(string))]
        [XmlArray("Fields")]
        public List<string> FieldDefs;
        public string Comments;

        [XmlIgnore]
        public string[] NeedBands
        {
            get
            {
                return this.NeedBandNames.Split('/');
            }
            set
            {
                this.NeedBandNames = string.Join("/", value);
            }
        }
    }

    [XmlRoot("VerifyFeatureSet")]
    [Serializable]
    public class FeatureSet
    {
        public VerifyFeatureSetProperties Properties;
        [XmlArrayItem("Feature", typeof(ImageFeatureExt))]
        [XmlArray("Features")]
        public List<ImageFeatureExt> Features;
        [XmlIgnore]
        private static string ConfigureFile { get; set; }
        [XmlIgnore]
        private static FeatureSet instance = null;

        static FeatureSet()
        { }

        public static FeatureSet LoadFile(string file)
        {
            if (instance == null || ConfigureFile != file)
            {
                ConfigureFile = file;
                return XmlSerialize.DeserializeXML<FeatureSet>(File.ReadAllText(file));
            }
            return instance;
        }
    }
}
