using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class SegmentQuadTreeViewModel : ViewModelBase
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
                    this.wc = this._param.ParaQTwc;
                    this.wp = this._param.ParaQTwp;
                    this.hdiff = this._param.ParaQThdiff;
                    this.Minsize = this._param.ParaQTMinsize;
                }
            }
        }
        private float _wc = 0.8F;
        private float _wp = 0.5F;
        private float _hdiff = 30F;
        private int _minsize = 200;

        public string Propertywc
        {
            get
            {
                return "wc";
            }
        }

        public float wc
        {
            get
            {
                return this._wc;
            }
            set
            {
                base.SetProperty(this.Propertywc, ref this._wc, value,()=>
                {
                    this.Param.ParaQTwc = value;
                });
            }
        }
        public string Propertywp
        {
            get
            {
                return "wp";
            }
        }

        public float wp
        {
            get
            {
                return this._wp;
            }
            set
            {
                base.SetProperty(this.Propertywp, ref this._wp, value,()=>
                {
                    this.Param.ParaQTwp = value;
                });
            }
        }
        public string Propertyhdiff
        {
            get
            {
                return "hdiff";
            }
        }

        public float hdiff
        {
            get
            {
                return this._hdiff;
            }
            set
            {
                base.SetProperty(this.Propertyhdiff, ref this._hdiff, value,()=>
                {
                    this.Param.ParaQThdiff = value;
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
                return this._minsize;
            }
            set
            {
                base.SetProperty(this.PropertyMinsize, ref this._minsize, value,()=>
                {
                    this.Param.ParaQTMinsize = value;
                });
            }
        }
    }
}
