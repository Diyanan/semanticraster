using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Semantic.Controls
{
    public abstract class MapViewModel : ViewModelBase
    {
        public Control Map { get; set; }
        public Control Toolbar { get; set; }

        public MapViewModel()
        {

        }

        public abstract List<FeatureTable> GetFeatureTables(LayerDefinition lyr);

        public abstract void ReadMxdPrj(string Mxdpath);

        public abstract List<string> GetAllLayerPath();

        public abstract void InitializeMap();

        public abstract void OperationsAfterLoaded();

        public abstract int GetRasterLayerBandsCount(LayerDefinition layerDef);

        public abstract void SetSampleTool(bool enable, Action<int,string, bool> handler);

        public abstract void SetSampleLayer(LayerDefinition layerDefn, string className);

        public abstract void SetSample(SampleEntry sample);

        public abstract IEnumerable<string> GetFieldName(LayerDefinition layerDefn);

        public abstract int GetFeatureId(LayerDefinition layerDefn, Coordinates coords);

        public abstract void SwitchSelectedFeature(bool canSelectedFeature);

        public abstract void SaveTrainFile(LayerDefinition layerDefn, List<string> fields, IEnumerable<SampleEntry> samples);

        public abstract void UpdateClass(LayerDefinition layerDefn, IEnumerable<string> res);

        // 根据group名称，获取图层路径列表
        public abstract List<string> GetGrouplayer(string groupname);

        public abstract string GetShpfilePath(string name);
    }
}
