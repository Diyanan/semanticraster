using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class CharacterListViewModel : ViewModelBase
    {
        private List<CharacterListEntryViewModel> _removeCharactersCache = new List<CharacterListEntryViewModel>();

        public ObservableCollectionEx<CharacterListEntryViewModel> Characters { get; private set; }

        public event EventHandler<bool> UpdateImageFeature = null;

        public CharacterListViewModel()
        {
            this.Characters = new ObservableCollectionEx<CharacterListEntryViewModel>();
            this.Characters.ItemAdded += ListEntryAdded;
            this.Characters.ItemRemoved += ListEntryRemoved;
            this.Characters.ItemDescendentPropertyChanged += ((sender, e) =>  this.OnSelectedCharactersChanged());
        }


        private void ListEntryAdded(CharacterListEntryViewModel item)
        {
            if (UpdateImageFeature != null)
            {
                this.UpdateImageFeature(item.Character.Model, true);
            }
        }

        private void ListEntryRemoved(CharacterListEntryViewModel item)
        {
            if (UpdateImageFeature != null)
            {
                this.UpdateImageFeature(item.Character.Model, false);
            }
        }

        private void OnSelectedCharactersChanged()
        {
            _removeCharactersCache.Clear();
            foreach (var entry in this.Characters)
            {
                if (entry.Character.IsSelected != true)
                {
                    _removeCharactersCache.Add(entry);
                }
            }

            foreach (var entry in this._removeCharactersCache)
            {
                entry.RemoveEntry();
            }
        }

        private void OnCharacterRemoved(CharacterListEntryViewModel listEntry)
        {
            if (listEntry != null && listEntry.Character != null)
            {
                this.Characters.Remove(listEntry);
                listEntry.Character.IsSelected = false;
            }
        }

        private CharacterListEntryViewModel CreateListEntryVM(CharacterViewModel character)
        {
            if (character == null)
                return null;
            CharacterListEntryViewModel result = new CharacterListEntryViewModel();
            result.Character = character;
            result.DisplayString = character.Name;
            result.RemoveCallback = this.OnCharacterRemoved;
            return result;
        }

        public bool ProcessNewSelectedCharacter(CharacterViewModel character)
        {
            this.Characters.Add(this.CreateListEntryVM(character));
            return true;
        }
    }
}
