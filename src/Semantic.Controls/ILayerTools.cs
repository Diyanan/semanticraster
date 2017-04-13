using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public interface ILayerTools
    {
        int GetRasterLayerBandsCount(LayerDefinition layerDef);

        IEnumerable<string> GetFieldName(LayerDefinition layerDefn);

        int GetFeatureId(LayerDefinition layerDefn, Coordinates coords);
    }
}
