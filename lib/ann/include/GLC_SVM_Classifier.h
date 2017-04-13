#include "svm.h"

struct SVM_Selection  //�洢��������Ϣ
{ 
	svm_model * model;
	double weight;
	double erroratio;

};
struct ClassifiedResutEveryTime_SVM_SVM  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
{ 

	int tuplesClassID;   //Ԫ������	
	double pre;//Ԥ��Ȩ��
	bool isvotied;

};
struct ResutEveryTime_SVM  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
{ 
	int tuplesClassID;   //Ԫ������	
	double pre;//Ԥ��Ȩ��
	bool isProseed;
};
class ClassifiedData_SVM
{
private:
	int i;                //Ԫ������ 
public:	
	int classificationTimes;   //�������
	ClassifiedResutEveryTime_SVM_SVM * resultEveryT;       //Ԫ��ķ�����Ϣ
	int classId;   //�������
	int DataIndex; //Ԫ���������е�����
	ClassifiedResutEveryTime_SVM_SVM Statistics[30];
	int StatisticsI;

public:
	ClassifiedData_SVM();
	~ClassifiedData_SVM(void);
	void SetData(int index, int classT);
private:
	ClassifiedData_SVM *m_ClassifiedData_SVM;
};

struct TuplesClassified_SVM  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
{ 
	int i;                //Ԫ������ 
	int sampleDataIndex; //Ԫ�������������е�����
	int tuplesClassID;   //Ԫ������
	double weight;       //Ԫ��Ȩ��
	int err;             //���Ԫ������࣬��err=1,����Ϊ0��
	double pre;
	ClassifiedData_SVM * clssified_data;
	double * data;


};
struct Tree_SVMNode_SVM  //���ڵ�
{ 
	int attIndex;     //�����������е���������ֵ 
	double splitVertex;//���ѵ�
	Tree_SVMNode_SVM *leftChild;//���ӽڵ�
	Tree_SVMNode_SVM *rightChild;//�Һ��ӽڵ�
	bool isLeaf;         //�ж��Ƿ���Ҷ�ӽڵ�
	int ClassID;        //����Ҷ�ӽڵ�ʱ�ж�����һ��
	int publicClassN;  //��ǰ�ڵ������������ݸ�����
	int allN;          //��ǰ�ڵ�����ݼ�����
	int publicClassID;  //��¼��ǰ�ڵ�Ķ�����
	TuplesClassified_SVM * tupInformation; //��¼Ҷ�ӽڵ���Ԫ��ķ�����Ϣ
	int errors;//��ΪҶ�ӽڵ�ʱ��¼�ýڵ�������ĸ���
	double pre;
};
struct Tree_SVMSelection_SVM  //�洢��������Ϣ
{ 
	Tree_SVMNode_SVM * Tree_SVM;	
	double weight;
	double erroratio;
	int leafs;
	int rules;//���µĹ�����Ŀ
	int ruleeSetRow;//���򼯵�����
	//CString * rulset;
	double erroratio_test;
	int publicClssId;
};
struct AttIndexTab_SVM  //���ڼ�¼�������������Ӧ����ֵ����Ϣ���棬��Ϣ������
{ 
	int attIndex;     //��������ֵ 
	double attInf;//������ֵ
	double attGain;//��Ϣ����
	double attGainRatio;//��Ϣ������
	double splittPoint;//��¼��ѷ��ѵ�ֵ
	int D1,D2;         //��¼���ѵ��������ݵĸ���

};
struct InfoAttTab_SVM
{ 
	double Info;     //��ֵ   
	double Weight;//Ȩ��

};
struct ClassUnique_SVM//���ڼ�¼����ֵ���������������е�������
{  

	int classID;        //��¼��������
	int cunt;
	ClassUnique_SVM * next;
};
struct AttValuesTab_SVM//���ڼ�¼����ֵ���������������е�������
{  
	double AttValue;
	int classID;        //��¼��������
	int AttValueIndex;
};
struct statisticsTab_SVM
{
	int valueID;//�ڴ˲��ü�¼ֵ��ֻ��Ψһֵ���
	int valueCount;//����ֵ�����ĸ���
	double Proportion;//��ռ�������ݵı���
};
struct svm_model
{
	svm_parameter param;	// parameter
	int nr_class;		// number of classes, = 2 in regression/one class svm
	int l;			// total #SV
	svm_node **SV;		// SVs (SV[l])
	double **sv_coef;	// coefficients for SVs in decision functions (sv_coef[k-1][l])
	double *rho;		// constants in decision functions (rho[k*(k-1)/2])
	double *probA;		// pariwise probability information
	double *probB;

