using Semantic.WpfCommon;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class AddFilterSelectionDialogViewModel : DialogViewModelBase
    {
        private readonly WeakEventListener<AddFilterSelectionDialogViewModel, object, PropertyChangedEventArgs> onFieldSelectionChanged;
        private ICommand _CreateCommand;
        private bool canAddFilter;

        public static string PropertyCreateCommand
        {
            get
            {
                return "CreateCommand";
            }
        }

        public ICommand CreateCommand
        {
            get
            {
                return this._CreateCommand;
            }
            set
            {
                this.SetProperty<ICommand>(AddFilterSelectionDialogViewModel.PropertyCreateCommand, ref this._CreateCommand, value, false);
            }
        }

        public ObservableCollectionEx<TableIslandViewModel> FilterCandidates { get; private set; }

        public string PropertyCanAddFilterUI
        {
            get
            {
                return "CanAddFilter";
            }
        }

        public bool CanAddFilter
        {
            get
            {
                return this.canAddFilter;
            }
            set
            {
                this.SetProperty<bool>(this.PropertyCanAddFilterUI, ref this.canAddFilter, value, false);
            }
        }

        public ModelWrapper Wrapper { get; set; }

        public AddFilterSelectionDialogViewModel(ILayerManager layerManager)
        {
            this.Wrapper = new ModelWrapper(layerManager);
            this.FilterCandidates = new ObservableCollectionEx<TableIslandViewModel>();
            this.onFieldSelectionChanged = new WeakEventListener<AddFilterSelectionDialogViewModel, object, PropertyChangedEventArgs>(this);
            this.onFieldSelectionChanged.OnEventAction = new Action<AddFilterSelectionDialogViewModel, object, PropertyChangedEventArgs>(AddFilterSelectionDialogViewModel.Fields_ItemPropertyChanged);
        }

        private void InitializeFilterCandidates(LayerViewModel layerVm)
        {
            this.FilterCandidates.Clear();
            var table = this.Wrapper.GetTable(layerVm.LayerDefinition);
            TableIsland island = new TableIsland();
            foreach (var lookupTable in table.LookupTables)
            {
                island.AddTable(lookupTable);
            }
            this.FilterCandidates.Add(new TableIslandViewModel(island, false));
        }

        public void Initialize(LayerViewModel layerVm)
        {
            this.CanAddFilter = false;
            InitializeFilterCandidates(layerVm);
            if (this.FilterCandidates == null)
                return;
            foreach (TableIslandViewModel island in this.FilterCandidates)
            {
                foreach (TableViewModel table in island.Tables)
                {
                    foreach (TableFieldViewModel field in table.Fields)
                    {
                        field.PropertyChanged -= new PropertyChangedEventHandler(this.onFieldSelectionChanged.OnEvent);
                        field.IsSelected = false;
                        field.PropertyChanged += new PropertyChangedEventHandler(this.onFieldSelectionChanged.OnEvent);
                    }
                }
            }
        }

        private static void Fields_ItemPropertyChanged(AddFilterSelectionDialogViewModel dialog, object item, PropertyChangedEventArgs e)
        {
            if (!e.PropertyName.Equals("IsSelected"))
                return;
            TableFieldViewModel tableFieldViewModel1 = item as TableFieldViewModel;
            if (tableFieldViewModel1 != null && tableFieldViewModel1.IsSelected)
            {
                dialog.CanAddFilter = true;
            }
            else
            {
                bool flag = false;
                foreach (TableIslandViewModel island in dialog.FilterCandidates)
                {
                    foreach (TableViewModel table in island.Tables)
                    {
                        foreach (TableFieldViewModel field in table.Fields)
                        {
                            if (field.IsSelected)
                            {
                                flag = true;
                                break;
                            }
                        }
                    }
                }
                dialog.CanAddFilter = flag;
            }
        }
    }
}
