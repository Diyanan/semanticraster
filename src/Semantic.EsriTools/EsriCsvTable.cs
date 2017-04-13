using ESRI.ArcGIS.DataSourcesOleDB;
using ESRI.ArcGIS.Geodatabase;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Semantic.EsriTools
{
    public class EsriCsvTable
    {
        private string DirectoryName { get; set; }
        public string Name { get; private set; }
        public ITable Table { get; set; }
        public EsriCsvTable(string csvPath)
        {
            this.DirectoryName = Path.GetDirectoryName(csvPath);
            this.Name = Path.GetFileName(csvPath);
            this.Table = OpenTable();
        }

        /// <summary>
        /// 打开属性表
        /// </summary>
        /// <param name="csvPath"></param>
        /// <returns></returns>
        private ITable OpenTable()
        {
            IWorkspaceFactory workspaceFactory = new TextFileWorkspaceFactoryClass();
            IWorkspace workspace = workspaceFactory.OpenFromFile(this.DirectoryName, 0);
            IFeatureWorkspace featureWorkspace = (IFeatureWorkspace)workspace;
            return featureWorkspace.OpenTable(this.Name);
        }
    }
}
