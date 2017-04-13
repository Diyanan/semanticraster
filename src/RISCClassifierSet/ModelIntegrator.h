#pragma once
#include "adaboosting.h"
class  ModelIntegrator
{
public:
	ModelIntegrator(void);
	~ModelIntegrator(void);

public:
	
//	void AdaBoostingTrainingRoles1(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI,CString & Eror);//��������
	void AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI);
	
	void SampleDataDataPartition(double** sampleData, int row, int line);

	int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
	//��ȡ����
	//void  GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath);
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio);//���ù����ļ�
private:

	int NonredundantRandom(int leftLimit, int rightLimit, int randomN);

	int m_randomNum;

	int* m_random;

	double m_sampleDataRatio;
	double** m_sampleDatas;
	int m_sampleDatasRow;
	int m_sampleDatasLine;
	int** m_attListIndex;

	double ** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	/////�������
	int m_treeHeight;
	//ʹ�õ�������
	int m_adaBoostingK;
	int m_leafsSize;

	// �����ļ�·��
	//CString m_ruleFilePath;

public:
	

private:
	
	//bool InorderForRuleSet(TreeNode* root, int tag, double erroratio,CString stRules,int rulesN);
	
	int m_ruleCount;
	
	//CString* m_ruleSet;
	
	int m_ruleSetI;
	

	int m_tringDataN;
	int* m_quede;
	
	AdaBoosting m_adaBoosting;
	//Ԫ����Ϣ
	TuplesClassified *m_tuplesInformation;
	
	int m_tuplesNum;
	
	//ģ�ͼ�
	TreeSelection*	m_treeSelection;

	void TestDataInitialization(double** testData , int row , int line);

	void SampleDataInitialization(double ** sampleData, int row, int line);

	void TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/);
public:
	/*void OutputRuleSet(CString rulesPath);
	void OutputRuleSet(CString rulesPath,int & ruleRow);
	
	CString* GetRuleSet(TreeSelection * trees, int treesN);
	*/
   // double* GetMeasuringAccuracy_separately(int tuplesN/*�������ݵĸ���*/);
private:
	/*TuplesClassified* MeasuringAccuracyMatch(TuplesClassified* MeasuringTuples,int tuplesNum,CString * rules,int rulsN);
	double MeasuringAccuracy(TuplesClassified* MeasuringTuples,int tuplesNum,CString * rules,int rulsN);
	*/
	/*CString * doReorderToRuleSet(CString * oldRuleSet, int rulesSetRow);*/
	
	double** m_testDetas;
	//void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified* tuples, int tuplesN,CString path);
public:
	// ���������������
	//CString* GetRuleSetAll(void);
	//// ��õ������Ĺ���
	//CString* GetRuleSet_SingleTree(TreeSelection * trees, int treei/*���ı��*/);
	//CString* GetRuleSet_SingleTree(TreeSelection * trees, int treei/*���ı��*/,int & ruleCount);
	// ��ӡ���������Ĳ��Ծ���
	//void OuntPutMeasuringAccuracy_separately(CString path);
	//void PrintTuples(TuplesClassified * tuples, int tuplesN, CString path);
	//void PrintRules(CString * rules, int rulesN , CString path);
private:
	// ����
	//TuplesClassified* Classify(TuplesClassified* T, int N , CString* rule,int ruleN,int &rightTuples/*��ȷ����ĸ���*/,int calssId);
	//������


public:
	// ���ý�����
	//void SetProgressBar(CProgressCtrl* progressBar);
private:
	// ����������
	//CProgressCtrl* m_progressBar;
	// ���ȿ���
	//int& m_progressI;
	int m_progressITemp;
public:
	//�������еؽ����򼯴洢�������У�
	//CString* GetRuleSet1by1(TreeSelection * trees/*���ɵ�����*/,int & ruleSetRow);
	// �������
	 TreeSelection * GetTreeSelection(int & treeSum);
	// ��ȡ��������
	double** GetTestData(int& testDataRow,int & testDataLine);
	double** GetSampleData(void);
};

