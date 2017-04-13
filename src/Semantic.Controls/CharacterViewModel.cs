using Semantic.Configure;
using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class CharacterViewModel : ViewModelBase
    {
        private string _Name;
        private string _Description;
        private bool? _IsSelected = false;
        private bool isAllSpecified = true;

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

        public static string PropertyDescription
        {
            get
            {
                return "Description";
            }
        }

        public string Description
        {
            get
            {
                return _Description;
            }
            set
            {
                SetProperty(PropertyDescription, ref _Description, value, false);
            }
        }


        public static string PropertyIsSelected
        {
            get
            {
                return "IsSelected";
            }
        }

        public bool? IsSelected
        {
            get
            {
                return _IsSelected;
            }
            set
            {
                SetProperty(PropertyIsSelected, ref _IsSelected, value, ()=>
                {
                    // switch All Selected
                    SwitchAllSelected(value);
                });
            }
        }

        public ImageFeature Model { get; private set; }
        public ObservableCollectionEx<TableFieldViewModel> Fields { get; private set; }

        public CharacterViewModel(ImageFeature model)
        {
            this.Model = model;
            this.Name = model.Name;
            this.Description = model.Comments;

            this.Fields = new ObservableCollectionEx<TableFieldViewModel>();
            this.Fields.ItemPropertyChanged += Fields_ItemPropertyChanged;
        }

        private void Fields_ItemPropertyChanged(TableFieldViewModel item, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == TableFieldViewModel.PropertyIsSelected)
            {
                // 开始字段选择
                this.isAllSpecified = false;
                this.IsSelected = IsAllSelected();
                // 结束字段选择
                this.isAllSpecified = true;
            }
        }

        /// <summary>
        /// 总开关切换（切换全不选择和全部选择）
        /// </summary>
        /// <param name="isAll"></param>
        private void SwitchAllSelected(bool? isAll)
        {
            if (this.isAllSpecified)
            {
                // 选择字段总开关
                bool? isSelected = isAll;
                bool? normalValue = new bool?(isSelected.HasValue && isSelected.GetValueOrDefault());
                foreach (var field in this.Fields)
                {
                    field.IsSelected = normalValue.Value;
                }
            }
        }

        /// <summary>
        /// 判断总开关状态（三种状态：全不选择；全不选择；部分选择）
        /// </summary>
        /// <returns></returns>
        public bool? IsAllSelected()
        {
            if (this.Fields.Where(r => r.IsSelected).Count() == 0)
            {
                return new bool?(false);
            }
            else if (this.Fields.Where(r => !r.IsSelected).Count() == 0)
            {
                return new bool?(true);
            }

            return new bool?();
        }
    }
}
