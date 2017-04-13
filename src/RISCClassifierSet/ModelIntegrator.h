#pragma once
#include "adaboosting.h"
class  ModelIntegrator
{
public:
	ModelIntegrator(void);
	~ModelIntegrator(void);

public:
	
//	void AdaBoostingTrainingRoles1(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI,CString & Eror);//无样本率
	void AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI);
	
	void SampleDataDataPartition(double** sampleData, int row, int line);

	int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
	//获取参数
	//void  GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath);
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio);//不用规则集文件
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
	/////传入参数
	int m_treeHeight;
	//使用迭代次数
	int m_adaBoostingK;
	int m_leafsSize;

	// 规则集文件路径
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
	//元组信息
	TuplesClassified *m_tuplesInformation;
	
	int m_tuplesNum;
	
	//模型集
	TreeSelection*	m_treeSelection;

	void TestDataInitialization(double** testData , int row , int line);

	void SampleDataInitialization(double ** sampleData, int row, int line);

	void TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/);
public:
	/*void OutputRuleSet(CString rulesPath);
	void OutputRuleSet(CString rulesPath,int & ruleRow);
	
	CString* GetRuleSet(TreeSelection * trees, int treesN);
	*/
   // double* GetMeasuringAccuracy_separately(int tuplesN/*测试数据的个数*/);
private:
	/*TuplesClassified* MeasuringAccuracyMatch(TuplesClassified* MeasuringTuples,int tuplesNum,CString * rules,int rulsN);
	double MeasuringAccuracy(TuplesClassified* MeasuringTuples,int tuplesNum,CString * rules,int rulsN);
	*/
	/*CString * doReorderToRuleSet(CString * oldRuleSet, int rulesSetRow);*/
	
	double** m_testDetas;
	//void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified* tuples, int tuplesN,CString path);
public:
	// 获得整个规则数组
	//CString* GetRuleSetAll(void);
	//// 获得单个树的规则
	//CString* GetRuleSet_SingleTree(TreeSelection * trees, int treei/*树的编号*/);
	//CString* GetRuleSet_SingleTree(TreeSelection * trees, int treei/*树的编号*/,int & ruleCount);
	// 打印出各个树的测试精度
	//void OuntPutMeasuringAccuracy_separately(CString path);
	//void PrintTuples(TuplesClassified * tuples, int tuplesN, CString path);
	//void PrintRules(CString * rules, int rulesN , CString path);
private:
	// 分类
	//TuplesClassified* Classify(TuplesClassified* T, int N , CString* rule,int ruleN,int &rightTuples/*正确分类的个数*/,int calssId);
	//进度条


public:
	// 设置进度条
	//void SetProgressBar(CProgressCtrl* progressBar);
private:
	// 进度条控制
	//CProgressCtrl* m_progressBar;
	// 进度控制
	//int& m_progressI;
	int m_progressITemp;
public:
	//逐行逐行地将规则集存储于数组中，
	//CString* GetRuleSet1by1(TreeSelection * trees/*生成的树集*/,int & ruleSetRow);
	// 获得树集
	 TreeSelection * GetTreeSelection(int & treeSum);
	// 获取测试数据
	double** GetTestData(int& testDataRow,int & testDataLine);
	double** GetSampleData(void);
};

