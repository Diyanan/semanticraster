using ESRI.ArcGIS.Controls;
using ESRI.ArcGIS.SystemUI;
using Semantic.Controls;
using System.Drawing;
using System;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Geometry;
using Semantic.EsriTools;
using Semantic.Engine;
using System.Collections.Generic;
using System.Linq;
using ESRI.ArcGIS.Geodatabase;

namespace Semantic.MainApplication
{
    public class EsriMapViewModel : MapViewModel, IVisualization, ILayerManager, ILayerTools,IJoinManager, IFeatureSelect
    {
        public AxMapControl MapControl
        {
            get
            {
                return (AxMapControl)base.Map;
            }
            set
            {
                base.Map = value;
            }
        }

        public AxToolbarControl ToolbarControl
        {
            get
            {
                return (AxToolbarControl)base.Toolbar;
            }
            set
            {
                base.Toolbar = value;
            }
        }

        private bool _BrushSample = false;

        public string PropertyBrushSample
        {
            get { return "BrushSample"; }
        }

        public bool BrushSample
        {
            get { return this._BrushSample; }
            set { SetProperty(PropertyBrushSample, ref _BrushSample, value); }
        }

        public NavigationTool Tool { get; private set; }

        public EsriMapViewModel()
        {
        }

        public override void InitializeMap()
        {
            this.MapControl = new AxMapControl();
            this.MapControl.SizeChanged += MapControl_SizeChanged;

            this.ToolbarControl = new AxToolbarControl();
        }

        private void MapControl_SizeChanged(object sender, EventArgs e)
        {
            this.MapControl.ActiveView.Refresh();
        }

        public override void OperationsAfterLoaded()
        {
            // Set the properties of AxMapControl.
            this.MapControl.Dock = System.Windows.Forms.DockStyle.None;
            this.MapControl.BorderStyle = esriControlsBorderStyle.esriNoBorder;
            // this.MapControl.BackColor = System.Drawing.Color.FromArgb(233, 233, 233);
            this.MapControl.ShowScrollbars = false;

            try
            {
                // Set the properties of toolbarControl.
                this.ToolbarControl.BackColor = Color.FromArgb(255, 255, 255);
                this.ToolbarControl.Orientation = esriToolbarOrientation.esriToolbarOrientationVertical;

                // 封装事件
                WireMapEvents();

                // Specify your .mxd file here.
                //string mxdFile = @"C:\Users\Lenovo\Desktop\test.mxd";
                //MapControl.LoadMxFile(mxdFile);

                // SetBuddyControl of MapControl
                this.ToolbarControl.SetBuddyControl(this.MapControl);
                // toolbarControl.AddItem("esriControls.ControlsOpenDocCommand");
                // toolbarControl.AddItem("esriControls.ControlsAddDataCommand");
                // toolbarControl.AddItem("esriControls.ControlsSaveAsDocCommand");
                this.ToolbarControl.AddItem("esriControls.ControlsMapNavigationToolbar");
                // toolbarControl.AddItem("esriControls.ControlsEditingToolbar");

                //this.ToolbarControl.AddItem("esriControls.ControlsEditingEditorMenu", 0, -1, false, 0, esriCommandStyles.esriCommandStyleIconOnly);
                //this.ToolbarControl.AddItem("esriControls.ControlsEditingEditTool", 0, -1, false, 0, esriCommandStyles.esriCommandStyleIconOnly);
                //this.ToolbarControl.AddItem("esriControls.ControlsEditingSketchTool", 0, -1, false, 0, esriCommandStyles.esriCommandStyleIconOnly);
                //this.ToolbarControl.AddItem("esriControls.ControlsEditingTargetToolControl", 0, -1, true, 0, esriCommandStyles.esriCommandStyleIconOnly);
                //this.ToolbarControl.AddItem("esriControls.ControlsEditingTaskToolControl", 0, -1, true, 0, esriCommandStyles.esriCommandStyleIconOnly);
                //this.ToolbarControl.AddItem("esriControls.ControlsUndoCommand", 0, -1, true, 0, esriCommandStyles.esriCommandStyleIconOnly);
                //this.ToolbarControl.AddItem("esriControls.ControlsRedoCommand", 0, -1, false, 0, esriCommandStyles.esriCommandStyleIconOnly);

            }
            catch
            { }

            // 初始化地图控件的当前工具
            this.Tool = new NavigationTool();
            this.Tool.OnCreate(this.MapControl.Object);
            this.MapControl.CurrentTool = this.Tool;
        }

