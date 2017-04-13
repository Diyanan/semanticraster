#pragma once
#include "glc_classifier.h"

class _declspec(dllexport) GLC_ANN :
	public GLC_Classifier
{
public:
	GLC_ANN(void);
	~GLC_ANN(void);
	void SetParameter(glc_ann_parameter* annparameter);                                     //设置参数，要三次重载，参数用结构体
	void SetData(); //获得分类数据
	void ExecutTrain(int & progressi);                                      //执行训练
	void ExecutTrain()   ;                                   //执行训练

	void GetModle(FANN::neural_net* svmmodle);                                         //获得模型
	FANN::neural_net* ReturnModle();
	void SetModle(FANN::neural_net* svmmodle); //设置模型，可用于已经生成的模型来进行分类
	short* ExecutClassify(double **data,int data_row,int data_line,int & progressi);                                   //执行分类
	short  ExecutClassify(float *data);                                   //执行分类
	short *GetTest(double **data=NULL,int data_row=0,int data_line=0);                                       //获得测试结果
	void GetAccuracyEvaluation(void);                           //获得精度评价
	bool DestroyModel();   //删除模型
private:
	glc_ann_parameter* m_annparameter;
	// 记录网络
	FANN::neural_net* m_network;
	// ann分类器
	GLCann_classifier* m_gann_classify;
};

