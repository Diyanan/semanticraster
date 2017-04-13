using System;
using System.Windows.Forms;
using OSGeo.GDAL;
using OSGeo.OGR;
using segment_csharp;
using System.Runtime.InteropServices;
using System.IO;

namespace Semantic.Engine
{
    public class SegmentCaculator : ICalculator
    {
        public SegmentParam Param { get; set; }
        private string ImgPath = "";
        private string SegShpPath = "";
        private int width,height,nBandCount,xNum,yNum,Block,Buffer;
        private double[] adfGeoTransForm = new double[6];
        private int[] stretchR = new int[2];
        private int[] strecthG = new int[2];
        private int[] strecthB = new int[2];
        private string proj;
        private PatchLoc[] patchloc;
        private byte[] ImgData;
        private int[] Labels;

        public event EventHandler OnProgressChanged;

        public SegmentCaculator(SegmentParam param)
        {
            this.Param = param;
        }
        public void Process()
        {
            DoSegment();
        }
        public void DoSegment()
        {
            ImgPath = Param.RasterFileName;
            SegShpPath = Path.GetDirectoryName(ImgPath) +"\\"+ Path.GetFileNameWithoutExtension(ImgPath) + "_Region.shp";
            //存在就删除
            if (File.Exists(SegShpPath))
            {
                File.Delete(SegShpPath);
                File.Delete(SegShpPath.Substring(0,SegShpPath.Length-4)+".prj");
                File.Delete(SegShpPath.Substring(0,SegShpPath.Length-4) + ".dbf");
                File.Delete(SegShpPath.Substring(0,SegShpPath.Length-4) + ".shx");
            }

            GetParam(ImgPath,1,0,0.02);
            GCHandle ImgObject = GCHandle.Alloc(ImgData, GCHandleType.Pinned);
            IntPtr ImgPtr = ImgObject.AddrOfPinnedObject();
            SegmentWrapper segmentExe = new SegmentWrapper();
            segmentExe.SetParam(width, height, nBandCount, proj, adfGeoTransForm, strecthB, strecthG, stretchR);
            switch (Param.Algorithm)
            {
                case 0:                    
                   // segmentExe.MultiSegment(ImgPtr, ref Labels, Param.ParaMultiSegScaleNum, Param.ParaMultiSegHdiff);
                    break;
                case 1:
                    segmentExe.MeanShift(ImgPtr,ref Labels, Param.ParaMSSigmaS, Param.ParaMSSigmaR, Param.ParaMSminsize);
                    break;
                case 2:
                    segmentExe.GraphCut(ImgPtr, ref Labels, Param.ParaGraphCutSigma, Param.ParaGraphCutThreshold, Param.ParaGraphCutMinsize);
                    break;
                case 3:
                    segmentExe.WaterShed(ImgPtr, ref Labels, Param.ParaWSminsize);
                    break;
                case 4:
                    segmentExe.QuadTree(ImgPtr, ref Labels, Param.ParaQTwc, Param.ParaQTwp, Param.ParaQThdiff, Param.ParaQTMinsize);
                    break;
                case 5:
                    segmentExe.SuperPixel(ImgPtr, ref Labels, Param.ParaSPcount, Param.ParaSPcompactness);
                    break;
                case 6:
                    segmentExe.Jseg(ImgPtr, ref Labels, Param.ParaJsegTquan, Param.ParaJsegThreshold, Param.ParaJsegMinsize);
                    break;
            }
            Ogr.RegisterAll();
            Gdal.SetConfigOption("SHAP_ENCODING", "");
            string strDriverName = "ESRI Shapefile";
            OSGeo.OGR.Driver oDriver = Ogr.GetDriverByName(strDriverName);
            DataSource oDS = oDriver.CreateDataSource(SegShpPath, null);
            OSGeo.OSR.SpatialReference spatial = new OSGeo.OSR.SpatialReference(proj);
            string shpName = Path.GetFileNameWithoutExtension(SegShpPath);
            Layer poLayer = oDS.CreateLayer(shpName, spatial, wkbGeometryType.wkbPolygon, null);
            FieldDefn oFiledID = new FieldDefn("Label", FieldType.OFTInteger);
            poLayer.CreateField(oFiledID, 1);
            FieldDefn oFiledClass = new FieldDefn("Zone", FieldType.OFTString);
            poLayer.CreateField(oFiledClass, 1);
            GetLayer(poLayer, Labels, width, height, proj, adfGeoTransForm);
            ImgObject.Free();
            oDS.Dispose();           
        }
        /// <summary>
        /// 获得影像信息
        /// </summary>
        /// <param name="imgPath">影像路径</param>
        /// <param name="block">块大小</param>
        /// <param name="buffer">缓冲区大小</param>
        /// <param name="StrecthRatio">拉伸比</param>
        public void GetParam(string imgPath,int block,int buffer,double StrecthRatio)
        {
            Gdal.AllRegister();
            Gdal.SetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
            Dataset imgset = Gdal.Open(imgPath, Access.GA_ReadOnly);
            width = imgset.RasterXSize;
            height = imgset.RasterYSize;
            nBandCount = imgset.RasterCount;
            Labels = new int[width * height];
            imgset.GetGeoTransform(adfGeoTransForm);
            proj = imgset.GetProjectionRef();
            //int[] bandmap = new int[3] { 3, 2, 1 };
            ImgData = new byte[width * height * 3];
            Int32[] ImgDataint = new Int32[width * height * nBandCount];         
            
            if (nBandCount > 3)
            {
                //根据百分比计算最大最小值
                GetMinMaxValue(imgset, StrecthRatio);
                imgset.ReadRaster(0, 0, width, height, ImgDataint, width, height, nBandCount, null,
                    nBandCount*sizeof(Int32), width * nBandCount*sizeof(Int32), sizeof(Int32));
                StretchIntToByte(ImgDataint,ref ImgData);
            }
            else
                imgset.ReadRaster(0, 0, width, height, ImgData, width, height, nBandCount, null,
                   nBandCount , width * nBandCount , 1);
            Block = block;
            Buffer = buffer;
            xNum = (width - 1) / block + 1;
            yNum = (height - 1) / block + 1;
            //计算块的起始位置
            GetPatchLoc();
            imgset.FlushCache();
        }
        /// <summary>
        /// 获得块的起始位置
        /// </summary>
        /// <returns></returns>
        public bool GetPatchLoc()
        {
            if(((width-(xNum-1)*Block)<Buffer)||((height-(yNum-1)*Block)<Buffer))
            {
                MessageBoxButtons error = MessageBoxButtons.OK;
                DialogResult errorMessage = MessageBox.Show("请重新设置块大小或缓冲区", "退出设置参数", error);
                return false;
            }
            patchloc = new PatchLoc[xNum * yNum];
            for (int i = 0; i < yNum; i++)
            {
                for (int j = 0; j < xNum; j++)
                {
                    if (i < yNum - 1)
                    {
                        if (j < xNum - 1)
                        {
                            patchloc[i * xNum + j].BlockPosX = Block * j;
                            patchloc[i * xNum + j].BlockPosY = Block * i;
                            patchloc[i * xNum + j].BlockXsize = Block + Buffer;
                            patchloc[i * xNum + j].BlockYsize = Block + Buffer;
                        }
                        else
                            if (j == xNum - 1)
                        {
                            patchloc[i * xNum + j].BlockPosX = Block * j;
                            patchloc[i * xNum + j].BlockPosY = Block * i;
                            patchloc[i * xNum + j].BlockXsize = width - (xNum - 1) * Block;
                            patchloc[i * xNum + j].BlockYsize = Block + Buffer;
                        }
                    }
                    else
                        if (i == yNum - 1)
                    {
                        if (j < xNum - 1)
                        {
                            patchloc[i * xNum + j].BlockPosX = Block * j;
                            patchloc[i * xNum + j].BlockPosY = Block * i;
                            patchloc[i * xNum + j].BlockXsize = Block + Buffer;
                            patchloc[i * xNum + j].BlockYsize = height - (yNum - 1) * Block;
                        }
                        else
                            if (j == xNum - 1)
                        {
                            patchloc[i * xNum + j].BlockPosX = Block * j;
                            patchloc[i * xNum + j].BlockPosY = Block * i;
                            patchloc[i * xNum + j].BlockXsize = width - (xNum - 1) * Block;
                            patchloc[i * xNum + j].BlockYsize = height - (yNum - 1) * Block;
                        }
                    }
                }
            }
            return true;
        }
        /// <summary>
        /// 获得拉伸比下最大最小值
        /// </summary>
        /// <param name="imgset"></param>
        /// <param name="strecthRatio"></param>
        private void GetMinMaxValue(Dataset imgset,double strecthRatio)
        {            
            double StretchRatio = strecthRatio;
            int width = imgset.RasterXSize;
            int heitht = imgset.RasterYSize;
            // int nBandCount = intputImg.RasterCount;
            double minR, maxR, minG, maxG, minB, maxB;
            minB = maxB = minG = maxG = minR = maxR = 0;
            Band bandB = imgset.GetRasterBand(1);
            Band bandG = imgset.GetRasterBand(2);
            Band bandR = imgset.GetRasterBand(3);
            double[] maxminB = { 0, 0 };
            double[] maxminG = { 0, 0 };
            double[] maxminR = { 0, 0 };

            bandB.ComputeRasterMinMax(maxminB,1);
            bandG.ComputeRasterMinMax(maxminG,1);
            bandR.ComputeRasterMinMax(maxminR,1);
            int buckets = (int)(maxminB[1] - maxminB[0] + 1);
            int imgSize = width * heitht;
            int[] pHistogramB = new int[buckets];
            double ratio = 0.0;
            //计算B波段的最大最小值
            bandB.GetHistogram(maxminB[0] - 0.5, maxminB[1] + 0.5, buckets, pHistogramB, 1, 0, null, null);
            if (maxminB[0] == 0)
            {
                imgSize -= pHistogramB[0];//除去背景值个数
            }
            for (int i = 0; i < buckets; i++)
            {
                if (i == 0 && maxminB[0] == 0)
                    continue;
                ratio += pHistogramB[i] / (double)imgSize;
                if (minB == 0 && ratio >= StretchRatio)
                {
                    minB = i + maxminB[0];
                }
                if (maxB == 0 && ratio >= (1 - StretchRatio))
                {
                    maxB = i + maxminB[0];
                    break;
                }
            }
            imgSize = width * heitht;
            //计算G波段最大最小值
            ratio = 0;
            buckets = (int)(maxminG[1] - maxminG[0] + 1);
            int[] pHistogramG = new int[buckets];
            bandG.GetHistogram(maxminG[0] - 0.5, maxminG[1] + 0.5, buckets, pHistogramG, 1, 0, null, null);
            if (maxminG[0] == 0)
            {
                imgSize -= pHistogramG[0];//除去背景值个数
            }
            for (int i = 0; i < buckets; i++)
            {
                if (i == 0 && maxminG[0] == 0)
                    continue;
                ratio += pHistogramG[i] / (double)imgSize;
                if (minG == 0 && ratio >= StretchRatio)
                {
                    minG = i + maxminG[0];
                }
                if (maxG == 0 && ratio >= (1 - StretchRatio))
                {
                    maxG = i + maxminG[0];
                    break;
                }
            }
            imgSize = width * heitht;
            //计算R波段的最大最小值
            ratio = 0;
            buckets = (int)(maxminR[1] - maxminR[0] + 1);
            int[] pHistogramR = new int[buckets];
            bandR.GetHistogram(maxminR[0] - 0.5, maxminR[1] + 0.5, buckets, pHistogramR, 1, 0, null, null);
            if (maxminR[0] == 0)
            {
                imgSize -= pHistogramR[0];
            }
            for (int i = 0; i < buckets; i++)
            {
                if (i == 0 && maxminR[0] == 0)
                    continue;
                ratio += pHistogramR[i] / (double)imgSize;
                if (minR == 0 && ratio >= StretchRatio)
                {
                    minR = i + maxminR[0];
                }
                if (maxR == 0 && ratio >= (1 - StretchRatio))
                {
                    maxR = i + maxminR[0];
                    break;
                }
            }
            strecthB[0] = (int)minB;
            strecthB[1] = (int)maxB;
            strecthG[0] = (int)minG;
            strecthG[1] = (int)maxG;
            stretchR[0] = (int)minR;
            stretchR[1] = (int)maxR;
        }
        private void GetLabelImg(string LabelPath, int[] Labels,int width,int height,string proj,double[] adfGeoTransform)
        {
            OSGeo.GDAL.Driver LabelSave = Gdal.GetDriverByName("GTiff");
            Dataset LabelImg = LabelSave.Create(LabelPath, width, height, 1, DataType.GDT_Int32, null);
            LabelImg.WriteRaster(0, 0, width, height, Labels, width, height, 1, null, 0, 0, 0);
            LabelImg.SetGeoTransform(adfGeoTransform);
            LabelImg.SetProjection(proj);
            LabelImg.FlushCache();
        }
        private void GetLayer(Layer poLayer,int[] labels,int width,int height,string proj,double[] adfGeoTransform)
        {
            Ogr.RegisterAll();
            Gdal.SetConfigOption("SHAP_ENCODING", "");
            int pixelOffset = sizeof(Int32);
            DataType datatype = DataType.GDT_Int32;
            OSGeo.GDAL.Driver drMemory = Gdal.GetDriverByName("MEM");

            Dataset dsMemory = drMemory.Create("", width, height, 0, datatype, null);
            dsMemory.SetProjection(proj);
            dsMemory.SetGeoTransform(adfGeoTransform);
            IntPtr buff = Marshal.AllocCoTaskMem(width * height * pixelOffset);
            System.Runtime.InteropServices.Marshal.Copy(labels, 0, buff, width * height);
            dsMemory.AddBand(datatype, new string[] { "DATAPOINTER=" + Convert.ToString(buff.ToInt64()), "PIXELOFFSET=" + pixelOffset, "LINEOFFSET=" + width * pixelOffset });
            Band srcBand = dsMemory.GetRasterBand(1);
            
            Dataset dstMemory = drMemory.Create("", width, height, 1, datatype, null);
            dstMemory.SetProjection(proj);
            dstMemory.SetGeoTransform(adfGeoTransform);
            Band dstBand = dstMemory.GetRasterBand(1);
            Gdal.SieveFilter(srcBand, null, dstBand, 10, 4, null, null, null);
            string[] papszOptions = { "8CONNECTED=8" };
            Gdal.Polygonize(dstBand, null, poLayer, 0, papszOptions, null, null);
            Marshal.FreeCoTaskMem(buff);
            dsMemory.Dispose();
            dstMemory.Dispose();
        }
        private void CreateLayer(string shpfileName,string proj,ref Layer poLayer)
        {
            Gdal.AllRegister();
            Ogr.RegisterAll();
            Gdal.SetConfigOption("SHAP_ENCODING", "");
            string strDriverName = "ESRI Shapefile";
            OSGeo.OGR.Driver oDriver = Ogr.GetDriverByName(strDriverName);
            DataSource oDS = oDriver.CreateDataSource(shpfileName, null);
            OSGeo.OSR.SpatialReference spatial = new OSGeo.OSR.SpatialReference(proj);
            string shpName = Path.GetFileNameWithoutExtension(shpfileName);
            poLayer = oDS.CreateLayer(shpName, spatial, wkbGeometryType.wkbPolygon, null);
            FieldDefn oFiledID = new FieldDefn("Label", FieldType.OFTInteger);
            poLayer.CreateField(oFiledID, 1);
            FieldDefn oFiledClass = new FieldDefn("Zone", FieldType.OFTString);
            poLayer.CreateField(oFiledClass, 1);
        }
        private void StretchIntToByte(Int32[] imgData,ref Byte[] data)
        {
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    int bval = (imgData[(j + i * width) * nBandCount]);
                    if (bval >=strecthB[0])
                    {
                        bval = (int)(255 * (bval - strecthB[0]) / (strecthB[1]-strecthB[0]));
                        if (bval > 255)
                            bval = 255;
                    }
                    else
                        bval = 0;
                    int gval = (imgData[(j + i * width) * nBandCount + 1]);
                    if (gval >= strecthG[0])
                    {
                        gval = (int)(255 * (gval - strecthG[0]) / (strecthG[1] - strecthG[0]));
                        if (gval > 255)
                            gval = 255;
                    }
                    else
                        gval = 0;

                    int rval = (imgData[(j + i * width) * nBandCount + 2]);
                    if (rval >= stretchR[0])
                    {
                        rval = (int)(255 * (rval - stretchR[0]) / (stretchR[1] - stretchR[0]));
                        if (rval > 255)
                            rval = 255;
                    }
                    else
                        rval = 0;
                    //以BIP方式输出图像数组
                    data[3 * (j + i * width) + 0] = (Byte)bval;
                    data[3 * (j + i * width) + 1] = (Byte)gval;
                    data[3 * (j + i * width) + 2] = (Byte)rval;
                }
            }
        }
       
    }   

    public struct PatchLoc
    {
       public int BlockPosX;
       public int BlockPosY;
       public int BlockXsize;
       public int BlockYsize;
    } 
}
