#pragma once
#include "risc_classifier.h"
#include "GLC_ANN.h"
class GLC_EXP RISC_Classifier_ANN :
	public RISC_Classifier
{
private: 
	GLC_ANN * m_glcann;
public:
	RISC_Classifier_ANN(void);
	~RISC_Classifier_ANN(void);
	/************************************************************************/
	/*                                                         */
	/************************************************************************/

	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(
		int inNum=14,                      //输入单元的个数，用于分类的特征个数
		int outNum=6,                      //输出单元的个数,输出类别的个数		
		int hidNum=8,                        //隐藏神经元的个数	，经验值，需要网格验证
		int layes=3,                          //网络的层数
		int connectinRate=1,              //链接参数
		float learningRate=0.007f,             //学习率
		float desiredError=0.0001f,          //终止错误率
		int iterationBetweenRepots=100,       //报告间隔
		int maxepoches=5000                  //最大迭代次数
	
		);             
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	int ExcuteClassify(float data[]);
	//删除模型
	int  Destroy();
};

