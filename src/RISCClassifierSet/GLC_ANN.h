#pragma once
#include "glc_classifier.h"

class _declspec(dllexport) GLC_ANN :
	public GLC_Classifier
{
public:
	GLC_ANN(void);
	~GLC_ANN(void);
	void SetParameter(glc_ann_parameter* annparameter);                                     //���ò�����Ҫ�������أ������ýṹ��
	void SetData(); //��÷�������
	void ExecutTrain(int & progressi);                                      //ִ��ѵ��
	void ExecutTrain()   ;                                   //ִ��ѵ��

	void GetModle(FANN::neural_net* svmmodle);                                         //���ģ��
	FANN::neural_net* ReturnModle();
	void SetModle(FANN::neural_net* svmmodle); //����ģ�ͣ��������Ѿ����ɵ�ģ�������з���
	short* ExecutClassify(double **data,int data_row,int data_line,int & progressi);                                   //ִ�з���
	short  ExecutClassify(float *data);                                   //ִ�з���
	short *GetTest(double **data=NULL,int data_row=0,int data_line=0);                                       //��ò��Խ��
	void GetAccuracyEvaluation(void);                           //��þ�������
	bool DestroyModel();   //ɾ��ģ��
private:
	glc_ann_parameter* m_annparameter;
	// ��¼����
	FANN::neural_net* m_network;
	// ann������
	GLCann_classifier* m_gann_classify;
};

