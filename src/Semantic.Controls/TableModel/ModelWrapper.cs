using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class ModelWrapper
    {
        public ILayerManager LayerManager { get; set; }

        public ModelWrapper(ILayerManager layerManager)
        {
            this.LayerManager = layerManager;
        }

        public ModelMetadata GetModelTables(LayerManager manager)
        {
            ModelMetadata modelMetadata = new ModelMetadata();

            this.GetModelTables(manager, modelMetadata);

            return modelMetadata;
        }

        public TableMetadata GetTable(LayerDefinition layerDefn)
        {
            DateTime lastRefresh = DateTime.MinValue;
            TableMetadata baseTable = new TableMetadata(layerDefn.Name.Substring(0, layerDefn.Name.IndexOf('.')), layerDefn.Name, lastRefresh);

            var fieldDefns = this.LayerManager.GetLayerFields(layerDefn);

            foreach (var field in fieldDefns)
            {
                var indexOfLastDot = field.Name.LastIndexOf('.');
                var tableName = indexOfLastDot < 0 ? baseTable.ModelName : field.Name.Substring(0, indexOfLastDot);
                var fieldName = indexOfLastDot < 0 ? field.Name : field.Name.Substring(indexOfLastDot + 1);
                TableMetadata table;
                if (string.IsNullOrEmpty(tableName))
                {
                    table = baseTable;
                }
                else
                {
                    table = baseTable.LookupTables.Where(t => t.ModelName == tableName).FirstOrDefault();
                    if (table == null)
                    {
                        table = new TableMetadata(tableName, tableName, lastRefresh);
                        baseTable.LookupTables.Add(table);
                    }
                }
                var dataType = field.DataType;
                TableColumn tableColumn = new TableColumn(table, fieldName, fieldName, dataType);
                table.AddField(tableColumn);
            }

            return baseTable;
        }

        private void GetModelTables(LayerManager manager, ModelMetadata modelMetadata)
        {
            Dictionary<string, TableMetadata> dictionary = new Dictionary<string, TableMetadata>();
            Dictionary<string, int> islandNumber = new Dictionary<string, int>();
            int num1 = 0;
            foreach (var layerDefn in manager.LayerDefinitions)
            {
                var table = GetTable(layerDefn);
                dictionary.Add(layerDefn.Name, table);
                islandNumber[layerDefn.Name] = num1++;
            }

            // TODO：添加表关系

            foreach (int num in islandNumber.Values.Distinct())
            {
                IEnumerable<string> enumerable = islandNumber.Keys.Where(modelTableName => islandNumber[modelTableName] == num);
                TableIsland island = new TableIsland();
                foreach (string index in enumerable)
                {
                    island.AddTable(dictionary[index]);
                }
                island.UpdateLookupTables();
                island.RankTables();
                island.Sort();
                modelMetadata.AddTableIsland(island);
            }
        }
    }
}
