using Semantic.Engine;
using Semantic.WpfExtensions;
using System.Windows.Media;

namespace Semantic.Controls
{
    public class ColorPickerButtonImageConverter : SimpleOneWayConverter<Color4F, ImageSource>
    {
        protected override bool TryConvert(Color4F source, out ImageSource result)
        {
            result = new DrawingImage()
            {
                Drawing = new GeometryDrawing()
                {
                    Brush = new SolidColorBrush(source.ToWindowsColor()),
                    Geometry = new RectangleGeometry()
                    {
                        Rect = new System.Windows.Rect(0.0, 0.0, 16.0, 16.0)
                    }
                }
            };
            return true;
        }
    }
}
