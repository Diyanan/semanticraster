using Semantic.WpfExtensions;

namespace Semantic.Controls
{
    public class ListFilterItemViewModel<T> : ViewModelBase
    {
        private bool? isSelected;

        public T Item { get; set; }

        public string Name { get; set; }

        public static string PropertyIsSelected
        {
            get
            {
                return "IsSelected";
            }
        }

        public bool? IsSelected
        {
            get
            {
                return this.isSelected;
            }
            set
            {
                this.SetProperty<bool?>(PropertyIsSelected, ref this.isSelected, value, false);
            }
        }

        public bool IsAll { get; set; }

        public bool IsBlank { get; set; }

        public int Index { get; set; }
    }
}
