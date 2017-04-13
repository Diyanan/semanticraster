#pragma once
//#include "svm.h"
#include "glc_classifier.h"
class GLC_SVM:
	public GLC_Classifier
{
public:
	GLC_SVM(void);
	~GLC_SVM(void);
	void SetParameter(svm_parameter* svmparameter);                                     //���ò�����Ҫ�������أ������ýṹ��
	void SetData(); //��÷�������
	void ExecutTrain(int & progressi);                                      //ִ��ѵ��
	void ExecutTrain()   ;                                   //ִ��ѵ��

	void GetModle(svm_model * svmmodle);                                         //���ģ��
	svm_model * ReturnModle() ;                                        //���ģ��
	void SetModle(svm_model * svmmodle); //����ģ�ͣ��������Ѿ����ɵ�ģ�������з���
	short* ExecutClassify(double **data,int data_row,int data_line,int & progressi);                                   //ִ�з���
	short ExecutClassify(svm_node *data);                                                 //ִ�з���
	short *GetTest(double **data=NULL,int data_row=0,int data_line=0);                                       //��ò��Խ��
	void GetAccuracyEvaluation(void);                           //��þ�������
	bool DestroyModel();   //ɾ��ģ��
private:
	// ����
	svm_parameter* m_svm_parameter;
	
	svm_problem *m_svm_problem;
	svm_node **m_nod_data;
	// ��¼ģ��
	svm_model *m_model;
};


