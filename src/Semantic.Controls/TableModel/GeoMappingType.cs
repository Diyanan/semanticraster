using System;

namespace Semantic.Controls
{
    [Serializable]
    public enum GeoMappingType
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
        Zip,
        Country,
        XCoord,
        YCoord,
    }
}
