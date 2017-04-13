using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class SegmentSuperPixelViewModel : ViewModelBase
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
                    this.spcount = this._param.ParaSPcount;
                    this.compactness = this._param.ParaSPcompactness;
                }
            }
        }
        private int _spcount = 500;
        private int _compactness = 15;
        public string Propertyspcount
        {
            get
            {
                return "spcount";
            }
        }

        public int spcount
        {
            get
            {
                return this._spcount;
            }
            set
            {
                base.SetProperty(this.Propertyspcount, ref this._spcount, value,()=>
                {
                    this.Param.ParaSPcount = value;
                });
            }
        }
        public string Propertycompactness
        {
            get
            {
                return "compactness";
            }
        }

        public int compactness
        {
            get
            {
                return this._compactness;
            }
            set
            {
                base.SetProperty(this.Propertycompactness, ref this._compactness, value,()=>
                {
                    this.Param.ParaSPcompactness = value;
                });
            }
        }
    }
}
