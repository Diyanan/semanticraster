#include "StdAfx.h"
#include "RISC_Classifier_AdTree.h"


RISC_Classifier_AdTree::RISC_Classifier_AdTree(void)
{
}


RISC_Classifier_AdTree::~RISC_Classifier_AdTree(void)
{
}
int RISC_Classifier_AdTree::Create()
{
	if (m_checkResult==1)
	{
		m_adClassifier=new ModelIntegrator();
		return 1;
	}
	else
		return 0;
}

//设置参数
void RISC_Classifier_AdTree::SetParameter(int treeHeigh/*树高*/,int leefSize/*叶子容纳样本个数*/,int adboostingK/*Adboosting迭代次数*/)
{

	m_adClassifier->GetControlsParameter(treeHeigh,adboostingK,leefSize,1);//获得参数
	
	//	m_Gtree.GetControlsParameter(treeHight,adboostingK,lifesize);//设置参数
	//int pr=0;
	//int &progressI=pr;
	//CProgressCtrl progressBar;
	//CString Err;
	//CString & e=Err;
	//m_Gtree.CreateTrainingSet(data,data_row,data_line,pr); //生成训练集
	//m_treeSelection=m_Gtree.GetTreeSelection();
}
void RISC_Classifier_AdTree::ExcuteTrain()
{
	//初始化属性集
	int * attList;
	int attlist_n=m_trainData_line-1;
	int *y;
	y=new int[attlist_n];
	for (int i=0;i<attlist_n;i++)
	{
		y[i]=i;
	}
	attList=y;
	int tmp=0;
	m_adClassifier->AdaBoostingTrainingRoles(m_trainData,m_trainData_row,m_trainData_line,attList,attlist_n,tmp);
	int treeSum=0;
	m_treeSelection=m_adClassifier->GetTreeSelection(treeSum);//获得决策树集合
	m_treeNum=treeSum;
}
int RISC_Classifier_AdTree::ExcuteClassify(double data[])
{
	DataClassification classification;

	short classID=classification.doClassifySingle(data,m_treeSelection,m_treeNum);
	return classID;
}
int  RISC_Classifier_AdTree::Destroy()
{
	try
	{
		if (m_adClassifier!=NULL)
		{
			delete m_adClassifier;
		}		
		return 1;
	}
	catch (ModelIntegrator* e)
	{
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}
