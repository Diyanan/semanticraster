using System;

namespace Semantic.Controls
{
    public class ModelQueryKeyColumn : ModelQueryColumn
    {
        public bool sortAscending;
        public bool sortDescending;

        public virtual KeyColumnDataType Type { get; set; }

        public bool SortAscending
        {
            get
            {
                return this.sortAscending;
            }
            set
            {
                if (value && this.SortDescending)
                    throw new ArgumentException();
                this.sortAscending = value;
            }
        }

        public bool SortDescending
        {
            get
            {
                return this.sortDescending;
            }
            set
            {
                if (value && this.SortAscending)
                    throw new ArgumentException();
                this.sortDescending = value;
            }
        }

        public bool Sort
        {
            get
            {
                return this.SortAscending || this.SortDescending;
            }
        }

        public bool UseForBuckets { get; set; }

        public bool DiscardNulls { get; set; }

        public ModelQueryMeasureColumn ModelQueryMeasureAlias { get; set; }

        public int[] Buckets { get; set; }

        public int GetBucketForRow(int row)
        {
            int num = Array.BinarySearch<int>(this.Buckets, row);
            if (num >= 0)
                return num;
            else
                return ~num - 1;
        }

        public int GetFirstRowInBucket(int row)
        {
            int num = Array.BinarySearch<int>(this.Buckets, row);
            if (num >= 0)
                return row;
            else
                return this.Buckets[~num - 1];
        }

        public int GetFirstRowInNextBucket(int row, int totalRows)
        {
            int length = this.Buckets.Length;
            int num = Array.BinarySearch<int>(this.Buckets, row);
            int index = num < 0 ? ~num : num + 1;
            if (index == length)
                return totalRows;
            else
                return this.Buckets[index];
        }

        public override void Reset()
        {
            this.ModelQueryMeasureAlias = null;
            this.Buckets = null;
            base.Reset();
        }
    }
}
