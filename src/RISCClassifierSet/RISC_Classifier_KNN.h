#pragma once
#include "risc_classifier.h"
#include "GLC_Knn.h"
class  GLC_EXP RISC_Classifier_KNN :
	 RISC_Classifier
{
private:
	GLC_Knn * m_glcknn;
	
public:
	RISC_Classifier_KNN(void);
	//~RISC_Classifier_KNN(void);
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
	 int m_row;
	 int test(double data[]);
};

