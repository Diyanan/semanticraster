using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    [Serializable]
    public class ProjectSettings : IEquatable<ProjectSettings>
    {
        public string Name { get; set; }
        public string Path { get; set; }

        public bool Equals(ProjectSettings other)
        {
            return this.Path == other.Path;
        }
    }
}
