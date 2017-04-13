using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Globalization;
using System.Windows.Input;
using System.Windows.Threading;

namespace Semantic.Controls
{
    public abstract class FilterViewModelBase : ViewModelBase
    {
        private static readonly AggregationFunction[] DefaultFunctions = new AggregationFunction[3]
        {
            AggregationFunction.None,
            AggregationFunction.Count,
            AggregationFunction.DistinctCount
        };
        private static readonly AggregationFunction[] UserDefinedFunctions = new AggregationFunction[1]
        {
            AggregationFunction.UserDefined
        };
        private static readonly AggregationFunction[] NumericFunctions = new AggregationFunction[7]
        {
            AggregationFunction.None,
            AggregationFunction.Sum,
            AggregationFunction.Average,
            AggregationFunction.Count,
            AggregationFunction.DistinctCount,
            AggregationFunction.Max,
            AggregationFunction.Min
        };
        public static string PropertyName = "Name";
        public static string PropertyIsExpanded = "IsExpanded";
        public static string PropertyDescription = "Description";
        public static string PropertyModeTooltip = "ModeTooltip";
        public static string PropertyFunction = "Function";
        public static string PropertyFunctionTooltip = "FunctionTooltip";
        public static string PropertyIsEnabled = "IsEnabled";
        public static string PropertyIsDefault = "IsDefault";
        private string description = Resources.FiltersTab_StatusProcessing;
        protected LayerViewModel layerViewModel;
        protected Dispatcher UIDispatcher;
        public ChangeAggregationFunction ChangeAggregationFunctionCallback;
        private string name;
        private bool isExpanded;
        private string modeTooltip;
        private AggregationFunction function;
        private string functionTooltip;
        private bool isEnabled;

        public ICommand DeleteCommand { get; set; }

        public ICommand ClearCommand { get; set; }

        public ICommand ModeCommand { get; set; }

        public ICommand ApplyCommand { get; set; }

        public string Name
        {
            get
            {
                return this.name;
            }
            set
            {
                this.SetProperty<string>(PropertyName, ref this.name, value, false);
            }
        }

        public bool IsExpanded
        {
            get
            {
                return this.isExpanded;
            }
            set
            {
                this.SetProperty<bool>(PropertyIsExpanded, ref this.isExpanded, value, false);
            }
        }

        public string Description
        {
            get
            {
                return this.description;
            }
            set
            {
                this.SetProperty<string>(PropertyDescription, ref this.description, value, false);
            }
        }

        public string ModeTooltip
        {
            get
            {
                return this.modeTooltip;
            }
            set
            {
                this.SetProperty<string>(PropertyModeTooltip, ref this.modeTooltip, value, false);
            }
        }

        public AggregationFunction Function
        {
            get
            {
                return this.function;
            }
            set
            {
                AggregationFunction previous = this.function;
                if (!this.SetProperty<AggregationFunction>(PropertyFunction, ref this.function, value, false) || this.ChangeAggregationFunctionCallback == null)
                    return;
                this.ChangeAggregationFunctionCallback(this, previous, this.Function);
            }
        }

        public string FunctionTooltip
        {
            get
            {
                return this.functionTooltip;
            }
            set
            {
                this.SetProperty<string>(PropertyFunctionTooltip, ref this.functionTooltip, value, false);
            }
        }

        public TableMemberDataType DataType { get; set; }

        public CultureInfo Culture { get; set; }

        public AggregationFunction[] Functions { get; private set; }

        public bool IsEnabled
        {
            get
            {
                return this.isEnabled;
            }
            set
            {
                this.SetProperty<bool>(PropertyIsEnabled, ref this.isEnabled, value, false);
            }
        }

        public abstract bool IsDefault { get; }

        public virtual bool IsModeChangeSupported
        {
            get
            {
                return true;
            }
        }

        protected FilterViewModelBase()
        {
            this.UIDispatcher = Dispatcher.CurrentDispatcher;
            this.ApplyCommand = new DelegatedCommand(new Action(this.ApplyFilter), new Predicate(this.CanApplyFilter));
            this.ClearCommand = new DelegatedCommand(new Action(this.ClearFilter));
        }

