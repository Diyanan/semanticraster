#include "StdAfx.h"
#include "RISC_Classifier_JC2.h"
RISC_Classifier_JC2::RISC_Classifier_JC2(void)
{
}


RISC_Classifier_JC2::~RISC_Classifier_JC2(void)
{
}
int RISC_Classifier_JC2::Create()
{
	if (m_checkResult==1)
	{
		m_int1=new GLC_Classifier_BeIntegrated();

		SetParameter();
		return 1;
	}
	else
		return 0;
}
//设置参数
void RISC_Classifier_JC2::SetParameter()
{
	//需要预设的参数，具体参考classifier.h
	int connectinRate=1;
	float learningRate=0.007f;	
	float desiredError=0.0001f;
	int iterationBetweenRepots=100;
	int maxepoches=5000;
	int layes=3;//	int layes=4;
	int inNum=38;//int inNum=55;	
	int hidNum=14;//	int hidNum=50;	
	int outNum=6;
	glc_ann_parameter parameter_ann;	
	parameter_ann.connectinRate=connectinRate;
	parameter_ann.learningRate=learningRate;
	parameter_ann.desiredError=desiredError;
	parameter_ann.iterationBetweenRepots=iterationBetweenRepots;
	parameter_ann.maxepoches=maxepoches;
	parameter_ann.layes=layes;
	parameter_ann.inNum=inNum;
	parameter_ann.hidNum=hidNum;
	parameter_ann.outNum=outNum;

	svm_parameter parameter_svm;	
	parameter_svm.svm_type =0;///*NU_SVC*/C_SVC; 
	parameter_svm.kernel_type = 0;//LINEAR/*RBF*/; 
	parameter_svm.degree = 3; 

	parameter_svm.gamma = 0.00001;//0.0001;//0.00001;//1;// 0.15;//0.25;//0.00001;//0.00001/*1.0/m_sampleDataLine*/; 
	parameter_svm.coef0 = 0; 
	parameter_svm.nu =0.5;// 0.5;  
	parameter_svm.cache_size = 100; 
	parameter_svm.C = 32;//32;//2;//25;//100; 
	parameter_svm.eps = 1e-3; 
	parameter_svm.p = 0.1;
	parameter_svm.shrinking = 1; 
	parameter_svm.probability = 0; 
	parameter_svm.nr_weight = 0; 

	TreeParameter parameter_tree;
	parameter_tree.LeatfSize=1;
	parameter_tree.TreeHeight=7;

	//设置组合的分类器
	int * subclassifier=new int[4];
	subclassifier[0]=RISC_CTree;
	subclassifier[1]=RISC_KNN ;	
	subclassifier[2]=RISC_SVM ;
	subclassifier[3]=RISC_Bayes;

	m_int1->Integrated_Set_Data(m_trainData,m_trainData_row,m_trainData_line);
	m_int1->Integrated_Set_Parameter(parameter_tree,parameter_svm,parameter_ann);

	double **test_data=new double *[m_trainData_row];
	for (int i=0;i<m_trainData_row;i++)
	{
		test_data[i]=new double[m_trainData_line];
		for (int j=0;j<m_trainData_line;j++)
		{
			test_data[i][j]=m_trainData[i][j];
		}
	}

	//m_int1->SetTestData(test_data,m_trainData_row,m_trainData_line);

}
//训练样本
void RISC_Classifier_JC2::ExcuteTrain()
{
	int xx=0;
	m_int1->Integrated_Execut_Train(xx,m_trainData,m_trainData_row,m_trainData_line);//在此使用测试数据比使用训练数据效果更佳

}
//执行分类
int RISC_Classifier_JC2::ExcuteClassify(double data[])
{
	try
	{
		int result=m_int1->Integrated_Execut_Classify_INT2(data,m_trainData_line-1);
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
int  RISC_Classifier_JC2::Destroy()
{
	try
	{
		if (m_int1!=NULL)
		{
			delete m_int1;
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
