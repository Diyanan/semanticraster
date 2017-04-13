using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using VerifyFeatureSetWrapper;
using System.Linq;
using OSGeo.OGR;
using Semantic.Configure;
using System.IO;

namespace Semantic.Engine
{
    public class FeatureCalculator : ICalculator
    {
        public FeatureParam Param { get; set; }

        // 结果集合
        public static ResultSet resultSet = new ResultSet();

        public event EventHandler OnProgressChanged;

        public OgrLayer OgrLayer { get; private set; }

        public GdalRasterSet RasterSet { get; private set; }

        public FeatureExport FeatureExport { get; private set; }

        public FeatureCalculator(FeatureParam param)
        {
            this.Param = param;
            this.OgrLayer = new OgrLayer(param.VectorLayer.DataSource, true);
            this.RasterSet = new GdalRasterSet(param.ImageLayer.DataSource);
            this.Param.CsvFile = GetCsvPath(param.VectorLayer.DataSource, param.UseDefaultField);
            this.FeatureExport = new FeatureExport(this.Param.CsvFile);
            this.FeatureExport.Initialize(param.CalFeatures, param.BandCount);
        }

        private string GetCsvPath(string layerPath, bool useDefaultField)
        {
            if (useDefaultField)
            {
                return Path.ChangeExtension(layerPath, ".csv");
            }

            return string.Format("{0}\\{1}1.csv", 
                Path.GetDirectoryName(layerPath), 
                Path.GetFileNameWithoutExtension(layerPath));
        }

        protected virtual void AddDoubleField(string fieldName)
        {
            this.OgrLayer.CreateDoubleField(fieldName);
        }

        private void VerifyAlgorithmCallBackProc(VerifyResultWrapper verifyResult)
        {
            if (verifyResult.resultValid)
            {
                resultSet.UpdateOrAdd(verifyResult);
            }
        }

        private unsafe IntPtr WrapBandIndex(List<int> bandIndex)
        {
            // 从List<int>构建环数组
            int unitSize = Marshal.SizeOf(typeof(int));
            IntPtr result = Marshal.AllocCoTaskMem(bandIndex.Count * unitSize);
            IntPtr pRing = result;
            for (int i = 0; i < bandIndex.Count; ++i)
            {
                *(int*)(void*)pRing = bandIndex[i];
                pRing += unitSize;
            }
            return result;
        }

        private unsafe IntPtr WrapGeoPoints(List<ImagePoint> pts)
        {
            int ptCount = pts.Count;
            int vertexSize = Marshal.SizeOf(typeof(WrapPoint));
            IntPtr result = Marshal.AllocCoTaskMem(ptCount * vertexSize);
            IntPtr pVertex = result;

            foreach (ImagePoint point in pts)
            {
                WrapPoint* pVertexElement = (WrapPoint*)(void*)pVertex;
                pVertexElement->x = point.Column;
                pVertexElement->y = point.Row;
                pVertex += vertexSize;
            }
            return result;
        }

