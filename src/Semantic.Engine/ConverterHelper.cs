using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public class ConverterHelper
    {
        public static string ConvertTimeUnit(long ms)
        {
            string usedTime = string.Empty;
            int hourMs = 60 * 60 * 1000;
            int minuteMs = 60 * 1000;
            int secondMs = 1000;
            if (ms > hourMs)
                usedTime = string.Format("{0} h", ((double)ms / (hourMs)).ToString(".##"));
            else if (ms > minuteMs)
                usedTime = string.Format("{0} m", ((double)ms / minuteMs).ToString(".##"));
            else if (ms > secondMs)
                usedTime = string.Format("{0} s", ((double)ms / secondMs).ToString(".##"));
            else if (ms == 0)
                usedTime = "0 ms";
            else
                usedTime = string.Format("{0} ms", ((double)ms / 1).ToString(".##"));

            return usedTime;
        }

    }
}
