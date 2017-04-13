using System;

namespace Semantic.Controls
{
    public struct Color4F
    {
        public float R;
        public float G;
        public float B;
        public float A;

        public Color4F(float a, float r, float g, float b)
        {
            this.A = a;
            this.R = r;
            this.G = g;
            this.B = b;
        }

        public float[] Rgba()
        {
            return new float[4]
            {
                this.R,
                this.G,
                this.B,
                this.A
            };
        }

        public uint ToUint()
        {
            return
                (((uint)(this.A * byte.MaxValue)) << 24) +
                (((uint)(this.B * byte.MaxValue)) << 16) +
                (((uint)(this.G * byte.MaxValue)) << 8) +
                ((uint)(this.R * byte.MaxValue));
        }

        public static Color4F FromUint(uint color)
        {
            return new Color4F(
                (color >> 24 & byte.MaxValue) / (float)byte.MaxValue,
                (color & byte.MaxValue) / (float)byte.MaxValue,
                (color >> 8 & byte.MaxValue) / (float)byte.MaxValue,
                (color >> 16 & byte.MaxValue) / (float)byte.MaxValue);
        }

        private Tuple<double, double, double> ToHSL()
        {
            double min = Math.Min(Math.Min(this.R, this.G), this.B);

            ColorComponents colorComponents = ColorComponents.Red;
            double max = this.R;
            if (this.G > max)
            {
                max = this.G;
                colorComponents = ColorComponents.Green;
            }
            if (this.B > max)
            {
                max = this.B;
                colorComponents = ColorComponents.Blue;
            }
            double l = (max + min) / 2.0;
            double s;
            double h;
            if (max == min)
            {
                h = s = 0.0;
            }
            else
            {
                double num = 0.0;
                double span = max - min;
                s = l > 0.5 ? span / (2.0 - max - min) : span / (max + min);
                switch (colorComponents)
                {
                    case ColorComponents.Red:
                        num = (this.G - this.B) / span + (this.G < this.B ? 6.0 : 0.0);
                        break;
                    case ColorComponents.Blue:
                        num = (this.R - this.G) / span + 4.0;
                        break;
                    case ColorComponents.Green:
                        num = (this.B - this.R) / span + 2.0;
                        break;
                }
                h = num / 6.0;
            }
            return new Tuple<double, double, double>(h, s, l);
        }

        public static Color4F Lerp(Color4F colorA, Color4F colorB, float factor)
        {
            float srcFactor = 1f - factor;
            float dstFactor = factor;
            return new Color4F(
                colorA.A * srcFactor + colorB.A * dstFactor,
                colorA.R * srcFactor + colorB.R * dstFactor,
                colorA.G * srcFactor + colorB.G * dstFactor,
                colorA.B * srcFactor + colorB.B * dstFactor);
        }

        public static Color4F? ApplyLightnessFactor(Color4F color, double lightnessFactor)
        {
            Tuple<double, double, double> tuple = color.ToHSL();
            double l = tuple.Item3 * lightnessFactor;
            if (l > 1.0 || l < 0.0)
                return new Color4F?();
            else
                return new Color4F?(FromHsl(tuple.Item1, tuple.Item2, l));
        }

        private static Color4F FromHsl(double h, double s, double l)
        {
            if (s == 0.0)
                return new Color4F(1f, (float)l, (float)l, (float)l);
            Func<double, double, double, double> func = (pl, ql, tl) =>
            {
                if (tl < 0.0)
                    ++tl;
                if (tl > 1.0)
                    --tl;
                if (tl < 1.0 / 6.0)
                    return pl + (ql - pl) * 6.0 * tl;
                if (tl < 0.5)
                    return ql;
                if (tl < 2.0 / 3.0)
                    return pl + (ql - pl) * (2.0 / 3.0 - tl) * 6.0;
                else
                    return pl;
            };
            double num1 = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
            double num2 = 2.0 * l - num1;
            return new Color4F(1f, (float)func(num2, num1, h + 1.0 / 3.0), (float)func(num2, num1, h), (float)func(num2, num1, h - 1.0 / 3.0));
        }

        private enum ColorComponents
        {
            Red,
            Blue,
            Green,
        }
    }
}
