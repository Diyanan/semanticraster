using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class VectorLayerStyleViewModel : ViewModelBase
    {
        public bool UsingSimpleSymbol { get; set; }

        public string FieldName { get; set; }
        public GeoClass GeoClass { get; set; }
    }
}
