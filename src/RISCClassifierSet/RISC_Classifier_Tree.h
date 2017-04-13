#pragma once
#include "risc_classifier.h"
#include "GLC_Tree.h"

class GLC_EXP RISC_Classifier_Tree :
	public RISC_Classifier
{
private: 
	GLC_Tree * m_glctree;
public:
	RISC_Classifier_Tree(void);
	~RISC_Classifier_Tree(void);

	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(int treeHeigh,int leefSize);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();
};

