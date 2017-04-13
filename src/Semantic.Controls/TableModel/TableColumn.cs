using Semantic.Engine;
using System;

namespace Semantic.Controls
{
    public class TableColumn : TableMember
    {
        public TableColumn(TableMetadata table, string name, string modelQueryName, TableMemberDataType dataType, bool visible = true)
          : base(table, name, modelQueryName, dataType, visible)
        {
        }

        internal TableColumn(TableColumn.SerializableTableColumn state)
        {
            this.Unwrap(state);
        }

        internal override TableField.SerializableTableField Wrap()
        {
            TableColumn.SerializableTableColumn serializableTableColumn = new TableColumn.SerializableTableColumn();
            base.Wrap(serializableTableColumn);
            return serializableTableColumn;
        }

        [Serializable]
        public class SerializableTableColumn : TableMember.SerializableTableMember
        {
            internal override TableField Unwrap()
            {
                return new TableColumn(this);
            }
        }
    }
}
