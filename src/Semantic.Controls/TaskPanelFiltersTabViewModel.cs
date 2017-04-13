using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class TaskPanelFiltersTabViewModel : ViewModelBase
    {
        private readonly AddFilterSelectionDialogViewModel filterSelectionDialogViewModel;
        private CultureInfo modelCulture;
        private LayerChooserViewModel _model;

        private GeoVisualization _GeoVisualization;

        public string PropertyModel
        {
            get
            {
                return "Model";
            }
        }

        public LayerChooserViewModel Model
        {
            get
            {
                return this._model;
            }
            set
            {
                this.SetProperty<LayerChooserViewModel>(this.PropertyModel, ref this._model, value, false);
            }
        }

        public LayerManagerViewModel LayerManagerVW { get; private set; }

        public ColorPickerViewModel ColorPickerViewModel { get; private set; }
        public GeoClassViewModel GeoClassViewModel { get; private set; }

        public ICommand AddFiltersCommand { get; private set; }

        public ICommand ApplyCommand { get; private set; }

        public ICommand ClearFiltersCommand { get; private set; }

        public ObservableCollectionEx<FilterViewModelBase> Filters { get; private set; }

        public LeafNodeViewModel LeafClass { get; private set; }

        public TaskPanelFiltersTabViewModel(LayerManagerViewModel layerManagerVM)
        {
            this.modelCulture = new CultureInfo("zh-CN");
            this.LayerManagerVW = layerManagerVM;
            this.Model = new LayerChooserViewModel(layerManagerVM, LayerType.Vector);
            this.Model.PropertyChanged += new PropertyChangedEventHandler(this.ModelOnPropertyChanged);

            this.Filters = new ObservableCollectionEx<FilterViewModelBase>();
            this.filterSelectionDialogViewModel = new AddFilterSelectionDialogViewModel(layerManagerVM.LayerManager)
            {
                CreateCommand = new DelegatedCommand(new Action(this.CreateFilters))
            };
            this.AddFiltersCommand = new DelegatedCommand(new Action(this.OnAddFilters), new Predicate(this.CanAddFilters));
            this.ApplyCommand = new DelegatedCommand(new Action(this.OnApplyClass), new Predicate(this.CanApplyClass));
            // geoclass
            string configureFile = AppDomain.CurrentDomain.BaseDirectory + "GeoClass.xml";
            this.GeoClassViewModel = new GeoClassViewModel(configureFile);
            this.GeoClassViewModel.PropertyChanged += GeoClassViewModel_PropertyChanged;

            // color
            this.ColorPickerViewModel = new ColorPickerViewModel(null);
            this.ColorPickerViewModel.PropertyChanged += ColorPickerViewModel_PropertyChanged;
        }

        private void ColorPickerViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == this.ColorPickerViewModel.PropertySelectedColor && this.LeafClass != null)
            {
                this.LeafClass.Color = this.ColorPickerViewModel.SelectedColor;
                if (this.Model.SelectedLayer != null)
                {
                    bool isFirst = this.LeafClass.Leaves != null && this.LeafClass.Leaves.Count > 0;
                    this.Model.SelectedLayer.SetStyle(this.GeoClassViewModel.Model, isFirst);
                }
            }
        }

        private void GeoClassViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == GeoClassViewModel.PropertySelectedItem)
            {
                // 保存，并切换过滤器
                if (this.LeafClass != null)
                {
                    this.LeafClass.Filter.SetFilterClausesFrom(this.Model.SelectedLayer.Filter);
                    this.Model.SelectedLayer.Filter.SetFilterClausesFrom(this.GeoClassViewModel.SelectedItem.Filter);
                    this.RepopulateFilters();
                }

                // 设置为选择的分类叶子节点
                this.LeafClass = this.GeoClassViewModel.SelectedItem;
                this.ColorPickerViewModel.SelectedColor = this.LeafClass.Color;
            }
        }

        private void ModelOnPropertyChanged(object sender, PropertyChangedEventArgs propertyChangedEventArgs)
        {
            if (!propertyChangedEventArgs.PropertyName.Equals(LayerChooserViewModel.PropertySelectedLayer))
                return;
            if (this.Model.SelectedLayer != null)
            {
                this.RepopulateFilters();
            }
            else
            {
                foreach (FilterViewModelBase filter in this.Filters)
                {
                    this.CleanupFilterCallbacks(filter);
                }
                this.Filters.RemoveAll();
            }
        }

        private void FilterClausesCleared()
        {
            foreach (FilterViewModelBase filter in this.Filters)
            {
                this.CleanupFilterCallbacks(filter);
            }
            this.Filters.RemoveAll();
        }

        private void RepopulateFilters()
        {
            this.Filters.Clear();
            int num = 0;
            foreach (FilterClause clause in this.Model.SelectedLayer.Filter.FilterClauses)
            {
                this.AddFilterViewModel(FilterViewModelBase.Get(this.Model.SelectedLayer, clause, false), num++);
            }
        }

        private void OnAddFilters()
        {
            this.filterSelectionDialogViewModel.Initialize(this.Model.SelectedLayer);
            this.Model.SelectedLayer.DialogServiceProvider.ShowDialog(this.filterSelectionDialogViewModel);
        }

        private bool CanAddFilters()
        {
            if (this.Model.SelectedLayer != null &&
                this.Model.SelectedLayer.LayerDefinition != null)
                // 字段数量大于1
                return true;
            else
                return false;
        }

        private void OnApplyClass()
        {
            this.Model.SelectedLayer.ApplyClass(this.LeafClass.Model);
        }

        private bool CanApplyClass()
        {
            return this.Model.SelectedLayer != null &&
                this.Model.SelectedLayer.LayerDefinition != null &&
                this.LeafClass != null &&
                this.Filters.Count() > 0;
        }

        private void CreateFilters()
        {
            this.Model.SelectedLayer.DialogServiceProvider.DismissDialog(this.filterSelectionDialogViewModel);
            foreach (TableIslandViewModel tableIslandViewModel in this.filterSelectionDialogViewModel.FilterCandidates)
            {
                if (tableIslandViewModel.IsEnabled)
                {
                    foreach (TableViewModel tableViewModel in tableIslandViewModel.Tables)
                    {
                        if (tableViewModel.IsEnabled)
                        {
                            foreach (TableFieldViewModel tableFieldViewModel in tableViewModel.Fields)
                            {
                                if (tableFieldViewModel.IsSelected && tableFieldViewModel.IsEnabled)
                                {
                                    TableMember tableMember = (TableMember)tableFieldViewModel.Model;
                                    FilterClause clause = null;
                                    TableMeasure tableMeasure = tableMember as TableMeasure;
                                    if (tableMeasure != null)
                                    {
                                        clause = new NumericRangeFilterClause(tableMeasure, AggregationFunction.UserDefined, new double?(), new double?());
                                    }
                                    else
                                    {
                                        switch (tableFieldViewModel.ColumnDataType)
                                        {
                                            case TableMemberDataType.String:
                                                clause = new CategoryFilterClause<string>(tableMember, Resources.Culture, null, false, false);
                                                break;
                                            case TableMemberDataType.Bool:
                                                clause = new CategoryFilterClause<bool>(tableMember, Resources.Culture, null, false, false);
                                                break;
                                            case TableMemberDataType.Double:
                                            case TableMemberDataType.Long:
                                            case TableMemberDataType.Currency:
                                                clause = new NumericRangeFilterClause(tableMember, AggregationFunction.Sum, new double?(), new double?());
                                                break;
                                            case TableMemberDataType.DateTime:
                                                clause = new AndOrFilterClause(tableMember, AggregationFunction.None, FilterPredicateOperator.And, new DateTimeFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<DateTimeFilterPredicateComparison>(), new DateTime?()), new DateTimeFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<DateTimeFilterPredicateComparison>(), new DateTime?()));
                                                break;
                                        }
                                    }
                                    if (clause != null)
                                        this.AddFilterViewModel(FilterViewModelBase.Get(this.Model.SelectedLayer, clause, true), this.Filters.Count);
                                }
                            }
                        }
                    }
                }
            }
        }

        private bool CanChangeMode(object obj)
        {
            FilterViewModelBase filterViewModelBase = obj as FilterViewModelBase;
            if (filterViewModelBase != null && filterViewModelBase.IsEnabled)
                return filterViewModelBase.IsDefault;
            else
                return false;
        }

        private void ChangeMode(object obj)
        {
            if (!this.CanChangeMode(obj))
                return;
            FilterViewModelBase filter = obj as FilterViewModelBase;
            if (filter == null)
                return;
            FilterViewModelBase compatibleFilter = filter.GetNextCompatibleFilter();
            int index;
            if (compatibleFilter == null || (index = this.Filters.IndexOf(filter)) < 0)
                return;
            this.CleanupFilterCallbacks(filter);
            this.Filters.RemoveAt(index);
            this.AddFilterViewModel(compatibleFilter, index);
        }

        private void AddFilterViewModel(FilterViewModelBase filter, int index)
        {
            filter.IsExpanded = true;
            filter.DeleteCommand = new DelegatedCommand(new Action<object>(this.DeleteFilter), filter);
            filter.ModeCommand = new DelegatedCommand(new Action<object>(this.ChangeMode), filter);
            filter.ChangeAggregationFunctionCallback = new FilterViewModelBase.ChangeAggregationFunction(this.ChangeAggregationFunctionCallback);
            filter.Culture = this.modelCulture;
            if (index >= this.Filters.Count)
                this.Filters.Add(filter);
            else
                this.Filters.Insert(index, filter);
        }

        private void ChangeAggregationFunctionCallback(FilterViewModelBase filterViewModel, AggregationFunction previous, AggregationFunction current)
        {
            if (filterViewModel == null || previous == current)
                return;
            FilterViewModelBase filterForFunction = filterViewModel.GetNextCompatibleFilterForFunction(previous);
            if (filterForFunction == null)
                return;
            int index;
            if ((index = this.Filters.IndexOf(filterViewModel)) < 0)
                return;
            this.CleanupFilterCallbacks(filterViewModel);
            this.Filters.RemoveAt(index);
            this.AddFilterViewModel(filterForFunction, index);
        }

        private void DeleteFilter(object toDelete)
        {
            FilterViewModelBase filter = (FilterViewModelBase)toDelete;
            filter.Delete();
            this.CleanupFilterCallbacks(filter);
            this.Filters.Remove(filter);
        }

        private void CleanupFilterCallbacks(FilterViewModelBase filter)
        {
            filter.DeleteCommand = null;
            filter.ModeCommand = null;
            filter.ChangeAggregationFunctionCallback = null;
        }
    }
}
