namespace Semantic.Controls
{
    public abstract class ModelQueryColumn
    {
        public TableMember TableColumn { get; set; }

        public dynamic Values { get; set; }

        public bool FetchValues { get; set; }

        public ModelQueryColumn AliasOf { get; set; }

        public virtual void Reset()
        {
            this.Values = null;
            this.AliasOf = null;
        }

        internal virtual void Shutdown()
        {
            this.Reset();
        }
    }
}