	// for classification only

	int *label;		// label of each class (label[k])
	int *nSV;		// number of SVs for each class (nSV[k])
	// nSV[0] + nSV[1] + ... + nSV[k-1] = l
	// XXX
	int free_sv;		// 1 if svm_model is created by svm_load_model
	// 0 if svm_model is created by svm_train
};
typedef struct statistics
{ 
	int classId;     //��ֵ   
	double PreWeightSum;//Ȩ��

} GLC_adSVM_Statistics;

class DataFilling_adsvm
{
public:
	DataFilling_adsvm(double** data, int row , int line);
	~DataFilling_adsvm(void);
private:
	// ��ȡSVM_node�õ�һ������������

	svm_node* SetSVM_node(double *characteristic,int characteristic_Num);
	// �洢������������
	svm_node** m_SVMnode;
public:
	// �����������������
	svm_node** SetSVMNodes(double** data, int row , int line);
	svm_node** GetSVMNodes();
	int m_SVMnode_row;
private:
	// ע����˱�ʵ�����ݶ���һ��ά
	int m_SVMnode_line;
public:
	// �洢���βμ�������������������ݼ���
	svm_problem GetSVMproblem(double* SampleClassId);
	// ����SVM����
	void SetSVMparameter(	int svm_type,//SVM���ͣ���ǰenum
		int kernel_type,//�˺���
		double degree,	/* for poly */ 
		double gamma,	/* for poly/rbf/sigmoid */
		double coef0,	/* for poly/sigmoid */

		/* these are for training only */
		double cache_size, /* in MB */
		double eps,	/* stopping criteria */
		double C,	/* for C_SVC, EPSILON_SVR and NU_SVR */
		int nr_weight,		/* for C_SVC */
		int *weight_label,	/* for C_SVC */
		double* weight,		/* for C_SVC */
		double nu,	/* for NU_SVC, ONE_CLASS, and NU_SVR */
		double p,	/* for EPSILON_SVR */
		int shrinking,	/* use the shrinking heuristics */
		int probability /* do probability estimates */
		);
private:
	// �洢SVM����
	svm_parameter m_svmparameter;
	// svm����
	svm_parameter m_svm_parameter;
public:
	// ��ò���
	svm_parameter GetSVMparameter(void);
private:
	// ��¼ѵ�����ݵ��������
	double *m_sampleClassId;
public:
	double * GetSampleTargetClassId(void);
	// �������
	void DataCleaner(void);
private:
	svm_node* m_svmnode_forone;
};


class Tree_SVM
{
public:
	Tree_SVM(void);
	~Tree_SVM(void);

	// ������
	double Info(bool isContinues, double * AttValues, int ValuesCount);

