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
	 //从文件中获取样本
	 //void LoardSampleFile(const char * samplesetFile);
	 //设置参数
	 void SetParameter(const char* parameter);
	 //训练样本
	 void ExcuteTrain();
	 //执行分类
	 int ExcuteClassify(double data[]);
	 //删除模型
	 int  Destroy();
	 int m_row;
	 int test(double data[]);
};

