using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class LayerManagerViewModel : PropertyChangeNotificationBase, INotifyPropertyChanges, INotifyPropertyChanged, INotifyPropertyChanging
    {
        private Action openSettingsAction;
        private Action changeCurrentSettingsAction;
        private IDialogServiceProvider _dialogProvider;
        private UndoItemViewModel _UndoItems;
        public UndoObservableCollection<LayerViewModel> Layers { get; private set; }

        public ILayerTools LayerTools { get; private set; }
        public LayerManager Model { get; private set; }
        public ILayerManager LayerManager { get; private set; }
        public IVisualization VisOperation { get; private set; }
        public IJoinManager JoinTalbeManager { get; private set; }

        public static string PropertyUndoItems
        {
            get
            {
                return "UndoItems";
            }
        }

        public UndoItemViewModel UndoItems
        {
            get
            {
                return this._UndoItems;
            }
            set
            {
                this.SetProperty<UndoItemViewModel>(PropertyUndoItems, ref this._UndoItems, value);
            }
        }

        public LayerSettingsViewModel Settings { get; private set; }

        public Action OpenSettingsAction
        {
            set
            {
                this.openSettingsAction = value;
            }
        }

        public Action ChangeCurrentSettingsAction
        {
            set
            {
                this.changeCurrentSettingsAction = value;
            }
        }

        //public StatusBarViewModel StatusBar
        //{
        //    get
        //    {
        //        if (this._hostWindowVM != null)
        //            return this._hostWindowVM.StatusBar;
        //        else
        //            return null;
        //    }
        //}

        public LayerManagerViewModel(ILayerManager layerManager, ILayerTools tools, IVisualization visOperation, IJoinManager joinTableManager, IDialogServiceProvider dialogProvider = null)
        {
            this.LayerTools = tools;
            this.LayerManager = layerManager;
            this.VisOperation = visOperation;
            this.JoinTalbeManager = joinTableManager;
            this._dialogProvider = dialogProvider;

            // 图层定义与存储
            this.Model = new LayerManager();
            this.Model.LayerDefinitions.ItemAdded += new ObservableCollectionExChangedHandler<LayerDefinition>(this.AddLayerDefinition);
            this.Model.LayerDefinitions.ItemRemoved += new ObservableCollectionExChangedHandler<LayerDefinition>(this.RemoveLayerDefinition);

            // 图层列表
            this.UndoItems = new UndoItemViewModel();
            this.Layers = new UndoObservableCollection<LayerViewModel>(
                this._UndoItems.UndoManager,
                new Func<LayerViewModel, ChangeType, string>(this.DescriptionGenerator));
            this.Layers.CollectionChanged += new NotifyCollectionChangedEventHandler(this.LayerList_CollectionChanged);
            this.Layers.ItemPropertyChanged += Layers_ItemPropertyChanged;

            // setting panel
            this.Settings = new LayerSettingsViewModel(this, null);
        }

        private void LayerList_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            // move layer
            if (e.Action == NotifyCollectionChangedAction.Add && this.Layers.CollapseRemoveAndAdd)
            {
                var item = (LayerViewModel)e.NewItems[0];
                var index = e.NewStartingIndex;

                this.LayerManager.MoveLayer(item.LayerDefinition, index);
                //this.SetSelectedItemValue(e.OldStartingIndex == 0 ? 0 : e.OldStartingIndex - 1, true, false);
            }
        }

        private string DescriptionGenerator(LayerViewModel sceneViewModel, ChangeType changeType)
        {
            switch (changeType)
            {
                case ChangeType.Add:
                    return Resources.Undo_NewLayer;
                case ChangeType.Remove:
                    return Resources.Undo_DeleteLayer;
                case ChangeType.Move:
                    return Resources.Undo_MoveLayer;
                default:
                    return string.Empty;
            }
        }

        public void OnNewView()
        {
            this.changeCurrentSettingsAction();
        }

        public void AddLayerDefinition(string path)
        {
            if (this.Model != null)
            {
                this.Model.AddLayerDefinition(path);
            }

            if (this.changeCurrentSettingsAction != null)
            {
                this.changeCurrentSettingsAction();
            }
        }

        private void Layers_ItemPropertyChanged(LayerViewModel item, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == item.PropertyVisible)
            {
                this.LayerManager.SetLayerVisible(item.LayerDefinition, item.Visible);
            }
        }

        
        private void AddLayerDefinition(LayerDefinition layerDef)
        {
            this.Layers.Insert(0, new LayerViewModel(layerDef, this._dialogProvider)
            {
                GeoVisualization = new GeoVisualization(layerDef, this.VisOperation),
                DeleteLayerCommand = new DelegatedCommand<LayerViewModel>(new Action<LayerViewModel>(this.OnExecuteDeleteLayer)),
                LayerSettingsCommand = new DelegatedCommand<LayerViewModel>(new Action<LayerViewModel>(this.OnLayerSettings))
            });

         //this.LayerManager.AddLayer(layerDef);
        }

        private void RemoveLayerDefinition(LayerDefinition item)
        {
            LayerViewModel result = null;
            foreach (LayerViewModel layer in this.Layers)
            {
                if (layer.LayerDefinition == item)
                {
                    result = layer;
                    break;
                }
            }
            if (result != null)
            {
                this.Layers.Remove(result);
                this.LayerManager.RemoveLayer(result.LayerDefinition);
            }
        }

        public void RelateTable(LayerDefinition layerDefn, string csvFile)
        {
            this.LayerManager.AddJoinTable(layerDefn, csvFile);
        }

        private void OnExecuteDeleteLayer(LayerViewModel sender)
        {
            if (this.Model != null && sender != null)
            {
                this.Model.RemoveLayerDefinition(sender.LayerDefinition);
            }
        }

        private void OnLayerSettings(LayerViewModel layer)
        {
            this.Settings.SetParentLayer(layer);
            this.openSettingsAction();
        }

        public void SetLayerManager(LayerManager layerManager)
        {
            // 初始化
            foreach (LayerDefinition layerDef in layerManager.LayerDefinitions)
            {
                if (this.Model != null)
                {
                    this.Model.AddLayerDefinition(layerDef);
                }
            }
        }
    }
}
