using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.IO;
using System.Linq;
using System.Text;
using System.Web;

namespace Jitbit.Utils
{
    /// <summary>
    /// Simple CSV export
    /// Example:
    ///   CsvExport myExport = new CsvExport();
    ///
    ///   myExport.AddRow();
    ///   myExport["Region"] = "New York, USA";
    ///   myExport["Sales"] = 100000;
    ///   myExport["Date Opened"] = new DateTime(2003, 12, 31);
    ///
    ///   myExport.AddRow();
    ///   myExport["Region"] = "Sydney \"in\" Australia";
    ///   myExport["Sales"] = 50000;
    ///   myExport["Date Opened"] = new DateTime(2005, 1, 1, 9, 30, 0);
    ///
    /// Then you can do any of the following three output options:
    ///   string myCsv = myExport.Export();
    ///   myExport.ExportToFile("Somefile.csv");
    ///   byte[] myCsvData = myExport.ExportToBytes();
    /// </summary>
    internal class CsvExport : IDisposable
    {
        private bool disposed = false;

        /// <summary>
        /// To keep the ordered list of column names
        /// </summary>
        private List<string> fields = new List<string>();

        /// <summary>
        /// The list of rows
        /// </summary>
        private List<Dictionary<string, object>> cacheRows = new List<Dictionary<string, object>>();

        /// <summary>
        /// The current row
        /// </summary>
        private Dictionary<string, object> currentRow { get { return cacheRows[cacheRows.Count - 1]; } }

        private bool hasHeaderBeenWritten = false;

        private TextWriter writer;

        /// <summary>
        /// Set a value on this column
        /// </summary>
        public object this[string field]
        {
            set
            {
                if (fields.Contains(field))
                {
                    currentRow[field] = value;
                }
            }
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="writer"></param>
        public CsvExport(TextWriter writer)
        {
            this.writer = writer;
        }

        /// <summary>
        /// Call this before setting any fields on a row
        /// </summary>
        public void AddRow()
        {
            if (cacheRows.Count > 500)
            {
                SyncToDisk();
            }
            cacheRows.Add(new Dictionary<string, object>());
        }

        public void AddRow<T>(T obj)
        {
            AddRow();
            var values = obj.GetType().GetProperties();
            foreach (var value in values)
            {
                this[value.Name] = value.GetValue(obj, null);
            }
        }

        /// <summary>
        /// Add a list of typed objects, maps object properties to CsvFields
        /// </summary>
        public void AddRows<T>(IEnumerable<T> list)
        {
            if (list.Any())
            {
                foreach (var obj in list)
                {
                    AddRow(obj);
                }
            }
        }

        /// <summary>
        /// Converts a value to how it should output in a csv file
        /// If it has a comma, it needs surrounding with double quotes
        /// Eg Sydney, Australia -> "Sydney, Australia"
        /// Also if it contains any double quotes ("), then they need to be replaced with quad quotes[sic] ("")
        /// Eg "Dangerous Dan" McGrew -> """Dangerous Dan"" McGrew"
        /// </summary>
        public static string MakeValueCsvFriendly(object value)
        {
            if (value == null) return "";
            if (value is INullable && ((INullable)value).IsNull) return "";
            if (value is DateTime)
            {
                if (((DateTime)value).TimeOfDay.TotalSeconds == 0)
                    return ((DateTime)value).ToString("yyyy-MM-dd");
                return ((DateTime)value).ToString("yyyy-MM-dd HH:mm:ss");
            }
            string output = value.ToString().Trim();
            if (output.Contains(",") || output.Contains("\"") || output.Contains("\n") || output.Contains("\r"))
                output = '"' + output.Replace("\"", "\"\"") + '"';

            if (output.Length > 30000) //cropping value for stupid Excel
            {
                if (output.EndsWith("\""))
                {
                    output = output.Substring(0, 30000);
                    if (output.EndsWith("\"") && !output.EndsWith("\"\"")) //rare situation when cropped line ends with a '"'
                        output += "\""; //add another '"' to escape it
                    output += "\"";
                }
                else
                    output = output.Substring(0, 30000);
            }
            return output;
        }

        /// <summary>
        /// add field
        /// </summary>
        /// <param name="fieldName"></param>
        public void AddField(string fieldName)
        {
            if (!this.fields.Contains(fieldName))
            {
                this.fields.Add(fieldName);
            }
        }

        private void SyncToDisk()
        {
            // 文件头
            if (!hasHeaderBeenWritten)
            {
                // The header
                var header = string.Join(",", fields.ToArray());
                this.writer.WriteLine(header);
                this.hasHeaderBeenWritten = true;
            }

            // 数据行
            foreach (Dictionary<string, object> row in cacheRows)
            {
                var line = string.Join(",", fields.Select(field => MakeValueCsvFriendly(row[field])).ToArray());
                this.writer.WriteLine(line);
            }

            this.cacheRows.Clear();
        }

        public void Dispose()
        {
            SyncToDisk();

            if (disposed)
            {
                return;
            }
            if (writer != null)
            {
                writer.Dispose();
            }
            disposed = true;
            writer = null;
        }
    }
}