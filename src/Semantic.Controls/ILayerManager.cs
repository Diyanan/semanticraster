using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    /// <summary>
    /// 图层管理（针对底层COM的高层封装：使用LayerDefinition表示图层）
    /// </summary>
    public interface ILayerManager
    {
        void AddLayer(LayerDefinition layerDef);

        List<string> GetAllLayerPath();

        void RemoveLayer(LayerDefinition layerDef);

        void MoveLayer(LayerDefinition layerDefn, int toIndex);

        void SetLayerVisible(LayerDefinition layerDefinition, bool visible);

        void AddJoinTable(LayerDefinition layerDefinition, string csvPath);

        IEnumerable<TableFieldDefninition> GetLayerFields(LayerDefinition layerDefinition);

    }
}
