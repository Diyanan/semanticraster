using System;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    public class StringFilterPredicate : FilterPredicate
    {
        public StringFilterPredicateComparison Operator { get; private set; }

        public string Value { get; private set; }

        public override bool Unfiltered
        {
            get
            {
                if (this.Operator == StringFilterPredicateComparison.Unknown)
                    return true;
                if (this.Operator != StringFilterPredicateComparison.IsBlank && this.Operator != StringFilterPredicateComparison.IsNotBlank)
                    return this.Value == null;
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

        public StringFilterPredicate(StringFilterPredicateComparison op, string value)
        {
            this.Operator = op;
            this.Value = value;
        }

        internal StringFilterPredicate(SerializableStringFilterPredicate state)
          : base(state)
        {
            this.Unwrap(state);
        }

        public override bool SameAs(FilterPredicate filterPredicate)
        {
            StringFilterPredicate stringFilterPredicate = filterPredicate as StringFilterPredicate;
            if (stringFilterPredicate == null || this.Operator != stringFilterPredicate.Operator)
                return false;
            else
                return this.Value == stringFilterPredicate.Value;
        }


        public override string ToString()
        {
            switch (this.Operator)
            {
                case StringFilterPredicateComparison.Is:
                    return "= " + this.Value;
                case StringFilterPredicateComparison.IsNot:
                    return "<> " + this.Value;
                case StringFilterPredicateComparison.IsBlank:
                    return "IS NULL";
                case StringFilterPredicateComparison.IsNotBlank:
                    return "IS NOT NULL";
                case StringFilterPredicateComparison.Contains:
                    return "LIKE '%" + this.Value + "%'";
                case StringFilterPredicateComparison.DoesNotContain:
                    return "NOT LIKE '%" + this.Value + "%'";
                case StringFilterPredicateComparison.StartsWith:
                    return "LIKE '" + this.Value + "%'";
                case StringFilterPredicateComparison.DoesNotStartWith:
                    return "NOT LIKE '" + this.Value + "%'";
            }
            return "";
        }

        internal override SerializableFilterPredicate Wrap()
        {
            SerializableStringFilterPredicate stringFilterPredicate = new SerializableStringFilterPredicate()
            {
                Operator = this.Operator,
                Value = this.Value
            };
            base.Wrap(stringFilterPredicate);
            return stringFilterPredicate;
        }

        internal override void Unwrap(SerializableFilterPredicate wrappedState)
        {
            if (wrappedState == null)
                throw new ArgumentNullException("wrappedState");
            SerializableStringFilterPredicate stringFilterPredicate = wrappedState as SerializableStringFilterPredicate;
            if (stringFilterPredicate == null)
                throw new ArgumentException("wrappedState must be of type SerializableStringFilterPredicate");
            base.Unwrap(stringFilterPredicate);
            this.Operator = stringFilterPredicate.Operator;
            this.Value = stringFilterPredicate.Value;
        }

        [Serializable]
        public class SerializableStringFilterPredicate : SerializableFilterPredicate
        {
            [XmlAttribute("Op")]
            public StringFilterPredicateComparison Operator;
            [XmlAttribute("Val")]
            public string Value;

            internal override FilterPredicate Unwrap()
            {
                return new StringFilterPredicate(this);
            }
        }
    }
}