        internal static FilterViewModelBase Get(LayerViewModel layer, FilterClause clause, bool add = false)
        {
            FilterViewModelBase result = null;

            if (clause is NumericRangeFilterClause)
            {
                RangeFilterViewModel rangeFilterViewModel = new RangeFilterViewModel(layer, clause as NumericRangeFilterClause);
                rangeFilterViewModel.ModeTooltip = 
                    clause.AggregationFunction == AggregationFunction.None ? 
                    Resources.FiltersTab_ListFilterModeTooltip : 
                    Resources.FiltersTab_AdvancedFilterModeTooltip;
                result = rangeFilterViewModel;
            }
            else if (clause is AndOrFilterClause)
            {
                ExpressionFilterViewModel expressionFilterViewModel = new ExpressionFilterViewModel(layer, clause as AndOrFilterClause);
                expressionFilterViewModel.ModeTooltip = 
                    clause.AggregationFunction != AggregationFunction.None || 
                    clause.TableMember.DataType != TableMemberDataType.String && 
                    clause.TableMember.DataType != TableMemberDataType.DateTime ? 
                    Resources.FiltersTab_RangeFilterModeTooltip : 
                    Resources.FiltersTab_ListFilterModeTooltip;
                result = expressionFilterViewModel;
            }
            else if (clause is CategoryFilterClause<string>)
            {
                ListFilterViewModel<string> listFilterViewModel = new ListFilterViewModel<string>(layer, clause as CategoryFilterClause<string>);
                listFilterViewModel.ModeTooltip = Resources.FiltersTab_AdvancedFilterModeTooltip;
                result = listFilterViewModel;
            }
            else if (clause is CategoryFilterClause<bool>)
            {
                ListFilterViewModel<bool> listFilterViewModel = new ListFilterViewModel<bool>(layer, clause as CategoryFilterClause<bool>);
                listFilterViewModel.ModeTooltip = Resources.FiltersTab_ListFilterModeTooltip;
                result = listFilterViewModel;
            }
            else if (clause is CategoryFilterClause<DateTime>)
            {
                ListFilterViewModel<DateTime> listFilterViewModel = new ListFilterViewModel<DateTime>(layer, clause as CategoryFilterClause<DateTime>);
                listFilterViewModel.ModeTooltip = Resources.FiltersTab_AdvancedFilterModeTooltip;
                result = listFilterViewModel;
            }
            else if (clause is CategoryFilterClause<double>)
            {
                ListFilterViewModel<double> listFilterViewModel = new ListFilterViewModel<double>(layer, clause as CategoryFilterClause<double>);
                listFilterViewModel.ModeTooltip = Resources.FiltersTab_AdvancedFilterModeTooltip;
                result = listFilterViewModel;
            }

            if (result != null)
            {
                result.DataType = clause.TableMember.DataType;
                result.Function = clause.AggregationFunction;
                result.Functions = GetAggregationFunctions(clause.TableMember);
                result.Name = GetName(clause.TableMember.Name, clause.AggregationFunction);
                if (add)
                {
                    layer.AddFilterClause(clause);
                }
            }
            return result;
        }

        public abstract void Delete();

        internal abstract FilterViewModelBase GetNextCompatibleFilter();

        internal abstract FilterViewModelBase GetNextCompatibleFilterForFunction(AggregationFunction previous);

        protected virtual void ApplyFilter()
        {
        }

        protected virtual bool CanApplyFilter()
        {
            return this.IsEnabled;
        }

        protected abstract void Clear();

        private void ClearFilter()
        {
            if (this.IsDefault)
                return;
            this.Clear();
        }

        protected void UpdateDescriptionToAllOrFiltered()
        {
            this.Description = this.IsDefault ? Resources.FiltersTab_StatusAll : Resources.FiltersTab_StatusFiltered;
        }

        private static string GetName(string name, AggregationFunction function)
        {
            switch (function)
            {
                case AggregationFunction.None:
                case AggregationFunction.UserDefined:
                    return name;
                case AggregationFunction.Sum:
                    return string.Format(Resources.FiltersTab_FieldSumName, name);
                case AggregationFunction.Average:
                    return string.Format(Resources.FiltersTab_FieldAvgName, name);
                case AggregationFunction.Count:
                case AggregationFunction.DistinctCount:
                    return string.Format(Resources.FiltersTab_FieldCountName, name);
                case AggregationFunction.Min:
                    return string.Format(Resources.FiltersTab_FieldMinName, name);
                case AggregationFunction.Max:
                    return string.Format(Resources.FiltersTab_FieldMaxName, name);
                default:
                    return null;
            }
        }

        private static AggregationFunction[] GetAggregationFunctions(TableMember tableMember)
        {
            if (tableMember is TableMeasure)
                return UserDefinedFunctions;
            switch (tableMember.DataType)
            {
                case TableMemberDataType.Unknown:
                    return null;
                case TableMemberDataType.String:
                case TableMemberDataType.Bool:
                case TableMemberDataType.DateTime:
                    return DefaultFunctions;
                case TableMemberDataType.Double:
                case TableMemberDataType.Long:
                case TableMemberDataType.Currency:
                    return NumericFunctions;
                default:
                    return null;
            }
        }

        public delegate void ChangeAggregationFunction(FilterViewModelBase filterViewModel, AggregationFunction previous, AggregationFunction current);
    }
}
