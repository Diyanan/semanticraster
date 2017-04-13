using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.DataSourcesOleDB;
using ESRI.ArcGIS.Geodatabase;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Semantic.EsriTools
{
    public static class FLJoinExtension
    {
        #region Helper

        private static IRelationshipClass CreateRelationship(string name, ITable dispTable, ITable relateTable, string foreignKey)
        {
            IMemoryRelationshipClassFactory fac = new MemoryRelationshipClassFactoryClass();
            return fac.Open(
                name,
                (IObjectClass)dispTable, dispTable.OIDFieldName,
                (IObjectClass)relateTable, foreignKey,
                "", "",
                esriRelCardinality.esriRelCardinalityOneToOne);
        }

        private static void ResetDispTable(IDisplayRelationshipClass dispRC, ITable table)
        {
            if (table is IRelQueryTable)
            {
                IRelQueryTable relQueryTable = table as IRelQueryTable;
                IRelQueryTableInfo rqtInfo = relQueryTable as IRelQueryTableInfo;
                dispRC.DisplayRelationshipClass(relQueryTable.RelationshipClass, rqtInfo.JoinType);
            }
            else
            {
                dispRC.DisplayRelationshipClass(null, esriJoinType.esriLeftOuterJoin);
            }
        }

        private static IEnumerable<IRelationshipClass> GetJoins(IFeatureLayer feaLayer)
        {
            IDisplayRelationshipClass dispRelClass = feaLayer as IDisplayRelationshipClass;
            List<IRelationshipClass> joins = new List<IRelationshipClass>();
            if (dispRelClass.RelationshipClass != null)
            {
                var relClass = dispRelClass.RelationshipClass;
                while (relClass != null)
                {
                    yield return relClass;
                    IObjectClass origClass = relClass.OriginClass;
                    IObjectClass destClass = relClass.DestinationClass;
                    IRelQueryTable relQueryTable = null;
                    if (origClass is IRelQueryTable)
                    {
                        relQueryTable = origClass as IRelQueryTable;
                    }
                    else if (destClass is IRelQueryTable)
                    {
                        relQueryTable = destClass as IRelQueryTable;
                    }
                    if (relQueryTable != null)
                    {
                        relClass = relQueryTable.RelationshipClass;
                    }
                    else
                    {
                        relClass = null;
                    }
                }
            }
        }

        private static IRelationshipClass GetJoin(IFeatureLayer feaLayer, string joinName)
        {
            foreach (var relClass in GetJoins(feaLayer))
            {
                IObjectClass destClass = relClass.DestinationClass;
                if (destClass.AliasName == joinName)
                {
                    return relClass;
                }
            }

            return null;
        }

        #endregion

        public static bool HasJoinTable(this IFeatureLayer feaLayer)
        {
            IDisplayRelationshipClass dispRC = feaLayer as IDisplayRelationshipClass;
            IDisplayTable dispTable = feaLayer as IDisplayTable;
            ITable table = dispTable.DisplayTable;
            return table is IRelQueryTable;
        }

        public static IEnumerable<string> GetAllJoinNames(this IFeatureLayer feaLayer)
        {
            foreach (var join in GetJoins(feaLayer))
            {
                yield return join.DestinationClass.AliasName;
            }
        }

        public static void JoinCsvTable(this IFeatureLayer feaLayer, EsriCsvTable csvTable)
        {
            var join = GetJoin(feaLayer, csvTable.Name);
            if (join != null)
            {
                System.Windows.Forms.MessageBox.Show("Related!!");
                return;
            }

            // 连接join
            ITable dispTable = ((IDisplayTable)feaLayer).DisplayTable;
            ITable relateTable = csvTable.Table;
            IRelationshipClass relClass = CreateRelationship(csvTable.Name, dispTable, relateTable, "fid");
            IDisplayRelationshipClass dispRelClass = feaLayer as IDisplayRelationshipClass;
            dispRelClass.DisplayRelationshipClass(relClass, esriJoinType.esriLeftOuterJoin);
        }

        public static void RemoveJoin(this IFeatureLayer feaLayer, string joinName)
        {
            var relClass = GetJoin(feaLayer, joinName);
            if (relClass != null)
            {
                IDisplayRelationshipClass dispRelClass = feaLayer as IDisplayRelationshipClass;
                ITable table = (ITable)relClass.OriginClass;
                ResetDispTable(dispRelClass, table);
            }
        }

        public static void RemoveAllJoin(this IFeatureLayer feaLayer)
        {
            IDisplayRelationshipClass dispRC = feaLayer as IDisplayRelationshipClass;
            dispRC.DisplayRelationshipClass(null, esriJoinType.esriLeftOuterJoin);
        }

        public static void RemoveLastJoin(this IFeatureLayer feaLayer)
        {
            if (!HasJoinTable(feaLayer))
            {
                System.Windows.Forms.MessageBox.Show("The layer or table is not joined");
                return;
            }
            IDisplayRelationshipClass dispRC = feaLayer as IDisplayRelationshipClass;
            IRelQueryTable relQueryTable = ((IDisplayTable)feaLayer).DisplayTable as IRelQueryTable;
            var table = relQueryTable.SourceTable;
            ResetDispTable(dispRC, table);
        }
    }
}
