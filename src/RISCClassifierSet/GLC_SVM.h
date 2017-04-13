#pragma once
//#include "svm.h"
#include "glc_classifier.h"
class GLC_SVM:
	public GLC_Classifier
{
public:
	GLC_SVM(void);
	~GLC_SVM(void);
	void SetParameter(svm_parameter* svmparameter);                                     //设置参数，要三次重载，参数用结构体
	void SetData(); //获得分类数据
	void ExecutTrain(int & progressi);                                      //执行训练
	void ExecutTrain()   ;                                   //执行训练

	void GetModle(svm_model * svmmodle);                                         //获得模型
	svm_model * ReturnModle() ;                                        //获得模型
	void SetModle(svm_model * svmmodle); //设置模型，可用于已经生成的模型来进行分类
	short* ExecutClassify(double **data,int data_row,int data_line,int & progressi);                                   //执行分类
	short ExecutClassify(svm_node *data);                                                 //执行分类
	short *GetTest(double **data=NULL,int data_row=0,int data_line=0);                                       //获得测试结果
	void GetAccuracyEvaluation(void);                           //获得精度评价
	bool DestroyModel();   //删除模型
private:
	// 参数
	svm_parameter* m_svm_parameter;
	
	svm_problem *m_svm_problem;
	svm_node **m_nod_data;
	// 记录模型
	svm_model *m_model;
};