        private void WireMapEvents()
        {
            this.MapControl.OnMouseMove += new IMapControlEvents2_Ax_OnMouseMoveEventHandler(mapControl_OnMouseMove);
        }

        private void mapControl_OnMouseMove(object sender, IMapControlEvents2_OnMouseMoveEvent e)
        {
            this.MapControl.Focus();
            // System.Console.WriteLine(e.mapX.ToString());
        }

        public ILayer GetLayer(LayerDefinition layerDef)
        {
            return EsriHelper.GetLayer(this.MapControl, layerDef.Name);
        }

        public void ZoomToLayer(LayerDefinition layerDef)
        {
            int layerIndex = EsriHelper.GetLayerIndex(this.MapControl, layerDef.Name);
            if (layerIndex >= 0)
            {
                ILayer layer = this.MapControl.get_Layer(layerIndex);
                IEnvelope envelope = layer.AreaOfInterest;
                this.MapControl.ActiveView.Extent = envelope;
            }
        }

        public override int GetRasterLayerBandsCount(LayerDefinition layerDef)
        {
            int layerIndex = EsriHelper.GetLayerIndex(this.MapControl, layerDef.Name);
            if (layerIndex >= 0)
            {
                IRasterLayer layer = this.MapControl.get_Layer(layerIndex) as IRasterLayer;
                EsriRasterLayer rasterLayer = new EsriRasterLayer(layer);
                return rasterLayer.BandCollection.Count;
            }
            return -1;
        }

