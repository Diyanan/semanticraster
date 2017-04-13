using ChangeDetectWrapper;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public class ClassifyCalculator : ICalculator
    {
        public ClassifyParam Param { get; set; }

        public IEnumerable<SampleEntry> Samples { get; set; }
        public List<string> GeoClassList { get; set; }

        public OgrLayer OgrLayer { get; private set; }

        public static string ClassFieldName = "Zone";

        public event EventHandler OnProgressChanged;

        public ClassifyCalculator(ClassifyParam param, IEnumerable<SampleEntry> samples)
        {
            this.Param = param;
            this.Samples = samples;
            this.GeoClassList = samples.Select(r => r.GeoClass).Distinct().ToList();
            this.OgrLayer = new OgrLayer(param.VectorLayer.DataSource, true);
        }

        /// <summary>
        /// 获取需要的属性字段值的列表
        /// </summary>
        /// <param name="feature"></param>
        /// <returns></returns>
        private List<double> GetFieldValues(OSGeo.OGR.Feature feature)
        {
            var fieldVals = new List<double>();
            foreach (var fld in this.Param.SelFields)
            {
                fieldVals.Add(feature.GetFieldAsDouble(fld));
            }
            // 测试集类型为0
            // fieldVals.Add(0);
            return fieldVals;
        }

        /// <summary>
        /// 生成训练集文件
        /// </summary>
        /// <param name="trainFile"></param>
        private void PrepareTrainFile(string trainFile)
        {
            using (var fs = File.CreateText(trainFile))
            {
                foreach (var s in this.Samples)
                {
                    var f = this.OgrLayer.GetFeatureById(s.OID);
                    var values = GetFieldValues(f);
                    // 类别编码
                    var code = this.GeoClassList.IndexOf(s.GeoClass);
                    values.Add(code);
                    var line = string.Join(",", values);
                    fs.WriteLine(line);
                }
            }
        }

        /// <summary>
        /// 生成测试集文件
        /// </summary>
        /// <param name="testFile"></param>
        public void PrepareTestFile(string testFile)
        {
            using (var fs = File.CreateText(testFile))
            {
                foreach (var f in this.OgrLayer.GetAllFeatures())
                {
                    var vals = GetFieldValues(f);
                    var code = -1;
                    vals.Add(code);
                    var line = string.Join(",", vals);
                    fs.WriteLine(line);
                }
            }
        }

        private static List<int> ParseResult(string outFile)
        {
            List<int> result = new List<int>();
            var fs = File.OpenRead(outFile);
            var sr = new StreamReader(fs, Encoding.Default);
            string line = null;
            while ((line = sr.ReadLine()) != null)
            {
                string strCode = line.Trim();
                int code = int.Parse(strCode);
                result.Add(code);
            }
            sr.Close();
            fs.Close();
            return result;
        }

        private void UpdateLayer(List<int> result)
        {
            for (int i = 0; i < result.Count; i++)
            {
                OSGeo.OGR.Feature feature = this.OgrLayer.GetFeatureById(i);
                // 解码
                var clsName = this.GeoClassList[result[i]];
                feature.SetField(ClassFieldName, clsName);
                this.OgrLayer.UpdateFeature(feature);
            }
        }

        protected virtual void AddClassField(string fieldName)
        {
            this.OgrLayer.CreateField(fieldName, OSGeo.OGR.FieldType.OFTString);
        }

        public void Process()
        {
            AddClassField(ClassFieldName);
            string root = AppDomain.CurrentDomain.BaseDirectory + "sample\\";
            if (!Directory.Exists(root))
            {
                Directory.CreateDirectory(root);
            }

            // 构造算法输入参数
            string trainFile = root + "train.txt";
            string testFile = root + "test.txt";
            string outFile = root + "out.txt";
            // 生成测试集
            // PrepareTestFile(testFile);

            // 生成训练集
            // PrepareTrainFile(trainFile);

            // 执行分类
            CDTWrapper cdt = new CDTWrapper();
            cdt.Classifier(trainFile, testFile, outFile, this.Param.Algorithm);

            // 结果处理
            //List<int> col = ParseResult(outFile);
            //UpdateLayer(col);
        }

        public static IEnumerable<string> GetClassResult(IEnumerable<SampleEntry> samples)
        {
            string root = AppDomain.CurrentDomain.BaseDirectory + "sample\\";
            string outFile = root + "out.txt";
            // 结果处理
            List<int> col = ParseResult(outFile);
            var geoClassList = samples.Select(r => r.GeoClass).Distinct().ToList();
            foreach (var c in col)
            {
                var res = c >= geoClassList.Count ? "" : geoClassList[c];
                yield return res;
            }
        }
    }
}
