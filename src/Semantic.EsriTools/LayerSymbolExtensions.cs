using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using Semantic.Controls;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.EsriTools
{
    public static class LayerSymbolExtensions
    {
        private static Color DefaultColor = Color.YellowGreen;

        public static ISimpleFillSymbol DefaultSymbol = new SimpleFillSymbolClass
        {
            Color = colorToIRgb(Color.YellowGreen),
            Style = esriSimpleFillStyle.esriSFSHollow,
            Outline = new SimpleLineSymbolClass
            {
                Color = colorToIRgb(Color.YellowGreen),
                Style = esriSimpleLineStyle.esriSLSSolid,
                Width = 1
            }
        };

        /// <summary>
        /// 通过图层获得存储路径
        /// </summary>
        public static string GetDataSource(this ILayer layer)
        {
            return (layer as FeatureLayerClass).Workspace.PathName;
        }


        private static string ParseFieldName(this ILayer layer, string field)
        {
            var fieldName = field;
            foreach (var f in layer.GetFieldDefns())
            {
                if (f.Name.IndexOf('.') > 0 && f.Name.IndexOf(field) == f.Name.Length - field.Length)
                {
                    fieldName = f.Name;
                    break;
                }
            }
            return fieldName;
        }

        /// <summary>
        /// windows系统颜色转IRgbColor
        /// </summary>
        /// <param name="color"></param>
        /// <returns></returns>
        private static IRgbColor colorToIRgb(Color4F color)
        {
            IRgbColor rgb = new RgbColorClass();
            rgb.Red = (int)(color.R * 255);
            rgb.Blue = (int)(color.B * 255);
            rgb.Green = (int)(color.G * 255);
            return rgb;
        }

        private static IRgbColor colorToIRgb(Color color)
        {
            IRgbColor rgb = new RgbColorClass();
            rgb.Red = color.R;
            rgb.Blue = color.B;
            rgb.Green = color.G;
            return rgb;
        }

        private static ISimpleFillSymbol GenerateFillSymbol(SymbolProperties prop, bool useFill)
        {
            esriSimpleFillStyle fillStyle = 
                useFill ?
                esriSimpleFillStyle.esriSFSSolid : 
                esriSimpleFillStyle.esriSFSHollow;
            ISimpleFillSymbol result = new SimpleFillSymbolClass
            {
                Color = colorToIRgb(prop.FillColor),
                Style = fillStyle,
                Outline = new SimpleLineSymbolClass
                {
                    Style = esriSimpleLineStyle.esriSLSSolid,
                    Color = colorToIRgb(prop.LineColor),
                    Width = prop.LineWidth
                }
            };

            return result;
        }

        public static SymbolProperties GenerateSymbolProp(ISimpleFillSymbol symbol, string catagory = "")
        {
            return new SymbolProperties
            {
                Category = catagory,
                FillColor = new Color4F
                {
                    R = ((float)(symbol.Color as IRgbColor).Red) / 255.0f,
                    G = ((float)(symbol.Color as IRgbColor).Green) / 255.0f,
                    B = ((float)(symbol.Color as IRgbColor).Blue) / 255.0f,
                },
                LineColor = new Color4F
                {
                    R = ((float)(symbol.Outline.Color as IRgbColor).Red) / 255.0f,
                    G = ((float)(symbol.Outline.Color as IRgbColor).Green) / 255.0f,
                    B = ((float)(symbol.Outline.Color as IRgbColor).Blue) / 255.0f,
                },
                LineWidth = (float)symbol.Outline.Width,
                Opacity = 1      
            };
        }

        public static SymbolProperties GenerateSymbolProp(IFillSymbol symbol, string catagory = "")
        {
            return new SymbolProperties
            {
                Category = catagory,
                FillColor = new Color4F
                {
                    R = ((float)(symbol.Color as IRgbColor).Red) / 255.0f,
                    G = ((float)(symbol.Color as IRgbColor).Green) / 255.0f,
                    B = ((float)(symbol.Color as IRgbColor).Blue) / 255.0f,
                },
                LineColor = new Color4F
                {
                    R = ((float)(symbol.Outline.Color as IRgbColor).Red) / 255.0f,
                    G = ((float)(symbol.Outline.Color as IRgbColor).Green) / 255.0f,
                    B = ((float)(symbol.Outline.Color as IRgbColor).Blue) / 255.0f,
                },
                LineWidth = (float)symbol.Outline.Width,
                Opacity = 1
            };
        }

        private static TableFieldDefninition ConvertFieldDefn(IField field)
        {
            var fieldType = field.Type;
            TableMemberDataType dataType = TableMemberDataType.Unknown;
            switch (fieldType)
            {
                case esriFieldType.esriFieldTypeDate:
                    dataType = TableMemberDataType.DateTime;
                    break;
                case esriFieldType.esriFieldTypeDouble:
                    dataType = TableMemberDataType.Double;
                    break;
                case esriFieldType.esriFieldTypeOID:
                case esriFieldType.esriFieldTypeInteger:
                    dataType = TableMemberDataType.Long;
                    break;
                case esriFieldType.esriFieldTypeString:
                    dataType = TableMemberDataType.String;
                    break;
            }
            if (dataType == TableMemberDataType.Unknown)
            {
                return null;
            }
            else
            {
                return new TableFieldDefninition
                {
                    Name = field.Name,
                    DataType = dataType
                };
            }
        }


        public static IEnumerable<TableFieldDefninition> GetFieldDefns(this ILayer layer)
        {
            ITable dispTable = ((IDisplayTable)layer).DisplayTable;
            var dispFields = dispTable.Fields;
            for (int i = 0; i < dispFields.FieldCount; i++)
            {
                var field = dispFields.Field[i];
                var result = ConvertFieldDefn(field);
                if (result != null)
                {
                    yield return result;
                }
            }
        }

        public static void SetSimpleStyle(this ILayer layer, SymbolProperties prop)
        {
            IGeoFeatureLayer geoFLyr = layer as IGeoFeatureLayer;
            ISimpleRenderer simpleRenderer = new SimpleRendererClass();
            simpleRenderer.Symbol = GenerateFillSymbol(prop, false) as ISymbol;
            geoFLyr.Renderer = simpleRenderer as IFeatureRenderer;
        }

        public static void SetUniqueValueStyle(this ILayer layer, string field, IEnumerable<SymbolProperties> props)
        {
            IGeoFeatureLayer geoFLyr = layer as IGeoFeatureLayer;
            IUniqueValueRenderer uvRenderer = new UniqueValueRendererClass();

            uvRenderer.FieldCount = 1;
            var fieldName = layer.ParseFieldName(field);
            uvRenderer.Field[0] = fieldName;

            foreach (var prop in props)
            {
                ISimpleFillSymbol fillSymbol = GenerateFillSymbol(prop, true);
                uvRenderer.AddValue(prop.Category, null, fillSymbol as ISymbol);
            }

            uvRenderer.DefaultSymbol = DefaultSymbol as ISymbol;
            uvRenderer.UseDefaultSymbol = true;

            geoFLyr.Renderer = uvRenderer as IFeatureRenderer;
        }

        public static void UpdateUniqueValueStyle(this ILayer layer, SymbolProperties prop)
        {
            IGeoFeatureLayer geoFLyr = layer as IGeoFeatureLayer;
            IUniqueValueRenderer uvRenderer = geoFLyr.Renderer as IUniqueValueRenderer;

            if (uvRenderer != null)
            {
                for (int i = 0; i < uvRenderer.ValueCount; i++)
                {
                    if (uvRenderer.Value[i] == prop.Category)
                    {
                        var symbol = GenerateFillSymbol(prop, true) as ISymbol;
                        uvRenderer.Symbol[prop.Category] = symbol;
                    }
                }
            }
        }


        public static void GetSymbol(this ILayer layer)
        {
            IGeoFeatureLayer geoLayer = layer as IGeoFeatureLayer;
            IFeatureRenderer featureRenderer = geoLayer.Renderer;

            if (featureRenderer is IUniqueValueRenderer)
            {
                IUniqueValueRenderer uniqueValueRenderer = featureRenderer as IUniqueValueRenderer;
                string field = uniqueValueRenderer.Field[0];
                for (int i = 0; i < uniqueValueRenderer.ValueCount; i++)
                {
                    var catagory = uniqueValueRenderer.Value[i];
                    ISimpleFillSymbol symbol = uniqueValueRenderer.Symbol[catagory] as ISimpleFillSymbol;
                    SymbolProperties prop = GenerateSymbolProp(symbol, catagory);
                }
            }

            if (featureRenderer is ISimpleRenderer)
            {
                ISimpleRenderer simpleRender = featureRenderer as ISimpleRenderer;
                ISimpleFillSymbol symbol = simpleRender.Symbol as ISimpleFillSymbol;
                GenerateSymbolProp(symbol);
            }
        }

        public static SymbolProperties GetMapVecSymbol(this ILayer layer)
        {
            IGeoFeatureLayer geoLayer = layer as IGeoFeatureLayer;
            IFeatureRenderer featureRenderer = geoLayer.Renderer;
            
            if (featureRenderer != null)
            {
                SymbolProperties prop = new SymbolProperties();
                if(featureRenderer is IUniqueValueRenderer)
                {
                    IUniqueValueRenderer uniqueValueRenderer = featureRenderer as IUniqueValueRenderer;
                    string field = uniqueValueRenderer.Field[0];
                    for (int i = 0; i < uniqueValueRenderer.ValueCount; i++)
                    {
                        var catagory = uniqueValueRenderer.Value[i];
                        ISimpleFillSymbol symbol = uniqueValueRenderer.Symbol[catagory] as ISimpleFillSymbol;
                        prop = GenerateSymbolProp(symbol, catagory);
                    }
                }
                else if (featureRenderer is ISimpleRenderer)
                {
                    ISimpleRenderer simpleRender = featureRenderer as ISimpleRenderer;

                    Type type = simpleRender.Symbol.GetType();
                    ISimpleFillSymbol symbol = simpleRender.Symbol as ISimpleFillSymbol;
                    
                    if (symbol !=null)
                    {                        
                        prop = GenerateSymbolProp(symbol);
                    }
                    else
                    {
                        IFillSymbol sym = simpleRender.Symbol as IFillSymbol;
                        prop = GenerateSymbolProp(sym);
                    }                 
                }
                return prop;
            }
            else
            {
                return null;
            }
            
        }
    }
}
