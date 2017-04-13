using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class SegmentWaterShedViewModel : ViewModelBase
    {
        private SegmentParam _param = null;
        public SegmentParam Param
        {
            get { return _param; }
            set
            {
                this._param = value;
                if (this._param != null)
                {
                    this.Minsize = this._param.ParaWSminsize;
                }
            }
        }
        private int _Minsize = 200;
        public string PropertyMinsize
        {
            get
            {
                return "Minsize";
            }
        }

        public int Minsize
        {
            get
            {
                return this._Minsize;
            }
            set
            {
                base.SetProperty(this.PropertyMinsize, ref this._Minsize, value,()=>
                {
                    this.Param.ParaWSminsize = value;
                });
            }
        }
    }
}
