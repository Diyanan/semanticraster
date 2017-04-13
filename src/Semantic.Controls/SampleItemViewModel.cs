using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class SampleItemViewModel : ViewModelBase
    {
        public int OID
        {
            get { return this.Model.OID; }
        }

        public string ClassName
        {
            get { return this.Model.GeoClass; }
        }

        public SampleEntry Model { get; private set; }

        public ICommand RemoveCommand { get; private set; }

        public Action<SampleItemViewModel> RemoveCallback { get; set; }


        public SampleItemViewModel(SampleEntry entry)
        {
            this.Model = entry;
            this.RemoveCommand = new DelegatedCommand(this.RemoveEntry);
        }

        public void RemoveEntry()
        {
            if (RemoveCallback != null)
            {
                this.RemoveCallback(this);
            }
        }
    }
}
