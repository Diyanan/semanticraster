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
void GLC_ANN::SetParameter(glc_ann_parameter* annparameter)                                    //���ò�����Ҫ�������أ������ýṹ��
{
	m_annparameter=annparameter;
}
void GLC_ANN::SetData() //��÷�������
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


	FANN::neural_net* network=m_gann_classify->Gann_get_modle();//���ڼ�¼����
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
	

	FANN::neural_net* network=m_gann_classify->Gann_get_modle();//���ڼ�¼����
	m_network=network;

	

}
void GLC_ANN::GetModle(FANN::neural_net* svmmodle)                                         //���ģ��
{
	svmmodle=m_network;	
}
FANN::neural_net* GLC_ANN::ReturnModle()                                         //���ģ��
{
	 return m_network;	
}
void GLC_ANN::SetModle(FANN::neural_net* svmmodle) //����ģ�ͣ��������Ѿ����ɵ�ģ�������з���
{
	m_network=svmmodle;
}
short* GLC_ANN::ExecutClassify(double **data,int data_row,int data_line,int & progressi)                                   //ִ�з���
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

short  GLC_ANN::ExecutClassify(float *data)                                   //ִ�з���
{ 
	short classid=m_gann_classify->Gann_ExecuteClassify_on_data_float(data);	
	return classid;	
}
short *GLC_ANN::GetTest(double **data,int data_row,int data_line)                                       //��ò��Խ��
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
void GLC_ANN::GetAccuracyEvaluation(void)                           //��þ�������
{

}
bool GLC_ANN::DestroyModel()   //ɾ��ģ��
{
	if (m_network!=NULL)
	{
		m_network=NULL;
		m_gann_classify->Gann_Destroy_Model();
		return true;
	}
	return false;
}