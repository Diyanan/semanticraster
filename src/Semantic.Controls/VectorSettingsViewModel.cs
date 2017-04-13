using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class VectorSettingsViewModel : ViewModelBase
    {
        private GeoVisualization _GeoVisualization = null;
        public SymbolProperties _Model = null;
        
        // 根据分层设色样式的字段，进行图层样式的修改
        public string[] _ColorScopeDisplayStrings;
        private int _SelectedColorScopeIndex;

        // 简单样式属性
        private float _LineWidth = 0;
        private short _OpacityFactor = 0;

        // 图层
        public LayerViewModel Model { get; private set; }

        public string PropertyColorScopeDisplayStrings
        {
            get
            {
                return "ColorScopeDisplayStrings";
            }
        }

        public string[] ColorScopeDisplayStrings
        {
            get
            {
                return this._ColorScopeDisplayStrings;
            }
            private set
            {
                this.SetProperty<string[]>(this.PropertyColorScopeDisplayStrings, ref this._ColorScopeDisplayStrings, value, false);
                if (value != null)
                {
                    this.SelectedColorScopeIndex = 0;
                }
            }
        }

        public string PropertySelectedColorScopeIndex
        {
            get
            {
                return "SelectedColorScopeIndex";
            }
        }

        public int SelectedColorScopeIndex
        {
            get
            {
                return this._SelectedColorScopeIndex;
            }
            set
            {
                this.SetProperty<int>(this.PropertySelectedColorScopeIndex, ref this._SelectedColorScopeIndex, value, false);
                if (this._GeoVisualization != null && this._SelectedColorScopeIndex >= 0)
                {
                    var catagory = this.ColorScopeDisplayStrings[value];
                    var symbol = this._GeoVisualization.GetSymbol(catagory);
                    RefreshSelectedColorState(symbol);
                }
            }
        }

        public string PropertyLineWidth
        {
            get
            {
                return "LineWidth";
            }
        }

        public float LineWidth
        {
            get
            {
                return this._LineWidth;
            }
            set
            {
                this.SetProperty<float>(this.PropertyLineWidth, ref this._LineWidth, value, ()=>
                {
                    if (this._Model != null)
                    {
                        this._Model.LineWidth = value;
                        if (this._GeoVisualization != null)
                        {
                            this._GeoVisualization.UpdateStyle();
                        }
                    }
                });
            }
        }

        public string PropertyOpacityFactor
        {
            get
            {
                return "OpacityFactor";
            }
        }

        public short OpacityFactor
        {
            get
            {
                return this._OpacityFactor;
            }
            set
            {
                this.SetProperty<short>(this.PropertyOpacityFactor, ref this._OpacityFactor, value, () =>
                {
                    if (this._Model != null)
                    {
                        this._Model.Opacity = value;
                        if (this._GeoVisualization != null)
                        {
                            this._GeoVisualization.SetTranspancy(value);
                        }
                    }
                });
            }
        }

        public ColorPickerViewModel LineColorPicker { get; private set; }
        public ColorPickerViewModel FillColorPicker { get; private set; }

        public JoinManagerViewModel JoinManager { get; private set; }

        public VectorSettingsViewModel(IJoinManager joinManager, List<Color4F> customColors)
        {
            this.JoinManager = new JoinManagerViewModel(joinManager);
            this.LineColorPicker = new ColorPickerViewModel(null);
            this.FillColorPicker = new ColorPickerViewModel(null);
            this.LineColorPicker.PropertyChanged += OnLineColorPickerPropertyChanged;
            this.FillColorPicker.PropertyChanged += OnFillColorPickerPropertyChanged;
        }

        private void OnFillColorPickerPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == this.FillColorPicker.PropertySelectedColor)
            {
                if (this._Model != null)
                {
                    this._Model.FillColor = this.FillColorPicker.SelectedColor;
                    if (this._GeoVisualization != null)
                    {
                        this._GeoVisualization.UpdateStyle();
                    }
                }
            }
        }

        private void OnLineColorPickerPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == this.LineColorPicker.PropertySelectedColor)
            {
                if (this._Model != null)
                {
                    this._Model.LineColor = this.LineColorPicker.SelectedColor; ;
                    if (this._GeoVisualization != null)
                    {
                        this._GeoVisualization.UpdateStyle();
                    }
                }
            }
        }

        private void RefreshColorScopes()
        {
            GeoVisualization geoVisualization = this._GeoVisualization;
            if (geoVisualization == null)
                return;
            IEnumerable<string> categories = geoVisualization.Categories;
            if (categories.Any())
            {
                this.ColorScopeDisplayStrings = categories.ToArray();
            }
            else
            {
                this.ColorScopeDisplayStrings = null;
            }
        }

        private void RefreshSelectedColorState(SymbolProperties symbol)
        {
            this._Model = symbol;
            this.FillColorPicker.SelectedColor = symbol.FillColor;
            this.LineColorPicker.SelectedColor = symbol.LineColor;
            this.LineWidth = symbol.LineWidth;
            this.OpacityFactor = symbol.Opacity;
        }

        public void SetLayer(LayerViewModel layer)
        {
            var vis = layer.GeoVisualization;
            if(vis != null)
            {
                this._GeoVisualization = vis;
                if (vis.IsSimpleStyle)
                {
                    this.ColorScopeDisplayStrings = null;
                    var symbol = vis.GetSymbol();
                    var symbol1 = vis.Vis.GetSimpleStyle(layer.LayerDefinition);
                    this.RefreshSelectedColorState(symbol);
                }
                else
                {
                    RefreshColorScopes();
                }
            }

            this.JoinManager.SetLayer(layer);
        }
    }
}
