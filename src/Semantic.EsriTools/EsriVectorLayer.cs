using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.DataSourcesFile;
using ESRI.ArcGIS.DataSourcesOleDB;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using Semantic.Controls;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.EsriTools
{
    public class EsriVectorLayer : ITableMember
    {
        public string Path { get; set; }

        public ILayer Layer { get; private set; }

        /// <summary>
        /// FeatureLayer
        /// </summary>
        public IFeatureLayer FeatureLayer
        {
            get
            {
                return this.Layer as IFeatureLayer;
            }
        }

        /// <summary>
        /// FeatureClass
        /// </summary>
        public IFeatureClass FeatureClass
        {
            get
            {
                return this.FeatureLayer.FeatureClass;
            }
        }


        /// <summary>
        /// 默认构造函数
        /// </summary>
        public EsriVectorLayer()
        { }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="path"></param>
        public EsriVectorLayer(string path)
        {
            this.Path = path;
            this.Layer = EsriHelper.OpenFeatureLayer(path);
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="layer"></param>
        public EsriVectorLayer(ILayer layer)
        {
            this.Layer = layer;
        }

        /// <summary>
        /// 新建字段
        /// </summary>
        /// <param name="name">字段名</param>
        /// <param name="aliasName">别名</param>
        /// <param name="FieldType">字段类型</param>
        public void AddField(string name, esriFieldType FieldType)
        {
            int index = this.FeatureClass.Fields.FindField(name);
            if (index == -1)
            {
                // ITable pClass = this.FeatureClass as ITable;
                // 新建字段
                IField pField = new FieldClass();
                IFieldEdit2 pFieldEdit = pField as IFieldEdit2;
                pFieldEdit.Name_2 = name;
                pFieldEdit.Type_2 = FieldType;
                if (FieldType == esriFieldType.esriFieldTypeString)
                {
                    pFieldEdit.Length_2 = 20;
                }

                // 添加字段
                // pClass.AddField(pField);

                this.FeatureClass.AddField(pField);
            }
        }

        /// <summary>
        /// 添加分类字段
        /// </summary>
        public void AddGeoClassField(string geoClass)
        {
            AddField(geoClass, esriFieldType.esriFieldTypeString);
        }

        /// <summary>
        /// 新建字段
        /// </summary>
        /// <param name="name">字段名</param>
        public void AddDoubleField(string name)
        {
            AddField(name, esriFieldType.esriFieldTypeDouble);
        }

        /// <summary>
        /// 删除字段
        /// </summary>
        /// <param name="name"></param>
        public void DeleteField(string name)
        {
            int index = this.FeatureClass.Fields.FindField(name);
            DeleteField(index);
        }

        /// <summary>
        /// 删除字段
        /// </summary>
        /// <param name="index"></param>
        public void DeleteField(int index)
        {
            if (index < 0 && index >= this.FeatureClass.Fields.FieldCount)
            {
                return;
            }
            IClass pClass = this.FeatureClass as IClass;
            // 删除字段
            IField field = this.FeatureClass.Fields.get_Field(index);
            pClass.DeleteField(field);
        }

        /// <summary>
        /// 查询图层中符合条件的要素（保留投影结果字段）
        /// select ** from layer where **
        /// </summary>
        /// <param name="query"></param>
        /// <returns></returns>
        public IEnumerable<IFeature> GetFeatureByQuery(IQueryFilter query)
        {
            //IQueryFilter queryFilter = new QueryFilterClass();
            //queryFilter.WhereClause = "HasChanged > 15";
            //queryFilter.SubFields = "Shape, CC";

            IFeatureCursor featureCursor = this.FeatureClass.Search(query, false); ;
            IFeature feature = null;
            while ((feature = featureCursor.NextFeature()) != null)
            {
                yield return feature;
            }
        }

        /// <summary>
        /// 获取所有要素
        /// </summary>
        /// <returns></returns>
        public IEnumerable<IFeature> GetAllFeatures()
        {
            return GetFeatureByQuery(null);
        }

        /// <summary>
        /// 根据id获取要素
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public IFeature GetFeatureById(int id)
        {
            IQueryFilter queryFilter = new QueryFilterClass();
            queryFilter.WhereClause = string.Format("\"{0}\" = {1}", this.FeatureClass.OIDFieldName, id);
            IFeatureCursor featureCursor = this.FeatureClass.Search(queryFilter, false); ;
            return featureCursor.NextFeature();
        }

        private IFields GetValidateFields()
        {
            IFields result;//用于获取正确字段的集合
            IFields pInFields = this.FeatureClass.Fields;//获取输入图层的要素类的字段集合
            //通过IFieldChecker检查字段的合法性，为输出SHP获得字段集合
            IFieldChecker pFieldChecker = new FieldChecker();
            IEnumFieldError pEnumFieldError = null;//用于获取错误字段的集合
            pFieldChecker.Validate(pInFields, out pEnumFieldError, out result);
            return result;
        }

        /// <summary>
        /// 获取字段的名字
        /// </summary>
        /// <returns></returns>
        public IEnumerable<string> GetAllFieldName()
        {
            IFields pInFields = this.FeatureClass.Fields;
            for (int i = 0; i < pInFields.FieldCount; i++)
            {
                yield return pInFields.get_Field(i).Name;
            }
        }

        /// <summary>
        /// 获取所有字段
        /// </summary>
        /// <param name="fields"></param>
        /// <returns></returns>
        private IEnumerable<IField> GetFields(IFields fields)
        {
            for (int i = 0; i < fields.FieldCount; i++)
            {
                if (fields.get_Field(i).Type == esriFieldType.esriFieldTypeGeometry)
                {
                    yield return fields.get_Field(i);
                }
            }
        }

        private TableFieldDefninition ConvertFieldDefn(IField field)
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

        public IEnumerable<TableFieldDefninition> GetFieldDefns()
        {
            ITable dispTable = ((IDisplayTable)this.Layer).DisplayTable;
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

        /// <summary>
        /// 查找几何字段
        /// </summary>
        /// <param name="fields"></param>
        /// <returns></returns>
        private IField GetGeomField(IFields fields)
        {
            return this.GetFields(fields).FirstOrDefault(f => f.Type == esriFieldType.esriFieldTypeGeometry);
        }

        /// <summary>
        /// 导出结果
        /// </summary>
        /// <param name="query"></param>
        /// <param name="selSet"></param>
        /// <param name="filePath"></param>
        /// <returns></returns>
        public bool ExportShapeFile(IQueryFilter query, ISelectionSet selSet, string filePath)
        {
            bool result = true;

            try
            {
                // 输入图层定义
                IDataset inputDataset = this.FeatureClass as IDataset;
                IDatasetName inputDatasetName = (IDatasetName)inputDataset.FullName;

                // 输出图层定义
                string fileNameWithoutExtension = System.IO.Path.GetFileNameWithoutExtension(filePath);
                string fileDirctory = System.IO.Path.GetDirectoryName(filePath);
                IWorkspaceFactory workspaceFactory = new ShapefileWorkspaceFactoryClass();
                IWorkspaceName outputWorkspaceName = workspaceFactory.Create(fileDirctory, fileNameWithoutExtension, null, 0);
                IFeatureDatasetName pOutFeatureDatasetName = null;
                IFeatureClassName pOutFeatureClassName = new FeatureClassNameClass();
                IDatasetName pOutDatasetName;
                pOutDatasetName = (IDatasetName)pOutFeatureClassName;
                pOutDatasetName.Name = fileNameWithoutExtension;
                pOutDatasetName.WorkspaceName = outputWorkspaceName;

                // 输出字段集合与几何字段定义
                IFields outFields = this.GetValidateFields();
                IField geomField = GetGeomField(outFields);
                IGeometryDef pOutGeometryDef = geomField.GeometryDef;
                IGeometryDefEdit pOutGeometryDefEdit = (IGeometryDefEdit)pOutGeometryDef;
                pOutGeometryDefEdit.GridCount_2 = 1;
                pOutGeometryDefEdit.set_GridSize(0, 1500000);

                // 导出数据
                IFeatureDataConverter2 featureDataConverter = new FeatureDataConverterClass();
                featureDataConverter.ConvertFeatureClass(inputDatasetName, query, selSet, pOutFeatureDatasetName, pOutFeatureClassName, pOutGeometryDef, outFields, "", 1000, 0);
            }
            catch (Exception)
            {
                result = false;
            }

            return result;
        }

        /// <summary>
        /// 构造查询过滤条件
        /// </summary>
        /// <returns></returns>
        private IQueryFilter GetQueryFilter(string where, List<string> fields)
        {
            IQueryFilter query = new QueryFilterClass();
            query.WhereClause = where;
            if (fields != null && fields.Count > 0)
            {
                query.SubFields = string.Join(",", fields.ToArray());
            }
            return query;
        }

        public IFeature GetRelFeature(Coordinates coords)
        {
            IPoint pt = new PointClass();
            pt.PutCoords(coords.Longitude, coords.Latitude);
            ISpatialReferenceFactory pSRF = new SpatialReferenceEnvironmentClass();
            pt.SpatialReference = pSRF.CreateGeographicCoordinateSystem((int)esriSRGeoCSType.esriSRGeoCS_WGS1984);

            ISpatialFilter spatialFilter = new SpatialFilterClass();
            spatialFilter.Geometry = pt;
            spatialFilter.SpatialRel = esriSpatialRelEnum.esriSpatialRelIntersects;

            IFeatureCursor cursor = this.FeatureClass.Search(spatialFilter, false);

            return cursor.NextFeature();
        }


        public static ISpatialFilter GetSpatialFilter(IGeometry geom, string geomFieldName, esriSpatialRelEnum sr)
        {
            ISpatialFilter pSpatialFilter = new SpatialFilterClass();
            pSpatialFilter.Geometry = geom;
            pSpatialFilter.GeometryField = geomFieldName;
            pSpatialFilter.SpatialRel = sr;
            // pSpatialFilter.SpatialRel = esriSpatialRelEnum.esriSpatialRelCrosses;
            return pSpatialFilter;
            //IFeatureCursor pFeatureCursor = featureClass.Search(pSpatialFilter, false);
            //IFeature pFeature = pFeatureCursor.NextFeature();
            //while (pFeature != null)
            //{
            //    yield return pFeature;
            //    pFeature = pFeatureCursor.NextFeature();
            //}
        }

        /// <summary>
        /// 获取选择的要素集合
        /// </summary>
        /// <returns></returns>
        private ISelectionSet GetSelectionSet()
        {
            IFeatureSelection featureSelection = this.FeatureLayer as IFeatureSelection;
            return featureSelection.SelectionSet;
        }

        public bool ExportShapeFile(string filePath)
        {
            return ExportShapeFile(null, null, filePath);
        }

        /// <summary>
        /// 设置过滤条件
        /// </summary>
        /// <param name="where"></param>
        public void SetFilter(string where)
        {
            IFeatureLayerDefinition pDef = (IFeatureLayerDefinition)this.FeatureLayer;
            pDef.DefinitionExpression = where;
        }

        /// <summary>
        /// windows系统颜色转IRgbColor
        /// </summary>
        /// <param name="color"></param>
        /// <returns></returns>
        private IRgbColor colorToIRgb(Color4F color, double opacity)
        {
            IRgbColor rgb = new RgbColorClass();
            rgb.Red = (int)(color.R * 255);
            rgb.Blue = (int)(color.B * 255);
            rgb.Green = (int)(color.G * 255);
            rgb.Transparency = (byte)(opacity * 255.0);
            return rgb;
        }

        private IRgbColor colorToIRgb(Color color)
        {
            IRgbColor rgb = new RgbColorClass();
            rgb.Red = color.R;
            rgb.Blue = color.B;
            rgb.Green = color.G;
            // rgb.Transparency = 1;
            return rgb;
        }

        public void SetSimpleStyle(SymbolProperties symbol)
        {
            IGeoFeatureLayer geoFLyr = this.Layer as IGeoFeatureLayer;
            ISimpleRenderer simpleRenderer = new SimpleRendererClass();
            ISimpleFillSymbol defaultFill = new SimpleFillSymbolClass();
            defaultFill.Style = esriSimpleFillStyle.esriSFSSolid;
            ILineSymbol lineSymbol = new SimpleLineSymbolClass();
            lineSymbol.Width = symbol.LineWidth;
            lineSymbol.Color = colorToIRgb(symbol.LineColor, 1);
            defaultFill.Outline = lineSymbol;
            defaultFill.Color = colorToIRgb(symbol.FillColor, symbol.Opacity);
            simpleRenderer.Symbol = defaultFill as ISymbol;
            geoFLyr.Renderer = simpleRenderer as IFeatureRenderer;
        }

        public SymbolProperties GetSimpleStyle()
        {
            SymbolProperties symbol = new SymbolProperties();

            IGeoFeatureLayer geoFLyr = this.Layer as IGeoFeatureLayer;
            IFeatureRenderer featureRenderer = geoFLyr.Renderer;
            ISimpleRenderer simpleRender = featureRenderer as ISimpleRenderer;
            ISimpleFillSymbol symbol1 = simpleRender.Symbol as ISimpleFillSymbol;

            return symbol;
        }

        /// <summary>
        /// 唯一值符号化
        /// </summary>
        /// <param name="field"></param>
        /// <param name="geoClass"></param>
        /// <param name="first"></param>
        public void SetUniqueValueStyle(string field, IEnumerable<SymbolProperties> symbols)
        {
            IFeatureClass featureClass = this.FeatureClass;
            if (featureClass.FindField(field) < 0)
            {
                this.AddGeoClassField(field);
            }

            var fieldName = field;
            foreach (var f in GetFieldDefns())
            {
                if (f.Name.IndexOf('.') > 0 && f.Name.IndexOf(field) == f.Name.Length - field.Length)
                {
                    fieldName = f.Name;
                    break;
                }
            }

            IGeoFeatureLayer geoFLyr = this.Layer as IGeoFeatureLayer;
            IUniqueValueRenderer uvRenderer = new UniqueValueRendererClass();

            foreach (var s in symbols)
            {
                ISimpleFillSymbol fillSymbol = new SimpleFillSymbolClass();
                fillSymbol.Color = colorToIRgb(s.FillColor, s.Opacity);
                fillSymbol.Style = esriSimpleFillStyle.esriSFSSolid;
                ILineSymbol uvlineSymbol = new SimpleLineSymbolClass();
                uvlineSymbol.Color = colorToIRgb(s.LineColor, s.Opacity);
                uvlineSymbol.Width = s.LineWidth;
                fillSymbol.Outline = uvlineSymbol;
                uvRenderer.AddValue(s.Category, null, fillSymbol as ISymbol);
            }
            uvRenderer.FieldCount = 1;
            uvRenderer.set_Field(0, fieldName);

            ISimpleFillSymbol defaultFill = new SimpleFillSymbolClass();
            defaultFill.Style = esriSimpleFillStyle.esriSFSHollow;
            ILineSymbol lineSymbol = new SimpleLineSymbolClass();
            lineSymbol.Color = colorToIRgb(Color.GreenYellow);
            defaultFill.Outline = lineSymbol;
            uvRenderer.DefaultSymbol = defaultFill as ISymbol;
            uvRenderer.UseDefaultSymbol = true;

            geoFLyr.Renderer = uvRenderer as IFeatureRenderer;
        }

        /// <summary>
        /// 设置分类
        /// </summary>
        /// <param name="query"></param>
        /// <param name="fieldname"></param>
        /// <param name="geoClass"></param>
        public void SetGeoClass(string query, string fieldname, string geoClass)
        {
            int fieldIndex = this.FeatureClass.FindField(fieldname);
            if (this.FeatureClass.FindField(fieldname) < 0)
            {
                this.AddGeoClassField(fieldname);
            }

            IQueryFilter queryFilter = new QueryFilterClass();
            queryFilter.WhereClause = query;

            IDisplayTable disTable = (IDisplayTable)this.FeatureLayer;
            ITable dispTable = ((IDisplayTable)this.FeatureLayer).DisplayTable;

            ICursor c = disTable.SearchDisplayTable(queryFilter, false);
            var row = c.NextRow();
            while (row != null)
            {
                var id = row.OID;
                var pFeature = this.FeatureClass.GetFeature(id);
                pFeature.set_Value(fieldIndex, geoClass);
                pFeature.Store();
                row = c.NextRow();
            }

            //IFeatureCursor pCursor = this.FeatureClass.Update(queryFilter, false);
            //IFeature pFeature = pCursor.NextFeature();
            //while (pFeature != null)
            //{
            //    pFeature.set_Value(fieldIndex, geoClass);
            //    pFeature.Store();
            //    pFeature = pCursor.NextFeature();
            //}
        }

        public int GetFieldIndex(string name, IFields fields)
        {
            for (int i = 0; i < fields.FieldCount; i++)
            {
                var index = fields.Field[i].Name.IndexOf(name);
                if (index != -1)
                {
                    return i;
                }
            }
            return -1;
        }

        public void GenerateTrainAndTestSet(List<string> fields, IEnumerable<SampleEntry> samples)
        {
            if (fields.Count <= 0)
                return;

            IQueryFilter queryFilter = new QueryFilterClass();
            queryFilter.SubFields = string.Join(",", fields) + ",Sample";

            IDisplayTable disTable = (IDisplayTable)this.FeatureLayer;
            ITable dispTable = ((IDisplayTable)this.FeatureLayer).DisplayTable;

            var list = fields.Select(f => GetFieldIndex(f, dispTable.Fields)).ToList();
            var sindex = GetFieldIndex("Sample", dispTable.Fields);

            ICursor c = disTable.SearchDisplayTable(queryFilter, false);
            var row = c.NextRow();

            string root = AppDomain.CurrentDomain.BaseDirectory + "sample\\";
            if (!Directory.Exists(root))
            {
                Directory.CreateDirectory(root);
            }

            // 构造算法输入参数
            string trainFile = root + "train.txt";
            string testFile = root + "test.txt";
            var geoClassList = samples.Select(r => r.GeoClass).Distinct().ToList();
            using (var fs = File.CreateText(testFile))
            {
                using (var trainfs = File.CreateText(trainFile))
                {
                    while (row != null)
                    {
                        var fieldVals = new List<double>();
                        for (int i = 0; i < list.Count; i++)
                        {
                            fieldVals.Add((double)row.Value[list[i]]);
                        }
                        var code = -1;
                        var geoClass = row.Value[sindex] as string;
                        if (geoClass.Trim() != "")
                        {
                            code = geoClassList.IndexOf(geoClass);
                        }
                        fieldVals.Add(code);
                        var line = string.Join(",", fieldVals);
                        if (code >= 0)
                        {
                            trainfs.WriteLine(line);
                        }
                        fs.WriteLine(line);
                        row = c.NextRow();
                    }
                }
            }
        }
    }
}
