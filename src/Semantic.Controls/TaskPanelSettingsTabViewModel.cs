using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public enum TaskPanelSettingsSubhead
    {
        LayerSettings,
        TaskSettings
    }

    public class TaskPanelSettingsTabViewModel : ViewModelBase
    {
        private int _SelectedSubheadIndex;

        public LayerSettingsViewModel LayerSettings { get; private set; }

        public TaskSettingsViewModel TaskSettings { get; private set; }

        public static string PropertySelectedSubheadIndex
        {
            get
            {
                return "SelectedSubheadIndex";
            }
        }

        public int SelectedSubheadIndex
        {
            get
            {
                return this._SelectedSubheadIndex;
            }
            set
            {
                if (!this.SetProperty<int>(TaskPanelSettingsTabViewModel.PropertySelectedSubheadIndex, ref this._SelectedSubheadIndex, value, false))
                    return;
                this.RaisePropertyChanged(TaskPanelSettingsTabViewModel.PropertySelectedSubheadIndex);
            }
        }

        public TaskPanelSettingsTabViewModel(LayerSettingsViewModel layerSettings, TaskSettingsViewModel taskSettings)
        {
            this.LayerSettings = layerSettings;
            this.TaskSettings = taskSettings;
        }
    }
}
