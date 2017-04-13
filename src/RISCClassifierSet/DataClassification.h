#pragma once
#include "ClassifiedData.h"
#include "Tree.h"
//typedef struct TuplesClassified  //存储分类后元组信息，可以存在于叶子节点上
//{ 
//	int i;                //元组的序号 
//	int sampleDataIndex; //元组在样本数据中的索引
//	int tuplesClassID;   //元组的类别
//	double weight;       //元组权重
//	int err;             //如果元组误分类，则err=1,否则为0；
//	double pre;
//
//};
//typedef struct TreeNode  //树节点
//{ 
//	int attIndex;     //属性在样本中的索引索引值 
//	double splitVertex;//分裂点
//	TreeNode *leftChild;//左孩子节点
//	TreeNode *rightChild;//右孩子节点
//	bool isLeaf;         //判断是否是叶子节点
//	int ClassID;        //当是叶子节点时判断是哪一类
//	int publicClassN;  //当前节点的最多数类数据个数、
//	int allN;          //当前节点的数据集总数
//	int publicClassID;  //记录当前节点的多数类
//	TuplesClassified * tupInformation; //记录叶子节点上元组的分类信息
//	int errors;//当为叶子节点时记录该节点错误分类的个数
//};
//typedef struct TreeSelection  //存储决策树信息
//{ 
//	TreeNode * Tree;	
//	double weight;
//	double erroratio;
//	int leafs;
//	int rules;//树下的规则数目
//	int ruleeSetRow;//规则集的行数
//	CString * rulset;
//	double erroratio_test;
//	int publicClssId;
//};

class DataClassification
{
public:
	DataClassification(double **data,int row,int line);
	DataClassification(void);
	~DataClassification(void);
private:
	// 存储数据
	double** m_data;
	// 数据行
	int m_dataRow;
	// 数据列
	int m_dataLine;
	// 数据元组，记录元组信息
	TuplesClassified* m_data_Tuples;
public:
	// 执行分类
	//ClassifiedData* doClassify(CString * rules,int rulsRow);
	//ClassifiedData* doClassify(TreeSelection * treeSelection,int treeSum);
	////ClassifiedData* doClassify(TreeSelection * treeSelection,int treeSum,CString & Eror);
	//ClassifiedData* doClassify();
	//预测单个数据
	bool  DataClassification::Tree_Classfy_1(TreeNode* root,double * classifydata,int &classID);
private:
	// 对元组分类
	//TuplesClassified* TuplesClassify(TuplesClassified* Tuples,int tuplesNum,CString * rules,int rulsN);
	//获得单棵树规则
	//CString* GetRuleSet_SingleTree(TreeSelection * trees, int treei/*树的编号*/);
	//bool InorderForRuleSet(TreeNode* root, int tag, double erroratio,CString stRules,int rulesN);
	// 规则存储数组
	//CString* m_ruleSet;
	// 规则数
	int m_ruleSetI;
	int m_ruleCount;
private:
	// 加权投票表决
	int WeightedVoting(ClassifiedData dataWaitingVote);
	// 交换
	void Swap(ClassifiedResutEveryTime  * a , ClassifiedResutEveryTime  * b);
	//void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified* tuples, int tuplesN,CString path);
	//void PrintTuples(TuplesClassified * tuples, int tuplesN, CString path);
public:
	bool ClassfyWithTree(TreeNode* root, double erroratio,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI);
	bool ClassfyWithTree1(TreeNode* root, double erroratio,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI);
	bool ClassfyWithTree2(TreeNode* root, double erroratio,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI,int tree_NO,int& isRoot);
private:
	//void Print(CString path,ClassifiedData data);
	ClassifiedData* m_classifideData;
public:
	// 获取分类之后的数据的类别，按照数据的序号返回
	short* GetClassifiedID(void);
private:
	// 记录分类数据类
	ClassifiedData *m_classData;
public:
	// 清除数据
	void Delete(void);
	// 执行分类
	short doClassifySingle(double* data,TreeSelection *treeSelection,int treenum);
};
void quicksort(TuplesClassified a[], int left, int right);
int partition(TuplesClassified data[],int lo,int hi);
void QuickSort(TuplesClassified a[], int lo, int hi);
