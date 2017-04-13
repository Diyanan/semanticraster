#pragma once
#include "GLC_Classifier.h"//#include "svm.h"
class DataFilling
{
public:
	DataFilling(double** data, int row , int line);
	~DataFilling(void);
private:
	// ��ȡSVM_node�õ�һ������������
	svm_node* SetSVM_node(double *characteristic,int characteristic_Num);
	// �洢������������
	svm_node** m_SVMnode;
public:
	// �����������������
	svm_node** SetSVMNodes(double** data, int row , int line);
	svm_node** GetSVMNodes();
	int m_SVMnode_row;
private:
	// ע����˱�ʵ�����ݶ���һ��ά
	int m_SVMnode_line;
public:
	// �洢���βμ�������������������ݼ���
	svm_problem GetSVMproblem(double* SampleClassId);
	// ����SVM����
	void SetSVMparameter(	int svm_type,//SVM���ͣ���ǰenum
		int kernel_type,//�˺���
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
	// �洢SVM����
	svm_parameter m_svmparameter;
	// svm����
	svm_parameter m_svm_parameter;
public:
	// ��ò���
	svm_parameter GetSVMparameter(void);
private:
	// ��¼ѵ�����ݵ��������
	double *m_sampleClassId;
public:
	double * GetSampleTargetClassId(void);
	// �������
	void DataCleaner(void);
private:
	svm_node* m_svmnode_forone;
};

