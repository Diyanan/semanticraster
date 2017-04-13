using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace Semantic.Configure
{
    public class VerifyFeatureSetProperties
    {
        public string Name;
        public string Version;
        public string ProductDate;
    }

    public class FieldDef
    {
        public string ShortName { get; set; }
        public bool Unified { get; set; }
    }

    public class ImageFeature : IEquatable<ImageFeature>
    {
        public string Name { get; set; }
        public string AlgorithmName;
        public List<string> NeedBandNames { get; set; }
        public FieldDef FieldDef { get; set; }
        public string Comments { get; set; }

        public bool Equals(ImageFeature other)
        {
            return this.AlgorithmName == other.AlgorithmName;
        }
    }

    public class FeautreSet
    {
        public string Name { get; set; }
        public List<ImageFeature> Features { get; set; }
    }

    public class VerifyFeatureSet
    {
        public VerifyFeatureSetProperties Properties { get; set; }
        public List<FeautreSet> FeatureSet { get; set; }

        public IEnumerable<ImageFeature> GetFeatures()
        {
            return this.FeatureSet.SelectMany(s => s.Features);
        }
    }
}
