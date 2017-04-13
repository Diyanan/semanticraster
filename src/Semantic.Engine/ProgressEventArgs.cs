using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    /// <summary>
    /// 进度报告时间参数
    /// </summary>
    public class ProgressEventArgs : EventArgs
    {
        public CancellationToken CancellationToken { get; private set; }

        public int Requested { get; private set; }

        public int Completed { get; private set; }

        public ProgressEventArgs(int requested, int completed, CancellationToken token)
        {
            this.Requested = requested;
            this.Completed = completed;
            this.CancellationToken = token;
        }
    }
}
