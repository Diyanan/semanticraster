#pragma once
#include "risc_classifier.h"
#include "ModelIntegrator.h"
#include "DataClassification.h"
class GLC_EXP RISC_Classifier_AdTree :
	public RISC_Classifier
{
private:
	ModelIntegrator * m_adClassifier;
	DataClassification m_dataClassification;
	TreeSelection* m_treeSelection;//��õľ���������
	int m_treeNum;
public:
	RISC_Classifier_AdTree(void);
	~RISC_Classifier_AdTree(void);
	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(int treeHeigh/*����*/,int leefSize/*Ҷ��������������*/,int adboostingK/*Adboosting��������*/);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();
};

