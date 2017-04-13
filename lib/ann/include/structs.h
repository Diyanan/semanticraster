#pragma once
#include "ClassifiedData.h"
#include "svm.h"
#include "GLC_ann_classifier.h"
#include "GLCann_classifier.h"
#include <vector>

enum { RISC_CTree=1, RISC_SVM=2, RISC_ANN=3, RISC_KNN=4, RISC_Bayes=5 };	/* �˴�Ϊ�������������� */
using std::vector;
//adboosting����
#include "ClassifiedData.h"
struct TuplesClassified  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
{ 
	int i;                 
	int sampleDataIndex; 
	int tuplesClassID;   
	double weight;       
	int err;             
	double pre;
	ClassifiedData * clssified_data;


};
struct TreeNode  //���ڵ�
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
struct TreeSelection  //�洢��������Ϣ
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
 struct AttIndexTab  //���ڼ�¼�������������Ӧ����ֵ����Ϣ���棬��Ϣ������
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

//typedef struct TuplesClassified  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
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
//typedef struct TreeNode  //���ڵ�
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

//typedef struct AttIndexTab  //���ڼ�¼�������������Ӧ����ֵ����Ϣ���棬��Ϣ������
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
	vector<int> dataIndex;//��¼���������ǩ�����е�����
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
struct ConfusionMatrix//���ڼ�¼��������
{  
	int classifierID;
	int nClass;     //������
	int **matrix; //��������
	float **acryMatrix;//������ʶ�����ܾ���
	float **confidenceMatrix;//���������ζȾ���
};
//������
typedef struct GLCTreeParameter  
{
	int TreeHeight;
	int LeatfSize;
	
}TreeParameter;
//struct glc_ann_parameter
//{	
//	int connectinRate;                  //���Ӳ���
//	float learningRate;             //ѧϰ��
//	float desiredError;            //��ֹ������
//	int iterationBetweenRepots;       //������
//	int maxepoches;                  //����������
//	int layes;                          //����Ĳ���
//	int hidNum;                        //������Ԫ�ĸ���
//	int inNum;	
//	int outNum;                       //�����Ԫ�ĸ���
//};