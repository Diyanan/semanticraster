using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Threading;

namespace Semantic.Controls
{
    public abstract class ModelQuery
    {
        public static DateTime UnknownDateTime
        {
            get
            {
                return DateTime.MaxValue;
            }
        }

        public string Name { get; protected set; }

        public Filter Filter { get; protected set; }

        public CancellationToken Cancellationtoken { get; protected set; }

        public string QueryString { get; protected set; }

        public int ResultsItemCount { get; protected set; }

        public bool QueryUsesAggregation { get; protected set; }

        public ModelQuery(string name, Filter filter, CancellationToken cancellationToken)
        {
            if (name == null)
                throw new ArgumentNullException("name");
            this.Name = name;
            this.Filter = new Filter();
            this.Filter.SetFilterClausesFrom(filter);
            this.Cancellationtoken = cancellationToken;
        }

        public abstract void Clear();

        protected internal virtual void Shutdown()
        {
            this.Name = string.Empty;
            this.QueryString = string.Empty;
            this.Filter = new Filter();
        }

        public abstract void AddKey(ModelQueryKeyColumn modelQueryKey);

        public abstract void AddMeasure(ModelQueryMeasureColumn modelQueryMeasure);

        public abstract void QueryData(CancellationToken cancellationToken);
    }
}
