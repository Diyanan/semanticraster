#include "StdAfx.h"

#include "RISC_Classifier_KNN.h"
//#include "CheckRegister.h"

RISC_Classifier_KNN::RISC_Classifier_KNN(void)
{
	
	/*m_trainData_row=0;
	m_trainData_line=0;
	m_trainData=NULL;*/

}


//RISC_Classifier_KNN::~RISC_Classifier_KNN(void)
//{
//}
int RISC_Classifier_KNN::Create()
{
	if (m_checkResult==1)
	{
		m_glcknn=new GLC_Knn();
		return 1;
	}
	else
		return 0;
}
////从文件中获取样本
//void RISC_Classifier_KNN::LoardSampleFile(const char * samplesetFile)
//{
//
//}
//设置参数
void RISC_Classifier_KNN::SetParameter(const char* parameter)
{

}
//训练样本
void RISC_Classifier_KNN::ExcuteTrain()
{
	m_glcknn->GLC_Knn_Train(m_trainData,m_trainData_row,m_trainData_line);
}
//执行分类
int RISC_Classifier_KNN::ExcuteClassify(double data[])
{
	try
	{
		int result=m_glcknn->GLC_Knn_ExcuteClassify(data);
		return result;
	}
	catch (GLC_Knn* e)
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
int  RISC_Classifier_KNN::Destroy()
{
	try
	{
		if (m_glcknn!=NULL)
		{
			delete m_glcknn;
		}		
		return 1;
	}
	catch (GLC_Knn* e)
	{
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}
int RISC_Classifier_KNN::test(double data[])
{
	return 0;
}