	// ��ɢ��������,������ɢ����б�
	double * discreteData(AttValuesTab_SVM * AttValues, int AttValuesCount);
	// ������ֵ
	double Info(statisticsTab_SVM* staTab, int staCount);
	// ѡ��������ѷ��ѵ�
	double fineDiscPoint(double * discPointList, double * attValueList);
	//private:
	// ��¼ĳһ���Ե�ֵ��������
	AttValuesTab_SVM *m_AttValuesTab_SVMle;
public:
	double findDiscPoint(double * discPoitList, int discNum, AttValuesTab_SVM * attValueslist,int attValuesNum,bool Tag);
	double findDiscPoint1(double * discPoitList, int discNum, AttValuesTab_SVM * attValueslist,int attValuesNum,int **attVlueList,int attN,int attL,bool Tag);
	// �����Ψһֵ����
	ClassUnique_SVM * getClassUnique_SVM(AttValuesTab_SVM * attValues, int attNum);
	// ������ڰ�A���ֶ�DԪ���������Ҫ��������Ϣ
	double InfoAtt(InfoAttTab_SVM * infoAttValues, int infoNum);
	// ��������Ϣ����
	double Gain(AttValuesTab_SVM * attValues, int attNum);
	double Gain1(AttValuesTab_SVM * attValues, int attNum,int **attVlueList,int attN,int attL);
	// ͳ�Ƶõ�ͳ�Ʊ�
	statisticsTab_SVM* AttValuesStatistics(ClassUnique_SVM* uniqueValuesLink);
	// �����Ϣ������
	double GainRatio(AttValuesTab_SVM * attValues, int attNum);
	// ѡ���������
	int ChooseAttribute(double ** sampleData, int row, int line, int * attIndex, int attNum);
	AttIndexTab_SVM ChooseAttribute1(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL);
	// ������
	Tree_SVMNode_SVM* BuildTree_SVM(double ** sampleData, int row, int line, int * attIndex, int attNum/*,Tree_SVMNode_SVM * root*/,bool outPut);
	void deleteClassUnique_SVM(ClassUnique_SVM *classLick);
private:
	// ÿ�����Լ�������еĵ��÷��ѵ��ֵ
	double m_splitPointValue;


public:
	Tree_SVMNode_SVM* BuildTree_SVM1(double ** sampleData, int row, int line,int **sampDataIndex,int m,int n,int ** attIndex, int attNum,int attL/*,Tree_SVMNode_SVM * root*/,bool outPut,int hight);
	//	Tree_SVMNode_SVM* BuildTree_SVM1(double ** sampleData, int row, int line,int **sampDataIndex,int m,int n,int * attIndex, int attNum/*,Tree_SVMNode_SVM * root*/,bool outPut,int hight);
	// ����ݹ����
	void InOrder(Tree_SVMNode_SVM * root);
	// �Ƴ�����
	void RemoveTree_SVM(Tree_SVMNode_SVM* root);
	// �жϼ�֦������ڵ�����ж�
	bool PostPrune(Tree_SVMNode_SVM* iNode);
	// �������
	void LeverOrder(Tree_SVMNode_SVM* root);
	void test(double ** sampleData, int row, int line, int * attIndex, int attNum);

private:
	int m_Tree_SVMHeight;
	int m_Dimension0;
	int m_Dimension1;
	int m_Dimension2;
	int m_Dimension;
public:
	int m_NodeSize;
private:
	// ���ڼ�������
	int m_k;
	// ��¼�������
	int m_splitPointAttIndex;
public:
	// ���㱻��ֵĸ���
	int CalculateErrors(Tree_SVMNode_SVM* root, double ** SampleDatas, int m , int n,TuplesClassified_SVM* tuples,int tuplesN);
	// �������ĸ��ڵ㣬�����Ԫ������������һ��Ԫ�����飬Ŀ����Ϊ���������������Ա�
	TuplesClassified_SVM * GetClassifiedTuples(Tree_SVMNode_SVM * root, int m);
	// ����һ�����飬��Ҷ�ӽڵ�ķ�����Ϣ��������
	void traverse(Tree_SVMNode_SVM* root, TuplesClassified_SVM* T, int i);
	// ������ʹ�������������
	void Traverse(Tree_SVMNode_SVM* root,double ** SampleDatas, int m , int n,TuplesClassified_SVM * tuples,int tuplesN,bool isReplaceTuples/*�Ƿ����Ԫ����Ϣ�������ڼ��������*/);
private:
	// �ڼ�������������ڼ�¼���Ԫ��ĸ���
	int m_erroNum;
	// ��ѷ���ֵ��
	int m_bestSplitPointValue;
public:
	// �������ֵ
	void getTree_SVMHeight(int h);
	// ����ģ�͵������
	double ErroRatioForModel(TuplesClassified_SVM * tuplesInfmation, int tuplesNum);
private:
	// �������ڼ�¼����Ԫ�������������е������Լ�Ԫ����ռ��Ȩ��
	TuplesClassified_SVM *m_tuplesInformation;
	// ��ʼ����¼Ԫ����Ϣ������
	void TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/);
public:
	// �����¼Ԫ����Ϣ������
	void TuplesInformationDelete(TuplesClassified_SVM *tuples);
private:
	// ��¼Ԫ����Ϣ������
	int m_tuplesNum;
public:
	// ����AdaBoosting����
	//void AdaBoosting(TuplesClassified_SVM * tuples, int tuplesN, int k);
	//// �зŻس�������������
	//TuplesClassified_SVM* SamplingReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	// ���������������
	double** GetSampleDatas(double** sampledates, int m , int n);
private:
	// ��¼��������
	double** m_sampleDatas;
	double** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	// ����������
	int m_sampleDatasRow;
	// ����������
	int m_sampleDatasLine;
