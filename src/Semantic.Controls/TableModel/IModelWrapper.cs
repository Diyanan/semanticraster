namespace Semantic.Controls
{
    public interface IModelWrapper
    {
        bool ConnectionsDisabled { get; }

        ModelMetadata GetTableMetadata();

        void RefreshAll();
    }
}
