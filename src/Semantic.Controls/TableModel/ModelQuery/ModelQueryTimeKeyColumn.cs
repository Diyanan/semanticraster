﻿using System;

namespace Semantic.Controls
{
    public class ModelQueryTimeKeyColumn : ModelQueryKeyColumn
    {
        public TimeChunkPeriod TimeChunk { get; set; }

        public DateTime[] StartTime
        {
            get
            {
                return (DateTime[])this.Values;
            }
        }

        public DateTime[] EndTime { get; set; }

        public DateTime Min { get; set; }

        public DateTime Max { get; set; }

        public override void Reset()
        {
            this.EndTime = null;
            this.Min = DateTime.MaxValue;
            this.Max = DateTime.MaxValue;
            base.Reset();
        }
    }
}
