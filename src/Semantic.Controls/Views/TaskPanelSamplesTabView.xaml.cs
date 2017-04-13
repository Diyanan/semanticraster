using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Semantic.Controls
{
    /// <summary>
    /// TaskPanelSamplesView.xaml 的交互逻辑
    /// </summary>
    public partial class TaskPanelSamplesTabView : UserControl
    {
        public TaskPanelSamplesTabView()
        {
            InitializeComponent();
        }

        private void dataGrid_LoadingRow(object sender, System.Windows.Controls.DataGridRowEventArgs e)
        {
            e.Row.Header = e.Row.GetIndex() + 1;
        }
    }
}
