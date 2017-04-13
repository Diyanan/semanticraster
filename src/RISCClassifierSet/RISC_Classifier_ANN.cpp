#include "StdAfx.h"
#include "RISC_Classifier_ANN.h"


RISC_Classifier_ANN::RISC_Classifier_ANN(void)
{
  
}


RISC_Classifier_ANN::~RISC_Classifier_ANN(void)
{
}
int RISC_Classifier_ANN::Create()
{
	if (m_checkResult==1)
	{
		m_glcann=new GLC_ANN();
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
void RISC_Classifier_ANN::SetParameter(
	int inNum,                      //���뵥Ԫ�ĸ��������ڷ������������
	int outNum,                      //�����Ԫ�ĸ���,������ĸ���		
	int hidNum,                        //������Ԫ�ĸ���	������ֵ����Ҫ������֤
	int layes,                          //����Ĳ���
	int connectinRate,              //���Ӳ���
	float learningRate,             //ѧϰ��
	float desiredError,          //��ֹ������
	int iterationBetweenRepots,       //������
	int maxepoches                  //����������
)
{
	try
	{
		glc_ann_parameter* parameter_ann=new glc_ann_parameter();	
		parameter_ann->connectinRate=connectinRate;
		parameter_ann->learningRate=learningRate;
		parameter_ann->desiredError=desiredError;
		parameter_ann->iterationBetweenRepots=iterationBetweenRepots;
		parameter_ann->maxepoches=maxepoches;
		parameter_ann->layes=layes;
		parameter_ann->inNum=inNum;
		parameter_ann->hidNum=hidNum;
		parameter_ann->outNum=outNum;
		m_glcann->GetData(m_trainData,m_trainData_row,m_trainData_line);
		m_glcann->SetParameter(parameter_ann);
	
	}
	catch (std::exception & e )
	{
		throw e;
	}
	catch(...)
	{
		throw "ANN���δ���";
	}

}
//ѵ������
void RISC_Classifier_ANN::ExcuteTrain()
{
	int p=1;
	
	
	m_glcann->ExecutTrain(p);
}
//ִ�з���
int RISC_Classifier_ANN::ExcuteClassify(double data[])
{
	try
	{
		float * anndata;
		anndata=new float[m_trainData_line];

		int result=m_glcann->ExecutClassify(anndata);
		delete [] anndata;
		return result;
	}
	catch (GLC_ANN* e)
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
int RISC_Classifier_ANN::ExcuteClassify(float data[])
{
	try
	{
		
		int result=m_glcann->ExecutClassify(data);
		return result;
	}
	catch (GLC_ANN* e)
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
int  RISC_Classifier_ANN::Destroy()
{
	try
	{
		if (m_glcann!=NULL)
		{
			delete m_glcann;
		}		
		return 1;
	}
	catch (GLC_ANN* e)
	{
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}