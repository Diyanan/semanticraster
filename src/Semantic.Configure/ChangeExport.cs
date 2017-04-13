using Jitbit.Utils;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Configure
{
    public class ChangeExport : IDisposable
    {
        private readonly string keyLabel = "fid";
        private readonly string disLabel = "dis";
        private readonly string pearsLabel = "pears";
        private readonly string angleLabel = "ang";
        private bool disposed = false;
        private string csvPath;
        private CsvExport csvExport;

        public ChangeExport(string csvPath)
        {
            this.csvPath = csvPath;
            this.csvExport = new CsvExport(File.CreateText(csvPath));
        }

        public void Initialize()
        {
            this.csvExport.AddField(this.keyLabel);
            this.csvExport.AddField(this.disLabel);
            this.csvExport.AddField(this.pearsLabel);
            this.csvExport.AddField(this.angleLabel);
        }

        public void ExportToFile(int fid, double dis, double pears, double ang)
        {
            this.csvExport.AddRow();
            this.csvExport[this.keyLabel] = fid;
            this.csvExport[this.disLabel] = dis;
            this.csvExport[this.pearsLabel] = pears;
            this.csvExport[this.angleLabel] = ang;
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
