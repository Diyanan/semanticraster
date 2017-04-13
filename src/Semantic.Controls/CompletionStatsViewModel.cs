using Semantic.WpfExtensions;
using System.ComponentModel;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class CompletionStatsViewModel : ViewModelBase
    {
        private string _OperationText;
        private bool _ProgressBarVisible;
        private long _Completed;
        private long _Requested;

        public string OperationText
        {
            get
            {
                return this._OperationText;
            }
            set
            {
                this.SetProperty<string>("OperationText", ref this._OperationText, value, false);
            }
        }

        public bool ProgressBarVisible
        {
            get
            {
                return this._ProgressBarVisible;
            }
            set
            {
                this.SetProperty<bool>("ProgressBarVisible", ref this._ProgressBarVisible, value, false);
            }
        }

        public long Completed
        {
            get
            {
                return this._Completed;
            }
            set
            {
                this.SetProperty<long>("Completed", ref this._Completed, value, false);
            }
        }

        public long Requested
        {
            get
            {
                return this._Requested;
            }
            set
            {
                this.SetProperty<long>("Requested", ref this._Requested, value, false);
            }
        }

        public CompletionStats Model { get; private set; }

        public CompletionStatsViewModel(CompletionStats model)
        {
            this.Model = model;
            if (this.Model == null)
                return;
            this.Model.PropertyChanged += new PropertyChangedEventHandler(this.Model_PropertyChanged);
            this.RefreshState();
        }

        private void Model_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            this.RefreshState();
        }

        private void RefreshState()
        {
            if (this.Model == null)
                return;
            if (this.Model.Pending)
            {
                this.ProgressBarVisible = false;
                this.OperationText = Resources.StatusBar_ResolvePending;
            }
            else if (this.Model.Failed)
            {
                this.OperationText = Resources.StatusBar_ResolveFailed;
                this.ProgressBarVisible = false;
            }
            else if (this.Model.Cancelled)
            {
                this.OperationText = Resources.StatusBar_ResolveCancelled;
                this.ProgressBarVisible = false;
            }
            else
            {
                this.Completed = this.Model.Completed;
                this.Requested = this.Model.Requested;
                this.ProgressBarVisible = this.Completed < this.Requested;
                if (this.ProgressBarVisible)
                    this.OperationText = string.Format(Resources.StatusBar_Resolving, this.Completed, this.Requested);
                else
                    this.OperationText = Resources.StatusBar_DoneResolving;
            }

        }
    }
}
