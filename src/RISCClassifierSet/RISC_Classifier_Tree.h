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
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(int treeHeigh,int leefSize);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};

