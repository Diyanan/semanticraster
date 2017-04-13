using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class SegmentGraphCutViewModel : ViewModelBase
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
                    this.Sigma = this._param.ParaGraphCutSigma;
                    this.K = this._param.ParaGraphCutThreshold;
                    this.Minsize = this._param.ParaGraphCutMinsize;
                }
            }
        }
        private float _Sigma = 0.8F;
        private float _K = 20F;
        private int _Minsize = 200;
        public string PropertySigma
        {
            get
            {
                return "Sigma";
            }
        }

        public float Sigma
        {
            get
            {
                return this._Sigma;
            }
            set
            {
                base.SetProperty(this.PropertySigma, ref this._Sigma, value,()=>
                {
                    this.Param.ParaGraphCutSigma = value;
                });
            }
        }
        public string PropertyK
        {
            get
            {
                return "K";
            }
        }

        public float K
        {
            get
            {
                return this._K;
            }
            set
            {
                base.SetProperty(this.PropertyK, ref this._K, value,()=>
                {
                    this.Param.ParaGraphCutThreshold = value;
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
                    this.Param.ParaGraphCutMinsize = value;
                });
            }
        }
    }
}
