using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class RemoveItemPlaceholder
    {
        private string resourceString;

        public RemoveItemPlaceholder(string resource)
        {
            this.resourceString = resource;
        }

        public override string ToString()
        {
            return this.resourceString;
        }
    }
}
