using System.Collections;

namespace Semantic.Controls
{
    public class ModelQueryIndexedKeyColumn : ModelQueryKeyColumn
    {
        public dynamic AllValues { get; set; }

        public bool PreserveValues { get; set; }

        public ArrayList AllPreservedValues { get; set; }

        public int[] PreservedValuesIndex { get; set; }

        public override void Reset()
        {
            this.AllValues = null;
            this.PreservedValuesIndex = null;
            base.Reset();
        }

        internal new virtual void Shutdown()
        {
            base.Shutdown();
            this.AllPreservedValues = null;
        }
    }
}
