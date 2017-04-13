#pragma once
#include "risc_classifier.h"
#include "GLC_SVM.h"
class GLC_EXP RISC_Classifier_SVM :
	public RISC_Classifier
{
private:
	GLC_SVM * m_svm;
public:
	RISC_Classifier_SVM(void);
	~RISC_Classifier_SVM(void);

	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(svm_parameter para_svm);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();
};

