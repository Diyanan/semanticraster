using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public enum SegmentationAlgorithm 
    {
        [DisplayString(typeof(Resources), "SegmentationAlogrithm_MultiSeg")]
        MultiSeg = 0,
        [DisplayString(typeof(Resources), "SegmentationAlogrithm_MS")]
        MS = 1,
        [DisplayString(typeof(Resources), "SegmentationAlogrithm_GraphCut")]
        GraphCut = 2,
        [DisplayString(typeof(Resources), "SegmentationAlogrithm_WaterShed")]
        WaterShed = 3,
        [DisplayString(typeof(Resources), "SegmentationAlogrithm_QuadTree")]
        QuadTree = 4,
        [DisplayString(typeof(Resources), "SegmentationAlogrithm_SuperPixel")]
        SuperPixel = 5,
        [DisplayString(typeof(Resources), "SegmentationAlogrithm_JSEG")]
        JSEG = 6,
    }
    public class SegmentSettingsViewModel : ViewModelBase
    {
        public static string PropertySegMethod = "SegMethod";
        private ViewModelBase _Setting = null;
        private SegmentationAlgorithm function = SegmentationAlgorithm.MS;
        private LayerChooserViewModel _LayerChooser;

        public SegmentationAlgorithm SegMethod
        {
            get
            {
                return this.function;
            }
            set
            {
                this.SetProperty<SegmentationAlgorithm>(PropertySegMethod, ref this.function, value, () =>
                {
                    this._Model.Param.Algorithm = (int)value;
                    switch (value)
                    {
                        case SegmentationAlgorithm.MS:
                            this.MS.Param = this._Model.Param;
                            this.Setting = this.MS;
                            break;
                        case SegmentationAlgorithm.GraphCut:
                            this.GraphCut.Param = this._Model.Param;
                            this.Setting = this.GraphCut;
                            break;
                        case SegmentationAlgorithm.JSEG:
                            this.JSEG.Param = this._Model.Param;
                            this.Setting = this.JSEG;
                            break;
                        case SegmentationAlgorithm.MultiSeg:
                            this.MultiSeg.Param = this._Model.Param;
                            this.Setting = this.MultiSeg;
                            break;
                        case SegmentationAlgorithm.QuadTree:
                            this.QuadTree.Param = this._Model.Param;
                            this.Setting = this.QuadTree;
                            break;
                        case SegmentationAlgorithm.SuperPixel:
                            this.SuperPixel.Param = this._Model.Param;
                            this.Setting = this.SuperPixel;
                            break;
                        case SegmentationAlgorithm.WaterShed:
                            this.WaterShed.Param = this._Model.Param;
                            this.Setting = this.WaterShed;
                            break;
                        default:
                            break;

                    }
                });
            }
        }
        public string PropertyLayerChooser
        {
            get
            {
                return "LayerChooser";
            }
        }

        public LayerChooserViewModel LayerChooser
        {
            get
            {
                return this._LayerChooser;
            }
            set
            {
                this.SetProperty<LayerChooserViewModel>(this.PropertyLayerChooser, ref this._LayerChooser, value, false);
            }
        }
        public static string PropertySetting
        {
            get
            {
                return "Setting";
            }
        }

        public ViewModelBase Setting
        {
            get
            {
                return _Setting;
            }

            set
            {
                this.SetProperty<ViewModelBase>(PropertySetting, ref _Setting, value, false);
            }
        }

        private SegTask _Model { get; set; }
        public LayerManagerViewModel LayerManagerVW { get; private set; }

        public List<SegmentationAlgorithm> Algorithms { get; private set; }

        public SegmentMSViewModel MS { get; set; }
        public SegmentMultiSegViewModel MultiSeg { get; set; }
        public SegmentWaterShedViewModel WaterShed { get; set; }
        public SegmentQuadTreeViewModel QuadTree { get; set; }
        public SegmentSuperPixelViewModel SuperPixel { get; set; }
        public SegmentJSEGViewModel JSEG { get; set; }
        public SegmentGraphCutViewModel GraphCut { get; set; }

        public SegmentSettingsViewModel(LayerManagerViewModel layerManagerVM)
        {
            this.Algorithms = new List<SegmentationAlgorithm>
                {
                    SegmentationAlgorithm.MultiSeg,
                    SegmentationAlgorithm.MS,
                    SegmentationAlgorithm.JSEG,
                    SegmentationAlgorithm.GraphCut,
                    SegmentationAlgorithm.QuadTree,
                    SegmentationAlgorithm.SuperPixel,
                    SegmentationAlgorithm.WaterShed,
                };

            this.LayerManagerVW = layerManagerVM;
            this.LayerChooser = new LayerChooserViewModel(layerManagerVM, LayerType.Raster);
            this.LayerChooser.PropertyChanged += new PropertyChangedEventHandler(this.ModelOnPropertyChanged);

            this.MS = new SegmentMSViewModel();
            this.MultiSeg = new SegmentMultiSegViewModel();
            this.WaterShed = new SegmentWaterShedViewModel();
            this.QuadTree = new SegmentQuadTreeViewModel();
            this.SuperPixel = new SegmentSuperPixelViewModel();
            this.JSEG = new SegmentJSEGViewModel();
            this.GraphCut = new SegmentGraphCutViewModel();
        }

        private void ModelOnPropertyChanged(object sender, PropertyChangedEventArgs e)
        {

            if (!e.PropertyName.Equals(LayerChooserViewModel.PropertySelectedLayer))
                return;
            if (this.LayerChooser.SelectedLayer != null)
            {
                // todo: 改变点选图层
                var layer = this.LayerChooser.SelectedLayer.LayerDefinition;
                this._Model.Param.RasterLayer = layer;
                this._Model.Param.RasterFileName = layer.DataSource;
            }
            else
            {
            }
        }

        internal void SetTask(SemanticTask model)
        {
            this._Model = model as SegTask;

            this.SegMethod = (SegmentationAlgorithm)this._Model.Param.Algorithm;
            if (this._Model.Param.RasterLayer != null)
            {
                foreach (var layer in this.LayerChooser.Layers)
                {
                    if (layer.LayerDefinition.Equals(this._Model.Param.RasterLayer))
                    {
                        this.LayerChooser.SelectedLayer = layer;
                    }
                }
            }
            else
            {
                this.LayerChooser.SelectedLayer = null;
            }
        }

    }
}
