using Microsoft.Win32;
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
    /// ChangeSettingsView.xaml 的交互逻辑
    /// </summary>
    public partial class ChangeSettingsView : UserControl
    {
        private ChangeSettingsViewModel _ViewModel;
        public ChangeSettingsView()
        {
            InitializeComponent();

            this.DataContextChanged += OnDataContextChanged;
        }

        private void OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            this._ViewModel = e.NewValue as ChangeSettingsViewModel;
        }

        private string GetCsvFile()
        {
            // 选择文件
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Filter = "表格数据|*.csv";
            if (openFile.ShowDialog() == true)
            {
                return openFile.FileName;
            }
            return "";
        }

        private void btnPreBrowser_Click(object sender, RoutedEventArgs e)
        {
            var file =  GetCsvFile();
            if (!string.IsNullOrEmpty(file))
            {
                this._ViewModel.PreFile = file;
            }
        }

        private void btnPostBrowser_Click(object sender, RoutedEventArgs e)
        {
            var file = GetCsvFile();
            if (!string.IsNullOrEmpty(file))
            {
                this._ViewModel.PostFile = file;
            }
        }

        private void btnSaveBrowser_Click(object sender, RoutedEventArgs e)
        {
            // 选择文件
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.Filter = "表格数据|*.csv";
            if (saveFile.ShowDialog() == true)
            {
                this._ViewModel.ChangeFile = saveFile.FileName;
            }
        }
    }
}
