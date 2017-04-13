
#pragma once
#include "Tree.h"
class  GLCTreeClassifier
{
public:
	GLCTreeClassifier(void);
	~GLCTreeClassifier(void);
	// ����ѵ����
	void CreateTrainingSet(double** trainData,int trainData_row,int trainData_line,/* CProgressCtrl* progressBar,*/int &progressBarI);//�����·����������
	//void CreateTrainingSet(double** trainData,int trainData_row,int trainData_line, /*CProgressCtrl* progressBar,*/int &progressBarI,CString & Eror);//��������
	void CreateTrainingSet(double** trainData,int trainData_row,int trainData_line);//����·��������
private:
	// ��¼ѵ�����Ĵ��·��
	//CString m_trainSet_path;
public:
	//��ÿ��Ʋ���
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio);
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize);
private:
	// �����������ĸ�
	int m_treeHeight;
	// ��������
	int adaBoostingK;
	// Ҷ�Ӵ�С
	int m_leafsSize;
	// ��������
	int m_adaBoostingK;
	// ������
	double m_sampleDataRatio;
	// ���ƽ�����
	//CProgressCtrl* m_progressBar;
public:
	// ִ�з��෵�ط�����
	short* ExecuteClassify(double** data, int row, int line);
	//short * ExecuteClassify(double** data, int row, int line,CProgressCtrl* progressBar,int &progressBarI,short * classifiedId=NULL);
private:
	// ѵ��������
	double** m_trainData;
	int m_trainData_row;
	int m_trainData_line;
	// ��¼���ɵ�����
	TreeSelection *m_treeSelct;
	TreeSelection* m_TreeSelection;
public:
	// �������
	TreeSelection* GetTreeSelection(void);
	// ʹ�õ���ֵԤ��ķ��෽��
	short* ExcuteClassifySingle(double** data, int data_row, int data_line,int & progresI);
	// ִ��ѵ��
	bool GLCTree_Execute_Train(double* data, int data_row, int dtat_line);
};

