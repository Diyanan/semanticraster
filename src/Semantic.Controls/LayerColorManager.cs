using Semantic.Engine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    /// <summary>
    /// 图层颜色管理器(包括预设颜色和自定义颜色)
    /// 其中预设颜色与主题相关，是根据输入的颜色样本和步长而生成的颜色序列（3072）；
    /// 自定义颜色为用户添加的颜色（1024）。
    /// </summary>
    public class LayerColorManager
    {
        private Color4F[] customColors = new Color4F[1024];
        private const int MaxVisualColors = 3072;
        private Color4F[] visualColors;

        public Color4F GetColor(int index)
        {
            return this.visualColors[index % this.visualColors.Length];
        }

        public void SetVisualColors(IList<Color4F> colors, IList<double> lightnessSteps)
        {
            Color4F[] result = new Color4F[Math.Min(colors.Count * lightnessSteps.Count, 3072)];
            int index = 0;
            for (int i = 0; i < lightnessSteps.Count; ++i)
            {
                for (int j = 0; j < colors.Count; ++j)
                {
                    if (index < result.Length)
                        result[index++] = Color4F.ApplyLightnessFactor(colors[j], lightnessSteps[i]) ?? new Color4F(1f, 1f, 1f, 1f);
                }
            }
            this.visualColors = result;
        }

        public int AddColor(Color4F color)
        {
            for (int i = 0; i < this.customColors.Length; ++i)
            {
                if ((int)this.customColors[i].ToUint() == 0)
                {
                    this.customColors[i] = color;
                    return -(i + 3072);
                }
            }
            return 0;
        }

        public bool RemoveColor(int colorIndex)
        {
            if (colorIndex > -3072 || colorIndex <= -4096)
                return false;
            this.customColors[-colorIndex - 3072] = new Color4F();
            return true;
        }
    }
}
