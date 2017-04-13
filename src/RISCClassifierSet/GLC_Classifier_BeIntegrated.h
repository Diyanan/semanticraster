#pragma once
#include "svm.h"
#include "GLC_Tree.h"
#include "GLC_Knn.h"
#include "GLC_SVM.h"
#include "GLC_ANN.h";
#include "GLC_Bayes.h"
#include "GLC_Class_Accuracy.h"

/*#include <string> 
using namespace std;*/ 
#include <vector>

using std::vector;

//typedef struct glc_intg_expert  //分类专家
//{ 
//	int index;
//	int classID;
//	float accuracy;
//	vector<int> expertClassifierIndex;
//}Expert;
//typedef struct glc_intg_classUniuq  
//{ 
//	int index;
//	int classID;
//	int nClassSample;//该类下的个数
//	double weight;
//}Uniuq;

typedef struct classUniuqStruct  
{ 
	int index;
	int classID;
	int nClassSample;//该类下的个数
	double weight;
}UniuqStruct;
class GLC_Classifier_BeIntegrated
{
public:
	GLC_Classifier_BeIntegrated(void);
	~GLC_Classifier_BeIntegrated(void);
	// 获得数据
	void Integrated_Set_Data(double** data, int data_row , int data_line);

private:
	// 数据
	double** m_data;
	int m_data_row;
	int m_data_line;
	//测试数据
	double** m_testdata;
	int m_testdata_row;
	int m_testdata_line;
	int * m_data_tag;
public:
	// 设置参数
	void Integrated_Set_Parameter(TreeParameter treeparameter,svm_parameter svmparameter,glc_ann_parameter annparameter);
private:
	TreeParameter m_treeparameter;
	svm_parameter m_svmparameter;
	glc_ann_parameter m_annparameter;
public:
	// 执行分类
    bool Integrated_Execut_Train(int& progressI,double **testdata=NULL,int testedata_row=0,int testedata_line=0,int intTyp=0);
	
	short Integrated_Execut_Classify_INT1(double* data, int data_line,int* classID_selection=NULL);

	short Integrated_Execut_Classify_INT2(double* data, int data_line,int* classID_selection=NULL);

	
private:
	// 树分类器
	GLC_Tree m_classifier_tree;
	// svm分类器
	GLC_SVM m_classifier_svm;
	// ann分类器
	GLC_ANN m_classifier_ann;
	//knn分类器
	GLC_Knn m_classifier_knn;
	//Bayes分类器
	GLC_Bayes m_classifier_Bayes;
	// 分类器
	GLC_Classifier* m_classifier;
	// 记录树模型
	GLC_Tree* m_model_tree;
	// 记录svm
	svm_model* m_model_svm;
	//记录 ann模型
	FANN::neural_net* m_modle_ann;
	//记录knn模型
	GLC_Knn* m_modle_knn;
	//记录bayes模型
	GLC_Bayes* m_modle_bayes;
	// 记录每次的验证结果用于获取类别精度矩阵
	short** m_calss_acryMatix_input;
	// 类别矩阵
	float** m_class_acuracymatrix;
	// 记录类别精度的类
	GLC_Class_Accuracy m_class_accuracy;
	GLC_Class_Accuracy m_class_accuracy1;
public:
	


	// 类别的个数
	int m_class_num;

private:
	
	ConfusionMatrix* m_cofusionMatrix;
	vector<ConfusionMatrix*> m_cofusionMatrix_selection;
	ConfusionMatrix* m_cofusionMatrix1;

	float* m_child_classifier_accuracy;
	float ** m_ClassAcryMatrix;

private:
	
public:
	// 记录各个分类器中总体精度最小值
	double m_minAccuracy_classifier;
	// 使用测试数据分类之后得到的类标号
	short* m_test_classified_IDs;
private:
	bool m_istest;
public:
	
	double m_avgAccuracy_classifier;
	
	

	
private:
	// 分类器的个数
	int m_nClassifier;
	// 记录分类器的编号
	int* m_classifierIndex;		


};

