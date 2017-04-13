using Microsoft.Win32;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class TaskPanelLayersTabViewModel : ViewModelBase
    {
        private LayerManagerViewModel _Model;//  图层管理
        private UndoObservableCollection<LayerViewModel> _LayerList;// 图层控制
     
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
                this.SetProperty<LayerManagerViewModel>(this.PropertyModel, ref this._Model, value, false);
            }
        }

        public static string PropertyLayerList
        {
            get
            {
                return "LayerList";
            }
        }

        public UndoObservableCollection<LayerViewModel> LayerList
        {
            get
            {
                return this._LayerList;
            }
            private set
            {
                this.SetProperty<UndoObservableCollection<LayerViewModel>>(
                    TaskPanelLayersTabViewModel.PropertyLayerList, ref this._LayerList, value, false);
            }
        }

        // public ObservableCollectionEx<LayerViewModel> Layers { get; private set; }

        public ICommand AddNewLayerCommand { get; private set; }

        public DragItemsHandler<LayerViewModel> LayersDragHandler { get; private set; }

        public DropItemsHandler LayersDropHandler { get; private set; }


        public TaskPanelLayersTabViewModel(LayerManagerViewModel layerManagerViewModel)
        {
            this.Model = layerManagerViewModel;
            this.LayerList = this.Model.Layers;
            this.AddNewLayerCommand = new DelegatedCommand(new Action(this.OnExecuteAddLayer));

            this.LayersDragHandler = new DragItemsHandler<LayerViewModel>((Collection<LayerViewModel>)this.LayerList, false);
            this.LayersDragHandler.CanDragItemCallback = new CanDragItemDelegate<LayerViewModel>(this.OnDragLayer);
            this.LayersDropHandler = new DropItemsHandler();
            this.LayersDropHandler.AddDroppableTypeHandlers<LayerViewModel>(new DropItemIntoCollectionDelegate<LayerViewModel>(this.OnDropLayer), (item => null));
        }

        private void OnDropLayer(LayerViewModel item, int index)
        {
            this.LayerList.Insert(index, item);
            this.LayerList.CollapseRemoveAndAdd = false;
            // this.SelectedItem = item;
        }

        private bool OnDragLayer(LayerViewModel item)
        {
            this.LayerList.CollapseRemoveAndAdd = true;
            return true;
        }

        private void OnExecuteAddLayer()
        {
            // 选择文件
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Filter = "空间数据|*.tif;*.img;*.shp|栅格文件|*.tif;*.img;|矢量文件|*.shp|所有文件|*.*";
            if (openFile.ShowDialog() == true)
            {
                string path = openFile.FileName;
                this.Model.AddLayerDefinition(path); // 解析mapcontrol获取图层的路径信息
            }
        }

        public void OnInitLayerManageTool()
        {
            this.LayerList.Clear();
            List<string> paths = new List<string>();
            paths = this.Model.LayerManager.GetAllLayerPath();
            for (int i = paths.Count-1; i >=0; i--)
            {
                this.Model.AddLayerDefinition(paths[i]); // 解析mapcontrol获取图层的路径信息
            }
        }
    }
}
