using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Semantic.Engine
{
    [Serializable]
    public class LeafNode
    {
        [XmlElement("NAME")]
        public string Name { get; set; }
        [XmlElement("COLOR")]
        public string ColorString { get; set; }

        [XmlElement("LeafNode")]
        public List<LeafNode> Leaves { get; set; }

        [XmlIgnore]
        public Color Color
        {
            get
            {
                string[] rgb = this.ColorString.Split(';');
                return Color.FromArgb(
                    Convert.ToInt32(rgb[0]),
                    Convert.ToInt32(rgb[1]),
                    Convert.ToInt32(rgb[2]));
            }
            set
            {
                this.ColorString =
                    value.R.ToString() + ";" +
                    value.G.ToString() + ";" +
                    value.B.ToString();
            }
        }
    }

    [XmlRoot("GeoClass")]
    [Serializable]
    public class GeoClass
    {
        [XmlElement("LeafNode")]
        public List<LeafNode> Leaves { get; set; }

        [XmlIgnore]
        private static string ConfigureFile { get; set; }
        [XmlIgnore]
        private static GeoClass instance = null;

        static GeoClass()
        { }

        public static GeoClass LoadFile(string file)
        {
            if (instance == null || ConfigureFile != file)
            {
                ConfigureFile = file;
                return XmlSerialize.DeserializeXML<GeoClass>(File.ReadAllText(file));
            }
            return instance;
        }
    }
}
