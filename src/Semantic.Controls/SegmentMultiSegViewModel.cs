using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class SegmentMultiSegViewModel : ViewModelBase
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
                    this.MultiSegScale = this._param.ParaMultiSegScaleNum;
                    this.MultiSegHdiff = this._param.ParaMultiSegHdiff;
                }
            }
        }
        private int _MultiSegScale = 3;
        private double _MultiSegHdiff = 50;

        public string PropertyMultiSegScale
        {
            get
            {
                return "MultiSegScale";
            }
        }

        public int MultiSegScale
        {
            get
            {
                return this._MultiSegScale;
            }
            set
            {
                base.SetProperty(this.PropertyMultiSegScale, ref this._MultiSegScale, value,()=>
                {
                    this.Param.ParaMultiSegScaleNum = value;
                });
            }
        }

        public string PropertyMultiSegHdiff
        {
            get
            {
                return "MultiSegHdiff";
            }
        }

        public double MultiSegHdiff
        {
            get
            {
                return this._MultiSegHdiff;
            }
            set
            {
                base.SetProperty(this.PropertyMultiSegHdiff, ref this._MultiSegHdiff, value,()=>
                {
                    this.Param.ParaMultiSegHdiff = value;
                });
            }
        }
    }
}
