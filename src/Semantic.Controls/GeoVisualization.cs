using Semantic.Engine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class GeoVisualization
    {
        private SymbolProperties symbolProperty = new SymbolProperties()
        {
            FillColor = new Color4F(1f, 0.388235f, 0.52549f, 0.082353f),
            LineWidth = 1,         
            LineColor = new Color4F(1f, 0.388235f, 0.52549f, 0.082353f)
        };

        //  当前地图加载的样式
        private SymbolProperties symbolProperty1 = new SymbolProperties()
        {
            FillColor = new Color4F(),
            LineWidth = 1,
            LineColor = new Color4F(1f, 0.388235f, 0.52549f, 0.082353f)
        };
        private Hashtable symbolPropertiesLookUpByGeoClass = new Hashtable();
        public static readonly Color4F DefaultColor = new Color4F(0.0f, 0.0f, 0.0f, 0.0f);

        public IVisualization Vis { get; private set; }
        public LayerDefinition Layer { get; private set; }

        public bool IsSimpleStyle { get; private set; }

        public GeoClass GeoClass { get; private set; }

        public string FieldName { get; private set; }

        public IEnumerable<string> Categories
        {
            get
            {
                GeoClass geoClass = this.GeoClass;
                if (geoClass == null)
                    return null;
                return geoClass.Leaves.Select(r => r.Name);
            }
        }

        public GeoVisualization(LayerDefinition layerDefn, IVisualization vis)
        {
            this.Vis = vis;
            this.Layer = layerDefn;
            this.IsSimpleStyle = true;
        }

        private IEnumerable<SymbolProperties> GetSymbols(GeoClass geoClass)
        {
            this.symbolPropertiesLookUpByGeoClass.Clear();
            foreach (var leaf in geoClass.Leaves)
            {
                var symbol = new SymbolProperties
                {
                    Category = leaf.Name,
                    Opacity = 1,
                    FillColor = ColorExtensions.ToColor4F(leaf.Color),
                    LineWidth = 1,
                    LineColor = ColorExtensions.ToColor4F(leaf.Color)
                };
                this.symbolPropertiesLookUpByGeoClass.Add(symbol.Category, symbol);
                yield return symbol;
            }
        }

        private IEnumerable<SymbolProperties> GetSymbols()
        {
            foreach (SymbolProperties symbol in this.symbolPropertiesLookUpByGeoClass.Values)
            {
                yield return symbol;
            }
        }

        /// <summary>
        /// 依据分类树的第某层子节点对图层进行分层设色
        /// </summary>
        /// <param name="geoClass"></param>
        public void SetGeoClass(string classField, GeoClass geoClass, int level)
        {
            this.IsSimpleStyle = false;
            // todo:
            // 使用某层的分类进行渲染
            // geoClass.Leaves.SelectMany(r => r.Leaves);
            this.FieldName = classField;
            this.GeoClass = geoClass;
            var symbols = GetSymbols(geoClass);
            SetStyle(symbols);
        }

        public void SetTranspancy(short transpancy)
        {
            this.Vis.SetTranspancy(this.Layer, transpancy);
        }

        public void SetStyle(IEnumerable<SymbolProperties> symbols)
        {
            this.Vis.SetUniqueStyle(this.Layer, this.FieldName, symbols);
        }

        /// <summary>
        /// 单一符号
        /// </summary>
        public void SetStyle(SymbolProperties symbol)
        {
            this.IsSimpleStyle = true;
            this.symbolProperty = symbol;
            this.Vis.SetSimpleStyle(this.Layer, symbol);
        }

        /// <summary>
        /// 更新图层分类信息
        /// </summary>
        /// <param name="filter"></param>
        /// <param name="leaf"></param>
        public void UpdateClass(Filter filter, string classField, LeafNode leaf)
        {
            string query = filter.ToString();
            Task.Factory.StartNew(() =>
            {
                this.Vis.UpdateGeoClass(this.Layer, classField, query, leaf.Name);
            });
        }

        /// <summary>
        /// 选中要素
        /// </summary>
        /// <param name="layerDefn"></param>
        /// <param name="filter"></param>
        /// <param name="geoClass"></param>
        /// <param name="first"></param>
        /// <param name="zoomToLayer"></param>
        public void SelectByFilter(Filter filter)
        {
            string query = filter.ToString();
            this.Vis.SelectFeature(this.Layer, query);
        }

        internal void UpdateStyle()
        {
            if (this.IsSimpleStyle)
            {
                this.SetStyle(this.symbolProperty);
            }
            else
            {
                var symbols = GetSymbols();
                this.SetStyle(symbols);
            }
        }

        public SymbolProperties GetSymbol(string catagory = null)
        {
            if (string.IsNullOrEmpty(catagory))
            {
                //  return this.symbolProperty;
                this.symbolProperty = this.Vis.GetSimpleStyle(this.Layer);
                return this.symbolProperty;
            }
            else
                return this.symbolPropertiesLookUpByGeoClass[catagory] as SymbolProperties;
        }

    }
}
