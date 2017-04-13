using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class CharacterIslandViewModel : ViewModelBase
    {
        public ObservableCollectionEx<CharacterSetViewModel> CharacterSets { get; private set; }

        public CharacterIslandViewModel()
        {
            this.CharacterSets = new ObservableCollectionEx<CharacterSetViewModel>();
        }

        public void AddCharacterSet(CharacterSetViewModel set)
        {
            this.CharacterSets.Add(set);
        }
    }
}
