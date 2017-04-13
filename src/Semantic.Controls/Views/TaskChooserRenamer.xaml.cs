using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
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
    /// TaskChooserRenamer.xaml 的交互逻辑
    /// </summary>
    public partial class TaskChooserRenamer : UserControl
    {
        private bool uncheckingRenameToggle;

        public TaskChooserRenamer()
        {
            InitializeComponent();
        }

        private void TaskSelectorBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (!this.uncheckingRenameToggle)
                this.TaskRenameButton.IsChecked = new bool?(false);
            else
                this.uncheckingRenameToggle = false;
        }

        private void TaskSelectorBox_Loaded(object sender, RoutedEventArgs e)
        {
            e.Handled = true;
            ((UIElement)sender).Focus();
            ((TextBoxBase)sender).SelectAll();
        }

        private void TaskRenameButton_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            bool? isChecked = ((ToggleButton)sender).IsChecked;
            if (!isChecked.HasValue || !isChecked.Value)
                return;
            this.uncheckingRenameToggle = true;
        }

        private void TaskRenameButton_Checked(object sender, RoutedEventArgs e)
        {
            this.TaskSelectorBox.Focus();
        }
    }
}
