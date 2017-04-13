using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Engine
{
    public static class PartitionExtension
    {
        public static IEnumerable<IEnumerable<T>> Partition<T>(this IEnumerable<T> list, int size)
        {
            while (list.Any()) { yield return list.Take(size); list = list.Skip(size); }
        }

        public static IEnumerable<IEnumerable<T>> Partition<T>(this IEnumerable<T> list, int count, int size)
        {
            var i = 0;
            var batch = count / size;
            if (count % size != 0)
            {
                batch++;
            }
            while (i++ < batch) { yield return list.Take(size); list = list.Skip(size); }
        }
    }
}
