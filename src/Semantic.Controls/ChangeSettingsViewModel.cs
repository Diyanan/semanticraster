using Semantic.Engine;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Semantic.Controls
{
    public class ChangeSettingsViewModel : ViewModelBase
    {
        private ChangeParam _Model { get; set; }
        private string _PreFile;
        private string _PostFile;
        private string _ChangeFile;

        public string PropertyPreFile
        {
            get {
                return "PreFile";
            }
        }

        public string PreFile
        {
            get
            {
                return _PreFile;
            }
            set
            {
                SetProperty(PropertyPreFile, ref _PreFile, value, () =>
                {
                    this._Model.PreFeatureFile = value;
                });
            }
        }


        public string PropertyPostFile
        {
            get
            {
                return "PostFile";
            }
        }

        public string PostFile
        {
            get
            {
                return _PostFile;
            }
            set
            {
                SetProperty(PropertyPostFile, ref _PostFile, value, () =>
                {
                    this._Model.PostFeatureFile = value;
                });
            }
        }


        public string PropertyChangeFile
        {
            get
            {
                return "ChangeFile";
            }
        }

        public string ChangeFile
        {
            get
            {
                return _ChangeFile;
            }
            set
            {
                SetProperty(PropertyChangeFile, ref _ChangeFile, value, () =>
                {
                    this._Model.ChangeFile = value;
                });
            }
        }

        public ChangeSettingsViewModel()
        { }

        public void SetTask(SemanticTask task)
        {
            var changeTask = task as ChangeTask;
            this._Model = changeTask.Param;
            this.PreFile = this._Model.PreFeatureFile;
            this.PostFile = this._Model.PostFeatureFile;
            this.ChangeFile = this._Model.ChangeFile;
        }
    }
}
