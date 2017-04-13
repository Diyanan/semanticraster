using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Ribbon;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Semantic.Controls
{
    /// <summary>
    /// ColorPickerView.xaml 的交互逻辑
    /// </summary>
    public partial class ColorPickerView : System.Windows.Controls.UserControl
    {
        public ColorPickerView()
        {
            InitializeComponent();
        }

        private void RibbonButton_Click(object sender, RoutedEventArgs e)
        {
            ColorPickerViewModel colorPickerViewModel = DataContext as ColorPickerViewModel;
            if (colorPickerViewModel == null)
                return;
            ColorDialog colorDialog = new ColorDialog();
            colorDialog.FullOpen = true;
            colorDialog.CustomColors = colorPickerViewModel.CustomColors.Select(new Func<Color4F, int>(ColorPickerView.Color4FToWindowsDialogCustomColor)).Take(16).ToArray();
            colorDialog.Color = colorPickerViewModel.SelectedColor.ToSystemColor();
            if (colorDialog.ShowDialog() == DialogResult.OK)
            {
                System.Drawing.Color color = colorDialog.Color;
                colorPickerViewModel.SelectedColor = new Color4F(1f, color.R / (float)byte.MaxValue, color.G / (float)byte.MaxValue, color.B / (float)byte.MaxValue);
                colorPickerViewModel.CustomColors.Clear();
                colorPickerViewModel.CustomColors.AddRange(colorDialog.CustomColors.Select(new Func<int, Color4F>(WindowsDialogCustomColorToColor4F)));
            }
        }

        private static int Color4FToWindowsDialogCustomColor(Color4F color)
        {
            return ((int)(color.B * (double)byte.MaxValue) << 16) + ((int)(color.G * (double)byte.MaxValue) << 8) + (int)(color.R * (double)byte.MaxValue);
        }

        private static Color4F WindowsDialogCustomColorToColor4F(int customColorIndex)
        {
            return new Color4F(1f, (customColorIndex & byte.MaxValue) / (float)byte.MaxValue, (customColorIndex >> 8 & byte.MaxValue) / (float)byte.MaxValue, (customColorIndex >> 16 & byte.MaxValue) / (float)byte.MaxValue);
        }

        private void ColorPickerButton_Initialized(object sender, RoutedEventArgs e)
        {
            DependencyObject focusScope = FocusManager.GetFocusScope(this);
            IInputElement focusedElement = FocusManager.GetFocusedElement(focusScope);
            FocusManager.SetFocusedElement(focusScope, ColorPickerButton);
            MoveFocus(new TraversalRequest(FocusNavigationDirection.Next));
            FocusManager.SetFocusedElement(focusScope, focusedElement);
        }

        private void ColorPickerDropdownGallery_SelectionChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            ColorPickerButton.InvalidateProperty(RibbonMenuButton.SmallImageSourceProperty);
        }

        private void ColorPickerDropdownGallery_Loaded(object sender, RoutedEventArgs e)
        {
            ColorPickerViewModel viewModel = DataContext as ColorPickerViewModel;
            if (viewModel != null)
            {
                ColorPickerDropdownGallery.SelectedItem = ColorPickerViewModel.Colors.FirstOrDefault(n => (int)n.Item1.ToUint() == (int)viewModel.SelectedColor.ToUint());
            }
        }
    }
}
