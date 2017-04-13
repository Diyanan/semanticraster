#include "StdAfx.h"
#include "RISC_Classifier_Tree.h"


RISC_Classifier_Tree::RISC_Classifier_Tree(void)
{
}


RISC_Classifier_Tree::~RISC_Classifier_Tree(void)
{
	int xx;
}
int RISC_Classifier_Tree::Create()
{
	if (m_checkResult==1)
	{
		m_glctree=new GLC_Tree();
		return 1;
	}
	else
		return 0;
}
////���ļ��л�ȡ����
//void RISC_Classifier_Bayes::LoardSampleFile(const char * samplesetFile)
//{
//
//}
//���ò���
void RISC_Classifier_Tree::SetParameter(int treeHeigh,int leefSize)
{
	try
	{
		TreeParameter treepara;
		treepara.LeatfSize=leefSize;
		treepara.TreeHeight=treeHeigh;
		m_glctree->SetParameter(treepara);
	}
	catch (std::exception & e )
	{
		throw e;
	}
	catch(...)
	{
		throw "Tree���δ���";
	}
	
}
//ѵ������
void RISC_Classifier_Tree::ExcuteTrain()
{
	int p=1;
 	m_glctree->GetData(m_trainData,m_trainData_row,m_trainData_line);
	m_glctree->ExecutTrain(p);
}
//ִ�з���
int RISC_Classifier_Tree::ExcuteClassify(double data[])
{
	try
	{
		int result=m_glctree->ExecutClassify(data);
		return result;
	}
	catch (GLC_Tree* e)
	{
		return -1;
	}
	catch (int * e)
	{
		return -1;
	}
	catch (...)
	{
		return -1;
	}

}
//ɾ��ģ��
int  RISC_Classifier_Tree::Destroy()
{
	try
	{
		if (m_glctree!=NULL)
		{
			delete m_glctree;
		}		
		return 1;
	}
	catch (GLC_Tree* e)
	{
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}