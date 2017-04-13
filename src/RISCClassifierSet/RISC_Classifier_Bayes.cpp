#include "StdAfx.h"
#include "RISC_Classifier_Bayes.h"


RISC_Classifier_Bayes::RISC_Classifier_Bayes(void)
{
}


RISC_Classifier_Bayes::~RISC_Classifier_Bayes(void)
{
}
int RISC_Classifier_Bayes::Create()
{
	if (m_checkResult==1)
	{
		m_glcbayers=new GLC_Bayes();
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
void RISC_Classifier_Bayes::SetParameter(const char* parameter)
{

}
//ѵ������
void RISC_Classifier_Bayes::ExcuteTrain()
{
	m_glcbayers->GLC_Bayes_Train(m_trainData,m_trainData_row,m_trainData_line);
}
//ִ�з���
int RISC_Classifier_Bayes::ExcuteClassify(double data[])
{
	try
	{
		int result=m_glcbayers->GLC_Bayes_ExcuteClassify(data);
		return result;
	}
	catch (GLC_Bayes* e)
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
int  RISC_Classifier_Bayes::Destroy()
{
	try
	{
		if (m_glcbayers!=NULL)
		{
			delete m_glcbayers;
		}		
		return 1;
	}
	catch (GLC_Bayes* e)
	{
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}