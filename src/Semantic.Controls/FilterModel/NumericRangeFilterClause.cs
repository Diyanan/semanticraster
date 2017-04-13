using Semantic.Engine;
using System;
using System.Diagnostics;
using System.Globalization;

namespace Semantic.Controls
{
    public class NumericRangeFilterClause : AndOrFilterClause
    {
        public double? AllowedMin { get; internal set; }

        public double? AllowedMax { get; internal set; }

        public double? Min
        {
            get
            {
                return ((NumericFilterPredicate)this.First).Value;
            }
        }

        public double? Max
        {
            get
            {
                return ((NumericFilterPredicate)this.Second).Value;
            }
        }

        public override bool HasUpdatableProperties
        {
            get
            {
                return true;
            }
        }

        public bool IsIntegerRange
        {
            get
            {
                if (this.AggregationFunction == AggregationFunction.Count || this.AggregationFunction == AggregationFunction.DistinctCount)
                    return true;
                if (this.TableMember.DataType == TableMemberDataType.Long && this.AggregationFunction != AggregationFunction.Average)
                    return this.AggregationFunction != AggregationFunction.UserDefined;
                else
                    return false;
            }
        }

        public NumericRangeFilterClause(TableMember tableMember, AggregationFunction afn, double? minVal = null, double? maxVal = null)
          : base(tableMember, afn, FilterPredicateOperator.And, new NumericFilterPredicate(NumericFilterPredicateComparison.IsGreaterThanOrEqualTo, minVal), new NumericFilterPredicate(NumericFilterPredicateComparison.IsLessThanOrEqualTo, maxVal))
        {
            if (afn == AggregationFunction.None)
            {
                switch (tableMember.DataType)
                {
                    case TableMemberDataType.Double:
                    case TableMemberDataType.Long:
                    case TableMemberDataType.Currency:
                        break;
                    default:
                        throw new ArgumentException("Table column data type " + tableMember.DataType.ToString() + " is not supported with AggregationFunction.None");
                }
            }
            this.AllowedMin = new double?();
            this.AllowedMax = new double?();
        }

        public NumericRangeFilterClause(NumericRangeFilterClause oldFilterClause, double? minVal = null, double? maxVal = null)
          : base(oldFilterClause, FilterPredicateOperator.And, new NumericFilterPredicate(NumericFilterPredicateComparison.IsGreaterThanOrEqualTo, minVal), new NumericFilterPredicate(NumericFilterPredicateComparison.IsLessThanOrEqualTo, maxVal))
        {
            this.AllowedMin = oldFilterClause.AllowedMin;
            this.AllowedMax = oldFilterClause.AllowedMax;
        }

        internal NumericRangeFilterClause(SerializableFilterClause state)
          : base(state)
        {
            this.AllowedMin = new double?();
            this.AllowedMax = new double?();
        }

        public bool TryGetProperties(out double? min, out double? max, out double allowedMin, out double allowedMax)
        {
            if (!this.AllowedMin.HasValue || !this.AllowedMax.HasValue)
            {
                min = new double?();
                max = new double?();
                allowedMin = double.NaN;
                allowedMax = double.NaN;
                return false;
            }
            else
            {
                min = this.Min;
                max = this.Max;
                allowedMin = this.AllowedMin.Value;
                allowedMax = this.AllowedMax.Value;
                return true;
            }
        }

        internal override bool UpdateProperties(int queryVersion, ModelQueryColumn queryColumn)
        {
            if (this.UpdatePropertiesInBase(queryVersion, queryColumn))
            {
                ModelQueryMeasureColumn queryMeasureColumn = queryColumn as ModelQueryMeasureColumn;
                if (queryMeasureColumn != null)
                {
                    if (double.IsNaN(queryMeasureColumn.Min) || double.IsNaN(queryMeasureColumn.Max))
                    {
                        this.AllowedMin = this.AllowedMax = new double?();
                    }
                    else
                    {
                        this.AllowedMin = new double?(queryMeasureColumn.Min);
                        this.AllowedMax = new double?(queryMeasureColumn.Max);
                    }
                    this.DispatchPropertiesUpdated();
                    return true;
                }
                else
                    VisualizationTraceSource.Current.TraceEvent(TraceEventType.Information, 0, "UpdateProperties(): returning false for AndOrFilterClause (table column = {0}, Agg Fn = {1}, queryVersion={2}, RequestedVersion={3}) - measure = null", this.TableMember.Name, this.AggregationFunction, queryVersion, this.CurrentVersion);
            }
            return false;
        }

        public override string ToString()
        {
            if (this.Min != null && this.Max != null)
                return string.Format("({0} > {1} AND {0} < {2})", this.TableMember.ModelQueryName, this.Min, this.Max);
            return "";
        }

        internal override SerializableFilterClause Wrap()
        {
            SerializableNumericRangeFilterClause rangeFilterClause = new SerializableNumericRangeFilterClause();
            base.Wrap(rangeFilterClause);
            return rangeFilterClause;
        }

        internal override void Unwrap(SerializableFilterClause wrappedState)
        {
            if (wrappedState == null)
                throw new ArgumentNullException("wrappedState");
            SerializableNumericRangeFilterClause rangeFilterClause = wrappedState as SerializableNumericRangeFilterClause;
            if (rangeFilterClause == null)
                throw new ArgumentException("wrappedState must be of type SerializableNumericRangeFilterClause");
            base.Unwrap(rangeFilterClause);
        }

        protected override void ValidatePredicateType(FilterPredicate predicateToTest, TableMember column, AggregationFunction afn)
        {
        }

        [Serializable]
        public class SerializableNumericRangeFilterClause : SerializableAndOrFilterClause
        {
            internal override FilterClause Unwrap(CultureInfo modelCulture)
            {
                return new NumericRangeFilterClause(this);
            }
        }
    }
}
