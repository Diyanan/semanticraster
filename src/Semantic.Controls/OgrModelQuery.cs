using OSGeo.GDAL;
using OSGeo.OGR;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class OgrModelQuery : ModelQuery
    {
        private List<ModelQueryKeyColumn> keys;
        private List<ModelQueryMeasureColumn> measures;

        private LayerDefinition LayerDefn { get; set; }

        // 构造函数
        public OgrModelQuery(string name, Filter filter, LayerDefinition layerDefn, CancellationToken cancellationToken)
            : base(name, filter, cancellationToken)
        {
            this.LayerDefn = layerDefn;
            this.keys = new List<ModelQueryKeyColumn>();
            this.measures = new List<ModelQueryMeasureColumn>();
            this.Clear();
        }

        public override void AddKey(ModelQueryKeyColumn modelQueryKey)
        {
            this.keys.Add(modelQueryKey);
        }

        public override void AddMeasure(ModelQueryMeasureColumn modelQueryMeasure)
        {
            this.measures.Add(modelQueryMeasure);
        }

        public override void Clear()
        {
            this.keys.Clear();
            this.measures.Clear();
            this.ResultsItemCount = 0;
            this.QueryUsesAggregation = false;
        }

        protected internal override void Shutdown()
        {
            this.Clear();
            this.keys = null;
            this.measures = null;
            base.Shutdown();
        }

        public string BuildKeyQuery()
        {
            string tableName = this.keys[0].TableColumn.Table.ModelName;
            StringBuilder builder = new StringBuilder();
            builder.Append("SELECT ");
            // concat fields
            var fields = String.Join(",", this.keys.Select(k => k.TableColumn.Name));
            builder.Append(fields);
            if (tableName.IndexOf(".") > 0)
            {
                tableName = tableName.Substring(0, tableName.IndexOf('.'));
            }

            builder.AppendFormat(" FROM {0}", tableName);

            return builder.ToString();
        }

        public string BuildMeasureQuery()
        {
            string tableName = this.measures[0].TableColumn.Table.ModelName;
            StringBuilder builder = new StringBuilder();
            builder.Append("SELECT ");
            var fields = String.Join(",",
                    this.measures.Select(m => string.Format("MIN({0}),MAX({0})", m.TableColumn.Name)));
            builder.Append(fields);
            if (tableName.IndexOf(".") > 0)
            {
                tableName = tableName.Substring(0, tableName.IndexOf('.'));
            }

            builder.AppendFormat(" FROM {0}", tableName);

            return builder.ToString();
        }

        private object[,] FetchResult(Layer layer)
        {
            var feature = layer.GetNextFeature();
            int fieldCount = feature.GetFieldCount();
            int featureCount = layer.GetFeatureCount(0);
            var resultSet = new object[featureCount, fieldCount];
            int row = 0;
            while (feature != null)
            {
                for (int i = 0; i < fieldCount; i++)
                {
                    var fieldType = feature.GetFieldDefnRef(i).GetFieldType();
                    switch (fieldType)
                    {
                        case FieldType.OFTDate:
                        case FieldType.OFTTime:
                        case FieldType.OFTDateTime:
                            int y, m, d, h, mm, s, flag;
                            feature.GetFieldAsDateTime(i, out y, out m, out d, out h, out mm, out s, out flag);
                            resultSet[row, i] = new DateTime(y, m, d, h, mm, s);
                            break;
                        case FieldType.OFTReal:
                            resultSet[row, i] = feature.GetFieldAsDouble(i);
                            break;
                        case FieldType.OFTInteger:
                            resultSet[row, i] = (long)feature.GetFieldAsInteger(i);
                            break;
                        case FieldType.OFTString:
                            resultSet[row, i] = feature.GetFieldAsString(i);
                            break;
                    }
                }
                feature = layer.GetNextFeature();
                row++;
            }
            return resultSet;
        }

        private void FetchAndSanitizeResults(CancellationToken cancellationToken, Layer layer, bool isKey)
        {
            var resultSet = FetchResult(layer);
            if (isKey)
            {
                for (int i = 0; i < this.keys.Count; ++i)
                {
                    if (this.keys[i].FetchValues)
                    {
                        cancellationToken.ThrowIfCancellationRequested();
                        this.SanitizeQueryResults(this.keys[i], resultSet, i, cancellationToken);
                        cancellationToken.ThrowIfCancellationRequested();
                    }
                }
            }
            else
            {
                for (int i = 0; i < this.measures.Count; ++i)
                {
                    if (this.measures[i].FetchValues)
                    {
                        cancellationToken.ThrowIfCancellationRequested();
                        this.SanitizeQueryResults(this.measures[i], resultSet, i, cancellationToken);
                        cancellationToken.ThrowIfCancellationRequested();
                    }
                }
            }

        }

        private OgrLayer GetLayer(string tableName)
        {
            OgrLayer layer = null;
            if (tableName.IndexOf(".") < 0)
            {
                layer = new OgrLayer(this.LayerDefn.DataSource, false);
            }
            else
            {
                var csv = System.IO.Path.GetDirectoryName(this.LayerDefn.DataSource) + "\\" + tableName;
                if (System.IO.File.Exists(csv))
                {
                    layer = new OgrLayer(csv, false);
                    layer.CorrectFieldDefn();
                }
            }
            return layer;
        }

        public override void QueryData(CancellationToken cancellationToken)
        {
            string query = "";
            Layer result = null;

            if (this.keys.Count > 0)
            {
                string tableName = this.keys[0].TableColumn.Table.ModelName;
                var layer = GetLayer(tableName);
                query = BuildKeyQuery();
                result = layer.ExecuteSQL(query);
                FetchAndSanitizeResults(cancellationToken, result, true);
                layer.Dispose();
            }
            if (this.measures.Count > 0)
            {
                string tableName = this.measures[0].TableColumn.Table.ModelName;
                var layer = GetLayer(tableName);
                query = BuildMeasureQuery();
                result = layer.ExecuteSQL(query);
                FetchAndSanitizeResults(cancellationToken, result, false);
                layer.Dispose();
            }
        }

        private T[] Sanitize<T>(object[,] values, int index)
        {
            int length = values.GetUpperBound(0) - values.GetLowerBound(0) + 1;
            T[] result = new T[length];
            for (int i = 0; i < length; ++i)
            {
                result[i] = (T)values[i, index];
            }
            return result.Distinct().OrderBy(r => r).ToArray();
        }

        private void SanitizeQueryResults(ModelQueryColumn modelQueryColumn, object[,] values, int index, CancellationToken cancellationToken)
        {
            if (modelQueryColumn is ModelQueryKeyColumn)
            {
                ModelQueryKeyColumn keyColumn = modelQueryColumn as ModelQueryKeyColumn;
                object result = null;
                switch (keyColumn.TableColumn.DataType)
                {
                    case TableMemberDataType.Bool:
                        result = this.Sanitize<bool>(values, index);
                        break;
                    case TableMemberDataType.String:
                        result = this.Sanitize<string>(values, index);
                        break;
                    case TableMemberDataType.Double:
                        result = this.Sanitize<double>(values, index);
                        break;
                    case TableMemberDataType.DateTime:
                        result = this.Sanitize<DateTime>(values, index);
                        break;
                    case TableMemberDataType.Long:
                        result = this.Sanitize<long>(values, index);
                        break;
                }
                if (keyColumn != null)
                {
                    keyColumn.Values = result;
                }
            }
            else if (modelQueryColumn is ModelQueryMeasureColumn)
            {
                object[] result = new object[2];
                result[0] = values[0, index * 2];
                result[1] = values[0, index * 2 + 1];
                ModelQueryMeasureColumn mesureColumn = modelQueryColumn as ModelQueryMeasureColumn;
                if (mesureColumn != null)
                {
                    mesureColumn.Values = result;
                    mesureColumn.Min = Convert.ToDouble(result[0]);
                    mesureColumn.Max = Convert.ToDouble(result[1]);
                }
            }
        }
    }
}
