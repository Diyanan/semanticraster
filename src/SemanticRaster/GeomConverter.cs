using ESRI.ArcGIS.Geometry;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.MainApplication
{
    public class GeomConverter
    {
        /// <summary>
        /// ogr geometry转esri geometry
        /// </summary>
        /// <param name="feature"></param>
        /// <returns></returns>
        //private static IGeometry ConvertOgrShape2EsriGeometry(OSGeo.OGR.Feature feature)
        //{
        //    Geometry shape = feature.GetGeometryRef();
        //    byte[] wkb = new byte[shape.WkbSize()];
        //    shape.ExportToWkb(wkb);
        //    IGeometryFactory2 factory = new GeometryEnvironment() as IGeometryFactory2;
        //    IGeometry geom = null;
        //    int size = 0;
        //    factory.CreateGeometryFromWkbVariant(wkb, out geom, out size);
        //    return geom;
        //}

    }
}
