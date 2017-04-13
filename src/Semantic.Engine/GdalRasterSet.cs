using OSGeo.GDAL;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    /// <summary>
    /// Gdal影像数据集
    /// </summary>
    public class GdalRasterSet : IDisposable
    {
        private double[] _transform = new double[6];

        public string Path { get; private set; }
        public Dataset RasterDs { get; private set; }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="path"></param>
        public GdalRasterSet(string path)
        {
            this.Path = path;
            this.RasterDs = OpenRaster(path);
            this.RasterDs.GetGeoTransform(this._transform);
        }

        /// <summary>
        /// 打开影像数据集
        /// </summary>
        /// <param name="imageFile">影像路径</param>
        /// <returns></returns>
        private Dataset OpenRaster(string imageFile)
        {
            // GdalConfiguration.ConfigureGdal();
            Gdal.AllRegister();
            Gdal.SetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
            return Gdal.Open(imageFile, Access.GA_ReadOnly);
        }


        /// <summary>
        /// 地理坐标转影像坐标
        /// </summary>
        /// <param name="coords"></param>
        /// <returns></returns>
        public ImagePoint ConvertToImageCoords(Coordinates coords)
        {
            double offset = this._transform[1] * this._transform[5] -
                this._transform[2] * this._transform[4];
            return new ImagePoint
            {
                Column = Convert.ToInt32(
                    (this._transform[5] * (coords.Longitude - this._transform[0]) -
                    this._transform[2] * (coords.Latitude - this._transform[3])) / offset + 0.5),
                Row = Convert.ToInt32(
                    (this._transform[1] * (coords.Latitude - this._transform[3]) -
                    this._transform[4] * (coords.Longitude - this._transform[0])) / offset + 0.5)
            };
        }

        /// <summary>
        /// 地理坐标串转影像坐标串
        /// </summary>
        /// <param name="coords">坐标串</param>
        /// <returns></returns>
        public IEnumerable<ImagePoint> ConvertToImageCoords(IEnumerable<Coordinates> coords)
        {
            foreach(var coord in coords)
            {
                yield return ConvertToImageCoords(coord);
            }
        }

        public void Dispose()
        {
            if (this.RasterDs != null)
            {
                this.RasterDs.Dispose();
                this.RasterDs = null;
            }
        }
    }
}
