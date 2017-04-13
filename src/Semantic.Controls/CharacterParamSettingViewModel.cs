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
using System.Windows.Input;

namespace Semantic.Controls
{
    public enum CharacterSettingState
    {
        PickCharacter,
        ChooseLayer
    }

    public class CharacterParamSettingViewModel : ViewModelBase
    {
        private FeatureTask _Model { get; set; }

        private CharacterSettingState _state;
        private ViewModelBase _CharacterWell;
        private ICommand _PrevCommand;
        private ICommand _NextCommand;

        private bool _ShapeCharacterChecked = true;
        private bool _BandCharacterChecked = true;
        private bool _TextureCharacterChecked = true;
        private bool _IndexCharacterChecked = true;

        public bool ShapeCharacterChecked
        {
            get
            {
                return _ShapeCharacterChecked;
            }

            set
            {
                this.SetProperty<bool>("ShapeCharacterChecked", ref this._ShapeCharacterChecked, value, () =>
                {
                    this.SetCharacterSetEnable("形状特征", value);
                });
            }
        }

        public bool BandCharacterChecked
        {
            get
            {
                return _BandCharacterChecked;
            }

            set
            {
                this.SetProperty<bool>("BandCharacterChecked", ref this._BandCharacterChecked, value, () =>
                {
                    this.SetCharacterSetEnable("光谱特征", value);
                });
            }
        }

        public bool TextureCharacterChecked
        {
            get
            {
                return _TextureCharacterChecked;
            }

            set
            {
                this.SetProperty<bool>("TextureCharacterChecked", ref this._TextureCharacterChecked, value, () =>
                {
                    this.SetCharacterSetEnable("纹理特征", value);
                });
            }
        }

        public bool IndexCharacterChecked
        {
            get
            {
                return _IndexCharacterChecked;
            }

            set
            {
                this.SetProperty<bool>("IndexCharacterChecked", ref this._IndexCharacterChecked, value, () =>
                {
                    this.SetCharacterSetEnable("指数特征", value);
                });
            }
        }


        public static string PropertyState
        {
            get
            {
                return "State";
            }
        }

        public CharacterSettingState State
        {
            get
            {
                return this._state;
            }
            set
            {
                this.SetProperty(PropertyState, ref this._state, value, false);
            }
        }

        public string PropertyCharacterWell
        {
            get
            {
                return "CharacterWell";
            }
        }

        public ViewModelBase CharacterWell
        {
            get
            {
                return this._CharacterWell;
            }
            set
            {
                this.SetProperty(this.PropertyCharacterWell, ref this._CharacterWell, value, false);
            }
        }
        public string PropertyPrevCommand
        {
            get
            {
                return "PrevCommand";
            }
        }

        public ICommand PrevCommand
        {
            get
            {
                return _PrevCommand;
            }
            set
            {
                SetProperty(PropertyPrevCommand, ref _PrevCommand, value, false);
            }
        }


        public string PropertyNextCommand
        {
            get
            {
                return "NextCommand";
            }
        }

        public ICommand NextCommand
        {
            get
            {
                return _NextCommand;
            }
            set
            {
                SetProperty(PropertyNextCommand, ref _NextCommand, value, false);
            }
        }

        public CharacterIslandBuilder Builder { get; private set; }
        public CharacterListViewModel CharacterList { get; private set; }
        public CharacterSettingsViewModel CharacterSetting { get; private set; }

        public ObservableCollectionEx<CharacterIslandViewModel> CharacterIslands { get; private set; }

        public CharacterParamSettingViewModel(LayerManagerViewModel lmVM, string configureFile)
        {
            // builder
            this.Builder = new CharacterIslandBuilder(configureFile);
            this.Builder.CharacterPropertyChangedHandler += new PropertyChangedEventHandler(this.CharacterPropertyChanged);

            // picker
            this.CharacterIslands = new ObservableCollectionEx<CharacterIslandViewModel>();
            this.CharacterIslands.Add(this.Builder.CharacterIsland);

            // picked list  
            this.CharacterList = new CharacterListViewModel();
            this.CharacterList.UpdateImageFeature += this.OnUpdateImageFeature;

            // setting
            this.CharacterSetting = new CharacterSettingsViewModel(lmVM);
            this.CharacterSetting.RasterBands.ItemPropertyChanged += OnBandIndexChanged;
            this.CharacterSetting.PropertyChanged += OnCharacterSettingPropertyChanged;

            this.PrevCommand = new DelegatedCommand(this.OnExecutePrevCommand);
            this.NextCommand = new DelegatedCommand(this.OnExecuteNextCommand);

            this.CharacterWell = this.CharacterList;
        }