        private void VerifySampleFeature(int featureID, List<ImagePoint> pixCoords)
        {
            FeatureCalculatorWrapper calculator = null;
            IntPtr wrapCoords = IntPtr.Zero;
            // fixme:临时方案，添加try...catch，后续应着手解决算法bug
            try
            {
                wrapCoords = WrapGeoPoints(pixCoords);
                // 初始化计算器
                calculator = new FeatureCalculatorWrapper(VerifyAlgorithmCallBackProc);
                calculator.Init(this.Param.ImageLayer.DataSource, featureID, pixCoords.Count, wrapCoords);
                // 计算
                foreach (var imageFeature in this.Param.CalFeatures)
                {
                    List<int> bandIndex = new List<int>();
                    if (imageFeature.NeedBandNames.Count <= 1)
                    {
                        for (int j = 0; j < this.Param.BandCount; j++)
                        {
                            bandIndex.Add(j + 1);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < imageFeature.NeedBandNames.Count; j++)
                        {
                            bandIndex.Add(this.Param.BandsIndex[imageFeature.NeedBandNames[j]]);
                        }
                    }

                    // 构造当前特征的波段索引
                    IntPtr wrapBands = WrapBandIndex(bandIndex);
                    calculator.Execute(bandIndex.Count, wrapBands, imageFeature.AlgorithmName);
                    Marshal.FreeCoTaskMem(wrapBands);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("初始化特征计算失败！FID {0}要素无法计算", featureID);
            }
            finally
            {
                Marshal.FreeCoTaskMem(wrapCoords);
                if (calculator != null)
                {
                    calculator.Dispose();
                    calculator = null;
                }
            }
        }

        /// <summary>
        /// 处理步骤：
        /// 1、设置参数：
        /// 包括矢量和影像文件、待计算特征列表、是否覆盖、波段组合映射等
        /// 2、根据待计算特征列表和是否覆盖参数，创建字段（**）
        /// 3、获取所有要素（**）
        /// 4、分段
        /// 5、并行
        /// 6、要素地理坐标转影像坐标
        /// 7、计算
        /// 8、缓存写入文件
        /// 9、重复5
        /// </summary>
        public void DoAnalysis(int count, IEnumerable<Tuple<int, IEnumerable<ImagePoint>>> features)
        {
            // 统计操作时间
            long totalAnalysisTime = 0;
            long totalUpdateTime = 0;
            Stopwatch timer = new Stopwatch();
            timer.Start();

            var cultureInfo = new System.Globalization.CultureInfo("en-US");
            int i = 1;
            var blockArray = features.Partition(5000);
            foreach (var block in blockArray)
            {
                // 清理缓存
                resultSet.Reset();

                Parallel.ForEach(block, new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount },
                    (Action<Tuple<int, IEnumerable<ImagePoint>>>)(tuple =>
                    {
                        Thread.CurrentThread.CurrentCulture = cultureInfo;
                        int featureID = tuple.Item1;
                        var coords = tuple.Item2;
                        Console.WriteLine("The {0} parallel execution start:", featureID);

                        //计算特征值
                        long analysisStartTime = timer.ElapsedMilliseconds;


                        VerifySampleFeature(featureID, coords.ToList());

                        long analysisTime = timer.ElapsedMilliseconds - analysisStartTime;
                        Console.WriteLine("{0}计算特征用时：{1} ms", featureID, analysisTime);
                        totalAnalysisTime += analysisTime;

                        // 报告进度
                        if(this.OnProgressChanged != null)
                        {
                            this.OnProgressChanged(this, new ProgressEventArgs(count, i, CancellationToken.None));
                        }

                        Interlocked.Increment(ref i);
                    }));

                long flushStartTime = timer.ElapsedMilliseconds;

                this.FeatureExport.ExportToFile(resultSet.GetAllResult());

                long flushTime = timer.ElapsedMilliseconds - flushStartTime;

                totalUpdateTime += flushTime;
            }

            Console.WriteLine("计算特征总时间：{0}", ConverterHelper.ConvertTimeUnit(totalAnalysisTime));
            Console.WriteLine("更新属性总时间：{0}", ConverterHelper.ConvertTimeUnit(totalUpdateTime));
            Console.WriteLine("总用时：{0}", ConverterHelper.ConvertTimeUnit(timer.ElapsedMilliseconds));

            // 显示完成统计消息提示
            StringBuilder sb = new StringBuilder();
            sb.AppendFormat("计算特征总时间：{0}", ConverterHelper.ConvertTimeUnit(totalAnalysisTime));
            sb.AppendFormat("更新属性总时间：{0}", ConverterHelper.ConvertTimeUnit(totalUpdateTime));
            sb.AppendFormat("总用时：{0}", ConverterHelper.ConvertTimeUnit(timer.ElapsedMilliseconds));
            // MessageBox.Show(sb.ToString());
        }

        private List<ImagePoint> GetImagePoints(OSGeo.OGR.Feature feature)
        {
            var result = new List<ImagePoint>();

            // 读取polygon
            Geometry polygon = null;
            var geometry = feature.GetGeometryRef();
            var polygonType = geometry.GetGeometryType();
            if (polygonType == wkbGeometryType.wkbPolygon)
            {
                polygon = geometry;
            }
            if (polygonType == wkbGeometryType.wkbMultiPolygon)
            {
                geometry.CloseRings();
                polygon = geometry.GetGeometryRef(0);
            }

            // 读取外环
            Geometry outRing = null;
            var boundary = polygon.GetBoundary();
            var lineType = boundary.GetGeometryType();
            if (lineType == wkbGeometryType.wkbLineString)
            {
                outRing = boundary;
            }
            if (lineType == wkbGeometryType.wkbMultiLineString)
            {
                outRing = boundary.GetGeometryRef(0);
            }

            // 读取点
            for (int i = 0; i < outRing.GetPointCount(); i++)
            {
                result.Add(this.RasterSet.ConvertToImageCoords(new Coordinates
                {
                    Longitude = outRing.GetX(i),
                    Latitude = outRing.GetY(i) }
                ));
            }
            return result;
        }

        protected virtual IEnumerable<Tuple<int, IEnumerable<ImagePoint>>> GetFeatures()
        {
            Feature feature = null;
            while ((feature = this.OgrLayer.Layer.GetNextFeature()) != null)
            {
                int featureId = feature.GetFID();
                var imagePoints = GetImagePoints(feature);
                yield return new Tuple<int, IEnumerable<ImagePoint>>(featureId, imagePoints);
            }
        }

        public void Process()
        {
            var count = this.OgrLayer.Layer.GetFeatureCount(0);
            var allFeatures = GetFeatures();
            this.DoAnalysis(count, allFeatures);
            this.FeatureExport.Dispose();
        }
    }
}
