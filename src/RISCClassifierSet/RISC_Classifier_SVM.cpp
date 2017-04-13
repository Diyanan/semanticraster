#include "StdAfx.h"
#include "RISC_Classifier_SVM.h"


RISC_Classifier_SVM::RISC_Classifier_SVM(void)
{
}


RISC_Classifier_SVM::~RISC_Classifier_SVM(void)
{
}
int RISC_Classifier_SVM::Create()
{
	if (m_checkResult==1)
	{
		m_svm=new GLC_SVM();
		return 1;
	}
	else
		return 0;
}
//从文件中获取样本
//void LoardSampleFile(const char * samplesetFile);
//设置参数
void RISC_Classifier_SVM::SetParameter(svm_parameter para_svm)
{


	try
	{
		svm_parameter * para=new svm_parameter();
		para->C=para_svm.C;
		para->cache_size=para_svm.cache_size;
		para->coef0=para_svm.coef0;
		para->degree=para_svm.degree;
		para->eps=para_svm.eps;
		para->gamma=para_svm.gamma;
		para->kernel_type=para_svm.kernel_type;
		para->nr_weight=para_svm.nr_weight;
		para->nu=para_svm.nu;
		para->p=para_svm.p;
		para->probability=para_svm.probability;
		para->shrinking=para_svm.shrinking;
		para->svm_type=para_svm.svm_type;
		para->weight=para_svm.weight;
		para->weight_label=para_svm.weight_label;
		m_svm->GetData(m_trainData,m_trainData_row,m_trainData_line);
		m_svm->SetParameter(para);

	}
	catch (std::exception & e )
	{
		throw e;
	}
	catch(...)
	{
		throw "SVM传参错误！";
	}

}
//训练样本
void RISC_Classifier_SVM::ExcuteTrain()
{
	int pr=0;
	
	m_svm->ExecutTrain(pr);
}
//执行分类
int RISC_Classifier_SVM::ExcuteClassify(double data[])
{
	try
	{
		int N=m_trainData_line-1;
		int svmindex=0;	
		svm_node* tempdata=new svm_node[N];
		for (int j=0;j<N-1;j++)
		{

			if (data[j]!=0)
			{
				tempdata[svmindex].index=j+1;
				tempdata[svmindex].value=data[j];
				svmindex++;
			}
			tempdata[svmindex].index=-1;
			tempdata[svmindex].value=NULL;
		}


		int classID= m_svm->ExecutClassify(tempdata);
		delete [] tempdata;
		return classID;
	}
	catch (std::exception* e)
	{
		
		return 0;

	}
	
}
//删除模型
int  RISC_Classifier_SVM::Destroy()
{
	try
	{
		if (m_svm!=NULL)
		{
			delete m_svm;
		}		
		return 1;
	}
	catch (GLC_SVM* e)
	{
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}