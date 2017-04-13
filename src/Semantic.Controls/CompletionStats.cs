using Semantic.WpfCommon;

namespace Semantic.Controls
{
    public class CompletionStats : PropertyChangedNotificationBase
    {
        private object _writerLock = new object();

        private long _Completed;
        private long _Requested;

        private bool _Pending = true;
        private bool _Cancelled;
        private bool _Failed;

        public object WriterLock
        {
            get
            {
                return this._writerLock;
            }
        }

        public string PropertyPending
        {
            get
            {
                return "Pending";
            }
        }

        public bool Pending
        {
            get
            {
                return this._Pending;
            }
            set
            {
                this.SetProperty<bool>(this.PropertyPending, ref this._Pending, value);
            }
        }

        public string PropertyCancelled
        {
            get
            {
                return "Cancelled";
            }
        }

        public bool Cancelled
        {
            get
            {
                return this._Cancelled;
            }
            set
            {
                this.SetProperty<bool>(this.PropertyCancelled, ref this._Cancelled, value);
            }
        }

        public string PropertyFailed
        {
            get
            {
                return "Failed";
            }
        }

        public bool Failed
        {
            get
            {
                return this._Failed;
            }
            set
            {
                lock (this.WriterLock)
                {
                    if (!value || value == this._Failed || this.Cancelled)
                        return;
                    this.SetProperty<bool>(this.PropertyFailed, ref this._Failed, value);
                    this.Pending = false;
                }
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
                this.SetProperty<long>("Completed", ref this._Completed, value);
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
                this.SetProperty<long>("Requested", ref this._Requested, value);
            }
        }
    }
}
