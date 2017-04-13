using Semantic.WpfExtensions;
using System;
using System.Windows;

namespace Semantic.Controls
{
    public partial class ChromelessWindowBase : Window
    {
        private WindowViewModel _viewModel;
        [ThreadStatic]
        private static ThemeResourceDictionary _mainThemeDictionary;
        [ThreadStatic]
        private static ThemeResourceDictionary _staticallyThemedTemplatesDictionary;

        public static ThemeResourceDictionary StaticallyThemedTemplatesDictionary
        {
            get
            {
                if (_staticallyThemedTemplatesDictionary == null)
                    InitThemedDictionaries();
                return _staticallyThemedTemplatesDictionary;
            }
        }

        public static ThemeResourceDictionary MainThemeDictionary
        {
            get
            {
                if (_mainThemeDictionary == null)
                    InitThemedDictionaries();
                return _mainThemeDictionary;
            }
        }

        public FrameworkElement ChromeControls
        {
            get
            {
                return this.ChromeBar;
            }
        }

        public ChromelessWindowBase()
        {
            this.InitializeComponent();
            this.DataContextChanged += new DependencyPropertyChangedEventHandler(this.OnDataContextChanged);
        }

        private void OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (this._viewModel != null)
            {
                if (this._viewModel.Window == this)
                    this._viewModel.Window = null;
            }
            this._viewModel = e.NewValue as WindowViewModel;
            if (this._viewModel == null)
                return;
            this._viewModel.Window = this;
            this.Width = this._viewModel.Width;
            this.Height = this._viewModel.Height;
        }

        private void OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (this.WindowState == WindowState.Maximized)
            {
                if (this.ResizeMode != ResizeMode.CanResize)
                    return;
                this.MaximizeBorder.Margin = new Thickness(6.0);
            }
            else
            {
                this.MaximizeBorder.Margin = new Thickness(0.0);
            }
        }

        private static void InitThemedDictionaries()
        {
            _mainThemeDictionary = new ThemeResourceDictionary()
            {
                StandardThemeSource = new Uri("/Semantic.Controls;component/Themes/Standard.xaml", UriKind.Relative),
                HighContrastThemeSource = new Uri("/Semantic.Controls;component/Themes/HighContrast.xaml", UriKind.Relative)
            };
            _staticallyThemedTemplatesDictionary = new ThemeResourceDictionary()
            {
                StandardThemeSource = new Uri("/Semantic.Controls;component/Themes/StaticallyThemedTemplates.xaml", UriKind.Relative),
                HighContrastThemeSource = new Uri("/Semantic.Controls;component/Themes/StaticallyThemedTemplates.xaml", UriKind.Relative)
            };
        }

    }
}