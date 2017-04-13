#pragma once
#include "risc_classifier.h"
#include "GLC_ANN.h"
class GLC_EXP RISC_Classifier_ANN :
	public RISC_Classifier
{
private: 
	GLC_ANN * m_glcann;
public:
	RISC_Classifier_ANN(void);
	~RISC_Classifier_ANN(void);
	/************************************************************************/
	/*                                                         */
	/************************************************************************/

	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(
		int inNum=14,                      //���뵥Ԫ�ĸ��������ڷ������������
		int outNum=6,                      //�����Ԫ�ĸ���,������ĸ���		
		int hidNum=8,                        //������Ԫ�ĸ���	������ֵ����Ҫ������֤
		int layes=3,                          //����Ĳ���
		int connectinRate=1,              //���Ӳ���
		float learningRate=0.007f,             //ѧϰ��
		float desiredError=0.0001f,          //��ֹ������
		int iterationBetweenRepots=100,       //������
		int maxepoches=5000                  //����������
	
		);             
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	int ExcuteClassify(float data[]);
	//ɾ��ģ��
	int  Destroy();
};

