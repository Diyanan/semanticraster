using Semantic.Engine;
using System;
using System.Collections.Generic;

namespace Semantic.Controls
{
    public class RangeFilterViewModel : FilterViewModelBase
    {
        private static string PropertyMin = "Min";
        private static string PropertyMax = "Max";
        private static string PropertySelectedMin = "SelectedMin";
        private static string PropertySelectedMax = "SelectedMax";
        private static string PropertyIsIntegerRange = "IsIntegerRange";
        private static string PropertyRangeStepSize = "RangeStepSize";
        private bool replace = true;
        private int maxRangeSteps = 100;
        private NumericRangeFilterClause numericRangeFilterClause;
        private double? min;
        private double? max;
        private double? selectedMin;
        private double? selectedMax;
        private bool isIntegerRange;
        private double rangeStepSize;

        public double? Min
        {
            get
            {
                return this.min;
            }
            set
            {
                this.SetProperty<double?>(PropertyMin, ref this.min, value, false);
            }
        }

        public double? Max
        {
            get
            {
                return this.max;
            }
            set
            {
                this.SetProperty<double?>(PropertyMax, ref this.max, value, false);
            }
        }

        public double? SelectedMin
        {
            get
            {
                return this.selectedMin;
            }
            set
            {
                this.SetProperty<double?>(PropertySelectedMin, ref this.selectedMin, 
                    RoundRangeValue(value, this.Min, this.Max, this.IsIntegerRange), false);
            }
        }

        public double? SelectedMax
        {
            get
            {
                return this.selectedMax;
            }
            set
            {
                this.SetProperty<double?>(PropertySelectedMax, ref this.selectedMax, 
                    RoundRangeValue(value, this.Min, this.Max, this.IsIntegerRange), false);
            }
        }

        public bool IsIntegerRange
        {
            get
            {
                return this.isIntegerRange;
            }
            set
            {
                this.SetProperty<bool>(PropertyIsIntegerRange, ref this.isIntegerRange, value, false);
            }
        }

        public double RangeStepSize
        {
            get
            {
                return this.rangeStepSize;
            }
            set
            {
                this.SetProperty<double>(PropertyRangeStepSize, ref this.rangeStepSize, value, false);
            }
        }

        public override bool IsDefault
        {
            get
            {
                return this.numericRangeFilterClause.Unfiltered;
            }
        }

        public RangeFilterViewModel(LayerViewModel layerVm, NumericRangeFilterClause filterClause)
        {
            this.layerViewModel = layerVm;
            this.numericRangeFilterClause = filterClause;
            this.numericRangeFilterClause.OnPropertiesUpdated += new EventHandler(this.filterClause_OnPropertiesUpdated);
            this.Init(true);
        }

        public override void Delete()
        {
            this.numericRangeFilterClause.OnPropertiesUpdated -= new EventHandler(this.filterClause_OnPropertiesUpdated);
            this.layerViewModel.RemoveFilterClause(this.numericRangeFilterClause);
        }

        protected override void Clear()
        {
            this.replace = false;
            this.SelectedMin = this.Min;
            this.SelectedMax = this.Max;
            this.replace = true;
            this.ReplaceFilterClause(true);
            this.Init(false);
        }

        protected override void AfterPropertyChange<T>(string propertyName, ref T property, T newValue)
        {
            if (!this.replace || !propertyName.Equals(PropertySelectedMin) && !propertyName.Equals(PropertySelectedMax))
                return;
            this.replace = false;
            if (this.Min.HasValue)
                this.SelectedMin = new double?(Math.Max(this.SelectedMin ?? this.Min.Value, this.Min.Value));
            if (this.Max.HasValue)
                this.SelectedMax = new double?(Math.Min(this.SelectedMax ?? this.Max.Value, this.Max.Value));
            this.ReplaceFilterClause(false);
            this.replace = true;
        }

        private void ReplaceFilterClause(bool clear)
        {
            NumericRangeFilterClause oldFilterClause = this.numericRangeFilterClause;
            oldFilterClause.OnPropertiesUpdated -= new EventHandler(this.filterClause_OnPropertiesUpdated);
            this.numericRangeFilterClause = new NumericRangeFilterClause(oldFilterClause, clear ? new double?() : this.SelectedMin, clear ? new double?() : this.SelectedMax);
            this.numericRangeFilterClause.OnPropertiesUpdated += new EventHandler(this.filterClause_OnPropertiesUpdated);
            this.layerViewModel.ReplaceFilterClause(oldFilterClause, this.numericRangeFilterClause);
            this.UpdateDescriptionToAllOrFiltered();
            this.RaisePropertyChanged(FilterViewModelBase.PropertyIsDefault);
        }

        private void filterClause_OnPropertiesUpdated(object sender, EventArgs e)
        {
            if (!object.ReferenceEquals(sender, this.numericRangeFilterClause))
                return;
            this.Init(false);
        }

        private void Init(bool isFirstRun = true)
        {
            this.replace = false;
            this.IsIntegerRange = this.numericRangeFilterClause.IsIntegerRange;
            double? min;
            double? max;
            double allowedMin;
            double allowedMax;
            if (this.numericRangeFilterClause.TryGetProperties(out min, out max, out allowedMin, out allowedMax))
            {
                this.IsEnabled = true;
                this.Min = new double?(allowedMin);
                this.Max = new double?(allowedMax);
                this.RangeStepSize = this.IsIntegerRange ? Math.Ceiling((allowedMax - allowedMin) / this.maxRangeSteps) : (allowedMax - allowedMin) / this.maxRangeSteps;
                this.SelectedMin = min.HasValue ? min : this.Min;
                this.SelectedMax = max.HasValue ? max : this.Max;
                this.UpdateDescriptionToAllOrFiltered();
            }
            else
            {
                this.SelectedMin = this.SelectedMax = this.Min = this.Max = new double?();
                this.Description = isFirstRun ? Resources.FiltersTab_StatusProcessing : Resources.FiltersTab_StatusInvalidRange;
                this.RangeStepSize = 0.0;
                this.IsEnabled = false;
            }
            this.RaisePropertyChanged(PropertyIsDefault);
            this.replace = true;
        }

