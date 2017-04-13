using System;
using System.Collections.Generic;
using System.Globalization;

namespace Semantic.Controls
{
    public class ModelMetadata
    {
        public List<TableIsland> TableIslands { get; private set; }

        public ModelMetadata()
        {
            this.TableIslands = new List<TableIsland>();
        }

        public void AddTableIsland(TableIsland island)
        {
            if (island == null)
                throw new ArgumentNullException("island");
            this.TableIslands.Add(island);
        }

        public void UpdateLookupTables()
        {
            foreach (TableIsland tableIsland in this.TableIslands)
                tableIsland.UpdateLookupTables();
        }

        public void RankTables()
        {
            foreach (TableIsland tableIsland in this.TableIslands)
                tableIsland.RankTables();
        }

        public void Sort()
        {
            foreach (TableIsland tableIsland in this.TableIslands)
                tableIsland.Sort();
        }

        public TableColumn Find(TableColumn col)
        {
            if (col == null || col.Table == null)
                return null;
            foreach (TableIsland tableIsland in this.TableIslands)
            {
                TableColumn tableColumn = tableIsland.Find(col);
                if (tableColumn != null)
                    return tableColumn;
            }
            return null;
        }

        public TableMeasure Find(TableMeasure measure)
        {
            if (measure == null || measure.Table == null)
                return null;
            foreach (TableIsland tableIsland in this.TableIslands)
            {
                TableMeasure tableMeasure = tableIsland.Find(measure);
                if (tableMeasure != null)
                    return tableMeasure;
            }
            return null;
        }

        public TableColumn FindVisibleTableColumnInModelMetadata(TableColumn col)
        {
            if (col == null)
                return null;
            TableColumn tableColumn = this.Find(col);
            if (tableColumn == null)
                return null;
            if (tableColumn.Visible && tableColumn.Table.Visible)
                return tableColumn;
            else
                return null;
        }

        public TableMeasure FindVisibleTableMeasureInModelMetadata(TableMeasure measure)
        {
            if (measure == null)
                return null;
            TableMeasure tableMeasure = this.Find(measure);
            if (tableMeasure == null)
                return null;
            if (tableMeasure.Visible && tableMeasure.Table.Visible)
                return tableMeasure;
            else
                return null;
        }
    }
}
