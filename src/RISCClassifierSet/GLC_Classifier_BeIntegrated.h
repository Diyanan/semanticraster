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

//typedef struct glc_intg_expert  //����ר��
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
//	int nClassSample;//�����µĸ���
//	double weight;
//}Uniuq;

typedef struct classUniuqStruct  
{ 
	int index;
	int classID;
	int nClassSample;//�����µĸ���
	double weight;
}UniuqStruct;
class GLC_Classifier_BeIntegrated
{
public:
	GLC_Classifier_BeIntegrated(void);
	~GLC_Classifier_BeIntegrated(void);
	// �������
	void Integrated_Set_Data(double** data, int data_row , int data_line);

private:
	// ����
	double** m_data;
	int m_data_row;
	int m_data_line;
	//��������
	double** m_testdata;
	int m_testdata_row;
	int m_testdata_line;
	int * m_data_tag;
public:
	// ���ò���
	void Integrated_Set_Parameter(TreeParameter treeparameter,svm_parameter svmparameter,glc_ann_parameter annparameter);
private:
	TreeParameter m_treeparameter;
	svm_parameter m_svmparameter;
	glc_ann_parameter m_annparameter;
public:
	// ִ�з���
    bool Integrated_Execut_Train(int& progressI,double **testdata=NULL,int testedata_row=0,int testedata_line=0,int intTyp=0);
	
	short Integrated_Execut_Classify_INT1(double* data, int data_line,int* classID_selection=NULL);

	short Integrated_Execut_Classify_INT2(double* data, int data_line,int* classID_selection=NULL);

	
private:
	// ��������
	GLC_Tree m_classifier_tree;
	// svm������
	GLC_SVM m_classifier_svm;
	// ann������
	GLC_ANN m_classifier_ann;
	//knn������
	GLC_Knn m_classifier_knn;
	//Bayes������
	GLC_Bayes m_classifier_Bayes;
	// ������
	GLC_Classifier* m_classifier;
	// ��¼��ģ��
	GLC_Tree* m_model_tree;
	// ��¼svm
	svm_model* m_model_svm;
	//��¼ annģ��
	FANN::neural_net* m_modle_ann;
	//��¼knnģ��
	GLC_Knn* m_modle_knn;
	//��¼bayesģ��
	GLC_Bayes* m_modle_bayes;
	// ��¼ÿ�ε���֤������ڻ�ȡ��𾫶Ⱦ���
	short** m_calss_acryMatix_input;
	// ������
	float** m_class_acuracymatrix;
	// ��¼��𾫶ȵ���
	GLC_Class_Accuracy m_class_accuracy;
	GLC_Class_Accuracy m_class_accuracy1;
public:
	


	// ���ĸ���
	int m_class_num;

private:
	
	ConfusionMatrix* m_cofusionMatrix;
	vector<ConfusionMatrix*> m_cofusionMatrix_selection;
	ConfusionMatrix* m_cofusionMatrix1;

	float* m_child_classifier_accuracy;
	float ** m_ClassAcryMatrix;

private:
	
public:
	// ��¼���������������徫����Сֵ
	double m_minAccuracy_classifier;
	// ʹ�ò������ݷ���֮��õ�������
	short* m_test_classified_IDs;
private:
	bool m_istest;
public:
	
	double m_avgAccuracy_classifier;
	
	

	
private:
	// �������ĸ���
	int m_nClassifier;
	// ��¼�������ı��
	int* m_classifierIndex;		


};

