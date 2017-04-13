using OSGeo.GDAL;
using OSGeo.OGR;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public class OgrLayer : ITableMember, IDisposable
    {
        public bool Writable { get; private set; }
        public string Name { get; private set; }
        public string Path { get; private set; }
        public DataSource DataSource{get;set;}
        public Layer Layer { get; private set; }

        /// <summary>
        /// 默认构造函数
        /// </summary>
        public OgrLayer()
        { }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="file"></param>
        /// <param name="writable"></param>
        public OgrLayer(string file, bool writable)
        {
            this.Name = System.IO.Path.GetFileNameWithoutExtension(file);
            this.Path = file;
            this.Writable = writable;
            OpenLayer(file, writable);
        }

        /// <summary>
        /// 打开图层
        /// </summary>
        /// <param name="file"></param>
        /// <param name="writable"></param>
        private void OpenLayer(string file, bool writable)
        {
            // GdalConfiguration.ConfigureOgr();
            Ogr.RegisterAll();
            Gdal.SetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
            Gdal.SetConfigOption("SHAPE_ENCODING", "");
            int update = writable ? 1 : 0;
            this.DataSource = Ogr.Open(file, update);
            this.Layer = this.DataSource.GetLayerByIndex(0);
        }

        /// <summary>
        /// 获取所有字段
        /// </summary>
        /// <returns></returns>
        public IEnumerable<string> GetFieldNames()
        {
            var featureDefn = this.Layer.GetLayerDefn();
            for (int i = 0; i < featureDefn.GetFieldCount(); i++)
            {
                var fieldDefn = featureDefn.GetFieldDefn(i);
                yield return fieldDefn.GetName().ToUpper();
            }
        }

        private TableFieldDefninition ConvertFieldDefn(FieldDefn field)
        {
            var fieldType = field.GetFieldType();
            TableMemberDataType dataType = TableMemberDataType.Unknown;
            switch (fieldType)
            {
                case FieldType.OFTDate:
                case FieldType.OFTTime:
                case FieldType.OFTDateTime:
                    dataType = TableMemberDataType.DateTime;
                    break;
                case FieldType.OFTReal:
                    dataType = TableMemberDataType.Double;
                    break;
                case FieldType.OFTInteger:
                    dataType = TableMemberDataType.Long;
                    break;
                case FieldType.OFTString:
                    dataType = TableMemberDataType.String;
                    break;
            }

            if (dataType == TableMemberDataType.Unknown)
            {
                return null;
            }
            else
            {
                return new TableFieldDefninition
                {
                    Name = field.GetName(),
                    DataType = dataType
                };
            }
        }

        /// <summary>
        /// 获取所有字段
        /// </summary>
        /// <returns></returns>
        public IEnumerable<TableFieldDefninition> GetFieldDefns()
        {
            var featureDefn = this.Layer.GetLayerDefn();
            for (int i = 0; i < featureDefn.GetFieldCount(); i++)
            {
                var field = featureDefn.GetFieldDefn(i);
                var result = ConvertFieldDefn(field);
                if (result != null)
                {
                    yield return result;
                }
            }
        }

        public void CorrectFieldDefn()
        {
            var featureDefn = this.Layer.GetLayerDefn();
            for (int i = 0; i < featureDefn.GetFieldCount(); i++)
            {
                var field = featureDefn.GetFieldDefn(i);
                if (field.GetName() == "fid")
                {
                    field.SetType(FieldType.OFTInteger);
                }
                else
                {
                    field.SetType(FieldType.OFTReal);
                }
            }
        }

        /// <summary>
        /// 创建字段
        /// </summary>
        /// <param name="field"></param>
        /// <param name="fieldType"></param>
        public void CreateField(string field, FieldType fieldType)
        {
            if (this.Writable)
            {
                //添加字段并附属性值
                if (this.Layer.GetLayerDefn().GetFieldIndex(field) < 0)
                {
                    FieldDefn oFieldID = new FieldDefn(field, fieldType);
                    this.Layer.CreateField(oFieldID, 1);
                }
            }
        }

        /// <summary>
        /// 创建DoubleField
        /// </summary>
        /// <param name="field"></param>
        public void CreateDoubleField(string field)
        {
            CreateField(field, FieldType.OFTReal);
        }

        /// <summary>
        /// 更新要素
        /// </summary>
        /// <param name="feature"></param>
        public void UpdateFeature(Feature feature)
        {
            this.Layer.SetFeature(feature);
        }

        /// <summary>
        /// 根据Id获取要素
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public Feature GetFeatureById(int id)
        {
            return this.Layer.GetFeature(id);
        }

        /// <summary>
        /// 获取所有要素
        /// </summary>
        /// <returns></returns>
        public IEnumerable<Feature> GetAllFeatures()
        {
            Feature feature = null;
            while ((feature = this.Layer.GetNextFeature()) != null)
            {
                yield return feature;
            }
        }

        /// <summary>
        /// 获取所有样本要素
        /// </summary>
        /// <param name="samples"></param>
        /// <returns></returns>
        public IEnumerable<Feature> GetSampleFeatures(IEnumerable<SampleEntry> samples)
        {
            foreach (var s in samples)
            {
                yield return GetFeatureById(s.OID);
            }
        }

        /// <summary>
        /// 刷新缓存到磁盘
        /// </summary>
        /// <param name="needRepack"></param>
        public void SyncToDisk(bool needRepack)
        {
            if (this.Writable)
            {
                if(needRepack)
                {
                    this.DataSource.ExecuteSQL("REPACK " + this.Name, null, "");
                }
                this.DataSource.SyncToDisk();
            }
        }

        public Layer ExecuteSQL(string sql)
        {
            return this.DataSource.ExecuteSQL(sql, null, "");
        }

        /// <summary>
        /// dispose
        /// </summary>
        public void Dispose()
        {
            if (this.Layer != null)
            {
                this.Layer.Dispose();
            }

            if (this.DataSource != null)
            {
                this.SyncToDisk(false);
                this.DataSource.Dispose();
            }
        }
    }
}