public:
	// �����������������
	int ** GetSampleDataIndex(TuplesClassified_SVM* tuples,int tuplesN);
	// ɾ������������
	void DeleteSampleDataIndex(TuplesClassified_SVM* tuples);
private:
	// ����������������
	int m_tuplesRow;
	// ����������������
	int m_tuplesLine;
	// ����������������
	int m_sampleDataIndexRow;
	// ����������������
	int m_sampleDataIndexLine;
public:
	// ������Լ�,attD��ʾ���Լ���ά��
	int ** GetAttIndexList(int attNum, int attD);
private:
	// ���Լ�
	int** m_attListIndex;
	// ɾ�����Լ�
	void DeletAttIndexList(int ** attIndexList, int attM , int attN);
public:
	// ��ʼ���������ݣ��������ݣ����Լ�������Ϣ
	void SampleDataInitialization(double ** sampleData, int row, int line);
private:
	// �ۼ�Ԫ�����
	int m_tuplesI;
	// ���̶ĺ���
	int GetChromoRoulette(TuplesClassified_SVM* tuples, int tuplesN);
	// ɾ���½����������ݼ�
	void DeleteSamplingReplacement(TuplesClassified_SVM* tuples);
	// �����㷨��������
	int m_adaBoostingK;
public:
	//// ����ӽ����ϻ�õĿ��Ʋ���
	//void GetControlsParameter(int Tree_SVMHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath);
	// �Ծ�����EBP��֦
	bool PostPruneEBP(Tree_SVMNode_SVM* root,TuplesClassified_SVM *tuples,int tuplesN);
private:
	// ����׳�
	int CalculateFactorial(int n);
	// ������������
	double CalculateConfidenceLLimit(int n, int e, double p);
	// ��������t��Ҷ�ڵ��Ԥ�����������֮��
	double ClaculateLeafPreErrorsADD(Tree_SVMNode_SVM* root);
	// ��������Ҷ�ӽڵ�
	void TraverseChildTree_SVM(Tree_SVMNode_SVM * root, double LeafPreErrorsADD);
	// ��������Ҷ�ӽڵ��Ԥ�������֮��
	double m_leafPreErrorsADD;
	// ������������
	int GetChildTree_SVMHeight(Tree_SVMNode_SVM* root);
	// ��¼������
	int m_childTree_SVMHeight;
	// ������Ҷ�Ӹ���
	int m_childTree_SVMLeafs;
	// �����Ľڵ�����
	int m_childTree_SVMNode_SVMs;
	// ���Ԥ����������
	double CalculatePreErrors(Tree_SVMNode_SVM* root,TuplesClassified_SVM * tuples,int tuplesN);
public:
	// ɾ�����ṹ
	void DeleteTree_SVM(Tree_SVMNode_SVM* root);
	// ��÷������
	int GetClassRules(Tree_SVMNode_SVM* root);
public:
	// ���������ȡ����
	//bool InorderForRules(Tree_SVMNode_SVM* root,int tag/*��¼��ǰ��Ϊ������*/,CString stRules/*��¼�ڵ�֮ǰ�Ĺ���*/);
private:
	// ��¼����ı�����ÿ��ʹ�ö�Ҫ��ʼ��
	//	CString m_strRules;
	// ʹ��ջ��¼�ڵ�Ĺ���
	//CString* m_stackForNodeRules;
	// ���ջ��
	int m_stackForNodeRulesTop;

public:
	// ���Ծ���
	//TuplesClassified_SVM* MeasuringAccuracyMatch(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
private:
	// ������������ķ������Ծ��ȣ�������MeasuringAccuracy����
	void InfoOrderForMeasuring(Tree_SVMNode_SVM* root,TuplesClassified_SVM* MeasuringTuples,int tuplesNum);
	// ����Ԫ��
	TuplesClassified_SVM* m_measuringTuples;
	//	double MeasuringAccuracy(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
public:
	// ��ò��Ծ���
	//double GetMeasuringAccuracy(int tuplesN,Tree_SVMNode_SVM * root,CString rulesPath);
	// ��ʼ����������
	void TestDataInitialization(double** testData , int row , int line);
	// ��ӡ��Ԫ����Ϣ
	//void PrintTuples(TuplesClassified_SVM * tuples, int tuplesN, CString path);
	// ��ӡ��������
	///void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified_SVM* tuples, int tuplesN , CString path);
	//// ���ڽ������õ�������Ԫ���滻ԭ����
	//void TuplesInfoReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	//void PrintSampledatsBySDataIndex(int ** S,int m,int n,CString path);
	// //��ӡ��������
	void PrintSampdatas(double** S, int m , int n);
	// ����Ҷ�������ܺ�
	int m_allDatas;
	// ����������
	int m_publics;
	// ��������
	int m_allErros;
