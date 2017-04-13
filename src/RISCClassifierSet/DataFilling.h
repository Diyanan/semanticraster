#pragma once
#include "GLC_Classifier.h"//#include "svm.h"
class DataFilling
{
public:
	DataFilling(double** data, int row , int line);
	~DataFilling(void);
private:
	// 获取SVM_node得到一个样本的特征
	svm_node* SetSVM_node(double *characteristic,int characteristic_Num);
	// 存储样本所有特征
	svm_node** m_SVMnode;
public:
	// 获得整个样本的特征
	svm_node** SetSVMNodes(double** data, int row , int line);
	svm_node** GetSVMNodes();
	int m_SVMnode_row;
private:
	// 注意多了比实际数据多了一个维
	int m_SVMnode_line;
public:
	// 存储本次参加运算的所有样本（数据集）
	svm_problem GetSVMproblem(double* SampleClassId);
	// 设置SVM参数
	void SetSVMparameter(	int svm_type,//SVM类型，见前enum
		int kernel_type,//核函数
		double degree,	/* for poly */ 
		double gamma,	/* for poly/rbf/sigmoid */
		double coef0,	/* for poly/sigmoid */

		/* these are for training only */
		double cache_size, /* in MB */
		double eps,	/* stopping criteria */
		double C,	/* for C_SVC, EPSILON_SVR and NU_SVR */
		int nr_weight,		/* for C_SVC */
		int *weight_label,	/* for C_SVC */
		double* weight,		/* for C_SVC */
		double nu,	/* for NU_SVC, ONE_CLASS, and NU_SVR */
		double p,	/* for EPSILON_SVR */
		int shrinking,	/* use the shrinking heuristics */
		int probability /* do probability estimates */
		);
private:
	// 存储SVM参数
	svm_parameter m_svmparameter;
	// svm参数
	svm_parameter m_svm_parameter;
public:
	// 获得参数
	svm_parameter GetSVMparameter(void);
private:
	// 记录训练数据的输入类别
	double *m_sampleClassId;
public:
	double * GetSampleTargetClassId(void);
	// 清除数据
	void DataCleaner(void);
private:
	svm_node* m_svmnode_forone;
};

