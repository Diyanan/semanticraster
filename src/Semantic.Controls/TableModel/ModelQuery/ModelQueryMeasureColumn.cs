namespace Semantic.Controls
{
    public class ModelQueryMeasureColumn : ModelQueryColumn
    {
        public AggregationFunction AggregationFunction { get; set; }

        public AccumulationType Accumulate { get; set; }

        public ModelQueryIndexedKeyColumn ModelQueryIndexedKey { get; set; }

        public double Min { get; set; }

        public double Max { get; set; }

        public ModelQueryKeyColumn ModelQueryKeyAlias { get; set; }

        public override void Reset()
        {
            this.Min = this.Max = double.NaN;
            this.ModelQueryKeyAlias = null;
            base.Reset();
        }

        public enum AccumulationType
        {
            NoAccumulation,
            AccumulateByBuckets,
            AccumulateByBucketsAndIndex,
        }
    }
}
