#pragma once
#include "glc_adaboosting_svm.h"
class AFX_EXT_CLASS GLC_SVM_Classify
{
public:
	GLC_SVM_Classify(void);
	~GLC_SVM_Classify(void);
	bool GLCSVM_Execute_Train(double** data, int data_row, int data_line,int adboostingK=10);
	bool GLCSVM_Execute_Train(double*data, int data_row, int data_line,int adboostingK=10);
	// ִ�з���
	short* GLCSVM_Excute_Classify(double* data, int data_row, int data_line);
	// �������
	bool GLCTree_Clear(void);
private:
	// svm����
	GLC_Adaboosting_SVM m_Gsvm;
	// svm�ĸ���
	int m_svmNum;
	// ��¼����֮���id
	short* m_classid;
};

