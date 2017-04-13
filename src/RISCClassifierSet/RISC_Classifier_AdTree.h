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
	TreeSelection* m_treeSelection;//获得的决策树集合
	int m_treeNum;
public:
	RISC_Classifier_AdTree(void);
	~RISC_Classifier_AdTree(void);
	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(int treeHeigh/*树高*/,int leefSize/*叶子容纳样本个数*/,int adboostingK/*Adboosting迭代次数*/);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};