        internal override FilterViewModelBase GetNextCompatibleFilterForFunction(AggregationFunction previous)
        {
            if (previous == this.Function)
                return this;
            FilterClause filterClause = null;
            if (this.Function != AggregationFunction.None)
            {
                filterClause = new NumericRangeFilterClause(this.numericRangeFilterClause.TableMember, this.Function, new double?(), new double?());
            }
            else
            {
                switch (this.DataType)
                {
                    case TableMemberDataType.Unknown:
                        break;
                    case TableMemberDataType.String:
                        filterClause = new CategoryFilterClause<string>(this.numericRangeFilterClause.TableMember, this.Culture, null, false, false);
                        break;
                    case TableMemberDataType.Bool:
                        filterClause = new CategoryFilterClause<bool>(this.numericRangeFilterClause.TableMember, this.Culture, null, false, false);
                        break;
                    case TableMemberDataType.Double:
                    case TableMemberDataType.Long:
                    case TableMemberDataType.Currency:
                        filterClause = new NumericRangeFilterClause(this.numericRangeFilterClause.TableMember, this.Function, new double?(), new double?());
                        break;
                    case TableMemberDataType.DateTime:
                        filterClause = new AndOrFilterClause(this.numericRangeFilterClause.TableMember, this.Function, FilterPredicateOperator.And, new DateTimeFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<DateTimeFilterPredicateComparison>(), new DateTime?()), new DateTimeFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<DateTimeFilterPredicateComparison>(), new DateTime?()));
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            if (filterClause == null)
                return null;
            this.numericRangeFilterClause.OnPropertiesUpdated -= new EventHandler(this.filterClause_OnPropertiesUpdated);
            this.layerViewModel.ReplaceFilterClause(this.numericRangeFilterClause, filterClause);
            return Get(this.layerViewModel, filterClause, false);
        }

        internal override FilterViewModelBase GetNextCompatibleFilter()
        {
            FilterClause filterClause = null;
            if (this.Function == AggregationFunction.None)
            {
                switch (this.DataType)
                {
                    case TableMemberDataType.Unknown:
                    case TableMemberDataType.String:
                    case TableMemberDataType.Bool:
                    case TableMemberDataType.DateTime:
                        break;
                    case TableMemberDataType.Double:
                    case TableMemberDataType.Long:
                    case TableMemberDataType.Currency:
                        filterClause = new CategoryFilterClause<double>(this.numericRangeFilterClause.TableMember, this.Culture, null, false, false);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            else
                filterClause = new AndOrFilterClause(this.numericRangeFilterClause.TableMember, this.Function, FilterPredicateOperator.And, new NumericFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<NumericFilterPredicateComparison>(), new double?()), new NumericFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<NumericFilterPredicateComparison>(), new double?()));
            if (filterClause == null)
                return null;
            this.numericRangeFilterClause.OnPropertiesUpdated -= new EventHandler(this.filterClause_OnPropertiesUpdated);
            this.layerViewModel.ReplaceFilterClause(this.numericRangeFilterClause, filterClause);
            return Get(this.layerViewModel, filterClause, false);
        }

        public static double? RoundRangeValue(double? value, double? Minimum, double? Maximum, bool noDecimals)
        {
            if (!value.HasValue)
                return new double?();
            if (!Maximum.HasValue || !Minimum.HasValue)
                return value;
            double? nullable1 = value;
            double? nullable2 = Maximum;
            if ((nullable1.GetValueOrDefault() != nullable2.GetValueOrDefault() ? 0 : (nullable1.HasValue == nullable2.HasValue ? 1 : 0)) == 0)
            {
                double? nullable3 = value;
                double? nullable4 = Minimum;
                if ((nullable3.GetValueOrDefault() != nullable4.GetValueOrDefault() ? 0 : (nullable3.HasValue == nullable4.HasValue ? 1 : 0)) == 0)
                {
                    double num1 = value.Value;
                    try
                    {
                        if (noDecimals)
                        {
                            num1 = Math.Round(value.Value, MidpointRounding.AwayFromZero);
                        }
                        else
                        {
                            int num2 = (int)Math.Ceiling(2.0 - Math.Log10(Maximum.Value - Minimum.Value));
                            num1 = Math.Round(value.Value, num2 < 0 ? 0 : num2, MidpointRounding.AwayFromZero);
                            double num3 = num1;
                            double? nullable5 = Maximum;
                            if ((num3 <= nullable5.GetValueOrDefault() ? 0 : (nullable5.HasValue ? 1 : 0)) != 0)
                                num1 = Maximum.Value;
                            double num4 = num1;
                            double? nullable6 = Minimum;
                            if ((num4 >= nullable6.GetValueOrDefault() ? 0 : (nullable6.HasValue ? 1 : 0)) != 0)
                                num1 = Minimum.Value;
                        }
                    }
                    catch
                    {
                    }
                    return new double?(num1);
                }
            }
            return value;
        }
    }
}
