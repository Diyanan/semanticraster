using Semantic.Engine;
using Semantic.WpfCommon;
using Semantic.WpfExtensions;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Xml.Serialization;

namespace Semantic.Controls
{
    public class ProjectManagerViewModel : ViewModelBase
    {
        public string RecentFile { get; private set; } 

        public ICommand OpenCommand { get; set; }
        public ICommand OpenMxdCommand { get; set; }
        public ICommand SaveCommand { get; set; }
        public ICommand RecentProjectGalleryItemCommand { get; set; }

        public ObservableCollectionEx<ProjectSettings> RecentList { get; set; }
        public TaskPanelLayersTabViewModel TaskPanellayersTabVM { get; set; }
        public HostControlViewModel HostControlVW { get; set; } 
        public SemanticProject Project { get; set; } 

        public List<string> Pre_Image { get; set; }
        public List<string> Pro_Image { get; set; }
        public string ShpPath { get; set; }
        public string PrjPath { get; set; }

        private void SetProject(SemanticProject project) 
        {
            //if (this.Project != null && this.Project.Tasks.Count > 0)
            //{
            //    if (MessageBox.Show(null, "保存现有工程？", "保存工程", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            //    {
            //        OnSaveProject();
            //    }
            //}

            var item = project.Model;
            if (this.RecentList.Contains(item))
            {
                this.RecentList.Remove(item);
            }
            if (item.Path != null)  
            {
                this.RecentList.Insert(0, item);
                SaveRecentList();
            }

            this.Project = project;
            this.HostControlVW.SetProject(project); 
        }

        private void SaveProject(SemanticProject project) 
        {
            var path = project.Model.Path + "\\project.srproj";
            using (var fs = File.CreateText(path))
            {
                project.Layers = this.HostControlVW.LayerManagerViewModel.Model.LayerDefinitions.ToList();
                var xml = XmlSerialize.SerializeXML<SemanticProject>(project);
                fs.Write(xml);
            }
        }

        //private void SaveMxdPrj(SemanticProject project)
        //{
        //    var path = project.Model.Path + "\\project.srproj"; 
        //    using (var fs = File.CreateText(path))
        //    {
                
        //    }

        //}

        private void CreatePrjDir(string path)
        {
            string dir0 = path + "\\PrjData";
            string dir1 = "pretreament";
            string dir2 = "cutimage";
            string dir3 = "segmentation";
            string dir4 = "calfeature";
            string dir5 = "changedetection";

            DirectoryInfo dir = new DirectoryInfo(dir0);
            dir.Create();
            dir.CreateSubdirectory(dir1);
            dir.CreateSubdirectory(dir2);
            dir.CreateSubdirectory(dir3);
            dir.CreateSubdirectory(dir4);
            dir.CreateSubdirectory(dir5);
        }

        private void OnSaveProject()
        {
            if (!Directory.Exists(this.Project.Model.Path))
            {
                FolderBrowserDialog fbDlg = new FolderBrowserDialog();
                fbDlg.Description = "设置工程保存目录";
                if (fbDlg.ShowDialog() == DialogResult.OK)
                {
                    this.Project.Model.Path = fbDlg.SelectedPath;
                }
            }
            SaveProject(this.Project);
        }

        /// <summary>
        /// 浏览工程目录
        /// </summary>
        private void OnBrowseProject()
        {
            // 选择文件
            FolderBrowserDialog fbDlg = new FolderBrowserDialog();
            fbDlg.Description = "浏览工程目录";
            if (fbDlg.ShowDialog() == DialogResult.OK)
            {
                var projectFile = fbDlg.SelectedPath + "\\project.srproj";
                if (File.Exists(projectFile))
                {
                    var project = XmlSerialize.DeserializeXML<SemanticProject>(File.ReadAllText(projectFile));
                    SetProject(project);
                }
            }
        }

        private void OnOpenProject(ProjectSettings item)
        {
            if (this.Project.Model.Equals(item)) 
                return;
            var path = item.Path;
            if (!File.Exists(path))
            {
                MessageBox.Show("工程文件不存在！");
                return;
            }
            try
            {
                //var project = XmlSerialize.DeserializeXML<SemanticProject>(File.ReadAllText(path));
                //SetProject(project);
                this.HostControlVW.Map.ReadMxdPrj(path);
                this.HostControlVW.TaskPanel.LayersTab.OnInitLayerManageTool();
            }
            catch
            { }
        }

        private void LoadRecentList()
        {
            if (File.Exists(this.RecentFile))
            {
                XmlSerializer xs = new XmlSerializer(typeof(List<ProjectSettings>));
                using (var fs = new FileStream(this.RecentFile, FileMode.Open))
                {
                    var list = xs.Deserialize(fs) as List<ProjectSettings>;
                    foreach (var item in list.Take(10))
                    {
                        if (!this.RecentList.Contains(item))
                        {
                            this.RecentList.Add(item);
                        }
                    }
                }
            }

            SetProject(new SemanticProject
            {
                Model = new ProjectSettings(){Name = "未命名工程"}
            });
        }

        public void SaveRecentList()
        {
            var list = this.RecentList.ToList();
            XmlSerializer xs = new XmlSerializer(typeof(List<ProjectSettings>));
            using (var fs = new FileStream(this.RecentFile, FileMode.Create))
            {
                xs.Serialize(fs, list);
            }
        }

        public ProjectManagerViewModel(HostControlViewModel hostControlVW)
        {
            this.HostControlVW = hostControlVW;
            this.TaskPanellayersTabVM = hostControlVW.TaskPanel.LayersTab;
            this.RecentFile = AppDomain.CurrentDomain.BaseDirectory + "conf\\recent.xml";
            this.RecentList = new ObservableCollectionEx<ProjectSettings>();

            this.OpenCommand = new DelegatedCommand(new Action(this.OnBrowseProject));
            this.SaveCommand = new DelegatedCommand(new Action(this.OnSaveProject));
            this.RecentProjectGalleryItemCommand = new DelegatedCommand<ProjectSettings>(new Action<ProjectSettings>(this.OnOpenProject));

            this.OpenMxdCommand = new DelegatedCommand(new Action(this.OnBrowseMxd));

            LoadRecentList();
        }

        private void OnBrowseMxd()
        {
            // 选择文件
            OpenFileDialog fbDlg = new OpenFileDialog();
            fbDlg.Title = "选择mxd文件";
            fbDlg.Filter = "地图文档(*.mxd)|*.mxd";
            if (fbDlg.ShowDialog() == DialogResult.OK)
            {
                var mxdFile = fbDlg.FileName;
                if (File.Exists(mxdFile))
                {
                    // 加载mxd
                    this.HostControlVW.Map.ReadMxdPrj(mxdFile);
    
                    // 初始化图层列表、任务列表
                    this.HostControlVW.TaskPanel.LayersTab.OnInitLayerManageTool();
                    this.HostControlVW.InitTaskPanel();

                    // 工程配置目录
                    this.Pre_Image = this.HostControlVW.Map.GetGrouplayer("PRE_IMAGE");
                    this.Pro_Image = this.HostControlVW.Map.GetGrouplayer("PRO_IMAGE");
                    this.ShpPath = this.HostControlVW.Map.GetShpfilePath("V_LCRA");
                    this.PrjPath = Directory.GetParent(mxdFile).ToString();
                    this.CreatePrjDir(this.PrjPath);

                    // 添加最近列表
                    this.Project.Model.Path = mxdFile;
                    SetProject(this.Project);
                }
            }
        }

    }
}
