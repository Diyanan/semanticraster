#pragma once
#include "ClassifiedData.h"
#include "svm.h"
#include "GLC_ann_classifier.h"
#include "GLCann_classifier.h"
#include <vector>

enum { RISC_CTree=1, RISC_SVM=2, RISC_ANN=3, RISC_KNN=4, RISC_Bayes=5 };	/* 此处为基分类器的类型 */
using std::vector;
//adboosting树中
#include "ClassifiedData.h"
struct TuplesClassified  //存储分类后元组信息，可以存在于叶子节点上
{ 
	int i;                 
	int sampleDataIndex; 
	int tuplesClassID;   
	double weight;       
	int err;             
	double pre;
	ClassifiedData * clssified_data;


};
struct TreeNode  //树节点
{ 
	int attIndex;      
	double splitVertex;
	TreeNode *leftChild;
	TreeNode *rightChild;
	bool isLeaf;        
	int ClassID;        
	int publicClassN; 
	int allN;          
	int publicClassID; 
	TuplesClassified * tupInformation; 
	int errors;
	double pre;
};
struct TreeSelection  //存储决策树信息
{ 
	TreeNode * Tree;	
	double weight;
	double erroratio;
	int leafs;
	int rules;
	int ruleeSetRow;
	
	double erroratio_test;
	int publicClssId;
};
 struct AttIndexTab  //用于记录属性索引及其对应的熵值，信息增益，信息增益率
{ 
	int attIndex;    
	double attInf;
	double attGain;
	double attGainRatio;
	double splittPoint;
	int D1,D2;         

};
struct InfoAttTab
{ 
	double Info;        
	double Weight;

};
 struct ClassUnique
{  

	int classID;        
	int cunt;
	ClassUnique * next;
};
 struct AttValuesTab
{  
	double AttValue;
	int classID;       
	int AttValueIndex;
};
 struct statisticsTab
{
	int valueID;
	int valueCount;
	double Proportion;
};
///////////////////////

inline 	unsigned __int64 GetCycleCount(	void)  

{
	_asm _emit 0x0f;  
	_asm _emit 0x31;   

}   

class Timer  

{
	unsigned __int64 m_startCycle;
	unsigned __int64 m_overhead; 
public:  
	Timer() : m_overhead(0)
	{ 
		Start(); 
		m_overhead = Stop(); 
	}
	void Start(	void)
	{  
		m_startCycle = GetCycleCount(); 
	}  
	unsigned __int64 Stop(void)  
	{  
		return	GetCycleCount() - m_startCycle - m_overhead;   
	}   

}; 

//typedef struct TuplesClassified  //存储分类后元组信息，可以存在于叶子节点上
//{ 
//	int i;                 
//	int sampleDataIndex; 
//	int tuplesClassID;   
//	double weight;       
//	int err;             
//	double pre;
//	ClassifiedData * clssified_data;
//
//
//
//};
//typedef struct TreeNode  //树节点
//{ 
//	int attIndex;      
//	double splitVertex;
//	TreeNode *leftChild;
//	TreeNode *rightChild;
//	bool isLeaf;        
//	int ClassID;        
//	int publicClassN; 
//	int allN;          
//	int publicClassID; 
//	TuplesClassified * tupInformation; 
//	int errors;
//	double pre;
//};

//typedef struct AttIndexTab  //用于记录属性索引及其对应的熵值，信息增益，信息增益率
//{ 
//	int attIndex;    
//	double attInf;
//	double attGain;
//	double attGainRatio;
//	double splittPoint;
//	int D1,D2;         
//
//};
//typedef struct InfoAttTab
//{ 
//	double Info;        
//	double Weight;
//
//};
//typedef struct ClassUnique
//{  
//
//	int classID;        
//	int cunt;
//	
//	ClassUnique * next;
//};
typedef struct ClassUnique1
{  

	int classID;        
	int cunt;
	vector<int> dataIndex;//记录该类在类标签数组中的索引
	ClassUnique1 * next;
};
//typedef struct AttValuesTab
//{  
//	double AttValue;
//	int classID;       
//	int AttValueIndex;
//};
//typedef struct statisticsTab
//{
//	int valueID;
//	int valueCount;
//	double Proportion;
//};
struct ConfusionMatrix//用于记录混淆矩阵
{  
	int classifierID;
	int nClass;     //类别个数
	int **matrix; //混淆矩阵
	float **acryMatrix;//分类器识别性能矩阵
	float **confidenceMatrix;//分类器信任度矩阵
};
//树参数
typedef struct GLCTreeParameter  
{
	int TreeHeight;
	int LeatfSize;
	
}TreeParameter;
//struct glc_ann_parameter
//{	
//	int connectinRate;                  //链接参数
//	float learningRate;             //学习率
//	float desiredError;            //终止错误率
//	int iterationBetweenRepots;       //报告间隔
//	int maxepoches;                  //最大迭代次数
//	int layes;                          //网络的层数
//	int hidNum;                        //隐藏神经元的个数
//	int inNum;	
//	int outNum;                       //输出单元的个数
//};