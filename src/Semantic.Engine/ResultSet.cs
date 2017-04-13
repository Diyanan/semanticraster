using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.Concurrent;
using VerifyFeatureSetWrapper;

namespace Semantic.Engine
{
    public class ResultSet
    {
        private ConcurrentDictionary<int, Dictionary<string, double[]>> Features =
                   new ConcurrentDictionary<int, Dictionary<string, double[]>>();

        public IEnumerable<KeyValuePair<int, Dictionary<string, double[]>>> GetAllResult()
        {
            return Features.AsEnumerable();
        }

        public Dictionary<string, double[]> RetrieveItem(int searchKey)
        {
            Dictionary<string, double[]> retrievedValue = null;
            if (Features.TryGetValue(searchKey, out retrievedValue))
            {
                return retrievedValue;
            }
            return null;
        }

        public void UpdateOrAdd(VerifyResultWrapper result)
        {
            int searchKey = result.fId;
            Dictionary<string, double[]> retrievedValue = null;
            if (Features.TryGetValue(searchKey, out retrievedValue))
            {
                retrievedValue.Add(result.featureName, result.results);
            }
            else
            {
                Dictionary<string, double[]> newValue = new Dictionary<string, double[]> 
                {
                    {result.featureName, result.results}
                };
                if (!Features.TryAdd(searchKey, newValue))
                {
                    Console.WriteLine("error at add result!");
                }
            }
        }

        public void DeleteItem(int fid)
        {
            Dictionary<string, double[]> retrievedValue = new Dictionary<string, double[]>();
            if (!Features.TryRemove(fid, out retrievedValue))
            {
                Console.WriteLine("error at delete result!");
            }
        }

        public void Reset()
        {
            this.Features = null;
            this.Features = new ConcurrentDictionary<int, Dictionary<string, double[]>>();
        }
    }
}
