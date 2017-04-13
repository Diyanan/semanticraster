
#pragma once
#include "Tree.h"
class  GLCTreeClassifier
{
public:
	GLCTreeClassifier(void);
	~GLCTreeClassifier(void);
	// 生成训练集
	void CreateTrainingSet(double** trainData,int trainData_row,int trainData_line,/* CProgressCtrl* progressBar,*/int &progressBarI);//带存放路径，进度条
	//void CreateTrainingSet(double** trainData,int trainData_row,int trainData_line, /*CProgressCtrl* progressBar,*/int &progressBarI,CString & Eror);//带进度条
	void CreateTrainingSet(double** trainData,int trainData_row,int trainData_line);//不带路径进度条
private:
	// 记录训练集的存放路劲
	//CString m_trainSet_path;
public:
	//获得控制参数
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio);
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize);
private:
	// 建立决策树的高
	int m_treeHeight;
	// 迭代次数
	int adaBoostingK;
	// 叶子大小
	int m_leafsSize;
	// 迭代次数
	int m_adaBoostingK;
	// 样本率
	double m_sampleDataRatio;
	// 控制进度条
	//CProgressCtrl* m_progressBar;
public:
	// 执行分类返回分类结果
	short* ExecuteClassify(double** data, int row, int line);
	//short * ExecuteClassify(double** data, int row, int line,CProgressCtrl* progressBar,int &progressBarI,short * classifiedId=NULL);
private:
	// 训练集数据
	double** m_trainData;
	int m_trainData_row;
	int m_trainData_line;
	// 记录生成的树集
	TreeSelection *m_treeSelct;
	TreeSelection* m_TreeSelection;
public:
	// 获得树集
	TreeSelection* GetTreeSelection(void);
	// 使用单个值预测的分类方法
	short* ExcuteClassifySingle(double** data, int data_row, int data_line,int & progresI);
	// 执行训练
	bool GLCTree_Execute_Train(double* data, int data_row, int dtat_line);
};

