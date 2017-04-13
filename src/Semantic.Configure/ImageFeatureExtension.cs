using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Configure
{
    public static class ImageFeatureExtension
    {
        public static IEnumerable<string> GetFieldNames(this ImageFeature imageFeature, int bandCount)
        {
            if (imageFeature.FieldDef.Unified)
            {
                yield return imageFeature.FieldDef.ShortName;
            }
            else
            {
                for (int bandIndex = 1; bandIndex <= bandCount; bandIndex++)
                {
                    yield return string.Format("{0}_{1}", imageFeature.FieldDef.ShortName, bandIndex.ToString());
                }
            }
        }
    }

}
