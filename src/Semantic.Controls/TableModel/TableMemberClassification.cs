using System;

namespace Semantic.Controls
{
    [Serializable]
    public enum TableMemberClassification
    {
        None,
        Latitude,
        Longitude,
        Address,
        Other,
        Street,
        City,
        County,
        State,
        PostalCode,
        Country,
        XCoord,
        YCoord,
    }
}
