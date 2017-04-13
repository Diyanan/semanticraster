using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class LayerViewModel : ViewModelBase
    {
        private string _DefaultSampleClassField = "Sample";
        private string _DefaultClassField = "Zone";
        private bool _Visible = true;
        private string _Name;
        private LayerDefinition _LayerDefinition;
        private ICommand _DeleteLayerCommand;
        private ICommand _LayerSettingsCommand;

        public string PropertyName
        {
            get
            {
                return "Name";
            }
        }

        public string Name
        {
            get
            {
                return this._Name;
            }
            set
            {
                if (!this.SetProperty(this.PropertyName, ref this._Name, value, false))
                    return;
                if (this.LayerDefinition != null)
                    this.LayerDefinition.Name = value;
            }
        }

        public string PropertyLayerDefinition
        {
            get
            {
                return "LayerDefinition";
            }
        }

        public LayerDefinition LayerDefinition
        {
            get
            {
                return this._LayerDefinition;
            }
            private set
            {
                this.SetProperty(this.PropertyLayerDefinition, ref this._LayerDefinition, value, false);
                this.Name = value.Name;
            }
        }

        public string PropertyVisible
        {
            get
            {
                return "Visible";
            }
        }

        public bool Visible
        {
            get
            {
                return this._Visible;
            }
            set
            {
                this.SetProperty(this.PropertyVisible, ref this._Visible, value, false);
                // 设置图层显隐
            }
        }

        public string PropertyDeleteLayerCommand
        {
            get
            {
                return "DeleteLayerCommand";
            }
        }

        public ICommand DeleteLayerCommand
        {
            get
            {
                return this._DeleteLayerCommand;
            }
            set
            {
                this.SetProperty(this.PropertyDeleteLayerCommand, ref this._DeleteLayerCommand, value, false);
            }
        }

        public string PropertyLayerSettingsCommand
        {
            get
            {
                return "LayerSettingsCommand";
            }
        }

        public ICommand LayerSettingsCommand
        {
            get
            {
                return this._LayerSettingsCommand;
            }
            set
            {
                this.SetProperty(this.PropertyLayerSettingsCommand, ref this._LayerSettingsCommand, value, false);
            }
        }

        public IDialogServiceProvider DialogServiceProvider { get; private set; }

        public Action<LayerDefinition, Filter, bool> VisualizeCallback { get; set; }

        public event EventHandler OnFilterClausesChanged;

        public Filter Filter { get; private set; }

        public GeoDataSource GeoData { get; private set; }

        public GeoVisualization GeoVisualization { get; set; }

        public LayerViewModel(LayerDefinition layerDef, IDialogServiceProvider dialogProvider)
        {
            this.LayerDefinition = layerDef;
            this.DialogServiceProvider = dialogProvider;
            this.Filter = new Filter();
            this.GeoData = new GeoDataSource(layerDef);
        }

        internal void SetFilter(Filter filter)
        {
            if (this.Filter.SetFilterClausesFrom(filter))
            {
                this.FilterChanged();
                this.OnExecuteVisualizeCommand(false);
            }
        }

        internal void AddFilterClause(FilterClause filter)
        {
            if (this.Filter.AddFilterClause(filter))
            {
                this.UpdateFilterProperties(filter);
                this.FilterChanged();
                this.OnExecuteVisualizeCommand(false);
            }
        }

        private void UpdateFilterProperties(FilterClause filterClause)
        {
            CancellationToken tocken = new CancellationToken();
            this.GeoData.Filter = this.Filter;
            this.GeoData.QueryData(tocken);
        }

        internal void ReplaceFilterClause(FilterClause filterClause, FilterClause newFilterClause)
        {
            if (this.Filter.ReplaceFilterClause(filterClause, newFilterClause))
            {
                this.UpdateFilterProperties(newFilterClause);
                this.FilterChanged();
                this.OnExecuteVisualizeCommand(false);
            }
        }

        internal void RemoveFilterClause(FilterClause filter)
        {
            if (this.Filter.RemoveFilterClause(filter))
            {
                this.FilterChanged();
                this.OnExecuteVisualizeCommand(false);
            }
        }

        internal void ClearFilter()
        {
            if (this.Filter.RemoveAllFilterClauses())
            {
                this.FilterChanged();
                this.OnExecuteVisualizeCommand(false);
            }
        }

        internal bool IsFiltered()
        {
            return this.Filter.HasFilterClauses;
        }

        private void FilterChanged()
        {
            if (this.OnFilterClausesChanged != null)
            {
                this.OnFilterClausesChanged(this, EventArgs.Empty);
            }
        }

        internal void ApplyClass(LeafNode leaf)
        {
            this.GeoVisualization.UpdateClass(this.Filter, this._DefaultClassField, leaf);
        }

        private void OnExecuteVisualizeCommand(bool zoomToData)
        {
            this.GeoVisualization.SelectByFilter(this.Filter);
        }

        public void SetStyle(GeoClass geoClass, bool first)
        {
            this.GeoVisualization.SetGeoClass(this._DefaultClassField, geoClass, 0);
            this.OnExecuteVisualizeCommand(false);
        }

        public void SetSampleStyle(GeoClass geoClass, bool first)
        {
            this.GeoVisualization.SetGeoClass(this._DefaultSampleClassField, geoClass, 0);
            this.OnExecuteVisualizeCommand(false);
        }

        public void SetStyle(SymbolProperties symbol)
        {
            this.GeoVisualization.SetStyle(symbol);
        }

    }
}
