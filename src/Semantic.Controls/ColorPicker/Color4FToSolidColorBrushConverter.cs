using Semantic.Engine;
using Semantic.WpfExtensions;
using System.Windows.Media;

namespace Semantic.Controls
{
    public class Color4FToSolidColorBrushConverter : SimpleOneWayConverter<Color4F, SolidColorBrush>
    {
        protected override bool TryConvert(Color4F source, out SolidColorBrush result)
        {
            result = new SolidColorBrush(source.ToWindowsColor());
            return true;
        }
    }
}
