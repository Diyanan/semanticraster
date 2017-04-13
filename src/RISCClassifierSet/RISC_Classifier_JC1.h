#pragma once
#include "risc_classifier.h"
#include "GLC_Classifier_BeIntegrated.h"
class GLC_EXP RISC_Classifier_JC1 :
	public RISC_Classifier
{
private:
	GLC_Classifier_BeIntegrated* m_int1;
	//���ò���
	void SetParameter();

public:
	RISC_Classifier_JC1(void);
	~RISC_Classifier_JC1(void);
	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);

	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();
};