        public void SelectFeature(LayerDefinition layerDefn, string query, GeoClass geoClass, bool first, bool zoomToLayer)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                if (geoClass != null)
                {
                    // esriLayer.SetUniqueValueStyle("Zone", geoClass, first);
                }
                esriLayer.SetFilter(query);
                this.MapControl.ActiveView.Refresh();
            }
        }

        public void ApplyClass(LayerDefinition layerDefn, string query, string leafClass)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                esriLayer.SetGeoClass(query, "Zone", leafClass);
            }
        }

        public override void SetSampleTool(bool enable, Action<int,string, bool> handler)
        {
            this.BrushSample = enable;
            this.Tool.CanBrushFeature = enable;
            if (enable)
            {
                this.Tool.SampleObject += handler;
                this.MapControl.CurrentTool = this.Tool;
            }
            else
            {
                this.Tool.SampleObject -= handler;
            }
        }

        public override void SetSampleLayer(LayerDefinition layerDefn, string className)
        {
            if (this.BrushSample)
            {
                var layer = this.GetLayer(layerDefn);
                if (layer != null)
                {
                    this.Tool.FeatureLayer = (IFeatureLayer)layer;
                    this.Tool.ReadSample();
                    this.Tool.className = className;
                }
            }
        }

        public override void SetSample(SampleEntry sample)
        {
            if (this.BrushSample)
            {
                var layer = this.GetLayer(sample.Layer);
                if (layer != null)
                {
                    this.MapControl.Map.ClearSelection();
                    EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                    var feature = esriLayer.GetFeatureById(sample.OID);
                    if (feature != null)
                    {
                        this.MapControl.Map.SelectFeature(layer, feature);
                        this.MapControl.ActiveView.Refresh();
                    }
                }
            }
        }

        public override IEnumerable<string> GetFieldName(LayerDefinition layerDefn)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                return esriLayer.GetAllFieldName();
            }
            return null;
        }


        public override int GetFeatureId(LayerDefinition layerDefn, Coordinates coords)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                var f = esriLayer.GetRelFeature(coords);
                if (f != null)
                {
                    return f.OID;
                }
            }

            return -1;
        }

        #region Visualization

        public void SelectFeature(LayerDefinition layerDefn, string query)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                esriLayer.SetFilter(query);
                this.MapControl.ActiveView.Refresh();
            }
        }

        public void SetTranspancy(LayerDefinition layerDefn, short transpancy)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                ILayerEffects pLayerEffects = (ILayerEffects)layer;
                if (pLayerEffects.SupportsTransparency == true)
                {
                    pLayerEffects.Transparency = transpancy;
                    this.MapControl.ActiveView.Refresh();
                }
            }
        }

        public void SetSimpleStyle(LayerDefinition layerDefn, SymbolProperties symbol)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                esriLayer.SetSimpleStyle(symbol);
                this.MapControl.ActiveView.Refresh();
            }
        }

        public SymbolProperties GetSimpleStyle(LayerDefinition layerDefn)
        {
            SymbolProperties symbol = new SymbolProperties();
            var layer = this.GetLayer(layerDefn);
            symbol = layer.GetMapVecSymbol();

            return symbol;
        }

        public void SetUniqueStyle(LayerDefinition layerDefn, string field, IEnumerable<SymbolProperties> symbols)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                esriLayer.SetUniqueValueStyle(field, symbols);
                this.MapControl.ActiveView.Refresh();
            }
        }

        public void UpdateGeoClass(LayerDefinition layerDefn, string field, string query, string leafClass)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                esriLayer.SetGeoClass(query, field, leafClass);
                this.MapControl.ActiveView.Refresh();
            }
        }

        public IEnumerable<string> GetDispBandLabel(LayerDefinition layerDefn)
        {
            int layerIndex = EsriHelper.GetLayerIndex(this.MapControl, layerDefn.Name);
            if (layerIndex >= 0)
            {
                IRasterLayer layer = this.MapControl.get_Layer(layerIndex) as IRasterLayer;
                EsriRasterLayer rasterLayer = new EsriRasterLayer(layer);
                for (int i = 0; i < rasterLayer.DispBandCollection.Count; i++)
                {
                    yield return rasterLayer.DispBandCollection.Item(i).Bandname;
                }
            }
        }

        public void SetDispBandLabel(LayerDefinition layerDefn, string redLabel, string greenLabel, string blueLabel)
        {
            int layerIndex = EsriHelper.GetLayerIndex(this.MapControl, layerDefn.Name);
            if (layerIndex >= 0)
            {
                IRasterLayer layer = this.MapControl.get_Layer(layerIndex) as IRasterLayer;
                EsriRasterLayer rasterLayer = new EsriRasterLayer(layer);
                rasterLayer.SetStyle(redLabel, greenLabel, blueLabel);
                this.MapControl.ActiveView.Refresh();
            }
        }

        #endregion

        #region LayerManager

        public void AddLayer(LayerDefinition layerDef)
        {
            // 
            ILayer layer = null;
            if (layerDef.LayerType == LayerType.Vector)
            {
                layer = EsriHelper.OpenFeatureLayer(layerDef.DataSource);
            }
            else
            {
                layer = EsriHelper.OpenRasterDataset(layerDef.DataSource);
            }

            if (layer != null)
            {
                layer.Name = layerDef.Name;
                this.MapControl.AddLayer(layer);
                ZoomToLayer(layerDef);
            }
        }

        public void RemoveLayer(LayerDefinition layerDef)
        {
            //
            int layerIndex = EsriHelper.GetLayerIndex(this.MapControl, layerDef.Name);
            if (layerIndex >= 0)
            {
                this.MapControl.DeleteLayer(layerIndex);
            }
        }

        public void MoveLayer(LayerDefinition layerDefn, int toIndex)
        {
            var fromIndex = EsriHelper.GetLayerIndex(this.MapControl, layerDefn.Name);
            this.MapControl.MoveLayerTo(fromIndex, toIndex);
            this.MapControl.ActiveView.Refresh();
        }


        public void SetLayerVisible(LayerDefinition layerDef, bool visible)
        {
            int layerIndex = EsriHelper.GetLayerIndex(this.MapControl, layerDef.Name);
            if (layerIndex >= 0)
            {
                ILayer layer = this.MapControl.get_Layer(layerIndex);
                layer.Visible = visible;
                this.MapControl.ActiveView.Refresh();
            }
        }

        #endregion

        public override void SwitchSelectedFeature(bool canSelectedFeature)
        {
            this.Tool.CanSelectFeature = canSelectedFeature;
        }

        public IEnumerable<TableFieldDefninition> GetLayerFields(LayerDefinition layerDefinition)
        {
            var layer = this.GetLayer(layerDefinition);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                return esriLayer.GetFieldDefns();
            }

            return null;
        }

        #region JoinManager

        public IEnumerable<string> GetAllJoins(LayerDefinition layerDefinition)
        {
            var layer = this.GetLayer(layerDefinition) as IFeatureLayer;
            if (layer != null)
            {
                return layer.GetAllJoinNames();
            }
            return null;
        }

        public void AddJoinTable(LayerDefinition layerDefinition, string csvPath)
        {
            var layer = this.GetLayer(layerDefinition) as IFeatureLayer;
            if (layer != null)
            {
                EsriCsvTable table = new EsriCsvTable(csvPath);
                layer.JoinCsvTable(table);
            }
        }

        public void RemoveJoinTable(LayerDefinition layerDefinition, string joinName)
        {
            var layer = this.GetLayer(layerDefinition) as IFeatureLayer;
            if (layer != null)
            {
                layer.RemoveJoin(joinName);
            }
        }

        public void RemoveLastJoinTable(LayerDefinition layerDefinition)
        {
            var layer = this.GetLayer(layerDefinition) as IFeatureLayer;
            if (layer != null)
            {
                layer.RemoveLastJoin();
            }
        }

        public void ClearAllJoinTable(LayerDefinition layerDefinition)
        {
            var layer = this.GetLayer(layerDefinition) as IFeatureLayer;
            if (layer != null)
            {
                layer.RemoveAllJoin();
            }
        }

        #endregion

        public override void SaveTrainFile(LayerDefinition layerDefn, List<string> fields, IEnumerable<SampleEntry> samples)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                esriLayer.GenerateTrainAndTestSet(fields, samples);
            }
        }

        public override void UpdateClass(LayerDefinition layerDefn, IEnumerable<string> res)
        {
            var layer = this.GetLayer(layerDefn);
            if (layer != null)
            {
                EsriVectorLayer esriLayer = new EsriVectorLayer(layer);
                int i = 0;
                IFeatureCursor featureCursor = esriLayer.FeatureClass.Update(null, false); 
                IFeature feature = null;
                while ((feature = featureCursor.NextFeature()) != null)
                {
                    feature.Value[3] = res.ElementAt(i);
                    featureCursor.UpdateFeature(feature);
                    i++;
                }
            }
            System.Windows.MessageBox.Show("分类完成！");
        }

        public override string GetShpfilePath(string name)
        {
            int Count = this.MapControl.LayerCount;
            string str=null;
            for (int i = 0; i < Count; i++)
            {
                if (this.MapControl.get_Layer(i).Name == name)
                {
                    IDataLayer2 dlayer = MapControl.get_Layer(i) as IDataLayer2;
                    IDatasetName ds = dlayer.DataSourceName as IDatasetName;
                    IWorkspaceName ws = ds.WorkspaceName;
                    str = ws.PathName + "\\" + MapControl.get_Layer(i).Name + ".shp";
                }
            }
            return str;
        }

        public override void ReadMxdPrj(string Mxdpath)
        {
            if (this.MapControl.CheckMxFile(Mxdpath))
            {
                this.MapControl.ActiveView.Clear();
                this.MapControl.LoadMxFile(Mxdpath);
                this.MapControl.Refresh();
            }
            else
            {
                System.Windows.MessageBox.Show("无法打开工程文件！");
            };
        }

        //public List<string> GetLayersPath()
        //{
        //    this.MapControl.Refresh();
        //    List<string> layerList = new List<string>();
        //    for (int i = 0; i < MapControl.LayerCount; i++)
        //    {
        //        IDataLayer2 dlayer = MapControl.get_Layer(i) as IDataLayer2;
        //        IDatasetName ds = dlayer.DataSourceName as IDatasetName;
        //        IWorkspaceName ws = ds.WorkspaceName;
        //        string str = ws.PathName + "\\" + MapControl.get_Layer(i).Name; 
        //        layerList.Add(str);
        //    }
        //    return layerList;
        //}

        public override List<string> GetAllLayerPath()
        {
            List<string> path = new List<string>();

            ESRI.ArcGIS.esriSystem.UID id = new ESRI.ArcGIS.esriSystem.UIDClass();
            id.Value = "{40A9E885-5533-11d0-98BE-00805F7CED21}";
            IEnumLayer layers = MapControl.Map.get_Layers(id, true);
            ILayer layer = null;
            while ((layer = layers.Next()) != null)
            {
                var l = layer as IFeatureLayer;
                var a = l.Name;
            }

            for (int i = 0; i < MapControl.LayerCount; i++)
            {
                ILayer pGL = MapControl.get_Layer(i);
                if (pGL is IGroupLayer)
                {
                   ICompositeLayer pGroupLayer = pGL as ICompositeLayer;
                   for (int j = 0; j < pGroupLayer.Count; j++)
                   {
                        ILayer pCompositeLayer;
                        pCompositeLayer = pGroupLayer.get_Layer(j);
                        IDataLayer2 dlayer = pCompositeLayer as IDataLayer2;
                        IDatasetName ds = dlayer.DataSourceName as IDatasetName;
                        IWorkspaceName ws = ds.WorkspaceName;
                        string str = ws.PathName + "\\" + pCompositeLayer.Name;
                        try
                        {
                            dlayer.GetType();
                            string str1 = (dlayer as ILayer).GetDataSource();
                        }
                        catch (Exception ex)
                        {
                        }
                        // string str1 = MapControl.get_Layer(i).GetDataSource();
                        path.Add(str);
                    }
                }
                else
                {
                    IDataLayer2 dlayer = pGL as IDataLayer2;
                    IDatasetName ds = dlayer.DataSourceName as IDatasetName;
                    IWorkspaceName ws = ds.WorkspaceName;
                    string str = ws.PathName + "\\" + pGL.Name;
                    try
                    {
                        dlayer.GetType();
                        string str1 = (dlayer as ILayer).GetDataSource();
                    }
                    catch (Exception ex)
                    {
                    }
                    // string str1 = MapControl.get_Layer(i).GetDataSource();
                    path.Add(str);
                }
            }
            return path;
        }

        public override List<string> GetGrouplayer(string groupname)
        {
            List<string> path = new List<string>();
            if (groupname != null)
            {
                #region
                for (int i = 0; i < MapControl.LayerCount; i++)
                {
                    ILayer pGL = MapControl.get_Layer(i);
                    if (pGL is IGroupLayer && pGL.Name == groupname)
                    {
                        ICompositeLayer pGroupLayer = pGL as ICompositeLayer;
                        for (int j = 0; j < pGroupLayer.Count; j++)
                        {
                            ILayer pCompositeLayer;
                            pCompositeLayer = pGroupLayer.get_Layer(j);
                            IDataLayer2 dlayer = pCompositeLayer as IDataLayer2;
                            IDatasetName ds = dlayer.DataSourceName as IDatasetName;
                            IWorkspaceName ws = ds.WorkspaceName;
                            string str = ws.PathName + "\\" + pCompositeLayer.Name;
                            try
                            {
                                dlayer.GetType();
                                string str1 = (dlayer as ILayer).GetDataSource();
                            }
                            catch (Exception ex)
                            {
                            }
                            // string str1 = MapControl.get_Layer(i).GetDataSource();
                            path.Add(str);
                        }
                    }
                }
                #endregion
                return path;
            }
            else
            {
                return null;
            }
        }

        public override List<FeatureTable> GetFeatureTables(LayerDefinition lyr)
        {
            var layer = this.GetLayer(lyr);
            List<FeatureTable> tablelist = new List<FeatureTable>();
            if(layer != null)
            {
                IFeatureLayer flayer = layer as IFeatureLayer;
                var featureclass = flayer.FeatureClass;
                IQueryFilter pQueryFilter = null;
                IFeatureCursor pFeatureCursor = null;
                IFeature pFeature = null;
                pQueryFilter = new QueryFilterClass();
                pQueryFilter.WhereClause = "";
                pFeatureCursor = featureclass.Search(pQueryFilter, false);
                pFeature = pFeatureCursor.NextFeature();
                while (pFeature != null)
                {
                    var ptable = new FeatureTable();
                    ptable.FID = int.Parse(pFeature.get_Value(pFeature.Fields.FindField("FID")).ToString());
                    ptable.CC = int.Parse(pFeature.get_Value(pFeature.Fields.FindField("CC")).ToString());
                    tablelist.Add(ptable);
                    pFeature = pFeatureCursor.NextFeature();
                }
                return tablelist;
             }
            else
            {
                return null;
            }
        }

        public void SelectFeature(LayerDefinition lyr, int fid)
        {
            this.MapControl.Refresh();
            var layer = this.GetLayer(lyr);
            if (layer != null)
            {
                this.MapControl.Map.ClearSelection();
                IFeatureLayer pFeatureLayer = layer as IFeatureLayer;
                var pFeatureClass = pFeatureLayer.FeatureClass;
                IFeature pFeature = pFeatureClass.GetFeature(fid);
                IEnvelope env = pFeature.ShapeCopy.Envelope;
                env.Expand(3, 3, true);
                this.MapControl.Extent = env;
                this.MapControl.Map.SelectFeature(layer, pFeature);
                this.MapControl.ActiveView.PartialRefresh(esriViewDrawPhase.esriViewGeography, null, this.MapControl.ActiveView.Extent);
            }
            else
            {

            }
            
        }
    }
}
