using Newtonsoft.Json;
using System.IO;

namespace Semantic.Configure
{
    /// <summary>
    /// json文件读写帮助类
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class JsonHelper<T>
    {
        /// <summary>
        /// 解析json文件
        /// </summary>
        /// <param name="jsonFile">文件路径</param>
        /// <returns>对象</returns>
        public static T Load(string jsonFile)
        {
            var json = File.ReadAllText(jsonFile);
            return JsonConvert.DeserializeObject<T>(json);
        }

        /// <summary>
        /// 保存json文件
        /// </summary>
        /// <param name="obj">对象</param>
        /// <param name="jsonFile">文件路径</param>
        public static void Save(T obj, string jsonFile)
        {
            using (var fs = File.Open(jsonFile, FileMode.Create))
            {
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    var json = JsonConvert.SerializeObject(obj);
                    sw.Write(json);
                }
            }
        }
    }
}
