using System.IO;

namespace Semantic.Controls
{
    public class JsonHelper<T>
    {
        public static T Load(string jsonFile)
        {
            var json = File.ReadAllText(jsonFile);
            return Newtonsoft.Json.JsonConvert.DeserializeObject<T>(json);
        }

        public static void Save(T obj, string jsonFile)
        {
            using (var fs = File.Open(jsonFile, FileMode.Create))
            {
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    var json = Newtonsoft.Json.JsonConvert.SerializeObject(obj);
                    sw.Write(json);
                }
            }
        }
    }
}
