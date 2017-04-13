using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class TaskSettingsViewModel : ViewModelBase
    {
        public LayerManagerViewModel LayerManagerVM { get; private set; }

        private TaskViewModel _ParentTask = null;

        private ViewModelBase _Setting = null;

        public CharacterParamSettingViewModel CharacterSetting { get; private set; }
        public ClassificationSettingsViewModel ClassifySetting { get; private set; }
        public SegmentSettingsViewModel SegmentSetting { get; private set; }
        public ChangeSettingsViewModel ChangeSetting { get; private set; }

        public static string PropertySetting
        {
            get
            {
                return "Setting";
            }
        }

        public ViewModelBase Setting
        {
            get
            {
                return _Setting;
            }

            set
            {
                this.SetProperty<ViewModelBase>(PropertySetting, ref _Setting, value, false);
            }
        }

        public static string PropertyParentTask
        {
            get { return "ParentTask"; }
        }

        public TaskViewModel ParentTask
        {
            get
            {
                return _ParentTask;
            }

            set
            {
                this.SetProperty<TaskViewModel>(PropertyParentTask, ref _ParentTask, value, false);
            }
        }

        public TaskSettingsViewModel(LayerManagerViewModel layerManagerVW)
        {
            this.LayerManagerVM = layerManagerVW;
            string configureFile = AppDomain.CurrentDomain.BaseDirectory + "conf\\FeatureSet.json";
            this.CharacterSetting = new CharacterParamSettingViewModel(layerManagerVW, configureFile);
            this.ClassifySetting = new ClassificationSettingsViewModel(layerManagerVW);
            this.SegmentSetting = new SegmentSettingsViewModel(layerManagerVW);
            this.ChangeSetting = new ChangeSettingsViewModel();
        }


        public void SetParentTask(TaskViewModel taskVW)
        {
            this.ParentTask = taskVW;
            var taskType = taskVW.Model.TaskType;
            switch (taskType)
            {
                case TaskType.Feature:
                    // var task = taskVW.Model as FeatureTask;
                    this.CharacterSetting.SetTask(taskVW.Model);
                    this.Setting = this.CharacterSetting;
                    break;
                case TaskType.Classify:
                    this.ClassifySetting.SetTask(taskVW.Model);
                    this.Setting = this.ClassifySetting;
                    break;
                case TaskType.Segment:
                    this.SegmentSetting.SetTask(taskVW.Model);
                    this.Setting = this.SegmentSetting;
                    break;
                case TaskType.ChangeDetection:
                    this.ChangeSetting.SetTask(taskVW.Model);
                    this.Setting = this.ChangeSetting;
                    break;
            }
        }
    }
}