private:
	// ��������Ҷ�Ӵ���Ԫ���б��˳��
	void TuplesOrderByTree_SVM(TuplesClassified_SVM* tuples, int tuplesN);
public:
	Tree_SVMNode_SVM* BuildTree_SVM2(double ** sampleData, int row, int line,TuplesClassified_SVM *tuples,int tuplesN,int ** attIndex, int attNum,int attL,bool outPut,int hight,TuplesClassified_SVM *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/,int & progressI/*���ڼ������*/);
	Tree_SVMNode_SVM* BuildTree_SVM2(double ** sampleData, int row, int line,TuplesClassified_SVM *tuples,int tuplesN,int ** attIndex, int attNum,int attL,bool outPut,int hight,TuplesClassified_SVM *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/);
private:
	// ��Ҷ�Ӵ�С
	int m_leafsSize;
public:
	// ͨ��Ԫ���б��ӡ��������
	void PrintSampleDataByTuples(TuplesClassified_SVM* tuples, int tuplesN);
	// ��þ�����ģ�ͼ�
	Tree_SVMSelection_SVM* GetTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs, int Tree_SVMN);
	// ɾ����ģ��ѡ��
	void DeleteTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs);
private:
	// ������ģ�ͼ�
	Tree_SVMSelection_SVM* m_Tree_SVMSelection_SVM;
public:
	//// ��ȡadaboosting֮��Ĺ���
	//int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum);
	//// �������ݷָ�Ϊѵ�����Ͳ��Լ�
	//void SampleDataDataPartition(double** sampleData, int row, int line);
	//// �������ظ������
	//int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
private:
	//// ���һ����Χ�ڲ��ظ��������
	//int NonredundantRandom(int leftLimit, int rightLimit, int randomN);
	//// ��Ҫ�Ļ��಻�ظ�������ĸ���
	//int m_randomNum;
	//// ��¼�������
	//int* m_random;
	//// ������
	//double m_sampleDataRatio;
public:
	// ��ù���
	//int GetintRuleSet(Tree_SVMSelection_SVM * Tree_SVMs, int Tree_SVMsN,CString rulesPath);
private:
	// ����������ȡ����
	//bool InorderForRuleSet(Tree_SVMNode_SVM* root, int tag, double erroratio,CString stRules,int rulesN);
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

public:
	// ��ù������Ĳ���
	//	void SetParameter(int Tree_SVMHeight, int leafsSize, CString rulePath);
	//�������
	void GetData(double ** sampleData, int row, int line,int ** attIndex);
	//// ��ʼ������
	//void doBuildTree_SVM(TuplesClassified_SVM *tuples,int tuplesN,int & progressI);
	//void doBuildTree_SVM(TuplesClassified_SVM *tuples,int tuplesN);
	// ������ĸ��ڵ�
	Tree_SVMNode_SVM* GetTree_SVMRoot(void);
private:
	// ��¼���ĸ����ڵ�
	Tree_SVMNode_SVM* m_Tree_SVMRoot;
public:
	// ����
	bool tagT;
	// ��������
	void doVariableToZero(void);
	// ���ƽ�����
	//void SetProgressBar(CProgressCtrl*  progressBar);
private:
	// ����������
	//CProgressCtrl* m_progressBar;
	// Ϊ�ڵ㸳ֵ
	void SetValuesForTree_SVMNode_SVM(Tree_SVMNode_SVM* Tree_SVMNode_SVM,int tuplesN,TuplesClassified_SVM *tuples,double ** sampleData,int sampleData_line,int publicClassID);
};
class AdaBoosting_SVM_adsvm
{
public:
	AdaBoosting_SVM_adsvm(void);
	~AdaBoosting_SVM_adsvm(void);


private:
	int m_Tree_SVMHeight;
	int m_Dimension0;
	int m_Dimension1;
	int m_Dimension2;
	int m_Dimension;

public:
	int m_NodeSize;
	// ����Ҷ�������ܺ�
	int m_allDatas;
	// ����������
	int m_publics;
	// ��������
	int m_allErros;
	int m_erroNum;
private:
	// ���ڼ�������
	int m_k;
	// ��¼�������
	int m_splitPointAttIndex;
public:

