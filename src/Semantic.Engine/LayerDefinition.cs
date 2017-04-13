using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public enum LayerType
    {
        Vector, Raster, None
    }

    [Serializable]
    public class LayerDefinition : IEquatable<LayerDefinition>
    {
        public string Name { get; set; }
        public string DataSource { get; set; }
        public LayerType LayerType { get; set; }

        public LayerDefinition()
        { }

        public LayerDefinition(string path)
        {
            this.DataSource = path;
            this.Name = Path.GetFileName(path);
            this.LayerType = Path.GetExtension(path).ToLower() == ".tif" ?
                     LayerType.Raster : LayerType.Vector;
            //if (Path.GetExtension(path).ToLower() == ".shp")
            //{
            //    this.LayerType = LayerType.Vector;
            //}
            //else if(Path.GetExtension(path).ToLower() == null)
            //    {
            //        this.LayerType = LayerType.Vector;
            //    }
            //    else
            //    {
            //        this.LayerType = LayerType.Raster;
            //    }
        }

        public bool Equals(LayerDefinition other)
        {
            return this.DataSource == other.DataSource;
        }
    }
}
