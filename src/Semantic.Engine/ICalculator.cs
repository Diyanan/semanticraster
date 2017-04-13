using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public interface ICalculator
    {
        event EventHandler OnProgressChanged;

        void Process();
    }
}