	TuplesClassified_SVM *m_tuplesInformation;
	// ��ʼ����¼Ԫ����Ϣ������
	void TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/);
public:
	// �����¼Ԫ����Ϣ������
	void TuplesInformationDelete(TuplesClassified_SVM *tuples);
private:
	// ��¼Ԫ����Ϣ������
	int m_tuplesNum;
public:
	// ����AdaBoosting����
	void doAdaBoosting(TuplesClassified_SVM * tuples, int tuplesN, int k,int & progressI);
	void doAdaBoosting_SVM_adsvm(TuplesClassified_SVM * tuples, int tuplesN, int k,int & progressI);
	// �зŻس�������������
	TuplesClassified_SVM* SamplingReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	TuplesClassified_SVM* SamplingReplacement_SVM(TuplesClassified_SVM* tuples, int tuplesN,double *targetclassId);
	// ���������������
	double** GetSampleDatas(double** sampledates, int m , int n);
private:
	// ��¼��������
	double** m_sampleDatas;
	double** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	// ����������
	int m_sampleDatasRow;
	// ����������
	int m_sampleDatasLine;
public:
	// �����������������
	int ** GetSampleDataIndex(TuplesClassified_SVM* tuples,int tuplesN);
	// ɾ������������
	void DeleteSampleDataIndex(TuplesClassified_SVM* tuples);
private:
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
public:
	// ��ʼ���������ݣ��������ݣ����Լ�������Ϣ
	void SampleDataInitialization(double ** sampleData, int row, int line);
	void SampleDataInitialization_SVM(double ** sampleData, int row, int line);

private:
	// �ۼ�Ԫ�����
	int m_tuplesI;
	// ���̶ĺ���
	int GetChromoRoulette(TuplesClassified_SVM* tuples, int tuplesN);
	// ɾ���½����������ݼ�
	void DeleteSamplingReplacement(TuplesClassified_SVM* tuples);
public:
	// �����㷨��������
	int m_adaBoostingK;
	svm_parameter * m_svm_parameter;
public:
	// ����ӽ����ϻ�õĿ��Ʋ���
	//void GetControlsParameter(int Tree_SVMHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath);
	//����������
	int CalculateErrors(Tree_SVMNode_SVM* root, double ** SampleDatas, int m , int n,TuplesClassified_SVM* tuples,int tuplesN);
	int CalculateErrors_SVM(svm_model* model,svm_problem* prob,double * targetClassid);
	//���������
	double ErroRatioForModel(TuplesClassified_SVM * tuplesInfmation, int tuplesNum);
	double AdaBoosting_SVM_adsvm::ErroRatioForModel_SVM(TuplesClassified_SVM * tuplesInfmation, int tuplesNum);
private:

	int GetClassRules(Tree_SVMNode_SVM* root);

public:
	//// ���������ȡ����
	//bool InorderForRules(Tree_SVMNode_SVM* root,int tag/*��¼��ǰ��Ϊ������*/,CString stRules/*��¼�ڵ�֮ǰ�Ĺ���*/);
private:
	// ��¼����ı�����ÿ��ʹ�ö�Ҫ��ʼ��
	//CString m_strRules;
	// ʹ��ջ��¼�ڵ�Ĺ���
	//CString* m_stackForNodeRules;
	// ���ջ��
	int m_stackForNodeRulesTop;

public:
	// ���Ծ���
	//TuplesClassified_SVM* MeasuringAccuracyMatch(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
private:
	// ������������ķ������Ծ��ȣ�������MeasuringAccuracy����
	//void InfoOrderForMeasuring(Tree_SVMNode_SVM* root,TuplesClassified_SVM* MeasuringTuples,int tuplesNum);
	// ����Ԫ��
	TuplesClassified_SVM* m_measuringTuples;
	//double MeasuringAccuracy(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
public:
	// ��ò��Ծ���
	//double GetMeasuringAccuracy(int tuplesN,Tree_SVMNode_SVM * root,CString rulesPath);
	// ��ʼ����������
	void TestDataInitialization(double** testData , int row , int line);
	// ��ӡ��Ԫ����Ϣ
	//void PrintTuples(TuplesClassified_SVM * tuples, int tuplesN, CString path);
	// ��ӡ��������
	//void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified_SVM* tuples, int tuplesN , CString path);
	// ���ڽ������õ�������Ԫ���滻ԭ����
	void TuplesInfoReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	//void PrintSampledatsBySDataIndex(int ** S,int m,int n,CString path);
	// //��ӡ��������
	void PrintSampdatas(double** S, int m , int n);


