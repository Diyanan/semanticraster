#pragma once
#include "glc_adaboosting_svm.h"
class AFX_EXT_CLASS GLC_SVM_Classify
{
public:
	GLC_SVM_Classify(void);
	~GLC_SVM_Classify(void);
	bool GLCSVM_Execute_Train(double** data, int data_row, int data_line,int adboostingK=10);
	bool GLCSVM_Execute_Train(double*data, int data_row, int data_line,int adboostingK=10);
	// 执行分类
	short* GLCSVM_Excute_Classify(double* data, int data_row, int data_line);
	// 清除数据
	bool GLCTree_Clear(void);
private:
	// svm对象
	GLC_Adaboosting_SVM m_Gsvm;
	// svm的个数
	int m_svmNum;
	// 记录分类之后的id
	short* m_classid;
};

