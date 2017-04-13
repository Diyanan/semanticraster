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
	

	// ���ڼ�������
	int m_k;
	// ��¼�������
	int m_splitPointAttIndex;
	// ��¼Ԫ����Ϣ������
	int m_tuplesNum;

	// ��¼��������
	double** m_sampleDatas;
	double** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	// ����������
	int m_sampleDatasRow;
	// ����������
	int m_sampleDatasLine;
	// ����������������
	int m_tuplesRow;
	// ����������������
	int m_tuplesLine;
	// ����������������
	int m_sampleDataIndexRow;
	// ����������������
	int m_sampleDataIndexLine;
	// ������Լ�,attD��ʾ���Լ���ά��
	int ** GetAttIndexList(int attNum, int attD);
	// ���Լ�
	int** m_attListIndex;
	// ɾ�����Լ�
	void DeletAttIndexList(int ** attIndexList, int attM , int attN);
	// �ۼ�Ԫ�����
	int m_tuplesI;
	// ���̶ĺ���
	int GetChromoRoulette(TuplesClassified* tuples, int tuplesN);
	// ɾ���½����������ݼ�
	void DeleteSamplingReplacement(TuplesClassified* tuples);
	// �����㷨��������
	int m_adaBoostingK;

	
	int GetClassRules(TreeNode* root);
	int m_stackForNodeRulesTop;


	// ������������ķ������Ծ��ȣ�������MeasuringAccuracy����
	void InfoOrderForMeasuring(TreeNode* root,TuplesClassified* MeasuringTuples,int tuplesNum);
	// ����Ԫ��
	TuplesClassified* m_measuringTuples;
	// ������ģ�ͼ�
	TreeSelection* m_treeSelection;
	// ���һ����Χ�ڲ��ظ��������
	int NonredundantRandom(int leftLimit, int rightLimit, int randomN);
	// ��Ҫ�Ļ��಻�ظ�������ĸ���
	int m_randomNum;
	// ��¼�������
	int* m_random;
	// ������
	double m_sampleDataRatio;
	// �ж������ĸ���
	int m_ruleCount;
	// ��¼���յõ��Ĺ���
	//CString* m_ruleSet;
	// ������
	int m_ruleSetI;
	// �����ļ�·��
	//CString m_ruleFilePath;
	// ѵ��Ԫ��ĸ���
	int m_tringDataN;
	int* m_quede;
	int m_leafsSize;
public:
	int m_NodeSize;
	// ����Ҷ�������ܺ�
	int m_allDatas;
	// ����������
	int m_publics;
	// ��������
	int m_allErros;
	int m_erroNum;
	TuplesClassified *m_tuplesInformation;
	// ��ʼ����¼Ԫ����Ϣ������
	void TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/);
	// �����¼Ԫ����Ϣ������
	void TuplesInformationDelete(TuplesClassified *tuples);
	// ����AdaBoosting����
	void doAdaBoosting(TuplesClassified * tuples, int tuplesN, int k,int & progressI);
	// �зŻس�������������
	TuplesClassified* SamplingReplacement(TuplesClassified* tuples, int tuplesN);
	// ���������������
	double** GetSampleDatas(double** sampledates, int m , int n);
	// �����������������
	int ** GetSampleDataIndex(TuplesClassified* tuples,int tuplesN);
	// ɾ������������
	void DeleteSampleDataIndex(TuplesClassified* tuples);
	// ��ʼ���������ݣ��������ݣ����Լ�������Ϣ
	void SampleDataInitialization(double ** sampleData, int row, int line);
	// ����ӽ����ϻ�õĿ��Ʋ���
	void GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio);
	//����������
	int CalculateErrors(TreeNode* root, double ** SampleDatas, int m , int n,TuplesClassified* tuples,int tuplesN);
	//���������
	double ErroRatioForModel(TuplesClassified * tuplesInfmation, int tuplesNum);
	// ��ʼ����������
	void TestDataInitialization(double** testData , int row , int line);

	// ���ڽ������õ�������Ԫ���滻ԭ����
	void TuplesInfoReplacement(TuplesClassified* tuples, int tuplesN);

	void WeightNormalization(TuplesClassified* tuples, int tuplesN,double errorRatio);
	void ReplaceWeightOnTupleList(TuplesClassified* Di, int tuplesN);
	void TuplesWeightInitialization(TuplesClassified* tuples, int tuplesN);
	TreeSelection* GetTreeSelection(TreeSelection* treeSelections, int treeN);

	void DeleteTreeSelection(TreeSelection* treeSelections);
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI);//�н�����
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum);
	// �������ݷָ�Ϊѵ�����Ͳ��Լ�
	void SampleDataDataPartition(double** sampleData, int row, int line);
	// �������ظ������
	int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
	// ���ظ���ģ�ͣ��ú���������������
	TreeSelection * GetCompoundModel(void);
	//�������
	void GetData(double ** sampleData, int row, int line,int ** attIndex);
	void run(int & progressI);


};