public:

	// qȨ�ع�һ��
	void WeightNormalization(TuplesClassified_SVM* tuples, int tuplesN,double errorRatio);
	void ReplaceWeightOnTupleList(TuplesClassified_SVM* Di, int tuplesN);
	// ��ʼ��Ԫ���б�Ȩ��
	void TuplesWeightInitialization(TuplesClassified_SVM* tuples, int tuplesN);

public:
	// ͨ��Ԫ���б��ӡ��������
	//void PrintSampleDataByTuples(TuplesClassified_SVM* tuples, int tuplesN,CString path);
	// ��þ�����ģ�ͼ�
	Tree_SVMSelection_SVM* GetTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs, int Tree_SVMN);
	// ɾ����ģ��ѡ��
	void DeleteTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs);
private:
	// ������ģ�ͼ�
	Tree_SVMSelection_SVM* m_Tree_SVMSelection_SVM;
public://double ** sampleData, int row, int line, int * attIndex, int attNum
	// ��ȡadaboosting֮��Ĺ���//
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI);//�н�����
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum);
	// �������ݷָ�Ϊѵ�����Ͳ��Լ�
	void SampleDataDataPartition(double** sampleData, int row, int line);
	// �������ظ������
	int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
private:
	// ���һ����Χ�ڲ��ظ��������
	int NonredundantRandom(int leftLimit, int rightLimit, int randomN);
	// ��Ҫ�Ļ��಻�ظ�������ĸ���
	int m_randomNum;
	// ��¼�������
	int* m_random;
	// ������
	double m_sampleDataRatio;
public:
	// ��ù���
	//int GetintRuleSet(Tree_SVMSelection_SVM * Tree_SVMs, int Tree_SVMsN,CString rulesPath);
private:
	// ����������ȡ����
	//bool InorderForRuleSet(Tree_SVMNode_SVM* root, int tag, double erroratio,CString stRules,int rulesN);
	// �ж������ĸ���
	int m_ruleCount;
	// ��¼���յõ��Ĺ���

	// ������
	int m_ruleSetI;

	// ѵ��Ԫ��ĸ���
	int m_tringDataN;
	int* m_quede;
	int m_leafsSize;

public:
	// ���ظ���ģ�ͣ��ú���������������
	Tree_SVMSelection_SVM * GetCompoundModel(void);
	//�������
	void GetData(double ** sampleData, int row, int line,int ** attIndex);
	void run(int & progressI);
	void run_SVM(int & progressI);
	// ���ý�����
	//void SetProgressBar(CProgressCtrl* progressBar);
private:


	// Ŀ����
	double** m_target_classsId;
	// ��¼svm��ģ�ͼ�
	// ��¼svm��ģ�ͼ�
	SVM_Selection* m_SVMSelection;
public:
	// ��ü�¼SVM������
	SVM_Selection* GetSVM_Selection(int& SVM_NUM);
};

