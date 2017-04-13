using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    /// <summary>
    /// 图层显示样式相关操作
    /// </summary>
    public interface IVisualization
    {
        void SelectFeature(LayerDefinition layerDefn, string query);
        void SetTranspancy(LayerDefinition layerDefn, short transpancy);
        void SetSimpleStyle(LayerDefinition layerDefn, SymbolProperties symbol);
        void SetUniqueStyle(LayerDefinition layerDefn, string field, IEnumerable<SymbolProperties> symbols);
        void UpdateGeoClass(LayerDefinition layerDefn, string field, string query, string leafClass);
        SymbolProperties GetSimpleStyle(LayerDefinition layerDefn);

        IEnumerable<string> GetDispBandLabel(LayerDefinition layerDefn);
        void SetDispBandLabel(LayerDefinition layerDefn, string redLabel, string greenLabel, string blueLabel);
    }
}
