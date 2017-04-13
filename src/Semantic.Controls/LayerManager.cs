using Semantic.Engine;
using Semantic.WpfCommon;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class LayerManager
    {
        private ObservableCollectionEx<LayerDefinition> layerDefinitions;

        public ObservableCollectionEx<LayerDefinition> LayerDefinitions  // 图层存储数组
        {
            get
            {
                return this.layerDefinitions;
            }
        }

        public LayerManager()
        {
            this.layerDefinitions = new ObservableCollectionEx<LayerDefinition>();
        }

        /// <summary>
        /// 新建图层，并添加到图层管理器中
        /// </summary>
        /// <param name="name"></param>
        /// <param name="modelTableName"></param>
        /// <param name="createGeoVisualization"></param>
        /// <param name="forInstructionsOnly"></param>
        /// <returns></returns>
        public LayerDefinition AddLayerDefinition(string path)
        {
            LayerDefinition layerDefinition = new LayerDefinition(path);

            this.layerDefinitions.Insert(0, layerDefinition);

            return layerDefinition;
        }

        /// <summary>
        /// 添加图层
        /// </summary>
        /// <param name="layerDefinition"></param>
        public void AddLayerDefinition(LayerDefinition layerDefinition)
        {
            this.layerDefinitions.Add(layerDefinition);
        }

        public bool RemoveLayerDefinition(LayerDefinition layerDefinition)
        {
            if (layerDefinition == null)
                return true;
            return this.layerDefinitions.Remove(layerDefinition);
        }

    }
}
