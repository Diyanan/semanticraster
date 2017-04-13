using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using Semantic.Configure;

namespace Semantic.Controls
{
    public class CharacterIslandBuilder : ViewModelBase
    {
        private CharacterIslandViewModel _CharacterIsland;

        public PropertyChangedEventHandler CharacterPropertyChangedHandler { get; set; }

        public string PropertyCharacterIsland
        {
            get
            {
                return "CharacterIsland";
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
                this.SetProperty(this.PropertyCharacterIsland, ref this._CharacterIsland, value, false);
            }
        }

        public ObservableCollectionEx<CharacterViewModel> Characters { get; private set; }

        public string ConfigureFile { get; private set; }

        public CharacterIslandBuilder(string file)
        {
            this.ConfigureFile = file;
            this.CharacterIsland = new CharacterIslandViewModel();
            this.Characters = new ObservableCollectionEx<CharacterViewModel>();
            this.Characters.ItemAdded += (item => item.PropertyChanged += new PropertyChangedEventHandler(this.OnCharacterPropertyChanged));
            this.Characters.ItemRemoved += (item => item.PropertyChanged -= new PropertyChangedEventHandler(this.OnCharacterPropertyChanged));
            InitCharacterIsland();
        }

        private void OnCharacterPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (CharacterPropertyChangedHandler != null)
            {
                CharacterPropertyChangedHandler(sender, e);
            }
        }

        private void InitCharacterIsland()
        {
            // 解析FeatureSet
            var characterSet = JsonHelper<VerifyFeatureSet>.Load(this.ConfigureFile);
            // 获取Set列表
            var sets = characterSet.FeatureSet;
            // 构建Island
            foreach (var s in sets)
            {
                var characterSetVW = new CharacterSetViewModel(s.Name);
                foreach (var item in s.Features)
                {
                    CharacterViewModel characterVW = new CharacterViewModel(item);
                    this.Characters.Add(characterVW);
                    characterSetVW.AddCharacter(characterVW);
                }

                this.CharacterIsland.CharacterSets.Add(characterSetVW);
            }
        }
    }
}
