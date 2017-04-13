#pragma once
#include "glc_classifier.h"
class GLC_Tree :
	public GLC_Classifier
{
public:
	GLC_Tree(void);
	~GLC_Tree(void);
	 
     void SetParameter(TreeParameter treeparameter);                                     //设置参数，要三次重载，参数用结构体
	 void SetData(); //获得分类数据
	 void ExecutTrain(int & progressi);                                      //执行训练
	 void GetModle(TreeNode * tree);                                         //获得模型
	 TreeNode * ReturnModle();                                         //获得模型
	 void SetModle(TreeNode * tree); //设置模型，可用于已经生成的模型来进行分类
	 short* ExecutClassify(double **data,int data_row,int data_line,int & progressi);                                   //执行分类
	 short ExecutClassify(double *data);                                                 //执行分类
	 short *GetTest(double **data=NULL,int data_row=0,int data_line=0);                                       //获得测试结果
	 void GetAccuracyEvaluation(void);                           //获得精度评价
     bool DestroyModel();   //删除模型
	 ClassUnique *getClassUnique(AttValuesTab * attValues, int attNum);
	 void deleteClassUnique(ClassUnique *classLick);
private:
	void DataFormat(void);
	// ***************************************************构建树结构*********************************************************************//
	TreeNode* Tree_Builde(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/,int & progressI/*用于计算进度*/);
	
	void SetValuesForTreeNode(TreeNode* treeNode,int tuplesN,TuplesClassified *tuples,double ** sampleData,int sampleData_line,int publicClassID);
	AttIndexTab ChooseAttribute1(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL);
	double Gain(AttValuesTab * attValues, int attNum);
	double GainRatio(AttValuesTab * attValues, int attNum);
	statisticsTab* AttValuesStatistics(ClassUnique* uniqueValuesLink);
	double* discreteData(AttValuesTab* AttValues, int AttValuesCount);
	double findDiscPoint(double * discPoitList, int discNum, AttValuesTab * attValueslist,int attValuesNum,bool Tag);
	
	double InfoAtt(InfoAttTab * infoAttValues, int infoNum);
	double Info(statisticsTab* staTab, int staCount);
	// ********************************************************************************************************************************//
	//***********************************************用树分类*****************************************************************//
	bool Tree_Classfy(TreeNode* root,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI,int& isRoot,double **data);
	bool Tree_Classfy_1(TreeNode* root,double * classifydata,int &classID);

	void DeleteTree(TreeNode* root);
private:
	int m_NodeSize;
	int m_leafsSize;
	bool tagT;
	int m_Dimension0;
	int m_Dimension2;
	int m_k;
	double m_splitPointValue;
	int m_treeHeight;
	TuplesClassified *m_tuples;
	int **attIndex;
	// 建立属性集
	int** m_attListIndex;
	// 建立样本索引
	int** m_spDataIndex;
	// 建立元组集
	TuplesClassified *m_initial_tuples;
	TreeNode * m_tree;//存储树结构
	// 存储分类后的ID
	short* m_classifiedID;
};

