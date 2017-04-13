using Semantic.WpfExtensions;
using System;

namespace Semantic.Controls
{
    [Serializable]
    public enum FilterPredicateOperator
    {
        [DisplayString(typeof(Resources), "FiltersTab_FilterPredicateOperator_And")]
        And,
        [DisplayString(typeof(Resources), "FiltersTab_FilterPredicateOperator_Or")]
        Or,
    }
}
