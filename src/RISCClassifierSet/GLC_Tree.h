#pragma once
#include "glc_classifier.h"
class GLC_Tree :
	public GLC_Classifier
{
public:
	GLC_Tree(void);
	~GLC_Tree(void);
	 
     void SetParameter(TreeParameter treeparameter);                                     //���ò�����Ҫ�������أ������ýṹ��
	 void SetData(); //��÷�������
	 void ExecutTrain(int & progressi);                                      //ִ��ѵ��
	 void GetModle(TreeNode * tree);                                         //���ģ��
	 TreeNode * ReturnModle();                                         //���ģ��
	 void SetModle(TreeNode * tree); //����ģ�ͣ��������Ѿ����ɵ�ģ�������з���
	 short* ExecutClassify(double **data,int data_row,int data_line,int & progressi);                                   //ִ�з���
	 short ExecutClassify(double *data);                                                 //ִ�з���
	 short *GetTest(double **data=NULL,int data_row=0,int data_line=0);                                       //��ò��Խ��
	 void GetAccuracyEvaluation(void);                           //��þ�������
     bool DestroyModel();   //ɾ��ģ��
	 ClassUnique *getClassUnique(AttValuesTab * attValues, int attNum);
	 void deleteClassUnique(ClassUnique *classLick);
private:
	void DataFormat(void);
	// ***************************************************�������ṹ*********************************************************************//
	TreeNode* Tree_Builde(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/,int & progressI/*���ڼ������*/);
	
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
	//***********************************************��������*****************************************************************//
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
	// �������Լ�
	int** m_attListIndex;
	// ������������
	int** m_spDataIndex;
	// ����Ԫ�鼯
	TuplesClassified *m_initial_tuples;
	TreeNode * m_tree;//�洢���ṹ
	// �洢������ID
	short* m_classifiedID;
};

