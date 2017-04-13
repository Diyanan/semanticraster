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
////从文件中获取样本
//void RISC_Classifier_Bayes::LoardSampleFile(const char * samplesetFile)
//{
//
//}
//设置参数
void RISC_Classifier_ANN::SetParameter(
	int inNum,                      //输入单元的个数，用于分类的特征个数
	int outNum,                      //输出单元的个数,输出类别的个数		
	int hidNum,                        //隐藏神经元的个数	，经验值，需要网格验证
	int layes,                          //网络的层数
	int connectinRate,              //链接参数
	float learningRate,             //学习率
	float desiredError,          //终止错误率
	int iterationBetweenRepots,       //报告间隔
	int maxepoches                  //最大迭代次数
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
		throw "ANN传参错误！";
	}

}
//训练样本
void RISC_Classifier_ANN::ExcuteTrain()
{
	int p=1;
	
	
	m_glcann->ExecutTrain(p);
}
//执行分类
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
//删除模型
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