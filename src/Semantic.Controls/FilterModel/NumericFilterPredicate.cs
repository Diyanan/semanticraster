using System;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    public class NumericFilterPredicate : FilterPredicate
    {
        public NumericFilterPredicateComparison Operator { get; private set; }

        public double? Value { get; private set; }

        public override bool Unfiltered
        {
            get
            {
                if (this.Operator == NumericFilterPredicateComparison.Unknown)
                    return true;
                if (this.Operator != NumericFilterPredicateComparison.IsBlank && this.Operator != NumericFilterPredicateComparison.IsNotBlank)
                    return !this.Value.HasValue;
                else
                    return false;
            }
        }

        public override bool HasUpdatableProperties
        {
            get
            {
                return false;
            }
        }

        public NumericFilterPredicate(NumericFilterPredicateComparison op, double? value)
        {
            if (value.HasValue && double.IsNaN(value.Value))
                throw new ArgumentException("value may not be double.NaN");
            this.Operator = op;
            this.Value = value;
        }

        internal NumericFilterPredicate(SerializableNumericFilterPredicate state)
          : base(state)
        {
            this.Unwrap(state);
        }

        public override bool SameAs(FilterPredicate filterPredicate)
        {
            NumericFilterPredicate numericFilterPredicate = filterPredicate as NumericFilterPredicate;
            if (numericFilterPredicate == null || this.Operator != numericFilterPredicate.Operator)
                return false;
            double? nullable1 = this.Value;
            double? nullable2 = numericFilterPredicate.Value;
            if (nullable1.GetValueOrDefault() == nullable2.GetValueOrDefault())
                return nullable1.HasValue == nullable2.HasValue;
            else
                return false;
        }

        public override string ToString()
        {
            switch (this.Operator)
            {
                case NumericFilterPredicateComparison.Is:
                    return "= " + this.Value;
                case NumericFilterPredicateComparison.IsNot:
                    return "<> " + this.Value;
                case NumericFilterPredicateComparison.IsBlank:
                    return "IS NULL";
                case NumericFilterPredicateComparison.IsNotBlank:
                    return "IS NOT NULL";
                case NumericFilterPredicateComparison.IsLessThan:
                    return "< " + this.Value;
                case NumericFilterPredicateComparison.IsLessThanOrEqualTo:
                    return "<= " + this.Value;
                case NumericFilterPredicateComparison.IsGreaterThan:
                    return "> " + this.Value;
                case NumericFilterPredicateComparison.IsGreaterThanOrEqualTo:
                    return ">= " + this.Value;
            }
            return "";
        }

        internal override SerializableFilterPredicate Wrap()
        {
            SerializableNumericFilterPredicate numericFilterPredicate = new SerializableNumericFilterPredicate()
            {
                Operator = this.Operator,
                Value = this.Value.HasValue ? this.Value.Value : double.NaN
            };
            base.Wrap(numericFilterPredicate);
            return numericFilterPredicate;
        }

        internal override void Unwrap(SerializableFilterPredicate wrappedState)
        {
            if (wrappedState == null)
                throw new ArgumentNullException("wrappedState");
            SerializableNumericFilterPredicate numericFilterPredicate = wrappedState as SerializableNumericFilterPredicate;
            if (numericFilterPredicate == null)
                throw new ArgumentException("wrappedState must be of type SerializableNumericFilterPredicate");
            base.Unwrap(numericFilterPredicate);
            this.Operator = numericFilterPredicate.Operator;
            this.Value = double.IsNaN(numericFilterPredicate.Value) ? new double?() : new double?(numericFilterPredicate.Value);
        }

        [Serializable]
        public class SerializableNumericFilterPredicate : SerializableFilterPredicate
        {
            [XmlAttribute("Op")]
            public NumericFilterPredicateComparison Operator;
            [XmlAttribute("Val")]
            public double Value;

            internal override FilterPredicate Unwrap()
            {
                return new NumericFilterPredicate(this);
            }
        }
    }
}
