using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class SegmentJSEGViewModel : ViewModelBase
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
                    this.tQuan = this._param.ParaJsegTquan;
                    this.Threshold = this._param.ParaJsegThreshold;
                    this.Minsize = this._param.ParaJsegMinsize;
                }
            }
        }
        private int _tQuan  = 200;
        private float _Threshold = 20F;
        private int _Minsize = 200;

        public string PropertytQuan
        {
            get
            {
                return "tQuan";
            }
        }

        public int tQuan
        {
            get
            {
                return this._tQuan;
            }
            set
            {
                base.SetProperty(this.PropertytQuan, ref this._tQuan, value,()=>
                {
                    this.Param.ParaJsegTquan = value;
                });
            }
        }
        public string PropertytThreshold
        {
            get
            {
                return "Threshold";
            }
        }

        public float Threshold
        {
            get
            {
                return this._Threshold;
            }
            set
            {
                base.SetProperty(this.PropertytThreshold, ref this._Threshold, value,()=>
                {
                    this.Param.ParaJsegThreshold = value;
                });
            }
        }
        public string PropertytMinsize
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
                base.SetProperty(this.PropertytMinsize, ref this._Minsize, value,()=>
                {
                    this.Param.ParaJsegMinsize = value;
                });
            }
        }
    }
}
