#pragma once
#include "structs.h"
#include "ClassifiedData.h"
#include "svm.h"
#include "GLC_ann_classifier.h"
#include "GLCann_classifier.h"
#include <vector>

using std::vector;

//inline 	unsigned __int64 GetCycleCount(	void)  
//
//{
//	_asm _emit 0x0f;  
//	_asm _emit 0x31;   
//
//}   
//
//class Timer  
//
//{
//	unsigned __int64 m_startCycle;
//	unsigned __int64 m_overhead; 
//public:  
//	Timer() : m_overhead(0)
//	{ 
//		Start(); 
//		m_overhead = Stop(); 
//	}
//	void Start(	void)
//	{  
//		m_startCycle = GetCycleCount(); 
//	}  
//	unsigned __int64 Stop(void)  
//	{  
//		return	GetCycleCount() - m_startCycle - m_overhead;   
//	}   
//
//}; 
//
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
//
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
//typedef struct ClassUnique1
//{  
//
//	int classID;        
//	int cunt;
//	vector<int> dataIndex;//记录该类在类标签数组中的索引
//	ClassUnique1 * next;
//};
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
//struct ConfusionMatrix//用于记录混淆矩阵
//{  
//	int classifierID;
//	int nClass;     //类别个数
//	int **matrix; //混淆矩阵
//	float **acryMatrix;//分类器识别性能矩阵
//	float **confidenceMatrix;//分类器信任度矩阵
//};
////树参数
//typedef struct GLCTreeParameter  
//{
//	int TreeHeight;
//	int LeatfSize;
//	
//}TreeParameter;

class  GLC_Classifier
{
public:
	GLC_Classifier(void);
	GLC_Classifier(double ** data,int data_row,int data_line);
	~GLC_Classifier(void);
	void GetData(double ** data,int data_row,int data_line)
	{
		m_data=data;
		m_data_row=data_row;
		m_data_line=data_line;
	}
	virtual void SetParameter(void)                                     //设置参数，要三次重载，参数用结构体
	{

	}
	virtual void SetParameter(TreeParameter treeparameter)                                    //设置参数，要三次重载，参数用结构体
	{
		//m_data_row=1222;
	}
	virtual void SetParameter(svm_parameter* svmparameter)
	{


	}
	virtual void SetParameter(glc_ann_parameter* annparameter)
	{
		

	}
	virtual void SetData() //获得分类数据
	{

	}
	virtual void ExecutTrain(int & progressi)   //执行训练
	{
		
	}
	virtual void ExecutTrain()                  //执行训练
	{

	}
	virtual void GetModle(TreeNode * tree) //获得树模型
	{

	}
    virtual void GetModle(svm_model * svmmodle)    //获得SVM模型
   {

   }
   virtual void GetModle(FANN::neural_net* svmmodle)      //获得ANN模型
   {
   }
	virtual void SetModle(TreeNode * tree) //设置树模型，可用于已经生成的模型来进行分类
	{

	}
	
	virtual void SetModle(svm_model * svmmodle) //设置SVM模型，可用于已经生成的模型来进行分类
	{

	}
	virtual void SetModle(FANN::neural_net* svmmodle)
	{

	}
	virtual void GetModle()
	{

	}
	virtual short* ExecutClassify(double **data,int data_row,int data_line,int & progressi)   //执行分类
	{
		return NULL;
	}
	virtual short ExecutClassify(double *data,int data_row,int data_line)                     //执行分类
	{
		return 0;
	}
	virtual short *GetTest(double **data=NULL,int data_row=0,int data_line=0)                 //获得测试结果
	{
		return NULL;
	}
	virtual void GetAccuracyEvaluation(void)                           //获得精度评价
	{

	}
	void ExecutClassify_clean()                                  //执行分类
	{
		if (m_classifiedresultID!=NULL)
		{
			delete [] m_classifiedresultID;
		}
	}
	virtual bool DestroyModel()                                  //删除模型
	{
		return true;
	}

	void GetTest_clean()                                       //获得测试结果
	{
		if (m_testclassID!=NULL)
		{
			delete [] m_testclassID;
		}

	}


private:
	

	virtual void DataFormat(void)
	{

	}
protected:
	double **m_data;int m_data_row,m_data_line;//获得分类数据
//	short* m_testclassID;
	
	//short* m_testclassID;
	short* m_testclassID;
	short* m_classifiedresultID;
};
