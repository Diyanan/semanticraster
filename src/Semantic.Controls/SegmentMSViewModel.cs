using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class SegmentMSViewModel : ViewModelBase
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
                    this.Minsize = this._param.ParaMSminsize;
                    this.SigmaR = this._param.ParaMSSigmaR;
                    this.SigmaS = this._param.ParaMSSigmaS;
                }
            }
        }
        private float _SigmaR = 6.5F;
        private int _SigmaS = 7;
        private int _Minsize = 200;
        public string PropertySigmaR
        {
            get
            {
                return "SigmaR";
            }
        }

        public float SigmaR
        {
            get
            {
                return this._SigmaR;
            }
            set
            {
                base.SetProperty(this.PropertySigmaR, ref this._SigmaR, value,()=>
                {
                    this.Param.ParaMSSigmaR = value;
                });
            }
        }
        public string PropertySigmaS
        {
            get
            {
                return "SigmaS";
            }
        }

        public int SigmaS
        {
            get
            {
                return this._SigmaS;
            }
            set
            {
                base.SetProperty(this.PropertySigmaS, ref this._SigmaS, value,()=>
                {
                    this.Param.ParaMSSigmaS = value;
                });
            }
        }

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
                    this.Param.ParaMSminsize = value;
                });
            }
        }
    }
}
