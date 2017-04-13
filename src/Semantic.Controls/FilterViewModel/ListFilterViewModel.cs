using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public abstract class ListFilterViewModel : FilterViewModelBase
    {
        public static string PropertySearchString = "SearchString";
        public static string PropertyIsListGreaterThanMaxShown = "IsListGreaterThanMaxShown";
        public static string PropertyShowingSeachResults = "ShowingSearchResults";
        public static string PropertyInSearchMode = "InSearchMode";
        public static string PropertyIsSearchEnabled = "IsSearchEnabled";
        private string searchString = Resources.FiltersTab_FilterSearchText;
        protected const int MaxShown = 50;
        private bool isListGreaterThanMaxShown;
        private bool showingSearchResults;
        private bool inSearchMode;

        public ICommand SearchCommand { get; set; }

        public ICommand ClearSearchCommand { get; set; }

        public string SearchString
        {
            get
            {
                return this.searchString;
            }
            set
            {
                if (!this.SetProperty<string>(PropertySearchString, ref this.searchString, value, false) || !this.InSearchMode)
                    return;
                this.OnSearch();
            }
        }

        public bool IsSearchSupported { get; protected set; }

        public bool IsListGreaterThanMaxShown
        {
            get
            {
                return this.isListGreaterThanMaxShown;
            }
            set
            {
                if (!this.SetProperty<bool>(PropertyIsListGreaterThanMaxShown, ref this.isListGreaterThanMaxShown, value, false))
                    return;
                this.RaisePropertyChanged(PropertyIsSearchEnabled);
            }
        }

        public bool ShowingSearchResults
        {
            get
            {
                return this.showingSearchResults;
            }
            set
            {
                this.SetProperty<bool>(PropertyShowingSeachResults, ref this.showingSearchResults, value, false);
            }
        }

        public bool InSearchMode
        {
            get
            {
                return this.inSearchMode;
            }
            set
            {
                if (!this.SetProperty<bool>(PropertyInSearchMode, ref this.inSearchMode, value, false))
                    return;
                this.RaisePropertyChanged(PropertyIsSearchEnabled);
                this.SearchString = this.inSearchMode ? string.Empty : Resources.FiltersTab_FilterSearchText;
            }
        }

        public bool IsSearchEnabled
        {
            get
            {
                if (!this.IsListGreaterThanMaxShown || !this.IsSearchSupported)
                    return this.InSearchMode;
                else
                    return true;
            }
        }

        public abstract override bool IsDefault { get; }

        internal abstract override FilterViewModelBase GetNextCompatibleFilterForFunction(AggregationFunction previous);

        protected abstract void OnSearch();
    }

    public class ListFilterViewModel<T> : ListFilterViewModel where T : IComparable<T>
    {
        private BitArray selectedItems = new BitArray(1);
        private bool replace = true;
        private bool isAllSpecified;
        private bool isBlankSpecified;
        private bool showBlank;
        private IEnumerable<T> allItems;
        private CategoryFilterClause<T> categoryFilterClause;

        public ObservableCollectionEx<ListFilterItemViewModel<T>> Fields { get; private set; }

        public override bool IsModeChangeSupported
        {
            get
            {
                return typeof(T) != typeof(bool);
            }
        }

        public override bool IsDefault
        {
            get
            {
                return this.categoryFilterClause.Unfiltered;
            }
        }

        public ListFilterViewModel(LayerViewModel fieldListPickerVm, CategoryFilterClause<T> clause)
        {
            this.IsEnabled = false;
            if (typeof(T) == typeof(string))
            {
                this.SearchCommand = new DelegatedCommand(new Action(this.OnSearch), () =>
              {
                  if (!string.IsNullOrEmpty(this.SearchString))
                      return this.InSearchMode;
                  else
                      return false;
              });
                this.ClearSearchCommand = new DelegatedCommand(new Action(this.ClearSearch), () => this.ShowingSearchResults);
                this.IsSearchSupported = true;
            }
            this.layerViewModel = fieldListPickerVm;
            this.Fields = new ObservableCollectionEx<ListFilterItemViewModel<T>>();
            this.Fields.ItemPropertyChanged += new ObservableCollectionExItemChangedHandler<ListFilterItemViewModel<T>>(this.FieldsOnItemPropertyChanged);
            this.categoryFilterClause = clause;
            this.categoryFilterClause.OnPropertiesUpdated += new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
            this.categoryFilterClause_OnPropertiesUpdated(this.categoryFilterClause, EventArgs.Empty);
        }

        protected override void OnSearch()
        {
            this.replace = false;
            this.Fields.RemoveAll();
            long count = 0L;
            long maxThusFar = -1L;
            SortedSet<long> results = new SortedSet<long>();
            string searchString = this.SearchString;

            Parallel.ForEach<T>(this.allItems, (f, pls, i) =>
            {
                if (pls.IsStopped && i > maxThusFar)
                    return;
                int num1 = this.Culture.CompareInfo.IndexOf(f.ToString(), searchString, 0, CompareOptions.IgnoreCase);
                if (num1 >= 0)
                {
                    lock (results)
                 results.Add(i);
                }
                int num2 = 100;
                long comparand;
                do
                {
                    comparand = Volatile.Read(ref maxThusFar);
                }
                while (i > comparand && Interlocked.CompareExchange(ref maxThusFar, i, comparand) != comparand && --num2 > 0);
                if (num1 < 0 || Interlocked.Increment(ref count) != 50L)
                    return;
                pls.Stop();
            });
            int num = 0;
            foreach (int index in results.Take(51))
            {
                if (++num != 51)
                {
                    T obj = this.allItems.ElementAt(index);
                    this.Fields.Add(new ListFilterItemViewModel<T>()
                    {
                        IsSelected = new bool?(this.selectedItems[index]),
                        Item = obj,
                        Name = obj.ToString(),
                        Index = index
                    });
                }
                else
                    break;
            }
            this.IsListGreaterThanMaxShown = num == 51;
            this.ShowingSearchResults = true;
            this.replace = true;
        }

        public void ClearSearch()
        {
            this.replace = false;
            this.InSearchMode = false;
            this.ShowingSearchResults = false;
            this.PopulateInitialFields();
            this.replace = true;
        }

        public override void Delete()
        {
            this.categoryFilterClause.OnPropertiesUpdated -= new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
            this.layerViewModel.RemoveFilterClause(this.categoryFilterClause);
        }

        protected override void Clear()
        {
            this.isAllSpecified = this.isBlankSpecified = this.showBlank = false;
            this.selectedItems.SetAll(false);
            this.Fields.RemoveAll();
            CategoryFilterClause<T> oldFilterClause = this.categoryFilterClause;
            oldFilterClause.OnPropertiesUpdated -= new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
            this.categoryFilterClause = new CategoryFilterClause<T>(oldFilterClause, null, false, false);
            this.categoryFilterClause.OnPropertiesUpdated += new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
            this.layerViewModel.ReplaceFilterClause(oldFilterClause, this.categoryFilterClause);
            this.categoryFilterClause_OnPropertiesUpdated(this.categoryFilterClause, EventArgs.Empty);
        }

        private void categoryFilterClause_OnPropertiesUpdated(object sender, EventArgs e)
        {
            if (!object.ReferenceEquals(sender, this.categoryFilterClause))
                return;
            this.replace = false;
            IEnumerable<T> selectableItems;
            BitArray selectedIndices;
            bool showBlank;
            bool blankSpecified;
            bool allSpecified;
            if (!this.categoryFilterClause.TryGetProperties(out selectableItems, out selectedIndices, out showBlank, out blankSpecified, out allSpecified))
                return;
            this.selectedItems = selectedIndices;
            this.allItems = selectableItems;
            this.isAllSpecified = allSpecified;
            this.isBlankSpecified = blankSpecified;
            this.showBlank = showBlank;
            this.UIDispatcher.Invoke(() =>
           {
               this.InSearchMode = this.ShowingSearchResults = this.IsListGreaterThanMaxShown = false;
               this.PopulateInitialFields();
               this.IsEnabled = true;
           });
            this.replace = true;
            this.UpdateDescriptionToAllOrFiltered();
            this.RaisePropertyChanged(PropertyIsDefault);
        }

        private void PopulateInitialFields()
        {
            this.Fields.RemoveAll();
            this.Fields.Add(new ListFilterItemViewModel<T>()
            {
                Item = default(T),
                Name = Resources.FiltersTab_FilterAll,
                IsAll = true,
                IsSelected = this.IsAllSelected(),
                Index = -1
            });
            if (this.showBlank)
                this.Fields.Add(new ListFilterItemViewModel<T>()
                {
                    Item = default(T),
                    Name = Resources.FiltersTab_FilterBlank,
                    IsBlank = true,
                    IsSelected = new bool?(this.IsBlankSelected()),
                    Index = -1
                });
            int index = 0;
            foreach (T obj in this.allItems.Take(50))
            {
                this.IsListGreaterThanMaxShown = index == 49;
                if (index == 49)
                    break;
                this.Fields.Add(new ListFilterItemViewModel<T>()
                {
                    IsSelected = new bool?(this.selectedItems[index]),
                    Item = obj,
                    Name = obj.ToString(),
                    Index = index
                });
                ++index;
            }
        }

        private bool? IsAllSelected()
        {
            return
                // 没有全选 并且 有选中的的
                !this.isAllSpecified && this.selectedItems.Cast<bool>().Any(si => si) ||
                // 全选 并且 有没选中的
                this.isAllSpecified && this.selectedItems.Cast<bool>().Any(si => !si) ||
                // 如果显示空数据，则 返回 null；否则，如果全选，返回真；否则返回假。
                this.isBlankSpecified ? new bool?() :
                (!this.isAllSpecified ? new bool?(false) : new bool?(true));
        }

        private bool IsBlankSelected()
        {
            if (this.isBlankSpecified && !this.isAllSpecified)
                return true;
            if (!this.isBlankSpecified)
                return this.isAllSpecified;
            else
                return false;
        }

        private void FieldsOnItemPropertyChanged(ListFilterItemViewModel<T> item, PropertyChangedEventArgs propertyChangedEventArgs)
        {
            if (!this.replace || !propertyChangedEventArgs.PropertyName.Equals(ListFilterItemViewModel<T>.PropertyIsSelected))
                return;
            this.replace = false;
            CategoryFilterClause<T> oldFilterClause1 = this.categoryFilterClause;
            oldFilterClause1.OnPropertiesUpdated -= new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
            if (item.IsAll)
            {
                ListFilterItemViewModel<T> filterItemViewModel1 = item;
                bool? isSelected = item.IsSelected;
                bool? nullable = new bool?(isSelected.HasValue && isSelected.GetValueOrDefault());
                filterItemViewModel1.IsSelected = nullable;
                this.isAllSpecified = item.IsSelected.Value;
                this.selectedItems.SetAll(this.isAllSpecified);
                this.isBlankSpecified = false;
                foreach (ListFilterItemViewModel<T> filterItemViewModel2 in this.Fields.Skip(1))
                {
                    filterItemViewModel2.IsSelected = new bool?(this.isAllSpecified);
                }
                CategoryFilterClause<T> oldFilterClause2 = oldFilterClause1;
                this.categoryFilterClause = new CategoryFilterClause<T>(categoryFilterClause, null, this.isBlankSpecified, isAllSpecified);
            }
            else if (item.IsSelected.HasValue)
            {
                if (!item.IsBlank)
                    this.selectedItems[item.Index] = item.IsSelected.Value;
                else
                    this.isBlankSpecified = item.IsSelected.Value && !this.isAllSpecified || !item.IsSelected.Value && this.isAllSpecified;
                if (this.Fields[0].IsAll)
                    this.Fields[0].IsSelected = this.IsAllSelected();
                this.categoryFilterClause = new CategoryFilterClause<T>(oldFilterClause1, this.allItems.Where((t, i) =>
                {
                    if (!this.isAllSpecified)
                        return this.selectedItems[i];
                    else
                        return !this.selectedItems[i];
                }), this.isBlankSpecified, this.isAllSpecified);
            }
            if (oldFilterClause1 != this.categoryFilterClause)
            {
                this.categoryFilterClause.OnPropertiesUpdated += new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
                this.layerViewModel.ReplaceFilterClause(oldFilterClause1, this.categoryFilterClause);
                this.UpdateDescriptionToAllOrFiltered();
                this.RaisePropertyChanged(PropertyIsDefault);
            }
            this.replace = true;
        }

        internal override FilterViewModelBase GetNextCompatibleFilterForFunction(AggregationFunction previous)
        {
            if (previous == this.Function)
                return this;
            if (this.Function == AggregationFunction.None)
                return null;
            NumericRangeFilterClause rangeFilterClause = new NumericRangeFilterClause(this.categoryFilterClause.TableMember, this.Function, new double?(), new double?());
            this.categoryFilterClause.OnPropertiesUpdated -= new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
            this.layerViewModel.ReplaceFilterClause(this.categoryFilterClause, rangeFilterClause);
            return Get(this.layerViewModel, rangeFilterClause, false);
        }

        internal override FilterViewModelBase GetNextCompatibleFilter()
        {
            FilterClause filterClause = null;
            if (this.Function == AggregationFunction.None)
            {
                switch (this.DataType)
                {
                    case TableMemberDataType.Unknown:
                    case TableMemberDataType.Bool:
                        break;
                    case TableMemberDataType.String:
                        filterClause = new AndOrFilterClause(this.categoryFilterClause.TableMember, this.Function, FilterPredicateOperator.And, new StringFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<StringFilterPredicateComparison>(), (string)null), new StringFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<StringFilterPredicateComparison>(), (string)null));
                        break;
                    case TableMemberDataType.Double:
                    case TableMemberDataType.Long:
                    case TableMemberDataType.Currency:
                        filterClause = new AndOrFilterClause(this.categoryFilterClause.TableMember, this.Function, FilterPredicateOperator.And, new NumericFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<NumericFilterPredicateComparison>(), new double?()), new NumericFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<NumericFilterPredicateComparison>(), new double?()));
                        break;
                    case TableMemberDataType.DateTime:
                        filterClause = new AndOrFilterClause(this.categoryFilterClause.TableMember, this.Function, FilterPredicateOperator.And, new DateTimeFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<DateTimeFilterPredicateComparison>(), new DateTime?()), new DateTimeFilterPredicate(ExpressionFilterViewModel.GetDefaultPredicate<DateTimeFilterPredicateComparison>(), new DateTime?()));
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            if (filterClause == null)
                return null;
            this.categoryFilterClause.OnPropertiesUpdated -= new EventHandler(this.categoryFilterClause_OnPropertiesUpdated);
            this.layerViewModel.ReplaceFilterClause(this.categoryFilterClause, filterClause);
            return Get(this.layerViewModel, filterClause, false);
        }
    }
}
