using Semantic.Engine;
using System;
using System.Diagnostics;
using System.Globalization;
using System.Text;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    public class AndOrFilterClause : FilterClause
    {
        public FilterPredicate First { get; protected set; }

        public FilterPredicate Second { get; protected set; }

        public FilterPredicateOperator Operator { get; protected set; }

        public override bool Unfiltered
        {
            get
            {
                if (this.First != null && !this.First.Unfiltered)
                    return false;
                if (this.Second != null)
                    return this.Second.Unfiltered;
                else
                    return true;
            }
        }

        public override bool HasUpdatableProperties
        {
            get
            {
                FilterPredicate filterPredicate = this.First ?? this.Second;
                if (filterPredicate != null)
                    return filterPredicate.HasUpdatableProperties;
                else
                    return false;
            }
        }

        protected FilterPredicateProperties FilterProperties { get; private set; }

        public AndOrFilterClause(TableMember column, AggregationFunction afn, FilterPredicateOperator predicateOperator, FilterPredicate first, FilterPredicate second)
            : base(column, afn)
        {
            if (first != null && second != null && first.GetType() != second.GetType())
                throw new ArgumentException("Both predicates must be of the same type");
            this.ValidatePredicateType(first ?? second, column, afn);
            this.First = first;
            this.Second = second;
            this.Operator = predicateOperator;
        }

        public AndOrFilterClause(AndOrFilterClause oldFilterClause, FilterPredicateOperator predicateOperator, FilterPredicate first, FilterPredicate second)
            : base(oldFilterClause)
        {
            if (first != null && second != null && first.GetType() != second.GetType())
                throw new ArgumentException("Both predicates must be of the same type");
            this.ValidatePredicateType(first ?? second, oldFilterClause.TableMember, oldFilterClause.AggregationFunction);
            this.First = first;
            this.Second = second;
            this.Operator = predicateOperator;
            this.FilterProperties = oldFilterClause.FilterProperties;
        }

        internal AndOrFilterClause(SerializableFilterClause state)
            : base(state)
        {
            this.Unwrap(state);
        }

        public bool TryGetProperties(out FilterPredicateProperties properties)
        {
            properties = this.FilterProperties;
            return this.FilterProperties != null;
        }

        internal bool UpdatePropertiesInBase(int queryVersion, ModelQueryColumn queryColumn)
        {
            return base.UpdateProperties(queryVersion, queryColumn);
        }

        internal override bool UpdateProperties(int queryVersion, ModelQueryColumn queryColumn)
        {
            bool flag = base.UpdateProperties(queryVersion, queryColumn) && (queryColumn != null);
            if (flag && queryColumn.Values != null)
            {
                if (this.HasUpdatableProperties)
                {
                    FilterPredicate predicate = this.First ?? this.Second;
                    if (predicate != null)
                    {
                        this.FilterProperties = predicate.UpdateProperties(queryColumn);
                    }
                    base.DispatchPropertiesUpdated();
                    return true;
                }
                VisualizationTraceSource.Current.TraceEvent(TraceEventType.Information, 0, "UpdateProperties(): returning false for AndOrFilterClause (table column = {0}, Agg Fn = {1}, queryVersion={2}, RequestedVersion={3}) - HasUpdatableProperties = false", new object[] { base.TableMember.Name, base.AggregationFunction, queryVersion, base.CurrentVersion });
            }
            return false;

        }

        internal override SerializableFilterClause Wrap()
        {
            SerializableAndOrFilterClause andOrFilterClause = new SerializableAndOrFilterClause();
            this.Wrap(andOrFilterClause);
            return andOrFilterClause;
        }

        protected override void Wrap(SerializableFilterClause wrappedState)
        {
            if (wrappedState == null)
                throw new ArgumentNullException("wrappedState");
            SerializableAndOrFilterClause andOrFilterClause = wrappedState as SerializableAndOrFilterClause;
            if (andOrFilterClause == null)
                throw new ArgumentException("wrappedState must be of type SerializableAndOrFilterClause");
            andOrFilterClause.Operator = this.Operator;
            andOrFilterClause.First = this.First == null ? null : this.First.Wrap();
            andOrFilterClause.Second = this.Second == null ? null : this.Second.Wrap();
            base.Wrap(andOrFilterClause);
        }

        internal override void Unwrap(SerializableFilterClause wrappedState)
        {
            if (wrappedState == null)
                throw new ArgumentNullException("wrappedState");
            SerializableAndOrFilterClause andOrFilterClause = wrappedState as SerializableAndOrFilterClause;
            if (andOrFilterClause == null)
                throw new ArgumentException("wrappedState must be of type AndOrFilterClause");
            base.Unwrap(andOrFilterClause);
            this.Operator = andOrFilterClause.Operator;
            this.First = andOrFilterClause.First == null ? null : andOrFilterClause.First.Unwrap();
            this.Second = andOrFilterClause.Second == null ? null : andOrFilterClause.Second.Unwrap();
        }

        protected virtual void ValidatePredicateType(FilterPredicate predicateToTest, TableMember column, AggregationFunction afn)
        {
            if (predicateToTest == null)
                return;
            switch (column.DataType)
            {
                case TableMemberDataType.String:
                    if (afn == AggregationFunction.None && !(predicateToTest is StringFilterPredicate))
                        throw new ArgumentException("With AggregationFunction.None, the predicate must be a String predicate");
                    if (afn == AggregationFunction.None || predicateToTest is NumericFilterPredicate)
                        break;
                    else
                        throw new ArgumentException("With AggregationFunction != None, the predicate must be a Numeric predicate");
                case TableMemberDataType.Bool:
                    if (afn == AggregationFunction.None)
                        throw new ArgumentException("AggregationFunction = None is not supported for Bool");
                    if (predicateToTest is NumericFilterPredicate)
                        break;
                    else
                        throw new ArgumentException("With AggregationFunction != None, the predicate must be a Numeric predicate");
                case TableMemberDataType.Double:
                case TableMemberDataType.Long:
                case TableMemberDataType.Currency:
                    if (predicateToTest is NumericFilterPredicate)
                        break;
                    else
                        throw new ArgumentException("The predicate must be a Numeric predicate");
                case TableMemberDataType.DateTime:
                    if (afn == AggregationFunction.None && !(predicateToTest is DateTimeFilterPredicate))
                        throw new ArgumentException("With AggregationFunction.None, the predicate must be a DateTime predicate");
                    if (afn == AggregationFunction.None || predicateToTest is NumericFilterPredicate)
                        break;
                    else
                        throw new ArgumentException("With AggregationFunction != None, the predicate must be a Numeric predicate");
                default:
                    throw new ArgumentException("Unsupported table column data type " + column.DataType.ToString());
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            if (this.First != null && !this.First.Unfiltered)
            {
                sb.AppendFormat("{0} {1}", this.TableMember.ModelQueryName, this.First.ToString());
            }
            if (this.Second != null && !this.Second.Unfiltered)
            {
                if (this.Operator == FilterPredicateOperator.And)
                {
                    sb.Append(" AND ");
                }
                else
                {
                    sb.Append(" OR ");
                }
                sb.AppendFormat("{0} {1}", this.TableMember.ModelQueryName, this.Second.ToString());
            }
            return sb.ToString();
        }

        [Serializable]
        public class SerializableAndOrFilterClause : SerializableFilterClause
        {
            [XmlAttribute("Op")]
            public FilterPredicateOperator Operator;
            [XmlElement("FirstNum", typeof(NumericFilterPredicate.SerializableNumericFilterPredicate))]
            [XmlElement("FirstDT", typeof(DateTimeFilterPredicate.SerializableDateTimeFilterPredicate))]
            [XmlElement("FirstStr", typeof(StringFilterPredicate.SerializableStringFilterPredicate))]
            public FilterPredicate.SerializableFilterPredicate First;
            [XmlElement("SecondDT", typeof(DateTimeFilterPredicate.SerializableDateTimeFilterPredicate))]
            [XmlElement("SecondNum", typeof(NumericFilterPredicate.SerializableNumericFilterPredicate))]
            [XmlElement("SecondStr", typeof(StringFilterPredicate.SerializableStringFilterPredicate))]
            public FilterPredicate.SerializableFilterPredicate Second;

            internal override FilterClause Unwrap(CultureInfo modelCulture)
            {
                return new AndOrFilterClause(this);
            }
        }
    }
}
