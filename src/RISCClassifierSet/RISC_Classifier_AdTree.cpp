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

//���ò���
void RISC_Classifier_AdTree::SetParameter(int treeHeigh/*����*/,int leefSize/*Ҷ��������������*/,int adboostingK/*Adboosting��������*/)
{

	m_adClassifier->GetControlsParameter(treeHeigh,adboostingK,leefSize,1);//��ò���
	
	//	m_Gtree.GetControlsParameter(treeHight,adboostingK,lifesize);//���ò���
	//int pr=0;
	//int &progressI=pr;
	//CProgressCtrl progressBar;
	//CString Err;
	//CString & e=Err;
	//m_Gtree.CreateTrainingSet(data,data_row,data_line,pr); //����ѵ����
	//m_treeSelection=m_Gtree.GetTreeSelection();
}
void RISC_Classifier_AdTree::ExcuteTrain()
{
	//��ʼ�����Լ�
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
	m_treeSelection=m_adClassifier->GetTreeSelection(treeSum);//��þ���������
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