class DataClassification_SVM
{
public:
	DataClassification_SVM();
	DataClassification_SVM(double **data,int row,int line);
	DataClassification_SVM::DataClassification_SVM(double **data,int row,int line,int classifyType=0);
	~DataClassification_SVM(void);
private:
	// �洢����
	double** m_data;
	// ������
	int m_dataRow;
	// ������
	int m_dataLine;
	// ����Ԫ�飬��¼Ԫ����Ϣ
	TuplesClassified_SVM* m_data_Tuples;
public:
	// ִ�з���
	//ClassifiedData_SVM* doClassify(CString * rules,int rulsRow);
	ClassifiedData_SVM* doClassify(Tree_SVMSelection_SVM * Tree_SVMSelection_SVM,int Tree_SVMSum);
	//	ClassifiedData_SVM* doClassify(Tree_SVMSelection_SVM * Tree_SVMSelection_SVM,int Tree_SVMSum,CString & Eror);
	ClassifiedData_SVM* doClassify();
	//Ԥ�ⵥ������
	//bool  DataClassification::Tree_Classfy_1(TreeNode* root,double * classifydata,int &classID);
private:
	// ��Ԫ�����
	//TuplesClassified_SVM* TuplesClassify(TuplesClassified_SVM* Tuples,int tuplesNum,CString * rules,int rulsN);
	//��õ���������
	///CString* GetRuleSet_SingleTree_SVM(Tree_SVMSelection_SVM * Tree_SVMs, int Tree_SVMi/*���ı��*/);
	//bool InorderForRuleSet(Tree_SVMNode_SVM* root, int tag, double erroratio,CString stRules,int rulesN);
	// ����洢����
	//CString* m_ruleSet;
	// ������
	int m_ruleSetI;
	int m_ruleCount;
private:
	// ��ȨͶƱ���
	int WeightedVoting(ClassifiedData_SVM dataWaitingVote);
	// ����
	void Swap(ClassifiedResutEveryTime_SVM_SVM  * a , ClassifiedResutEveryTime_SVM_SVM  * b);
	//void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified_SVM* tuples, int tuplesN,CString path);
	//void PrintTuples(TuplesClassified_SVM * tuples, int tuplesN, CString path);
public:
	bool ClassfyWithTree_SVM(Tree_SVMNode_SVM* root, double erroratio,TuplesClassified_SVM *tuples,int tuplesN,TuplesClassified_SVM *tuplesResult,int & tuplesResultI);
	bool ClassfyWithTree_SVM1(Tree_SVMNode_SVM* root, double erroratio,TuplesClassified_SVM *tuples,int tuplesN,TuplesClassified_SVM *tuplesResult,int & tuplesResultI);
	bool ClassfyWithTree_SVM2(Tree_SVMNode_SVM* root, double erroratio,TuplesClassified_SVM *tuples,int tuplesN,TuplesClassified_SVM *tuplesResult,int & tuplesResultI,int Tree_SVM_NO,int& isRoot);
private:
	//void Print(CString path,ClassifiedData_SVM data);
	ClassifiedData_SVM* m_classifideData;
public:
	// ��ȡ����֮������ݵ���𣬰������ݵ���ŷ���
	int* GetClassifiedID(void);
private:
	// ��¼����������
	ClassifiedData_SVM *m_classData;
public:
	// ʹ��SVM����
	short* doClassify_SVM(SVM_Selection * svmselection,int svmnum,int &progressI/*���ƽ�����*/);
private:
	// ������ڷ��������
	DataFilling_adsvm* m_FilledData_SVM;
public:
	// ���ڵ������ݵķ���
	short doClassify_SVM_Single(svm_node * pre,int data_line,SVM_Selection *svmselection,int svmNum);
	short doClassify_SVM_Single(double* data,int data_line,SVM_Selection *svmselection,int svmNum);
};



class GLC_Adaboosting_SVM
{
public:
	GLC_Adaboosting_SVM(void);
	~GLC_Adaboosting_SVM(void);
	// ѵ��adaSVMģ��
	bool G_adaSVM_ExecutTraining(double** traindata,int traindatrow,int traindatline,int adboostingK/*=10*/);
private:
	// adaSvmģ��
	SVM_Selection *m_svm_modles;
public:
	// ���adaSVMģ��
	SVM_Selection * G_adaSVM_GetModel(void);
	short * G_adaSVM_ExecutClassify(double **adata,int row,int line,int& progressI );
	short * G_adaSVM_ExecutClassify_Single(double **adata,int row,int line,int& progressI );
private:
	// ��������
	int m_adaboostingK;
public:
	svm_parameter* m_svm_parameter;
};


class  GLC_SVM_Classify
{
public:
	GLC_SVM_Classify(void);
	~GLC_SVM_Classify(void);
	bool GLCSVM_Execute_Train(double** data, int data_row, int data_line,int adboostingK/*=10*/);
	bool GLCSVM_Execute_Train(double*data, int data_row, int data_line,int adboostingK/*=10*/);
	// ִ�з���
	short* GLCSVM_Excute_Classify(double* data, int data_row, int data_line);
	// �������
	bool GLCSVM_Clear(void);
private:
	// svm����
	GLC_Adaboosting_SVM m_Gsvm;
	// svm�ĸ���
	int m_svmNum;
	// ��¼����֮���id
	short* m_classid;
	svm_parameter* m_svm_parameter;
public:
	void GLCSVM_Set_Parameter(svm_parameter* parameter);
};

void quicksort(TuplesClassified_SVM a[], int left, int right);
int partition(TuplesClassified_SVM data[],int lo,int hi);
void QuickSort(TuplesClassified_SVM a[], int lo, int hi);