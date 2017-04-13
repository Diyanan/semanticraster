using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class StatusBarViewModel : ViewModelBase
    {
        private string _status = Resources.StatusBar_Ready;
        private const int FailureThreshold = 4;
        private LayerManagerViewModel _layerManagerVM;
        private LayerViewModel _selectedLayer;
        private CompletionStatsViewModel _CompletionStats;
        private SelectionStats _SelectionStats;
        private HostControlViewModel hostControlViewModel;

        public string PropertyCompletionStats
        {
            get
            {
                return "CompletionStats";
            }
        }

        public CompletionStatsViewModel CompletionStats
        {
            get
            {
                return this._CompletionStats;
            }
            set
            {
                this.SetProperty<CompletionStatsViewModel>(this.PropertyCompletionStats, ref this._CompletionStats, value, false);
            }
        }

        public string PropertySelectionStats
        {
            get
            {
                return "SelectionStats";
            }
        }

        public SelectionStats SelectionStats
        {
            get
            {
                return this._SelectionStats;
            }
            set
            {
                if (this._SelectionStats != null)
                    this._SelectionStats.SelectionUpdate -= new Action(this.OnSelectionStatsUpdated);
                this.SetProperty<SelectionStats>(this.PropertySelectionStats, ref this._SelectionStats, value, false);
                if (this._SelectionStats == null)
                    return;
                this._SelectionStats.SelectionUpdate += new Action(this.OnSelectionStatsUpdated);
            }
        }

        public static string PropertyStatus
        {
            get
            {
                return "Status";
            }
        }

        public string Status
        {
            get
            {
                return this._status;
            }
            set
            {
                this.SetProperty<string>(StatusBarViewModel.PropertyStatus, ref this._status, value, false);
            }
        }

        public StatusBarViewModel(LayerManagerViewModel layerManagerVM, HostControlViewModel model)
        {
            if (model == null)
                return;
            this.hostControlViewModel = model;
            this._selectedLayer = null;
            this._layerManagerVM = layerManagerVM;
            layerManagerVM.PropertyChanged += ((sender, args) =>
            {
                //if (!(args.PropertyName == layerManagerVM.PropertySelectedLayer))
                //    return;
                // this.UpdateSelectedLayer();
            });
            // this.hostControlViewModel.Map.PropertyChanged += new PropertyChangedEventHandler(this.GlobeOnPropertyChanged);
            this.UpdateSelectedLayer();
        }

        private void GlobeOnPropertyChanged(object sender, PropertyChangedEventArgs propertyChangedEventArgs)
        {
            if (this.hostControlViewModel == null ||
                this.hostControlViewModel.Map == null)
                return;
        }

        private void OnSelectionStatsUpdated()
        {
        }

        private void UpdateSelectedLayer()
        {
        }

        private void UpdateCompletionStatsViewModel(CompletionStats stats)
        {
            this.CompletionStats = new CompletionStatsViewModel(stats);
        }

        public void TaskProcessStarted(CompletionStats stats)
        {
            this.UpdateCompletionStatsViewModel(stats);
        }
    }
}
