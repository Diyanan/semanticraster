#pragma once
#include "ClassifiedData.h"
#include "Tree.h"
//typedef struct TuplesClassified  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
//{ 
//	int i;                //Ԫ������ 
//	int sampleDataIndex; //Ԫ�������������е�����
//	int tuplesClassID;   //Ԫ������
//	double weight;       //Ԫ��Ȩ��
//	int err;             //���Ԫ������࣬��err=1,����Ϊ0��
//	double pre;
//
//};
//typedef struct TreeNode  //���ڵ�
//{ 
//	int attIndex;     //�����������е���������ֵ 
//	double splitVertex;//���ѵ�
//	TreeNode *leftChild;//���ӽڵ�
//	TreeNode *rightChild;//�Һ��ӽڵ�
//	bool isLeaf;         //�ж��Ƿ���Ҷ�ӽڵ�
//	int ClassID;        //����Ҷ�ӽڵ�ʱ�ж�����һ��
//	int publicClassN;  //��ǰ�ڵ������������ݸ�����
//	int allN;          //��ǰ�ڵ�����ݼ�����
//	int publicClassID;  //��¼��ǰ�ڵ�Ķ�����
//	TuplesClassified * tupInformation; //��¼Ҷ�ӽڵ���Ԫ��ķ�����Ϣ
//	int errors;//��ΪҶ�ӽڵ�ʱ��¼�ýڵ�������ĸ���
//};
//typedef struct TreeSelection  //�洢��������Ϣ
//{ 
//	TreeNode * Tree;	
//	double weight;
//	double erroratio;
//	int leafs;
//	int rules;//���µĹ�����Ŀ
//	int ruleeSetRow;//���򼯵�����
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
	// �洢����
	double** m_data;
	// ������
	int m_dataRow;
	// ������
	int m_dataLine;
	// ����Ԫ�飬��¼Ԫ����Ϣ
	TuplesClassified* m_data_Tuples;
public:
	// ִ�з���
	//ClassifiedData* doClassify(CString * rules,int rulsRow);
	//ClassifiedData* doClassify(TreeSelection * treeSelection,int treeSum);
	////ClassifiedData* doClassify(TreeSelection * treeSelection,int treeSum,CString & Eror);
	//ClassifiedData* doClassify();
	//Ԥ�ⵥ������
	bool  DataClassification::Tree_Classfy_1(TreeNode* root,double * classifydata,int &classID);
private:
	// ��Ԫ�����
	//TuplesClassified* TuplesClassify(TuplesClassified* Tuples,int tuplesNum,CString * rules,int rulsN);
	//��õ���������
	//CString* GetRuleSet_SingleTree(TreeSelection * trees, int treei/*���ı��*/);
	//bool InorderForRuleSet(TreeNode* root, int tag, double erroratio,CString stRules,int rulesN);
	// ����洢����
	//CString* m_ruleSet;
	// ������
	int m_ruleSetI;
	int m_ruleCount;
private:
	// ��ȨͶƱ���
	int WeightedVoting(ClassifiedData dataWaitingVote);
	// ����
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
	// ��ȡ����֮������ݵ���𣬰������ݵ���ŷ���
	short* GetClassifiedID(void);
private:
	// ��¼����������
	ClassifiedData *m_classData;
public:
	// �������
	void Delete(void);
	// ִ�з���
	short doClassifySingle(double* data,TreeSelection *treeSelection,int treenum);
};
void quicksort(TuplesClassified a[], int left, int right);
int partition(TuplesClassified data[],int lo,int hi);
void QuickSort(TuplesClassified a[], int lo, int hi);
