using Semantic.Engine;
using System.Collections.Generic;

namespace Semantic.Controls
{
    public interface IJoinManager
    {
        IEnumerable<string> GetAllJoins(LayerDefinition layer);
        void AddJoinTable(LayerDefinition layer, string csvPath);
        void RemoveJoinTable(LayerDefinition layer, string joinName);
        void RemoveLastJoinTable(LayerDefinition layer);
        void ClearAllJoinTable(LayerDefinition layer);
    }
}