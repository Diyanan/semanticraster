using System;

namespace Semantic.Controls
{
    public abstract class XYField : GeoField
    {
        [Serializable]
        public class SerializableXYField : GeoField.SerializableGeoField
        {
            public TableColumn.SerializableTableColumn XCoord;
            public TableColumn.SerializableTableColumn YCoord;

            internal override TableField Unwrap()
            {
                return (TableField)new LatLongField(this);
            }
        }
    }
}
