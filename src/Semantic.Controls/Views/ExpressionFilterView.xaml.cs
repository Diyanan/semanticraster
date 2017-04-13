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
    public partial class ExpressionFilterView : UserControl
    {
        public ExpressionFilterView()
        {
            InitializeComponent();
        }

        private void AndOp_OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            ExpressionFilterViewModel expressionFilterViewModel = this.DataContext as ExpressionFilterViewModel;
            if (expressionFilterViewModel == null)
                return;
            expressionFilterViewModel.IsAndConnector = true;
        }

        private void OrOp_OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            ExpressionFilterViewModel expressionFilterViewModel = this.DataContext as ExpressionFilterViewModel;
            if (expressionFilterViewModel == null)
                return;
            expressionFilterViewModel.IsAndConnector = false;
        }
    }
}
