using System;

namespace Semantic.Engine
{
    [Serializable]
    public enum TableMemberDataType
    {
        Unknown,
        String,
        Bool,
        Double,
        Long,
        DateTime,
        Currency,
    }

    public class TableFieldDefninition
    {
        public string Name { get; set; }
        public TableMemberDataType DataType { get; set; }
    }
}