using Csv;
using Semantic.Configure;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public class ChangeCalculator : ICalculator
    {
        public event EventHandler OnProgressChanged;
        private ChangeParam param;

        public ChangeCalculator(ChangeParam param)
        {
            this.param = param;
        }

        private VectorPN ConvertCsvLine(ICsvLine line)
        {
            var vec = new VectorPN(line.ColumnCount - 1);
            vec.Label = Convert.ToInt32(line[0]);
            for (int i = 1; i < line.ColumnCount; i++)
            {
                try
                {
                    vec[i - 1] = Convert.ToDouble(line[i]);
                }
                catch
                {
                    return new VectorPN(1);
                }
            }
            return vec;
        }

        public double EulidSim(VectorPN inA, VectorPN inB)
        {
            return 1.0 / (1.0 + (inA - inB).Length());
        }

        public static double Pearson(IEnumerable<double> dataA, IEnumerable<double> dataB)
        {
            int n = 0;
            double r = 0.0;

            double meanA = 0;
            double meanB = 0;
            double varA = 0;
            double varB = 0;

            // WARNING: do not try to "optimize" by summing up products instead of using differences.
            // It would indeed be faster, but numerically much less robust if large mean + low variance.

            using (IEnumerator<double> ieA = dataA.GetEnumerator())
            using (IEnumerator<double> ieB = dataB.GetEnumerator())
            {
                while (ieA.MoveNext())
                {
                    ieB.MoveNext();

                    double currentA = ieA.Current;
                    double currentB = ieB.Current;

                    double deltaA = currentA - meanA;
                    double scaleDeltaA = deltaA / ++n;

                    double deltaB = currentB - meanB;
                    double scaleDeltaB = deltaB / n;

                    meanA += scaleDeltaA;
                    meanB += scaleDeltaB;

                    varA += scaleDeltaA * deltaA * (n - 1);
                    varB += scaleDeltaB * deltaB * (n - 1);
                    r += (deltaA * deltaB * (n - 1)) / n;
                }
            }

            return r / Math.Sqrt(varA * varB);
        }

        public double PearsSim(VectorPN inA, VectorPN inB)
        {
            return 0.5 + 0.5 * Pearson(inA.Data, inB.Data);
        }

        public double CosSim(VectorPN inA, VectorPN inB)
        {
            var num = inA.Dot(inB);
            var denom = inA.Length() * inB.Length();
            return 0.5 + 0.5 * (num / denom);
        }

        public void Process()
        {
            var f1 = param.PreFeatureFile;
            var f2 = param.PostFeatureFile;
            var c1 = param.ChangeFile;
            using (FileStream f1s = File.OpenRead(f1), f2s = File.OpenRead(f2))
            {
                var options = new CsvOptions
                {
                    HeaderMode = HeaderMode.HeaderPresent,
                    Separator = ',',
                    ValidateColumnCount = true
                };
                var preVectors = CsvReader.ReadFromStream(f1s, options).Select(line => ConvertCsvLine(line));
                var postVectors = CsvReader.ReadFromStream(f2s, options).Select(line => ConvertCsvLine(line));
                var l1 = preVectors.ToList();
                var l2 = postVectors.ToList();
                var csv = new ChangeExport(c1);
                csv.Initialize();
                var res = from preVec in l1
                          from postVec in l2
                          where preVec != null && postVec != null && preVec.Label == postVec.Label
                          select new
                          {
                              fid = preVec.Label,
                              dis = EulidSim(preVec, postVec),
                              pear = PearsSim(preVec, postVec),
                              ang = CosSim(preVec, postVec)
                          };

                // 报告进度
                if (this.OnProgressChanged != null)
                {
                    this.OnProgressChanged(this, new ProgressEventArgs(2, 1, CancellationToken.None));
                }

                foreach (var item in res)
                {
                    csv.ExportToFile(item.fid, item.dis, item.pear, item.ang);
                    Console.WriteLine("Processing " + item.fid);
                }
                csv.Dispose();

                // 报告进度
                if (this.OnProgressChanged != null)
                {
                    this.OnProgressChanged(this, new ProgressEventArgs(2, 2, CancellationToken.None));
                }

            }
        }
    }
}
