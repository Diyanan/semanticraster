using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.Controls;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.SystemUI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Semantic.MainApplication
{
    /// <summary>
    /// 漫游、单选与采样工具
    /// </summary>
    public sealed class NavigationTool : BaseTool
    {
        private IHookHelper m_hookHelper;

        public bool CanSelectFeature { get; set; }
        public bool CanBrushFeature { get; set; }
        public IFeatureLayer FeatureLayer { get; set; }
        public IFeature selFeature { get; set; }
        public string className { get; set; }
        public event Action<int> SelectObject;
        public event Action<int,string, bool> SampleObject;

        public NavigationTool()
        { }

        public override void OnCreate(object hook)
        {
            if (m_hookHelper == null)
                m_hookHelper = new HookHelperClass();
            m_hookHelper.Hook = hook;

            var map = hook as IMapControl4;
            map.KeyIntercept = (int)esriKeyIntercept.esriKeyInterceptArrowKeys;
            map.AutoKeyboardScrolling = true;
            map.AutoMouseWheel = true;
        }

        public override void OnMouseDown(int Button, int Shift, int X, int Y)
        {
            if (this.FeatureLayer == null)
                return;
            if (Button != (int)Keys.LButton)
                return;
            m_hookHelper.FocusMap.ClearSelection();

            IFeatureClass featureClass = this.FeatureLayer.FeatureClass;
            IPoint pt = m_hookHelper.ActiveView.ScreenDisplay.DisplayTransformation.ToMapPoint(X, Y);
            ISpatialFilter tSF = new SpatialFilterClass();
            tSF.Geometry = pt as IGeometry;
            tSF.SpatialRel = esriSpatialRelEnum.esriSpatialRelWithin;

            IFeatureCursor featureCursor = featureClass.Search(tSF, false);
            IFeature feature = featureCursor.NextFeature();
            if (feature != null)
            {
                selFeature = feature;

                // 单选高亮显示
                if (CanSelectFeature)
                {
                    m_hookHelper.FocusMap.SelectFeature(this.FeatureLayer, selFeature);
                    m_hookHelper.ActiveView.PartialRefresh(esriViewDrawPhase.esriViewGeoSelection, null, m_hookHelper.ActiveView.Extent);

                    if (this.SelectObject != null)
                    {
                        this.SelectObject(feature.OID);
                    }
                }

            }

            base.OnMouseDown(Button, Shift, X, Y);
        }

        public override void OnMouseMove(int Button, int Shift, int X, int Y)
        {
            var mapCtrl = m_hookHelper.Hook as IMapControl4;
            if (Button == 0)
            {
                return;
            }
            if (Button == 1)
            {
                try
                {
                    // 漫游
                    mapCtrl.ActiveView.ScreenDisplay.TrackPan();
                }
                catch
                { }
            }
            base.OnMouseMove(Button, Shift, X, Y);
        }

        public override void OnDblClick()
        {
            if (selFeature != null && this.CanBrushFeature)
            {
                // 采样本
                int classIndex = selFeature.Fields.FindField("Sample");
                if (classIndex >= 0)
                {
                    string oldClass = selFeature.get_Value(classIndex) as string;
                    if (oldClass != null && oldClass.Trim() != "")
                    {
                        selFeature.set_Value(classIndex, "");
                        if (this.SampleObject != null)
                        {
                            this.SampleObject(selFeature.OID, className, true);
                        }
                    }
                    else
                    {
                        selFeature.set_Value(classIndex, className);
                        if (this.SampleObject != null)
                        {
                            this.SampleObject(selFeature.OID, className, false);
                        }
                    }
                    selFeature.Store();
                }

                m_hookHelper.ActiveView.PartialRefresh(esriViewDrawPhase.esriViewGeography, this.FeatureLayer, m_hookHelper.ActiveView.Extent);
            }
        }

        internal void ReadSample()
        {
            if (this.FeatureLayer != null)
            {
                int classIndex = this.FeatureLayer.FeatureClass.FindField("Sample");

                IQueryFilter filter = new QueryFilterClass();
                filter.WhereClause = "Sample <> ''";

                IFeatureCursor cursor = this.FeatureLayer.FeatureClass.Search(filter, false);
                IFeature feature = cursor.NextFeature();
                while (feature != null)
                {
                    if (this.SampleObject != null)
                    {
                        var field = feature.Value[classIndex] as string;
                        this.SampleObject(feature.OID, field, false);
                    }
                    feature = cursor.NextFeature();
                }
            }
        }
    }
}
