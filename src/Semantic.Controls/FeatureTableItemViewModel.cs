using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class FeatureTableItemViewModel : ViewModelBase
    {
        public int cc;
        public int fid;

        public int FID
        {
            get
            {
                return fid;
            }
            set
            {
                this.SetProperty<int>("FID", ref this.fid, value, false);
            }
        }

        public int CC
        {
            get
            {
                return cc;
            }
            set
            {
                this.SetProperty<int>("CC", ref this.cc, value, false);
            }
        }

        public FeatureTableItemViewModel()
        { }

    }
}
