using Jitbit.Utils;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Semantic.Configure
{
    public class FeatureExport : IDisposable
    {
        private readonly string keyLabel = "fid";
        private bool disposed = false;
        private int bandCount;
        private List<ImageFeature> calFeatures;
        private string csvPath;
        private CsvExport csvExport;

        public FeatureExport(string csvPath)
        {
            this.csvPath = csvPath;
            this.csvExport = new CsvExport(File.CreateText(csvPath));
        }

        public void Initialize(List<ImageFeature> calFeatures, int bandCount)
        {
            this.calFeatures = calFeatures;
            this.bandCount = bandCount;

            this.csvExport.AddField(this.keyLabel);
            foreach (var imageFeature in this.calFeatures)
            {
                var fieldNames = imageFeature.GetFieldNames(bandCount);
                foreach (var fn in fieldNames)
                {
                    this.csvExport.AddField(fn);
                }
            }
        }

        public void ExportToFile(IEnumerable<KeyValuePair<int, Dictionary<string, double[]>>> resultSet)
        {
            foreach (var item in resultSet.OrderBy(p => p.Key))
            {
                // 添加行
                this.csvExport.AddRow();
                this.csvExport[this.keyLabel] = item.Key;
                var result = item.Value;
                foreach (var imageFeature in this.calFeatures)
                {
                    if (result.Keys.Contains(imageFeature.AlgorithmName))
                    {
                        double[] values = result[imageFeature.AlgorithmName];
                        int i = 0;
                        var fieldNames = imageFeature.GetFieldNames(bandCount);
                        foreach (var fn in fieldNames)
                        {
                            this.csvExport[fn] = values[i++];
                        }
                    }
                }
            }
        }

        public void Dispose()
        {
            if (disposed)
            {
                return;
            }
            if (this.csvExport != null)
            {
                this.csvExport.Dispose();
            }

            disposed = true;
            this.csvExport = null;
        }
    }
}
