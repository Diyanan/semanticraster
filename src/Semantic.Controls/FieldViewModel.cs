using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class FieldViewModel : ViewModelBase
    {
        private string _Name;
        private bool _IsSelected;

        public static string PropertyName
        {
            get
            {
                return "Name";
            }
        }

        public string Name
        {
            get
            {
                return _Name;
            }
            set
            {
                SetProperty(PropertyName, ref _Name, value, false);
            }
        }

        public static string PropertyIsSelected
        {
            get
            {
                return "IsSelected";
            }
        }

        public bool IsSelected
        {
            get
            {
                return _IsSelected;
            }
            set
            {
                SetProperty(PropertyIsSelected, ref _IsSelected, value, false);
            }
        }

        public FieldViewModel(string field)
        {
            this.Name = field;
        }
    }
}