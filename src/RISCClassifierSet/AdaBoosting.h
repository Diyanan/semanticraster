#pragma once
#include "Tree.h"

class AdaBoosting
{
public:
	AdaBoosting(void);
	~AdaBoosting(void);
	
	
private:
	int m_treeHeight;
	int m_Dimension0;
	int m_Dimension1;
	int m_Dimension2;
	int m_Dimension;
	

	// 用于计算树高
	int m_k;
	// 记录最佳索引
	int m_splitPointAttIndex;
	// 记录元组信息的数量
	int m_tuplesNum;

	// 记录整个样本
	double** m_sampleDatas;
	double** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	// 样本数据行
	int m_sampleDatasRow;
	// 样本数据列
	int m_sampleDatasLine;
	// 样本数据索引集行
	int m_tuplesRow;
	// 样本数据索引集列
	int m_tuplesLine;
	// 样本数据索引集行
	int m_sampleDataIndexRow;
	// 样本数据索引集列
	int m_sampleDataIndexLine;
	// 获得属性集,attD表示属性集的维数
	int ** GetAttIndexList(int attNum, int attD);
	// 属性集
	int** m_attListIndex;
	// 删除属性集
	void DeletAttIndexList(int ** attIndexList, int attM , int attN);
	// 累计元组个数
	int m_tuplesI;
	// 轮盘赌函数
	int GetChromoRoulette(TuplesClassified* tuples, int tuplesN);
	// 删除新建的样本数据集
	void DeleteSamplingReplacement(TuplesClassified* tuples);
	// 提升算法迭代次数
	int m_adaBoostingK;

	
	int GetClassRules(TreeNode* root);
	int m_stackForNodeRulesTop;


	// 用中序遍历树的方法测试精度，被函数MeasuringAccuracy调用
	void InfoOrderForMeasuring(TreeNode* root,TuplesClassified* MeasuringTuples,int tuplesNum);
	// 测试元组
	TuplesClassified* m_measuringTuples;
	// 决策树模型集
	TreeSelection* m_treeSelection;
	// 获得一定范围内不重复的随机数
	int NonredundantRandom(int leftLimit, int rightLimit, int randomN);
	// 需要的互相不重复随机数的个数
	int m_randomNum;
	// 记录随机数组
	int* m_random;
	// 样本率
	double m_sampleDataRatio;
	// 判断条件的个数
	int m_ruleCount;
	// 记录最终得到的规则集
	//CString* m_ruleSet;
	// 计数器
	int m_ruleSetI;
	// 规则集文件路径
	//CString m_ruleFilePath;
	// 训练元组的个数
	int m_tringDataN;
	int* m_quede;
	int m_leafsSize;
public:
	int m_NodeSize;
	// 所有叶子数据总和
	int m_allDatas;
	// 多数类总数
	int m_publics;
	// 错误总数
	int m_allErros;
	int m_erroNum;
	TuplesClassified *m_tuplesInformation;
	// 初始化记录元组信息的数组
	void TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/);
	// 清除记录元组信息的数组
	void TuplesInformationDelete(TuplesClassified *tuples);
	// 进行AdaBoosting提升
	void doAdaBoosting(TuplesClassified * tuples, int tuplesN, int k,int & progressI);
	// 有放回抽样，返回样本
	TuplesClassified* SamplingReplacement(TuplesClassified* tuples, int tuplesN);
	// 获得整个样本数据
	double** GetSampleDatas(double** sampledates, int m , int n);
	// 获得样本数据索引集
	int ** GetSampleDataIndex(TuplesClassified* tuples,int tuplesN);
	// 删除样本索引集
	void DeleteSampleDataIndex(TuplesClassified* tuples);
	// 初始化样本数据，保存数据，属性集，等信息
	void SampleDataInitialization(double ** sampleData, int row, int line);
	// 保存从界面上获得的控制参数
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio);
	//计算错误个数
	int CalculateErrors(TreeNode* root, double ** SampleDatas, int m , int n,TuplesClassified* tuples,int tuplesN);
	//计算错误率
	double ErroRatioForModel(TuplesClassified * tuplesInfmation, int tuplesNum);
	// 初始化测试数据
	void TestDataInitialization(double** testData , int row , int line);

	// 用于将抽样得到的样本元组替换原来的
	void TuplesInfoReplacement(TuplesClassified* tuples, int tuplesN);

	void WeightNormalization(TuplesClassified* tuples, int tuplesN,double errorRatio);
	void ReplaceWeightOnTupleList(TuplesClassified* Di, int tuplesN);
	void TuplesWeightInitialization(TuplesClassified* tuples, int tuplesN);
	TreeSelection* GetTreeSelection(TreeSelection* treeSelections, int treeN);

	void DeleteTreeSelection(TreeSelection* treeSelections);
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI);//有进度条
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum);
	// 样本数据分割为训练集和测试集
	void SampleDataDataPartition(double** sampleData, int row, int line);
	// 产生不重复随机数
	int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
	// 返回复合模型，该函数可以用来重载
	TreeSelection * GetCompoundModel(void);
	//获得数据
	void GetData(double ** sampleData, int row, int line,int ** attIndex);
	void run(int & progressI);


};

