#include "StdAfx.h"
#include "GLC_ANN.h"
#include "floatfann.h"
#include "fann_cpp.h"

#include <iostream>
//using namespace std;

GLC_ANN::GLC_ANN(void)
	: m_annparameter(NULL)
	, m_network(NULL)
	, m_gann_classify(NULL)
{
	m_gann_classify=new GLCann_classifier;
}


GLC_ANN::~GLC_ANN(void)
{
}
void GLC_ANN::SetParameter(glc_ann_parameter* annparameter)                                    //设置参数，要三次重载，参数用结构体
{
	m_annparameter=annparameter;
}
void GLC_ANN::SetData() //获得分类数据
{

}
void GLC_ANN::ExecutTrain(int & progressi)
{

	
	int connectinRate=m_annparameter->connectinRate;
	float learningRate=m_annparameter->learningRate;	
	float desiredError=m_annparameter->desiredError;
	int iterationBetweenRepots=m_annparameter->iterationBetweenRepots;
	int maxepoches=m_annparameter->maxepoches;
	int layes=m_annparameter->layes;
	int inNum=m_annparameter->inNum;
	//int inNum=55;
	int hidNum=m_annparameter->hidNum;
	//	int hidNum=50;
	int outNum=m_annparameter->outNum;
	int progressI=0;
	int &prI=progressI;
	//GLCann_classifier* gann;
	

	m_gann_classify->Gann_set_parameter(connectinRate,learningRate,desiredError,iterationBetweenRepots,maxepoches,layes,inNum,hidNum,outNum);

	

	m_gann_classify->Gann_ExecuteTraining_on_data(m_data,m_data_row,m_data_line,prI);


	FANN::neural_net* network=m_gann_classify->Gann_get_modle();//用于记录网络
	m_network=network;



}
void GLC_ANN::ExecutTrain()
{
	
	//p_global_Progress->SetPos(100);

	int connectinRate=m_annparameter->connectinRate;
	float learningRate=m_annparameter->learningRate;	
	float desiredError=m_annparameter->desiredError;
	int iterationBetweenRepots=m_annparameter->iterationBetweenRepots;
	int maxepoches=m_annparameter->maxepoches;
	int layes=m_annparameter->layes;
	int inNum=m_annparameter->inNum;
	//int inNum=55;
	int hidNum=m_annparameter->hidNum;
	//	int hidNum=50;
	int outNum=m_annparameter->outNum;
	int progressI=0;
	int &prI=progressI;
	//GLCann_classifier* gann;
	

	m_gann_classify->Gann_set_parameter(connectinRate,learningRate,desiredError,iterationBetweenRepots,maxepoches,layes,inNum,hidNum,outNum);

	

	m_gann_classify->Gann_ExecuteTraining_on_data(m_data,m_data_row,m_data_line,prI);
	

	FANN::neural_net* network=m_gann_classify->Gann_get_modle();//用于记录网络
	m_network=network;

	

}
void GLC_ANN::GetModle(FANN::neural_net* svmmodle)                                         //获得模型
{
	svmmodle=m_network;	
}
FANN::neural_net* GLC_ANN::ReturnModle()                                         //获得模型
{
	 return m_network;	
}
void GLC_ANN::SetModle(FANN::neural_net* svmmodle) //设置模型，可用于已经生成的模型来进行分类
{
	m_network=svmmodle;
}
short* GLC_ANN::ExecutClassify(double **data,int data_row,int data_line,int & progressi)                                   //执行分类
{ 
	/*float ** data_f=new float *[data_row];
	for (int i=0;i<data_row;i++)
	{
	data_f[i]=new float [data_line];
	for (int j=0;j<data_line;j++)
	{
	data_f[i][j]=(float)data[i][j];
	}

	}*/
	/*for (int i=0;i<data_row;i++)
	{
	for (int j=0;j<data_line;j++)
	{
	cout<<data[i][j]<<" ";
	}
	cout<<endl;
	}*/
	/*short *classid=m_gann_classify->Gann_ExecuteClassify_on_data_double(data,data_row,data_line,progressi);*/
	short *classid=new short[data_row];
	int temp=0;
	int& t=temp;
	for (int i=0;i<data_row;i++)
	{
		temp=-1;
		double** tempdata=new double*[1];
		tempdata[0]=data[i];
		m_gann_classify->Gann_ExecuteClassify_on_data_double(tempdata,1,data_line,temp);
		delete tempdata;
		classid[i]=temp;
	}
	if (progressi==-1)
	{
		progressi=temp;
		return NULL;
	}
	if (progressi==0)
	{
		return classid;
	}
	
	
	//short *classid=m_gann_classify->Gann_ExecuteClassify_on_data(data_f,data_row,data_line,progressi);
	/*for (int i=0;i<data_row;i++)
	{
	delete data_f[i];
	}
	delete data_f[];*/
	
}

short  GLC_ANN::ExecutClassify(float *data)                                   //执行分类
{ 
	short classid=m_gann_classify->Gann_ExecuteClassify_on_data_float(data);	
	return classid;	
}
short *GLC_ANN::GetTest(double **data,int data_row,int data_line)                                       //获得测试结果
{
	int progressI=0;
	int &prI=progressI;

	if (data==NULL)
	{
		
		short* classsid=ExecutClassify(m_data,m_data_row,m_data_line-1,prI);
		return classsid;
	}
	short* classsid=ExecutClassify(data,data_row,data_line-1,prI);
	m_testclassID=classsid;
	return classsid;

}
void GLC_ANN::GetAccuracyEvaluation(void)                           //获得精度评价
{

}
bool GLC_ANN::DestroyModel()   //删除模型
{
	if (m_network!=NULL)
	{
		m_network=NULL;
		m_gann_classify->Gann_Destroy_Model();
		return true;
	}
	return false;
}