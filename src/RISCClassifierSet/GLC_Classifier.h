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
//
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
//typedef struct ClassUnique1
//{  
//
//	int classID;        
//	int cunt;
//	vector<int> dataIndex;//��¼���������ǩ�����е�����
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
//struct ConfusionMatrix//���ڼ�¼��������
//{  
//	int classifierID;
//	int nClass;     //������
//	int **matrix; //��������
//	float **acryMatrix;//������ʶ�����ܾ���
//	float **confidenceMatrix;//���������ζȾ���
//};
////������
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
	virtual void SetParameter(void)                                     //���ò�����Ҫ�������أ������ýṹ��
	{

	}
	virtual void SetParameter(TreeParameter treeparameter)                                    //���ò�����Ҫ�������أ������ýṹ��
	{
		//m_data_row=1222;
	}
	virtual void SetParameter(svm_parameter* svmparameter)
	{


	}
	virtual void SetParameter(glc_ann_parameter* annparameter)
	{
		

	}
	virtual void SetData() //��÷�������
	{

	}
	virtual void ExecutTrain(int & progressi)   //ִ��ѵ��
	{
		
	}
	virtual void ExecutTrain()                  //ִ��ѵ��
	{

	}
	virtual void GetModle(TreeNode * tree) //�����ģ��
	{

	}
    virtual void GetModle(svm_model * svmmodle)    //���SVMģ��
   {

   }
   virtual void GetModle(FANN::neural_net* svmmodle)      //���ANNģ��
   {
   }
	virtual void SetModle(TreeNode * tree) //������ģ�ͣ��������Ѿ����ɵ�ģ�������з���
	{

	}
	
	virtual void SetModle(svm_model * svmmodle) //����SVMģ�ͣ��������Ѿ����ɵ�ģ�������з���
	{

	}
	virtual void SetModle(FANN::neural_net* svmmodle)
	{

	}
	virtual void GetModle()
	{

	}
	virtual short* ExecutClassify(double **data,int data_row,int data_line,int & progressi)   //ִ�з���
	{
		return NULL;
	}
	virtual short ExecutClassify(double *data,int data_row,int data_line)                     //ִ�з���
	{
		return 0;
	}
	virtual short *GetTest(double **data=NULL,int data_row=0,int data_line=0)                 //��ò��Խ��
	{
		return NULL;
	}
	virtual void GetAccuracyEvaluation(void)                           //��þ�������
	{

	}
	void ExecutClassify_clean()                                  //ִ�з���
	{
		if (m_classifiedresultID!=NULL)
		{
			delete [] m_classifiedresultID;
		}
	}
	virtual bool DestroyModel()                                  //ɾ��ģ��
	{
		return true;
	}

	void GetTest_clean()                                       //��ò��Խ��
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
	double **m_data;int m_data_row,m_data_line;//��÷�������
//	short* m_testclassID;
	
	//short* m_testclassID;
	short* m_testclassID;
	short* m_classifiedresultID;
};
