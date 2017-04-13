using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    [Serializable]
    public class SymbolProperties
    {
        [XmlAttribute("Category")]
        public string Category { get; set; }

        [XmlElement("Opacity")]
        public short Opacity { get; set; }

        [XmlElement("FillColor")]
        public Color4F FillColor { get; set; }

        [XmlElement("LineWidth")]
        public float LineWidth { get; set; }

        [XmlElement("LineColor")]
        public Color4F LineColor { get; set; }

        [XmlElement("ColorSet")]
        public bool ColorSet { get; set; }

        public SymbolProperties()
        {

        }

        public SymbolProperties(string catagory)
        {
            this.Category = catagory;
        }
    }
}
