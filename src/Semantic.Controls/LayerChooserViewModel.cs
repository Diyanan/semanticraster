using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class LayerChooserViewModel : ViewModelBase
    {
        private LayerManagerViewModel _Model;
        private LayerViewModel _SelectedLayer;

        public ObservableCollectionEx<LayerViewModel> Layers { get; private set; }

        public LayerType LayerChooserType { get; set; }

        public static string PropertySelectedLayer
        {
            get
            {
                return "SelectedLayer";
            }
        }

        public LayerViewModel SelectedLayer
        {
            get
            {
                return this._SelectedLayer;
            }
            set
            {
                base.SetProperty(PropertySelectedLayer, ref this._SelectedLayer, value);
            }
        }

        public string PropertyModel
        {
            get
            {
                return "Model";
            }
        }

        public LayerManagerViewModel Model
        {
            get
            {
                return this._Model;
            }
            set
            {
                base.SetProperty(this.PropertyModel, ref this._Model, value);
            }
        }


        public LayerChooserViewModel(LayerManagerViewModel visModel, LayerType chooserType)
        {
            this.Model = visModel;
            this.LayerChooserType = chooserType;
            this.Layers = new ObservableCollectionEx<LayerViewModel>();
            foreach (var layerVW in this.Model.Layers)
            {
                this.BranchLayer(layerVW);
            }

            this.Model.Layers.ItemAdded += new ObservableCollectionExChangedHandler<LayerViewModel>(this.BranchLayer);
            this.Model.Layers.ItemRemoved += new ObservableCollectionExChangedHandler<LayerViewModel>(this.RemoveLayer);
        }

        private void BranchLayer(LayerViewModel layerVW)
        {
            var layerDef = layerVW.LayerDefinition;
            if (this.LayerChooserType == LayerType.None ||
                layerDef.LayerType == this.LayerChooserType)
            {
                this.Layers.Add(layerVW);
            }
        }

        private void RemoveLayer(LayerViewModel layerVW)
        {
            LayerViewModel result = null;
            foreach (LayerViewModel layer in this.Layers)
            {
                if (layer.LayerDefinition == layerVW.LayerDefinition)
                {
                    result = layer;
                    break;
                }
            }
            if (result != null)
            {
                this.Layers.Remove(result);
            }
        }
    }
}
