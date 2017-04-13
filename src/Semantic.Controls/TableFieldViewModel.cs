using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class TableFieldViewModel : ViewModelBase
    {
        private bool _IsEnabled = true;
        private string _Name;
        private bool _IsSelected;
        private bool _IsTimeField;
        private bool _IsCategory;
        private TableMemberDataType _ColumnDataType;
        private GeoFieldMappingType _ColumnClassification;

        public static string PropertyName
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
                return _Name;
            }
            set
            {
                SetProperty(PropertyName, ref _Name, value, false);
            }
        }

        public static string PropertyIsSelected
        {
            get
            {
                return "IsSelected";
            }
        }

        public bool IsSelected
        {
            get
            {
                return _IsSelected;
            }
            set
            {
                SetProperty(PropertyIsSelected, ref _IsSelected, value, false);
            }
        }

        public string PropertyIsTimeField
        {
            get
            {
                return "IsTimeField";
            }
        }

        public bool IsTimeField
        {
            get
            {
                return _IsTimeField;
            }
            set
            {
                SetProperty(PropertyIsTimeField, ref _IsTimeField, value, false);
            }
        }

        public string PropertyIsCategory
        {
            get
            {
                return "IsCategory";
            }
        }

        public bool IsCategory
        {
            get
            {
                return _IsCategory;
            }
            set
            {
                SetProperty(PropertyIsCategory, ref _IsCategory, value, false);
            }
        }

        public string PropertyDataType
        {
            get
            {
                return "ColumnDataType";
            }
        }

        public TableMemberDataType ColumnDataType
        {
            get
            {
                return _ColumnDataType;
            }
            set
            {
                SetProperty(PropertyDataType, ref _ColumnDataType, value, false);
            }
        }

        public string PropertyClassification
        {
            get
            {
                return "ColumnClassification";
            }
        }

        public GeoFieldMappingType ColumnClassification
        {
            get
            {
                return _ColumnClassification;
            }
            set
            {
                SetProperty(PropertyClassification, ref _ColumnClassification, value, false);
            }
        }

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
                return _IsEnabled;
            }
            set
            {
                SetProperty(PropertyIsEnabled, ref _IsEnabled, value, false);
            }
        }

        public TableField Model { get; private set; }

        public bool IsTableMeasure { get; private set; }

        public TableFieldViewModel()
        {
        }

        public TableFieldViewModel(TableField model, bool disabled)
            : this()
        {
            TableFieldViewModel tableFieldViewModel = this;
            Model = model;
            Name = model.Name;
            IsEnabled = !disabled;

            TableColumn tableColumn = Model as TableColumn;
            TableMeasure tableMeasure = Model as TableMeasure;
            if (tableColumn != null)
            {
                ColumnDataType = tableColumn.DataType;
                IsTimeField = tableColumn != null && tableColumn.DataType == TableMemberDataType.DateTime;
                IsCategory = true;
                IsTableMeasure = false;
                ColumnClassification = TableMemberClassificationUtil.GeoFieldType(tableColumn.Classification);
            }
            else if (tableMeasure != null)
            {
                ColumnDataType = tableMeasure.DataType;
                IsTimeField = false;
                IsCategory = false;
                IsTableMeasure = true;
                ColumnClassification = GeoFieldMappingType.None;
            }
        }

        public AggregationFunction DefaultAggregationFunction()
        {
            if (IsTableMeasure)
                return AggregationFunction.UserDefined;
            return ColumnDataType == TableMemberDataType.Double || ColumnDataType == TableMemberDataType.Long || ColumnDataType == TableMemberDataType.Currency ? AggregationFunction.Sum : AggregationFunction.Count;
        }
    }
}
