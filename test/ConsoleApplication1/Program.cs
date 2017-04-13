using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Semantic.Engine;
using OSGeo.GDAL;
using OSGeo.OGR;
using OSGeo.OSR;
using System.Runtime.InteropServices;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            string inputpath = "";
            string resultpath = resultFile;
            OSGeo.GDAL.Gdal.AllRegister();
            OSGeo.GDAL.Gdal.SetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
            OSGeo.GDAL.Gdal.SetConfigOption("SHAPE_ENCODING", "");
            OSGeo.GDAL.Dataset img = Gdal.Open(inputpath, Access.GA_ReadOnly);
            int width = img.RasterXSize;
            int height = img.RasterYSize;
            int nBandCount = img.RasterCount;
            int L = width * height;
            double[] adfGeotransform = new double[6];
            string adfgeostr = "";
            img.GetGeoTransform(adfGeotransform);

            //将投影参数写入字符串
            for (int i = 0; i < 6; i++)
            {
                adfgeostr = adfgeostr + adfGeotransform[i].ToString() + ",";
            }
            adfgeostr = adfgeostr.Substring(0, adfgeostr.Length - 1);
            string proj = img.GetProjectionRef();
            Byte[] imgdate = new Byte[L * nBandCount];
            int[] label = new int[L];
            int[] outlabel = new int[L * 3];
            int[] bandmap = new int[3] { 3, 2, 1 };
            img.ReadRaster(0, 0, width, height, imgdate, width, height, nBandCount, bandmap, 3, width * nBandCount, 1);
            img.Dispose();
            
            GCHandle hObject = GCHandle.Alloc(imgdate, GCHandleType.Pinned);
            IntPtr pobject = hObject.AddrOfPinnedObject();
            int[] bandr = { 20, 125 };
            int[] bandg = { 10, 258 };
            int[] bandb = { 1, 254 };
            SegmentExecute qt = new SegmentWrapper();
            qt.SetParam(width, height, nBandCount, proj, adfGeotransform, bandr, bandg, bandb);
            qt.MeanShift(pobject, ref label, 7, 6.5F, 200);
            int index = 0;
            foreach (int lavelValue in label)
            {
                outlabel[index] = lavelValue;
                index++;
            }
            //qt.MultiSegment(pobject, ref outlabel, 3,50);
            string inputfile = "F:\\bigdata6000.tif";
            string outpuffile = "D:\\temp3";
            //qt.SeamLessCon(inputfile,outpuffile,1500,200);
            //SuperPixels.CSuperPixels sp = new SuperPixels.CSuperPixels();
            //sp.getSuperPixels(imgdate,ref label, parameter, 200, 15.0);           
            //创建一个内存影像
            int pixeloffset = sizeof(Int32);
            OSGeo.GDAL.Driver drMemory = Gdal.GetDriverByName("MEM");
            Dataset dsMemory = drMemory.Create("", width, height, 0, DataType.GDT_Int32, null);
            dsMemory.SetProjection(proj);
            dsMemory.SetGeoTransform(adfGeotransform);
            IntPtr buff = Marshal.AllocCoTaskMem(width * height * sizeof(int));
            System.Runtime.InteropServices.Marshal.Copy(label, 0, buff, width * height);

            string[] amemstring = {
                                      "DATAPOINTER=" + Convert.ToString(buff.ToInt64()),
                                      "PIXELOFFSET=" + pixeloffset,
                                      "LINEOFFSET=" + width * pixeloffset
                                  };
            dsMemory.AddBand(DataType.GDT_Int32, amemstring);
            int[] labelNew = new int[width * height];

            //OSGeo.GDAL.Driver drv = Gdal.GetDriverByName("GTiff");
            //drv.CreateCopy("abc.tif", dsMemory, 0, null, null, null);

            dsMemory.ReadRaster(0, 0, width, height, labelNew, width, height, 1, null, 0, 0, 0);

            Ogr.RegisterAll();
            string strDriverName = "ESRI Shapefile";
            OSGeo.OGR.Driver oDriver = OSGeo.OGR.Ogr.GetDriverByName(strDriverName);
            OSGeo.OGR.DataSource oDS = oDriver.CreateDataSource(@"E:\testpolygon.shp", null);
            OSGeo.OSR.SpatialReference spatial = new OSGeo.OSR.SpatialReference(proj);
            OSGeo.OGR.Layer oLayer = oDS.CreateLayer("testpolygon", spatial, OSGeo.OGR.wkbGeometryType.wkbPolygon, null);
            FieldDefn oFiledID = new FieldDefn("Label", FieldType.OFTInteger);
            oLayer.CreateField(oFiledID, 1);
            Band srcBand = dsMemory.GetRasterBand(1);
            string[] papszOptions = { "8CONNECTED=8" };
            Gdal.Polygonize(srcBand, null, oLayer, 0, papszOptions, null, null);
            oDS.Dispose();
            imgdate = null;
            label = null;
            hObject.Free();
            Marshal.FreeCoTaskMem(buff);
            //GC.Collect();
            MessageBoxButtons messbutton = MessageBoxButtons.OK;
            DialogResult success = MessageBox.Show("分割完成", "退出分割", messbutton);
            if (success == DialogResult.OK)
            {
                return;
            }
        }
    }
}
