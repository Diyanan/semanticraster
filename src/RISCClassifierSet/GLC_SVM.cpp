#include "StdAfx.h"
#include "GLC_SVM.h"
#include "DataFilling.h"
#include <iostream>
using namespace std;


GLC_SVM::GLC_SVM(void)
	: m_svm_parameter(NULL)
	, m_svm_problem(NULL)
	, m_nod_data(NULL)
	, m_model(NULL)
{
}


GLC_SVM::~GLC_SVM(void)
{
}
void GLC_SVM::SetParameter(svm_parameter* svmparameter)                                    //���ò�����Ҫ�������أ������ýṹ��
{
	
	m_svm_parameter=svmparameter;
	//cout<<"���ԣ�"<<m_svm_parameter->C<<endl;
}
void GLC_SVM::SetData() //��÷�������
{

}
void GLC_SVM::ExecutTrain(int & progressi)
{
	
	DataFilling data(m_data,m_data_row,m_data_line-1);//֮���Լ�ȥ1����Ϊ���һ�в���Ҫ	
	double *targetClassId_train=new double[m_data_row];
	for (int i=0;i<m_data_row;i++)
	{
		targetClassId_train[i]=m_data[i][m_data_line-1];
		//cout<<".";
	}
	svm_problem *problem=NULL;
	problem=&data.GetSVMproblem(targetClassId_train); //***************************���乤��Ҫɾ��



	m_svm_problem=problem;
	svm_problem *problemNew=new svm_problem;
	*problemNew=*problem;
	svm_node **nod_data=problem->x;
	m_nod_data=nod_data;	
	//cout<<m_svm_parameter->C<<endl;
	svm_parameter *svmparameter=new svm_parameter;
	*svmparameter=*m_svm_parameter;
	//cout<<svmparameter->C<<endl;
	m_model=svm_train(problemNew,svmparameter);
    delete []targetClassId_train;



}
void GLC_SVM::ExecutTrain()
{
	DataFilling data(m_data,m_data_row,m_data_line-1);//֮���Լ�ȥ1����Ϊ���һ�в���Ҫ	
	double *targetClassId_train=new double[m_data_row];
	for (int i=0;i<m_data_row;i++)
	{
		targetClassId_train[i]=m_data[i][m_data_line-1];
	}
	svm_problem *problem=&data.GetSVMproblem(targetClassId_train); //***************************���乤��Ҫɾ��
	

	m_svm_problem=problem;
	svm_node **nod_data=problem->x;
	m_nod_data=nod_data;	
	m_model=svm_train(m_svm_problem,m_svm_parameter);
    delete []targetClassId_train;



}
void GLC_SVM::GetModle(svm_model * svmmodle)                                         //���ģ��
{
	svmmodle=m_model;
}
svm_model * GLC_SVM::ReturnModle()                                         //���ģ��
{
	return m_model;
}
void GLC_SVM::SetModle(svm_model * svmmodle) //����ģ�ͣ��������Ѿ����ɵ�ģ�������з���
{
	m_model=svmmodle;
}
short* GLC_SVM::ExecutClassify(double **data,int data_row,int data_line,int & progressi)                                   //ִ�з���
{
	if (progressi==-1)
	{
		int svmindex=0;
		svm_node * pre=new svm_node[data_line];
		for (int i=0;i<data_row;i++)
		{
			for (int j=0;j<data_line-1;j++)
			{
				if (data[i][j]!=0)
				{
					pre[j].index=data[i][j];
					svmindex++;
				}
			}
			pre[svmindex].index=-1;
			pre[svmindex].value=NULL;
		}
	
		double tempPre=svm_predict(m_model,pre);	
		progressi=(int)tempPre;
		delete pre;		
		return NULL;
	
		//DataFilling svmdata(data,data_row,data_line);//	
	 //   double *targetClassId_train=new double[m_data_row];

  //      svm_problem *problem=&svmdata.GetSVMproblem(targetClassId_train); //***************************���乤��Ҫɾ��
	 //   svm_node **nod_classify=problem->x;//***************************���乤��Ҫɾ��
		//svm_node * pre=nod_classify[0];
	
		//double tempPre=svm_predict(m_model,pre);	
		//progressi=(int)tempPre;
		//svmdata.DataCleaner();
		//delete targetClassId_train;		
		//return NULL;

	}
	DataFilling svmdata(data,data_row,data_line);//	
	double *targetClassId_train=new double[m_data_row];
    svm_problem *problem=&svmdata.GetSVMproblem(targetClassId_train); //***************************���乤��Ҫɾ��
	svm_node **nod_classify=problem->x;//***************************���乤��Ҫɾ��
	short *classfiedID=new short[data_row];

	for (int i=0;i<data_row;i++)
	{
		svm_node * pre=nod_classify[i];
		double tempPre=svm_predict(m_model,pre);
		classfiedID[i]=(short)tempPre;
	}
	m_classifiedresultID=classfiedID;
	

	/*svm_node * pre=nod_data[radomI];
	double tempPre=svm_predict(model,pre);*/
	return classfiedID;
}
short  GLC_SVM::ExecutClassify(svm_node *data)                                                //ִ�з���
{	
	double tempPre=svm_predict(m_model,data);		
	return (int)tempPre;
}
short *GLC_SVM::GetTest(double **data,int data_row,int data_line)                                       //��ò��Խ��
{
	int progressI=0;
	int &prI=progressI;

	if (data==NULL)
	{
		short* classsid=ExecutClassify(m_data,m_data_row,m_data_line,prI);
		return classsid;
	}
	short* classsid=ExecutClassify(data,data_row,data_line,prI);
	m_testclassID=classsid;
	return classsid;
	

}
void GLC_SVM::GetAccuracyEvaluation(void)                           //��þ�������
{

}
bool GLC_SVM::DestroyModel()   //ɾ��ģ��
{
	if (m_model!=NULL)
	{
		svm_destroy_model(m_model);
		return true;
	}
	return false;
}
/*
DataFilling data(sampleData,sampleData_row,sampleData_line-1);//֮���Լ�ȥ1����Ϊ���һ�в���Ҫ	
svm_problem *problem=&data.GetSVMproblem(targetClassId_train);
svm_node **nod_data=problem->x;
*/