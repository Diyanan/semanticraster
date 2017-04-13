#include "StdAfx.h"
#include "GLC_Tree_Classifier.h"
//#include "GLC_classifier.h"
#include "DataClassification.h"
//#include "GLC_Tree_Visual.h"


GLC_Tree_Classifier::GLC_Tree_Classifier(void)
	: m_treeNum(0)
	, m_classid(NULL)
	, m_treeSelection(NULL)
{
}


GLC_Tree_Classifier::~GLC_Tree_Classifier(void)
{
}
// 执行训练
bool GLC_Tree_Classifier::GLCTree_Execute_Train(double** data, int data_row, int data_line,int treeHight,int adboostingK,int lifesize)
{
	m_treeNum=adboostingK;
	try
	{
		//执行训练；

		m_Gtree.GetControlsParameter(treeHight,adboostingK,lifesize);//设置参数
		int pr=0;
		int &progressI=pr;
	
		m_Gtree.CreateTrainingSet(data,data_row,data_line,pr); //生成训练集
		m_treeSelection=m_Gtree.GetTreeSelection();
		return true;
	}
	catch (std::exception ex)
	{
		return false;
		throw ex.what();
	}
	

}
bool GLC_Tree_Classifier::GLCTree_Execute_Train(double* data, int data_row, int data_line,int treeHight,int adboostingK,int lifesize)
{
	m_treeNum=adboostingK;
	
	try
	{
		//一位数组转换成二维数组
		double ** traindata=new double *[data_row];
		int tempindex=0;
		for (int i=0;i<data_row;i++)
		{
			for (int j=0;j<data_line;j++)
			{
				traindata[i][j]=data[tempindex];
				tempindex;
			}
		}
		//执行训练；

		m_Gtree.GetControlsParameter(treeHight,adboostingK,lifesize);//设置参数
		int pr=0;
		int &progressI=pr;
	
		m_Gtree.CreateTrainingSet(traindata,data_row,data_line,pr); //生成训练集
		m_treeSelection=m_Gtree.GetTreeSelection();
		//删除数组
		for (int i=0;i<data_row;i++)
		{
			delete [] traindata[i];
		}
		delete [] traindata;


		return true;
	}
	catch (std::exception ex)
	{
		return false;
	}
	
}

//// 初始化可视化窗口
//bool GLC_Tree_Classifier::GLCTree_Set_Visual(CTreeCtrl* TreeCtrl,CString* attsname, int attsnum,CString* classname,int classnum)
//{
//	TRY 
//	{
//		if (TreeCtrl==NULL)
//		{
//			throw "Error1:无法找到树控件地址！";
//		}
//		if (attsname==NULL)
//		{
//			throw "Error2:无法找到属性名数组地址！";
//		}
//		if (classname==NULL)
//		{
//			throw "Error3:无法找到类别名数组地址！";
//		}
//		TreeSelection * treeSelection=m_Gtree.GetTreeSelection();
//		GLC_Tree_Visual treeVisual(TreeCtrl);
//		treeVisual.SetTreeCtr(treeSelection,m_treeNum,attsname,attsnum,classname,classnum);
//		return true;
//	}
//	CATCH (CMemoryException, e)
//	{
//		throw "Error4：可视化失败！";
//		return false;
//	}
//	END_CATCH
//
//}

// 执行分类
short* GLC_Tree_Classifier::GLCTree_Excute_Classify(double* data, int data_row, int data_line,int &progressi)
{
	try
	{
		//外部 全局变量 用于进度条控制
		/*extern CProgressCtrl* p_global_Progress;
		extern CStatic* p_global_Percent;
		extern CStatic* p_global_jdmc;
		int raster_percent = 0;
		p_global_jdmc->SetWindowText(_T("执行分类:"));*/

		int dataindex=0;
		short * classid=new short[data_row];
		TreeSelection * treeSelection=m_treeSelection;//m_Gtree.GetTreeSelection();
		DataClassification classification;
		double * classifydata;
		for (int i=0;i<data_row;i++)
		{
			classifydata=new double[data_line];
			for (int j=0;j<data_line;j++)
			{
				classifydata[j]=data[dataindex];
				dataindex++;
			}
			classid[i]=classification.doClassifySingle(classifydata,treeSelection,m_treeNum);
			delete []classifydata;
			progressi++;
		}
			
		
		m_classid=classid;
		return classid;
	}
	catch (std::exception ex)
	{
		return 0;
	}

	
	
	
}
// 执行分类
short  GLC_Tree_Classifier::GLCTree_Excute_Classify(double* data, int data_line)
{

	try
	{
		TreeSelection * treeSelection=m_treeSelection;
		if (data==NULL||data_line<1||treeSelection==NULL)
		{
			AfxThrowMemoryException();
		}
		DataClassification classification;

		short classID=classification.doClassifySingle(data,treeSelection,m_treeNum);


		return classID;
	}
	catch (std::exception ex)
	{
		return 0;
	}
	
	
}



// 清除数据
bool GLC_Tree_Classifier::GLCTree_Clear(void)
{
	if (m_classid==NULL)
	{
		return false;
	}
	else
	{
		delete [] m_classid;
		return true;
	}
	
}


TreeSelection* GLC_Tree_Classifier::GLCTree_Get_TreeSelection(void)
{
	return m_treeSelection;
}


int GLC_Tree_Classifier::GLC_Set_TreeModle(TreeSelection* treeSelection)
{
	if (treeSelection==NULL)
	{
		m_treeSelection=m_Gtree.GetTreeSelection();
		return 0;
	}
	m_treeSelection=treeSelection;
	return 1;
}
