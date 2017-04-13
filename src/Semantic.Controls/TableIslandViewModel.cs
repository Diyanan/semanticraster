using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;

namespace Semantic.Controls
{
    public class TableIslandViewModel : ViewModelBase
    {
        private bool _IsEnabled = true;

        public string PropertyIsEnabled
        {
            get
            {
                return "IsEnabled";
            }
        }

        public bool IsEnabled
        {
            get
            {
                return this._IsEnabled;
            }
            set
            {
                base.SetProperty<bool>(this.PropertyIsEnabled, ref this._IsEnabled, value, this.OnIsEnabledChanged);
            }
        }

        public ObservableCollectionEx<TableViewModel> Tables { get; private set; }

        public event TableIslandViewModel.IslandTableFieldSelectedChangedDelegate IslandTableFieldSelectedChanged;

        public TableIslandViewModel()
        {
            this.Tables = new ObservableCollectionEx<TableViewModel>();
            this.Tables.ItemAdded += (ObservableCollectionExChangedHandler<TableViewModel>)(item =>
            {
                item.TableFieldSelectedChanged += this.OnIslandTableFieldSelectedChanged;
                item.IsEnabled = this.IsEnabled;
            });
            this.Tables.ItemRemoved += (ObservableCollectionExChangedHandler<TableViewModel>)(item =>
            {
                item.TableFieldSelectedChanged -= this.OnIslandTableFieldSelectedChanged;
                item.IsEnabled = true;
            });
        }

        public TableIslandViewModel(TableIsland model, bool disableTableMeasures)
            : this()
        {
            foreach (TableMetadata table in model.Tables)
            {
                if (table.Visible)
                    this.Tables.Add(new TableViewModel(table, disableTableMeasures));
            }
        }

        public bool ContainsField(TableField field)
        {
            foreach (TableViewModel tableViewModel in this.Tables)
            {
                foreach (TableFieldViewModel tableFieldViewModel in tableViewModel.Fields)
                {
                    if (tableFieldViewModel.Model == field)
                        return true;
                }
            }
            return false;
        }

        private void OnIslandTableFieldSelectedChanged(TableViewModel table, TableFieldViewModel field)
        {
            if (this.IslandTableFieldSelectedChanged == null)
                return;
            this.IslandTableFieldSelectedChanged(this, table, field);
        }

        private void OnIsEnabledChanged()
        {
            foreach (TableViewModel tableViewModel in this.Tables)
                tableViewModel.IsEnabled = this.IsEnabled;
        }

        public delegate void IslandTableFieldSelectedChangedDelegate(TableIslandViewModel island, TableViewModel table, TableFieldViewModel field);
    }
}
