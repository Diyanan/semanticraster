using Microsoft.Win32;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Semantic.Controls
{
    public class JoinManagerViewModel : ViewModelBase
    {
        public ICommand AddJoinTableCommand { get; set; }
        public ICommand RemoveLastJoinTableCommand { get; set; }
        public ICommand ClearAllTableCommand { get; set; }

        public ObservableCollectionEx<JoinEntityViewModel> JoinTables { get; private set; }

        public IJoinManager Manager { get; set; }

        public LayerViewModel LayerVM { get; set; }

        public JoinManagerViewModel(IJoinManager manager)
        {
            this.Manager = manager;
            this.JoinTables = new ObservableCollectionEx<JoinEntityViewModel>();

            this.AddJoinTableCommand = new DelegatedCommand(this.AddTable);
            this.RemoveLastJoinTableCommand = new DelegatedCommand(this.RemoveLastTable, new Predicate(() => this.JoinTables.Count > 0));
            this.ClearAllTableCommand = new DelegatedCommand(this.ClearAllTable, new Predicate(() => this.JoinTables.Count > 0));
        }

        private void RefreshJoins()
        {
            this.JoinTables.Clear();
            foreach (var joinName in this.Manager.GetAllJoins(this.LayerVM.LayerDefinition))
            {
                var joinEntity = new JoinEntityViewModel(joinName);
                joinEntity.RemoveCallback = this.OnJoinRemoved;
                this.JoinTables.Add(joinEntity);
            }
        }

        private void ClearAllTable()
        {
            this.Manager.ClearAllJoinTable(this.LayerVM.LayerDefinition);
            RefreshJoins();
        }

        private void RemoveLastTable()
        {
            this.Manager.RemoveLastJoinTable(this.LayerVM.LayerDefinition);
            RefreshJoins();
        }

        private void AddTable()
        {
            // 选择文件
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Filter = "表格数据|*.csv";
            if (openFile.ShowDialog() == true)
            {
                string csvPath = openFile.FileName;
                this.Manager.AddJoinTable(this.LayerVM.LayerDefinition, csvPath);
                RefreshJoins();
            }
        }

        private void OnJoinRemoved(JoinEntityViewModel listEntry)
        {
            if (listEntry != null)
            {
                this.Manager.RemoveJoinTable(this.LayerVM.LayerDefinition, listEntry.DisplayString);
                RefreshJoins();
            }
        }

        public void SetLayer(LayerViewModel layerVM)
        {
            this.LayerVM = layerVM;
            RefreshJoins();
        }
    }
}
