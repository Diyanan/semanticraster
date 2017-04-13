namespace Semantic.Controls
{
    public static class ColorExtensions
    {
        public static Color4F ToColor4F(this System.Drawing.Color c)
        {
            return new Color4F(c.A / (float)byte.MaxValue, c.R / (float)byte.MaxValue, c.G / (float)byte.MaxValue, c.B / (float)byte.MaxValue);
        }

        public static Color4F ToColor4F(this System.Windows.Media.Color c)
        {
            return new Color4F(c.A / (float)byte.MaxValue, c.R / (float)byte.MaxValue, c.G / (float)byte.MaxValue, c.B / (float)byte.MaxValue);
        }


        public static System.Drawing.Color ToSystemColor(this Color4F c)
        {
            return System.Drawing.Color.FromArgb(
                (int)(c.A * byte.MaxValue),
                (int)(c.R * byte.MaxValue),
                (int)(c.G * byte.MaxValue),
                (int)(c.B * byte.MaxValue));
        }

        public static System.Windows.Media.Color ToWindowsColor(this Color4F c)
        {
            return System.Windows.Media.Color.FromArgb(
                (byte)(c.A * byte.MaxValue),
                (byte)(c.R * byte.MaxValue),
                (byte)(c.G * byte.MaxValue),
                (byte)(c.B * byte.MaxValue));
        }

    }
}
