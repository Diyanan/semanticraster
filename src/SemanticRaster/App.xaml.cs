using ESRI.ArcGIS.esriSystem;
using Semantic.Configure;
using Semantic.Controls;
using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace Semantic.MainApplication
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {

            ESRI.ArcGIS.RuntimeManager.Bind(ESRI.ArcGIS.ProductCode.EngineOrDesktop);
            InitializeEngineLicense();

            base.OnStartup(e);
        }

        private void InitializeEngineLicense()
        {
            AoInitialize aoi = new AoInitializeClass();

            //More license choices can be included here.
            esriLicenseProductCode productCode =
                esriLicenseProductCode.esriLicenseProductCodeAdvanced;
            if (aoi.IsProductCodeAvailable(productCode) == esriLicenseStatus.esriLicenseAvailable)
            {
                aoi.Initialize(productCode);
            }
        }

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            EsriMapViewModel mapVW = new EsriMapViewModel();  
            CalculatorFactory factory = new EsriCalculatorFactory(mapVW);
            VisualizationModel model = new VisualizationModel(mapVW, factory, mapVW, mapVW, mapVW, mapVW);
            HostControlViewModel hostControlViewModel = new HostControlViewModel(model);
            hostControlViewModel.CheckTable.selection = mapVW;
            ChromelessWindowBase hostWindow = new ChromelessWindowBase();
            hostControlViewModel.Title = "地理国情变化检测系统(v1.0)";
            hostWindow.DataContext = hostControlViewModel;
            hostWindow.Show();
        }

    }
}
