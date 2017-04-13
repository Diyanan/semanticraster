using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class TaskPanelSamplesTabViewModel : ViewModelBase
    {
        private LayerChooserViewModel _model;
        private SampleItemViewModel _SelectedSample;

        public string PropertyModel
        {
            get
            {
                return "Model";
            }
        }

        public LayerChooserViewModel Model
        {
            get
            {
                return this._model;
            }
            set
            {
                this.SetProperty<LayerChooserViewModel>(this.PropertyModel, ref this._model, value, false);
            }
        }

        public LayerManagerViewModel LayerManagerVW { get; private set; }

        public SampleItemViewModel SelectedItem
        {
            get; set;
        }

        public static string PropertySelectedSample
        {
            get
            {
                return "SelectedSample";
            }
        }

        public SampleItemViewModel SelectedSample
        {
            get
            {
                return this._SelectedSample;
            }
            set
            {
                base.SetProperty(PropertySelectedSample, ref this._SelectedSample, value, ()=>
                {
                    if (this.SetSample != null && value != null)
                    {
                        this.SetSample(value.Model);
                    }
                });
            }
        }


        public ObservableCollectionEx<SampleItemViewModel> Samples { get; private set; }

        public LeafNodeViewModel LeafClass { get; private set; }

        public LayerDefinition Layer { get; private set; }

        public GeoClassViewModel GeoClassViewModel { get; private set; }

        public Action<LayerDefinition, string> SetLayer;

        public Action<SampleEntry> SetSample;

        public TaskPanelSamplesTabViewModel(LayerManagerViewModel layerManagerVM)
        {
            this.Samples = new ObservableCollectionEx<SampleItemViewModel>();
            this.LayerManagerVW = layerManagerVM;
            this.Model = new LayerChooserViewModel(layerManagerVM, LayerType.Vector);
            this.Model.PropertyChanged += new PropertyChangedEventHandler(this.ModelOnPropertyChanged);
            // geoclass
            string configureFile = AppDomain.CurrentDomain.BaseDirectory + "GeoClass.xml";
            this.GeoClassViewModel = new GeoClassViewModel(configureFile);
            this.GeoClassViewModel.PropertyChanged += GeoClassViewModel_PropertyChanged;
        }

        private void ModelOnPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (!e.PropertyName.Equals(LayerChooserViewModel.PropertySelectedLayer))
                return;
            if (this.Model.SelectedLayer != null)
            {
                // todo: 改变点选图层
                this.Layer = this.Model.SelectedLayer.LayerDefinition;
                StartSample();
            }
        }

        private void GeoClassViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == GeoClassViewModel.PropertySelectedItem)
            {
                // 设置为选择的分类叶子节点
                this.LeafClass = this.GeoClassViewModel.SelectedItem;
                StartSample();
            }
        }

        public void AddSample(int oid, string clsName)
        {
            if (this.LeafClass == null || this.Layer == null)
            {
                return;
            }
            var entry = new SampleEntry
            {
                GeoClass = clsName,
                Layer = this.Layer,
                OID = oid
            };
            this.Samples.Add(new SampleItemViewModel(entry));
        }

        internal void StartSample()
        {
            if (this.SetLayer != null)
            {
                this.Samples.Clear();
                var clsName = this.LeafClass != null ? this.LeafClass.Model.Name : "";
                if (this.Model.SelectedLayer != null)
                {
                    this.SetLayer(this.Model.SelectedLayer.LayerDefinition, clsName);
                    this.Model.SelectedLayer.SetSampleStyle(this.GeoClassViewModel.Model, true);
                }
            }
        }

        public void DeleteSample(int oid)
        {
            var sample = this.Samples.FirstOrDefault(r => r.OID == oid);
            if (sample != null)
            {
                this.Samples.Remove(sample);
            }
        }

        public IEnumerable<SampleEntry> GetSamples()
        {
            return this.Samples.Select(r=>r.Model);
        }
    }
}
