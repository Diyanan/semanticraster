using System;

namespace Semantic.Controls
{
    public class DateTimeFilterPredicateProperties : FilterPredicateProperties
    {
        public DateTime? AllowedMin { get; set; }

        public DateTime? AllowedMax { get; set; }
    }
}
