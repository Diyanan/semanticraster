#pragma once
#include "risc_classifier.h"
#include "GLC_Bayes.h"
class GLC_EXP RISC_Classifier_Bayes :
	public RISC_Classifier
{

private:
	GLC_Bayes * m_glcbayers;

public:
	
	RISC_Classifier_Bayes(void);
	~RISC_Classifier_Bayes(void);
	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(const char* parameter);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();	
};

 