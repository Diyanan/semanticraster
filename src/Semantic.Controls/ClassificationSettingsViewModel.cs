using Semantic.Configure;
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
    public enum ClassificationAlgorithm
    {
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_ANN")]
        ANN = 0,
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_SVM")]
        SVM = 1,
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_Bayers")]
        Bayers = 2,
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_KNN")]
        KNN = 3,
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_Tree")]
        Tree = 4,
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_AdTree")]
        AdTree = 5,
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_JC1")]
        JC1 = 6,
        [DisplayString(typeof(Resources), "ClassificationAlgorithm_JC2")]
        JC2 = 7
    }

    /// <summary>
    /// 实现分类任务参数设置面板逻辑
    /// </summary>
    public class ClassificationSettingsViewModel : ViewModelBase
    {
        public static string PropertyFunction = "Function";
        public static string PropertyCharacterIsland = "CharacterIsland";

        private ClassificationAlgorithm function = ClassificationAlgorithm.ANN;
        private CharacterIslandViewModel _CharacterIsland;
        private LayerChooserViewModel _LayerChooser;

        public ClassificationAlgorithm Function
        {
            get
            {
                return this.function;
            }
            set
            {
                this.SetProperty<ClassificationAlgorithm>(PropertyFunction, ref this.function, value, () =>
                {
                    this._Model.Param.Algorithm = (int)value;
                });
            }
        }

        public CharacterIslandViewModel CharacterIsland
        {
            get
            {
                return this._CharacterIsland;
            }
            set
            {
                this.SetProperty(PropertyCharacterIsland, ref this._CharacterIsland, value, false);
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

        public LayerManagerViewModel LayerManagerVW { get; private set; }

        public List<ClassificationAlgorithm> Algorithms { get; private set; }

        private ClassifyTask _Model { get; set; }

        public ObservableCollectionEx<TableFieldViewModel> LayerFields { get; private set; }

        public ClassificationSettingsViewModel(LayerManagerViewModel layerManagerVM)
        {
            this.Algorithms = new List<ClassificationAlgorithm>
                {
                    ClassificationAlgorithm.ANN,
                    ClassificationAlgorithm.SVM,
                    ClassificationAlgorithm.Bayers,
                    ClassificationAlgorithm.KNN,
                    ClassificationAlgorithm.Tree,
                    ClassificationAlgorithm.AdTree,
                    ClassificationAlgorithm.JC1,
                    ClassificationAlgorithm.JC2
                };
            this.LayerFields = new ObservableCollectionEx<TableFieldViewModel>();
            this.LayerFields.ItemPropertyChanged += LayerFields_ItemPropertyChanged;

            this.LayerManagerVW = layerManagerVM;
            this.LayerChooser = new LayerChooserViewModel(layerManagerVM, LayerType.Vector);
            this.LayerChooser.PropertyChanged += new PropertyChangedEventHandler(this.ModelOnPropertyChanged);
        }

        private void LayerFields_ItemPropertyChanged(TableFieldViewModel item, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == TableFieldViewModel.PropertyIsSelected)
            {
                if (item.IsSelected)
                {
                    if (!this._Model.Param.SelFields.Contains(item.Model.Name))
                    {
                        this._Model.Param.SelFields.Add(item.Model.Name);
                    }
                }
                else
                {
                    this._Model.Param.SelFields.Remove(item.Model.Name);
                }
            }
        }

        private void ModelOnPropertyChanged(object sender, PropertyChangedEventArgs propertyChangedEventArgs)
        {
            if (!propertyChangedEventArgs.PropertyName.Equals(LayerChooserViewModel.PropertySelectedLayer))
                return;
            if (this.LayerChooser.SelectedLayer != null)
            {
                // todo: 改变点选图层
                var layer = this.LayerChooser.SelectedLayer.LayerDefinition;
                this._Model.Param.VectorLayer = layer;
                this.CharacterIsland = GetIsland(layer);
            }
            else
            {
                this.LayerFields.Clear();
                this.CharacterIsland = null;
            }
        }

        public void SetTask(SemanticTask task)
        {
            this._Model = task as ClassifyTask;

            this.Function = (ClassificationAlgorithm)this._Model.Param.Algorithm;

            if (this._Model.Param.VectorLayer != null)
            {
                foreach (var layer in this.LayerChooser.Layers)
                {
                    if (layer.LayerDefinition == this._Model.Param.VectorLayer)
                    {
                        this.LayerChooser.SelectedLayer = layer;
                    }
                }
            }
            else
            {
                this.LayerChooser.SelectedLayer = null;
            }

            // 更新字段列表
            foreach (var field in this.LayerFields)
            {
                bool needCal = false;
                foreach (var f in this._Model.Param.SelFields)
                {
                    if (field.Model.Name == f)
                    {
                        needCal = true;
                        break;
                    }
                }
                field.IsSelected = needCal;
            }
        }

        public CharacterIslandViewModel GetIsland(LayerDefinition layerDefn)
        {
            this.LayerFields.Clear();

            var result = new CharacterIslandViewModel();
            string configureFile = AppDomain.CurrentDomain.BaseDirectory + "conf\\FeatureSet.json";
            var vfs = JsonHelper<VerifyFeatureSet>.Load(configureFile);


            ModelWrapper wrapper = new ModelWrapper(this.LayerManagerVW.LayerManager);
            var table = wrapper.GetTable(layerDefn);
            var fields = table.LookupTables.SelectMany(t => t.Fields);

            List<ImageFeature> selFeatures = new List<ImageFeature>();

            foreach (var field in fields)
            {
                var feature = vfs.GetFeatures().Where(r => field.Name.Contains(r.FieldDef.ShortName)).FirstOrDefault();
                if (feature != null && !selFeatures.Contains(feature))
                {
                    selFeatures.Add(feature);
                }
            }

            foreach (var s in vfs.FeatureSet)
            {
                var characterSetVW = new CharacterSetViewModel(s.Name);
                var subSet = selFeatures.Where(f => s.Features.Contains(f));

                foreach (var item in subSet)
                {
                    CharacterViewModel characterVW = new CharacterViewModel(item);
                    foreach (var field in fields)
                    {
                        if (field.Name.Contains(item.FieldDef.ShortName))
                        {
                            var fieldVW = new TableFieldViewModel(field, false);
                            this.LayerFields.Add(fieldVW);
                            characterVW.Fields.Add(fieldVW);
                        }
                    }
                    characterSetVW.AddCharacter(characterVW);
                }

                result.CharacterSets.Add(characterSetVW);
            }

            return result;
        }
    }
}
