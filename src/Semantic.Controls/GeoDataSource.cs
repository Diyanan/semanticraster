using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    /// <summary>
    /// 基于空间图层的属性查询
    /// </summary>
    public class GeoDataSource
    {
        public LayerDefinition Layer { get; set; }
        public Filter Filter { get; set; }
        public string Name { get; set; }
        private long queryCount;

        public GeoDataSource(LayerDefinition layerDefn)
        {
            this.Layer = layerDefn;
        }

        protected ModelQuery InstantiateModelQuery(string name, Filter filter, CancellationToken cancellationToken)
        {
            return new OgrModelQuery(name, filter, this.Layer, cancellationToken);
        }

        public void QueryData(CancellationToken cancellationToken)
        {
            Filter filterForMainQuery = this.Filter ?? new Filter();
            List<Tuple<FilterClause, Filter>> forPropertyQueries = filterForMainQuery.GetFiltersForPropertyQueries();
            forPropertyQueries.ForEach(filterClauseWithFilter =>
            Task.Factory.StartNew(
                () =>
                this.RunQueryForFilterProperties(filterClauseWithFilter.Item1,
                filterClauseWithFilter.Item2,
                cancellationToken)));
        }

        public void RunQueryForFilterProperties(FilterClause filterClause, Filter filter, CancellationToken cancellationToken)
        {
            ModelQuery modelQuery = null;
            try
            {
                TableMember tableMember = null;
                modelQuery = this.InstantiateModelQuery("QueryForFilterProperties", filter, cancellationToken);
                List<Tuple<TableField, AggregationFunction>> list = null;
                ModelQueryKeyColumn key = null;
                ModelQueryMeasureColumn measure = null;
                if (filterClause is NumericRangeFilterClause)
                {
                    list = new List<Tuple<TableField, AggregationFunction>>();
                    list.Add(new Tuple<TableField, AggregationFunction>(
                        filterClause.TableMember,
                        filterClause.AggregationFunction));
                }
                else if (filterClause is CategoryFilterClause<double>)
                {
                    ModelQueryKeyColumn modelQueryKeyColumn = new ModelQueryKeyColumn();
                    modelQueryKeyColumn.TableColumn = filterClause.TableMember;
                    modelQueryKeyColumn.Type = KeyColumnDataType.Double;
                    modelQueryKeyColumn.SortAscending = true;
                    modelQueryKeyColumn.SortDescending = false;
                    modelQueryKeyColumn.UseForBuckets = false;
                    modelQueryKeyColumn.DiscardNulls = false;
                    modelQueryKeyColumn.FetchValues = true;
                    key = modelQueryKeyColumn;
                    modelQuery.AddKey(key);
                }
                else if (filterClause is CategoryFilterClause<string>)
                {
                    ModelQueryKeyColumn modelQueryKeyColumn = new ModelQueryKeyColumn();
                    modelQueryKeyColumn.TableColumn = filterClause.TableMember;
                    modelQueryKeyColumn.Type = KeyColumnDataType.String;
                    modelQueryKeyColumn.SortAscending = true;
                    modelQueryKeyColumn.SortDescending = false;
                    modelQueryKeyColumn.UseForBuckets = false;
                    modelQueryKeyColumn.DiscardNulls = false;
                    modelQueryKeyColumn.FetchValues = true;
                    key = modelQueryKeyColumn;
                    modelQuery.AddKey(key);
                }
                else if (filterClause is CategoryFilterClause<DateTime>)
                {
                    ModelQueryKeyColumn modelQueryKeyColumn = new ModelQueryKeyColumn();
                    modelQueryKeyColumn.TableColumn = filterClause.TableMember;
                    modelQueryKeyColumn.Type = KeyColumnDataType.DateTime;
                    modelQueryKeyColumn.SortAscending = true;
                    modelQueryKeyColumn.SortDescending = false;
                    modelQueryKeyColumn.UseForBuckets = false;
                    modelQueryKeyColumn.DiscardNulls = false;
                    modelQueryKeyColumn.FetchValues = true;
                    key = modelQueryKeyColumn;
                    modelQuery.AddKey(key);
                    tableMember = filterClause.TableMember;
                }
                else if (filterClause is CategoryFilterClause<bool>)
                {
                    ModelQueryKeyColumn modelQueryKeyColumn = new ModelQueryKeyColumn();
                    modelQueryKeyColumn.TableColumn = filterClause.TableMember;
                    modelQueryKeyColumn.Type = KeyColumnDataType.Bool;
                    modelQueryKeyColumn.SortAscending = false;
                    modelQueryKeyColumn.SortDescending = true;
                    modelQueryKeyColumn.UseForBuckets = false;
                    modelQueryKeyColumn.DiscardNulls = false;
                    modelQueryKeyColumn.FetchValues = true;
                    key = modelQueryKeyColumn;
                    modelQuery.AddKey(key);
                }
                else if (filterClause is AndOrFilterClause)
                {
                    ModelQueryKeyColumn modelQueryKeyColumn = new ModelQueryKeyColumn();
                    modelQueryKeyColumn.TableColumn = filterClause.TableMember;
                    modelQueryKeyColumn.Type = KeyColumnDataType.DateTime;
                    modelQueryKeyColumn.SortAscending = true;
                    modelQueryKeyColumn.SortDescending = false;
                    modelQueryKeyColumn.UseForBuckets = false;
                    modelQueryKeyColumn.DiscardNulls = true;
                    modelQueryKeyColumn.FetchValues = true;
                    key = modelQueryKeyColumn;
                    modelQuery.AddKey(key);
                }
                else
                {
                    throw new NotImplementedException("Unknown filter clause type" + filterClause.GetType().Name);
                }

                if (list != null)
                {
                    ModelQueryMeasureColumn queryMeasureColumn = new ModelQueryMeasureColumn();
                    queryMeasureColumn.TableColumn = list[0].Item1 as TableMember;
                    queryMeasureColumn.AggregationFunction = list[0].Item2;
                    queryMeasureColumn.Accumulate = ModelQueryMeasureColumn.AccumulationType.NoAccumulation;
                    queryMeasureColumn.ModelQueryIndexedKey = null;
                    queryMeasureColumn.FetchValues = true;
                    measure = queryMeasureColumn;
                    modelQuery.AddMeasure(measure);
                }
                cancellationToken.ThrowIfCancellationRequested();
                modelQuery.QueryData(cancellationToken);
                cancellationToken.ThrowIfCancellationRequested();
                if (key != null)
                    filterClause.UpdateProperties(filter.MajorVersion, key);
                else
                    filterClause.UpdateProperties(filter.MajorVersion, measure);
            }
            catch (Exception ex)
            {

            }
            finally
            {
                try
                {
                    if (modelQuery != null)
                        modelQuery.Shutdown();
                }
                catch (Exception ex)
                {
                }
            }
        }

    }
}
