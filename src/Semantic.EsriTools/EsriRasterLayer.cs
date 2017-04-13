using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.DataSourcesRaster;
using ESRI.ArcGIS.Geodatabase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.EsriTools
{
    public class EsriRasterLayer
    {
        public string Path { get; private set; }
        public IRasterLayer RasterLayer { get; private set; }

        public IRasterDataset RasterDataSet
        {
            get
            {
                return ((IRaster2)this.RasterLayer.Raster).RasterDataset;
            }
        }

        public IRasterBandCollection DispBandCollection
        {
            get
            {
                return this.RasterLayer.Raster as IRasterBandCollection;
            }
        }

        public IRasterBandCollection BandCollection
        {
            get
            {
                return this.RasterDataSet as IRasterBandCollection;
            }
        }

        public EsriRasterLayer()
        { }

        public EsriRasterLayer(string path)
        {
            this.Path = path;
            this.RasterLayer = new RasterLayer();
            this.RasterLayer.CreateFromFilePath(path);
        }

        public EsriRasterLayer(IRasterLayer layer)
        {
            this.RasterLayer = layer;
        }

        public void SetStyle(string redLabel, string greenLabel, string blueLabel)
        {
            IRasterRenderer rasterRender = new RasterRGBRendererClass();
            rasterRender.Raster = this.RasterLayer.Raster;
            IRasterRGBRenderer rasterRGBRender = rasterRender as IRasterRGBRenderer;
            rasterRender.Update();
            rasterRGBRender.UseBlueBand = true;
            rasterRGBRender.UseGreenBand = true;
            rasterRGBRender.UseRedBand = true;
            rasterRGBRender.RedBandIndex = BandCollection.get_BandIndex(redLabel);
            rasterRGBRender.GreenBandIndex = BandCollection.get_BandIndex(greenLabel);
            rasterRGBRender.BlueBandIndex = BandCollection.get_BandIndex(blueLabel);
            IRasterProps rProps = this.RasterLayer.Raster as IRasterProps;
            rProps.NoDataValue = 0;
            this.RasterLayer.Renderer = rasterRGBRender as IRasterRenderer;
        }
    }
}
