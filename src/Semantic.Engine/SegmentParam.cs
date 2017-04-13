using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    [Serializable]
    public class SegmentParam
    {
        //多尺度分割参数
        public int ParaMultiSegScaleNum { get; set; }
        public double ParaMultiSegHdiff { get; set; }
        //Meanshift分割
        public float ParaMSSigmaR { get; set; }
        public int ParaMSSigmaS { get; set; }
        public int ParaMSminsize { set; get; }
        //图割算法
        public float ParaGraphCutSigma { set; get; }
        public float ParaGraphCutThreshold { get; set; }
        public int ParaGraphCutMinsize { get; set; }
        //JSEG分割算法
        public int ParaJsegTquan { get; set; }
        public float ParaJsegThreshold { get; set; }
        public int ParaJsegMinsize { get; set; }
        //quadTree分割算法
        public float ParaQTwc { get; set; }
        public float ParaQTwp { get; set; }
        public float ParaQThdiff { get; set; }
        public int ParaQTMinsize { get; set; }
        //superpixel分割算法
        public int ParaSPcount { get; set; }
        public int ParaSPcompactness { get; set; }
        //watershed分割算法
        public int ParaWSminsize { get; set; }
        //二次分割

        // 分割影像
        public LayerDefinition RasterLayer { get; set; }
        public int Algorithm { get; set; }
        public string RasterFileName { get; set; }

        public SegmentParam()
        {
            ParaGraphCutMinsize = 200;
            ParaGraphCutSigma = 0.8F;
            ParaGraphCutThreshold = 20F;
            ParaJsegMinsize = 200;
            ParaJsegThreshold = 30;
            ParaJsegTquan = 200;
            ParaMultiSegHdiff = 30;
            ParaMultiSegScaleNum = 3;
            ParaMSminsize = 200;
            ParaMSSigmaR = 6.5F;
            ParaMSSigmaS = 7;
            ParaQThdiff = 20;
            ParaQTMinsize = 200;
            ParaQTwc = 0.8F;
            ParaQTwp = 0.5F;
            ParaWSminsize = 200;
            ParaSPcompactness = 15;
            ParaSPcount = 500;
        }

    }
}