        private void OnBandIndexChanged(RasterBandViewModel item, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == RasterBandViewModel.PropertyBandIndex)
            {
                this._Model.Param.BandsIndex[item.BandLabel] = item.BandIndex;
            }
        }

        private void OnCharacterSettingPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            var setting = sender as CharacterSettingsViewModel;
            if (e.PropertyName == CharacterSettingsViewModel.PropertyBandCount)
            {
                this._Model.Param.BandCount = setting.BandCount;
            }
            if (e.PropertyName == CharacterSettingsViewModel.PropertyUseDefaultFields)
            {
                this._Model.Param.UseDefaultField = setting.UseDefaultFields;
            }
            if (e.PropertyName == CharacterSettingsViewModel.PropertyVectorLayer)
            {
                this._Model.Param.VectorLayer = setting.VectorLayer;
            }
            if (e.PropertyName == CharacterSettingsViewModel.PropertyRasterLayer)
            {
                this._Model.Param.ImageLayer = setting.RasterLayer;
                this._Model.Param.BandCount = setting.BandCount;
            }
        }

        private void OnUpdateImageFeature(object sender, bool isAdd)
        {
            ImageFeature imageFeature = sender as ImageFeature;
            if (isAdd)
            {
                if (!this._Model.Param.CalFeatures.Contains(imageFeature))
                {
                    this._Model.Param.CalFeatures.Add(imageFeature);
                }
            }
            else
            {
                this._Model.Param.CalFeatures.Remove(imageFeature);
            }

            this.CharacterSetting.InitBandMap(this._Model.Param.NeedBands);
        }

        private void SetCharacterSetEnable(string featureType, bool enable)
        {
            foreach (var island in this.CharacterIslands)
            {
                foreach (var set in island.CharacterSets)
                {
                    if (set.Name == featureType)
                    {
                        set.IsEnable = enable;
                    }
                }
            }
        }

        public void SetTask(SemanticTask task)
        {
            this._Model = task as FeatureTask;

            foreach (var c in this.Builder.Characters)
            {
                bool haveSelected = this._Model.Param.CalFeatures.Contains(c.Model);
                if (c.IsSelected != haveSelected)
                {
                    c.IsSelected = haveSelected;
                }
            }

            if (this._Model.Param.ImageLayer != null)
            {
                foreach (var layer in this.CharacterSetting.RasterLayerChooser.Layers)
                {
                    if (layer.LayerDefinition.Equals(this._Model.Param.ImageLayer))
                    {
                        this.CharacterSetting.RasterLayerChooser.SelectedLayer = layer;
                        break;
                    }
                }
            }
            else
            {
                this.CharacterSetting.RasterLayerChooser.SelectedLayer = null;
            }

            foreach (var band in this.CharacterSetting.RasterBands)
            {
                if (this._Model.Param.BandsIndex.ContainsKey(band.BandLabel))
                {
                    band.BandIndex = this._Model.Param.BandsIndex[band.BandLabel];
                }
            }


            if (this._Model.Param.VectorLayer != null)
            {
                foreach (var layer in this.CharacterSetting.VectorLayerChooser.Layers)
                {
                    if (layer.LayerDefinition.Equals(this._Model.Param.VectorLayer))
                    {
                        this.CharacterSetting.VectorLayerChooser.SelectedLayer = layer;
                        break;
                    }
                }
            }
            else
            {
                this.CharacterSetting.VectorLayerChooser.SelectedLayer = null;
            }

            this.CharacterSetting.UseDefaultFields = this._Model.Param.UseDefaultField;

            SetState(CharacterSettingState.PickCharacter);
        }

        private void CharacterPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            var item = sender as CharacterViewModel;
            if (e.PropertyName == CharacterViewModel.PropertyIsSelected && item.IsSelected == true)
            {
                this.CharacterList.ProcessNewSelectedCharacter(item);
            }
        }

        private void SetState(CharacterSettingState state)
        {
            if (state == CharacterSettingState.PickCharacter)
            {
                this.State = CharacterSettingState.PickCharacter;
                this.CharacterWell = this.CharacterList;
            }
            else
            {
                if (state == CharacterSettingState.ChooseLayer)
                {
                    this.State = CharacterSettingState.ChooseLayer;
                    this.CharacterWell = this.CharacterSetting;
                }
            }
        }

        private CharacterSettingState GetNextState()
        {
            return this.State + 1;
        }

        private CharacterSettingState GetPrevState()
        {
            return this.State - 1;
        }

        private void OnExecuteNextCommand()
        {
            var next = GetNextState();
            this.SetState(next);
        }

        private void OnExecutePrevCommand()
        {
            var prev = GetPrevState();
            this.SetState(prev);
        }

    }
}
