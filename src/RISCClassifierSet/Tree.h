#pragma once
//#include "glcstruct.h"
#include "structs.h"
using namespace std;
using std::vector;

class Tree
{
public:
	Tree(void);
	~Tree(void);


	double Info(bool isContinues, double * AttValues, int ValuesCount);
	
	
	double * discreteData(AttValuesTab * AttValues, int AttValuesCount);
	
	double Info(statisticsTab* staTab, int staCount);
	
	double fineDiscPoint(double * discPointList, double * attValueList);
	
	
	AttValuesTab *m_AttValuesTable;
public:
	double findDiscPoint(double * discPoitList, int discNum, AttValuesTab * attValueslist,int attValuesNum,bool Tag);
	double findDiscPoint1(double * discPoitList, int discNum, AttValuesTab * attValueslist,int attValuesNum,int **attVlueList,int attN,int attL,bool Tag);
	
	ClassUnique * getClassUnique(AttValuesTab * attValues, int attNum);
	
	double InfoAtt(InfoAttTab * infoAttValues, int infoNum);
	
	double Gain(AttValuesTab * attValues, int attNum);
	double Gain1(AttValuesTab * attValues, int attNum,int **attVlueList,int attN,int attL);
	
	statisticsTab* AttValuesStatistics(ClassUnique* uniqueValuesLink);
	
	double GainRatio(AttValuesTab * attValues, int attNum);
	
	int ChooseAttribute(double ** sampleData, int row, int line, int * attIndex, int attNum);
	AttIndexTab ChooseAttribute1(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL);
	vector<AttIndexTab>  ChooseAttribute2(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL);
	TreeNode* BuildTree(double ** sampleData, int row, int line, int * attIndex, int attNum/*,TreeNode * root*/,bool outPut);
	void deleteClassUnique(ClassUnique *classLick);

	TreeNode* BuildTree1(double ** sampleData, int row, int line,int **sampDataIndex,int m,int n,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight);


	void InOrder(TreeNode * root);

	void RemoveTree(TreeNode* root);

	bool PostPrune(TreeNode* iNode);

	void test(double ** sampleData, int row, int line, int * attIndex, int attNum);
private:
	
	double m_splitPointValue;
	int m_treeHeight;
	int m_Dimension0;
	int m_Dimension1;
	int m_Dimension2;
	int m_Dimension;
public:
	int m_NodeSize;
private:
	
	int m_k;
	
	int m_splitPointAttIndex;
public:
	
	int CalculateErrors(TreeNode* root, double ** SampleDatas, int m , int n,TuplesClassified* tuples,int tuplesN);
	
	TuplesClassified * GetClassifiedTuples(TreeNode * root, int m);
	
	void traverse(TreeNode* root, TuplesClassified* T, int i);
	
	void Traverse(TreeNode* root,double ** SampleDatas, int m , int n,TuplesClassified * tuples,int tuplesN,bool isReplaceTuples/*是否更新元组信息，不用于计算错误率*/);
private:
	
	int m_erroNum;
	
	int m_bestSplitPointValue;
public:
	
	void getTreeHeight(int h);
	
	double ErroRatioForModel(TuplesClassified * tuplesInfmation, int tuplesNum);
private:
	
	TuplesClassified *m_tuplesInformation;

	void TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/);
public:
	
	void TuplesInformationDelete(TuplesClassified *tuples);
private:
	
	int m_tuplesNum;
	public:
	
	double** GetSampleDatas(double** sampledates, int m , int n);
private:
	
	double** m_sampleDatas;
	double** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	
	int m_sampleDatasRow;
	
	int m_sampleDatasLine;
public:
	
	int ** GetSampleDataIndex(TuplesClassified* tuples,int tuplesN);
	
	void DeleteSampleDataIndex(TuplesClassified* tuples);
private:
	
	int m_tuplesRow;
	
	int m_tuplesLine;
	
	int m_sampleDataIndexRow;
	
	int m_sampleDataIndexLine;
public:
	
	int ** GetAttIndexList(int attNum, int attD);
private:
	
	int** m_attListIndex;
	
	void DeletAttIndexList(int ** attIndexList, int attM , int attN);
public:
	
	void SampleDataInitialization(double ** sampleData, int row, int line);
private:
	
	int m_tuplesI;

	int GetChromoRoulette(TuplesClassified* tuples, int tuplesN);
	
	void DeleteSamplingReplacement(TuplesClassified* tuples);
	
	int m_adaBoostingK;
public:
	
	void GetTreeParameter(int treeHeight,int leafsSize);

	bool PostPruneEBP(TreeNode* root,TuplesClassified *tuples,int tuplesN);
private:
	
	int CalculateFactorial(int n);

	double CalculateConfidenceLLimit(int n, int e, double p);
	
	double ClaculateLeafPreErrorsADD(TreeNode* root);

	void TraverseChildTree(TreeNode * root, double LeafPreErrorsADD);
	
	double m_leafPreErrorsADD;

	int GetChildTreeHeight(TreeNode* root);
	
	int m_childTreeHeight;

	int m_childTreeLeafs;

	int m_childTreeNodes;

	double CalculatePreErrors(TreeNode* root,TuplesClassified * tuples,int tuplesN);
public:

	void DeleteTree(TreeNode* root);

private:
	
	
	int m_stackForNodeRulesTop;
	void InfoOrderForMeasuring(TreeNode* root,TuplesClassified* MeasuringTuples,int tuplesNum);
	
	TuplesClassified* m_measuringTuples;
public:
	
	
	
	int m_allDatas;
	
	int m_publics;
	
	int m_allErros;
private:
	
	void TuplesOrderByTree(TuplesClassified* tuples, int tuplesN);
public:
	
	TreeNode* BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL,bool outPut,int hight,TuplesClassified *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/,int & progressI/*用于计算进度*/);
	TreeNode* BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL,bool outPut,int hight,TuplesClassified *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/);
private:
	
	int m_leafsSize;
public:
	
	TreeSelection* GetTreeSelection(TreeSelection* treeSelections, int treeN);
	
	void DeleteTreeSelection(TreeSelection* treeSelections);
private:
	
	TreeSelection* m_treeSelection;

	
private:
	
	
	int m_ruleCount;
	
	
	int m_ruleSetI;
	
	int m_tringDataN;
	int* m_quede;

public:
	
	void GetData(double ** sampleData, int row, int line,int ** attIndex);
	
	void doBuildTree(TuplesClassified *tuples,int tuplesN,int & progressI);
	void doBuildTree(TuplesClassified *tuples,int tuplesN);
	
	TreeNode* GetTreeRoot(void);
private:
	
	TreeNode* m_treeRoot;
public:
	
	bool tagT;
	
	void doVariableToZero(void);
	
private:
	
	void SetValuesForTreeNode(TreeNode* treeNode,int tuplesN,TuplesClassified *tuples,double ** sampleData,int sampleData_line,int publicClassID);
};

