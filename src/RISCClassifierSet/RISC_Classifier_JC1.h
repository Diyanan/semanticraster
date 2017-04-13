#pragma once
#include "risc_classifier.h"
#include "GLC_Classifier_BeIntegrated.h"
class GLC_EXP RISC_Classifier_JC1 :
	public RISC_Classifier
{
private:
	GLC_Classifier_BeIntegrated* m_int1;
	//设置参数
	void SetParameter();

public:
	RISC_Classifier_JC1(void);
	~RISC_Classifier_JC1(void);
	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);

	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};

