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
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(svm_parameter para_svm);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};

