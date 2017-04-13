using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public interface IFeatureSelect
    {
        void SelectFeature(LayerDefinition lyr,int fid);
    }
}
