using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class LeafNodeViewModel : ViewModelBase
    {
        private bool _IsExpanded = true;
        private bool _IsSelected;
        private string _DisplayName;
        private Color4F _Color;


        public string PropertyIsExpanded
        {
            get
            {
                return "IsExpanded";
            }
        }

        public bool IsExpanded
        {
            get
            {
                return this._IsExpanded;
            }
            set
            {
                this.SetProperty<bool>(this.PropertyIsExpanded, ref this._IsExpanded, value, false);
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

        public string DisplayName
        {
            get { return this.Model.Name; }
            set
            {
                SetProperty("Name", ref this._DisplayName, value, () => { this.Model.Name = _DisplayName; });
            }
        }
        public Color4F Color
        {
            get
            {
                return _Color;
            }
            set
            {
                SetProperty("Color", ref this._Color, value, () => { this.Model.Color = this._Color.ToSystemColor(); });
            }
        }

        public LeafNode Model { get; private set; }

        public ObservableCollectionEx<LeafNodeViewModel> Leaves { get; private set; }

        public Filter Filter { get; private set; }

        public LeafNodeViewModel(LeafNode model)
        {
            this.Model = model;
            this._DisplayName = this.Model.Name;
            this._Color = ColorExtensions.ToColor4F(this.Model.Color);
            this.Leaves = new ObservableCollectionEx<LeafNodeViewModel>();
            foreach (var leaf in this.Model.Leaves)
            {
                this.Leaves.Add(new LeafNodeViewModel(leaf));
            }

            this.Filter = new Filter();
        }
    }

    public class GeoClassViewModel : ViewModelBase
    {
        private bool _IsOpen = false;
        private LeafNodeViewModel _selectedItem;

        public bool IsOpen
        {
            get { return _IsOpen; }
            set { SetProperty("IsOpen", ref this._IsOpen, value); }
        }

        public static string PropertySelectedItem
        {
            get { return "SelectedItem"; }
        }

        public LeafNodeViewModel SelectedItem
        {
            get { return _selectedItem; }
            set { SetProperty(PropertySelectedItem, ref this._selectedItem, value, () => { IsOpen = false; }); }
        }

        public GeoClass Model { get; private set; }

        public ObservableCollectionEx<LeafNodeViewModel> Leaves { get; private set; }

        public GeoClassViewModel(string file)
        {
            this.Model = GeoClass.LoadFile(file);
            this.Leaves = new ObservableCollectionEx<LeafNodeViewModel>();

            foreach (var leaf in this.Model.Leaves)
            {
                this.Leaves.Add(new LeafNodeViewModel(leaf));
            }
        }
    }
}
