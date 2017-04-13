using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Controls;
using ESRI.ArcGIS.DataSourcesFile;
using ESRI.ArcGIS.DataSourcesRaster;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.EsriTools
{
    public class EsriHelper
    {
        /// <summary>
        /// 从shapefile路径打开featurelayer
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        public static IFeatureLayer OpenFeatureLayer(string path)
        {
            string directoryName = System.IO.Path.GetDirectoryName(path);
            string fileName = System.IO.Path.GetFileName(path);

            // 打开shapefile
             IWorkspaceFactory workSpaceFractory = new ShapefileWorkspaceFactory();
            IWorkspace workSpace = workSpaceFractory.OpenFromFile(directoryName, 0);
            IFeatureClass featureClass = ((IFeatureWorkspace)workSpace).OpenFeatureClass(fileName);
            return new FeatureLayerClass
            {
                Name = featureClass.AliasName,
                FeatureClass = featureClass
            };
        }

        /// <summary>
        /// 从image路径打开rasterdataset
        /// </summary>
        /// <param name="imgFile"></param>
        /// <returns></returns>
        public static IRasterLayer OpenRasterDataset(string imgFile)
        {
            string imgPath = System.IO.Path.GetDirectoryName(imgFile);
            string imgName = System.IO.Path.GetFileName(imgFile);

            // 打开影像
            IWorkspaceFactory rWkspFactory = new RasterWorkspaceFactoryClass();
            IWorkspace rWksp = rWkspFactory.OpenFromFile(imgPath, 0);
            IRasterWorkspace2 rasterWksp = rWksp as IRasterWorkspace2;
            IRasterDataset rasterDs = rasterWksp.OpenRasterDataset(imgName);
            IRasterLayer pRasterLy = new RasterLayerClass();
            pRasterLy.CreateFromDataset(rasterDs);
            return pRasterLy;
        }

        /// <summary>
        /// 获取所有图层
        /// </summary>
        /// <param name="mapControl"></param>
        /// <returns></returns>
        public static IEnumerable<ILayer> GetAllLayers(AxMapControl mapControl)
        {
            for (int i = 0; i < mapControl.LayerCount; i++)
            {
                yield return mapControl.get_Layer(i);
            }
        }

        /// <summary>
        /// 获取图层索引
        /// </summary>
        /// <param name="mapControl"></param>
        /// <param name="layerName"></param>
        /// <returns></returns>
        public static int GetLayerIndex(AxMapControl mapControl, string layerName)
        {
            int layerIndex = -1;
            for (int i = 0; i < mapControl.LayerCount; i++)
            {
                ILayer layer = mapControl.get_Layer(i);
                if (layer.Name == layerName)
                {
                    layerIndex = i;
                    break;
                }
            }
            return layerIndex;
        }

        /// <summary>
        /// 根据名称获取图层
        /// </summary>
        /// <param name="mapControl"></param>
        /// <param name="layerName"></param>
        /// <returns></returns>
        public static ILayer GetLayer(AxMapControl mapControl, string layerName)
        {
            for (int i = 0; i < mapControl.LayerCount; i++)
            {
                ILayer layer = mapControl.get_Layer(i);
                if (layer.Name == layerName)
                {
                    return layer;
                }
            }
            return null; ;
        }

        /// <summary>
        /// 获取多边形要素
        /// </summary>
        /// <param name="feature"></param>
        /// <returns></returns>
        public static IPolygon ConverterToPolygon(IFeature feature)
        {
            return feature.Shape as IPolygon;
        }

        /// <summary>
        /// 获取要素Id
        /// </summary>
        /// <param name="feature"></param>
        /// <returns></returns>
        public static int GetFeatureId(IFeature feature)
        {
            return feature.HasOID ? feature.OID : Convert.ToInt32(feature.Value[0]);
        }

        /// <summary>
        /// 获取多边形要素中的坐标串
        /// </summary>
        /// <param name="poly"></param>
        /// <returns></returns>
        public static IEnumerable<Coordinates> GetPoints(IPolygon poly)
        {
            IPointCollection ptCollection = poly as IPointCollection;
            int ptCount = ptCollection.PointCount;
            for (int i = 0; i < ptCount; i++)
            {
                var pt = ptCollection.get_Point(i);
                yield return new Coordinates
                {
                    Latitude = pt.Y,
                    Longitude = pt.X
                };
            }
        }

        /// <summary>
        /// 获取多边形要素中的坐标串
        /// </summary>
        /// <param name="feature"></param>
        /// <returns></returns>
        public static IEnumerable<Coordinates> GetPoints(IFeature feature)
        {
            if (feature.Shape is IPolygon)
            {
                IPolygon polygon = feature.Shape as IPolygon;
                return GetPoints(polygon);
            }

            return null;
        }
    }
}
