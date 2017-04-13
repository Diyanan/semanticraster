using System.Windows.Controls;

namespace Semantic.Controls
{
    /// <summary>
    /// UserControl1.xaml 的交互逻辑
    /// </summary>
    public partial class MapHostView : UserControl
    {
        private MapViewModel MapVM { get; set; }

        public MapHostView()
        {
            InitializeComponent();
        }

        private void OnDataContextChanged(object sender, System.Windows.DependencyPropertyChangedEventArgs e)
        {
            this.MapVM = e.NewValue as MapViewModel;
            this.MapVM.InitializeMap();
            mapHost.Child = this.MapVM.Map;
            toolbarHost.Child = this.MapVM.Toolbar;
        }

        private void OnLoaded(object sender, System.Windows.RoutedEventArgs e)
        {
            this.MapVM.OperationsAfterLoaded();
        }
    }
}
