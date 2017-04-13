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
////从文件中获取样本
//void RISC_Classifier_Bayes::LoardSampleFile(const char * samplesetFile)
//{
//
//}
//设置参数
void RISC_Classifier_Bayes::SetParameter(const char* parameter)
{

}
//训练样本
void RISC_Classifier_Bayes::ExcuteTrain()
{
	m_glcbayers->GLC_Bayes_Train(m_trainData,m_trainData_row,m_trainData_line);
}
//执行分类
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
//删除模型
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