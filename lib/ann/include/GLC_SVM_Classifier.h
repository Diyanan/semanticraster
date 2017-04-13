#include "svm.h"

struct SVM_Selection  //存储决策树信息
{ 
	svm_model * model;
	double weight;
	double erroratio;

};
struct ClassifiedResutEveryTime_SVM_SVM  //存储分类后元组信息，可以存在于叶子节点上
{ 

	int tuplesClassID;   //元组的类别	
	double pre;//预测权重
	bool isvotied;

};
struct ResutEveryTime_SVM  //存储分类后元组信息，可以存在于叶子节点上
{ 
	int tuplesClassID;   //元组的类别	
	double pre;//预测权重
	bool isProseed;
};
class ClassifiedData_SVM
{
private:
	int i;                //元组的序号 
public:	
	int classificationTimes;   //分类次数
	ClassifiedResutEveryTime_SVM_SVM * resultEveryT;       //元组的分类信息
	int classId;   //最终类别
	int DataIndex; //元组在数据中的索引
	ClassifiedResutEveryTime_SVM_SVM Statistics[30];
	int StatisticsI;

public:
	ClassifiedData_SVM();
	~ClassifiedData_SVM(void);
	void SetData(int index, int classT);
private:
	ClassifiedData_SVM *m_ClassifiedData_SVM;
};

struct TuplesClassified_SVM  //存储分类后元组信息，可以存在于叶子节点上
{ 
	int i;                //元组的序号 
	int sampleDataIndex; //元组在样本数据中的索引
	int tuplesClassID;   //元组的类别
	double weight;       //元组权重
	int err;             //如果元组误分类，则err=1,否则为0；
	double pre;
	ClassifiedData_SVM * clssified_data;
	double * data;


};
struct Tree_SVMNode_SVM  //树节点
{ 
	int attIndex;     //属性在样本中的索引索引值 
	double splitVertex;//分裂点
	Tree_SVMNode_SVM *leftChild;//左孩子节点
	Tree_SVMNode_SVM *rightChild;//右孩子节点
	bool isLeaf;         //判断是否是叶子节点
	int ClassID;        //当是叶子节点时判断是哪一类
	int publicClassN;  //当前节点的最多数类数据个数、
	int allN;          //当前节点的数据集总数
	int publicClassID;  //记录当前节点的多数类
	TuplesClassified_SVM * tupInformation; //记录叶子节点上元组的分类信息
	int errors;//当为叶子节点时记录该节点错误分类的个数
	double pre;
};
struct Tree_SVMSelection_SVM  //存储决策树信息
{ 
	Tree_SVMNode_SVM * Tree_SVM;	
	double weight;
	double erroratio;
	int leafs;
	int rules;//树下的规则数目
	int ruleeSetRow;//规则集的行数
	//CString * rulset;
	double erroratio_test;
	int publicClssId;
};
struct AttIndexTab_SVM  //用于记录属性索引及其对应的熵值，信息增益，信息增益率
{ 
	int attIndex;     //属性索引值 
	double attInf;//属性熵值
	double attGain;//信息增益
	double attGainRatio;//信息增益率
	double splittPoint;//记录最佳分裂点值
	int D1,D2;         //记录分裂点两侧数据的个数

};
struct InfoAttTab_SVM
{ 
	double Info;     //熵值   
	double Weight;//权重

};
struct ClassUnique_SVM//用于记录属性值及其在样本数据中的行索引
{  

	int classID;        //记录类标号属性
	int cunt;
	ClassUnique_SVM * next;
};
struct AttValuesTab_SVM//用于记录属性值及其在样本数据中的行索引
{  
	double AttValue;
	int classID;        //记录类标号属性
	int AttValueIndex;
};
struct statisticsTab_SVM
{
	int valueID;//在此不用记录值，只给唯一值编号
	int valueCount;//属性值分量的个数
	double Proportion;//所占整个数据的比例
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
	int classId;     //熵值   
	double PreWeightSum;//权重

} GLC_adSVM_Statistics;

class DataFilling_adsvm
{
public:
	DataFilling_adsvm(double** data, int row , int line);
	~DataFilling_adsvm(void);
private:
	// 获取SVM_node得到一个样本的特征

	svm_node* SetSVM_node(double *characteristic,int characteristic_Num);
	// 存储样本所有特征
	svm_node** m_SVMnode;
public:
	// 获得整个样本的特征
	svm_node** SetSVMNodes(double** data, int row , int line);
	svm_node** GetSVMNodes();
	int m_SVMnode_row;
private:
	// 注意多了比实际数据多了一个维
	int m_SVMnode_line;
public:
	// 存储本次参加运算的所有样本（数据集）
	svm_problem GetSVMproblem(double* SampleClassId);
	// 设置SVM参数
	void SetSVMparameter(	int svm_type,//SVM类型，见前enum
		int kernel_type,//核函数
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
	// 存储SVM参数
	svm_parameter m_svmparameter;
	// svm参数
	svm_parameter m_svm_parameter;
public:
	// 获得参数
	svm_parameter GetSVMparameter(void);
private:
	// 记录训练数据的输入类别
	double *m_sampleClassId;
public:
	double * GetSampleTargetClassId(void);
	// 清除数据
	void DataCleaner(void);
private:
	svm_node* m_svmnode_forone;
};


class Tree_SVM
{
public:
	Tree_SVM(void);
	~Tree_SVM(void);

	// 计算熵
	double Info(bool isContinues, double * AttValues, int ValuesCount);

	// 离散连续数据,返回离散点的列表
	double * discreteData(AttValuesTab_SVM * AttValues, int AttValuesCount);
	// 计算熵值
	double Info(statisticsTab_SVM* staTab, int staCount);
	// 选择属性最佳分裂点
	double fineDiscPoint(double * discPointList, double * attValueList);
	//private:
	// 记录某一属性的值，和索引
	AttValuesTab_SVM *m_AttValuesTab_SVMle;
public:
	double findDiscPoint(double * discPoitList, int discNum, AttValuesTab_SVM * attValueslist,int attValuesNum,bool Tag);
	double findDiscPoint1(double * discPoitList, int discNum, AttValuesTab_SVM * attValueslist,int attValuesNum,int **attVlueList,int attN,int attL,bool Tag);
	// 获得类唯一值链表
	ClassUnique_SVM * getClassUnique_SVM(AttValuesTab_SVM * attValues, int attNum);
	// 计算基于按A划分对D元组分类所需要的期望信息
	double InfoAtt(InfoAttTab_SVM * infoAttValues, int infoNum);
	// 计算获得信息增益
	double Gain(AttValuesTab_SVM * attValues, int attNum);
	double Gain1(AttValuesTab_SVM * attValues, int attNum,int **attVlueList,int attN,int attL);
	// 统计得到统计表
	statisticsTab_SVM* AttValuesStatistics(ClassUnique_SVM* uniqueValuesLink);
	// 获得信息增益量
	double GainRatio(AttValuesTab_SVM * attValues, int attNum);
	// 选择分裂属性
	int ChooseAttribute(double ** sampleData, int row, int line, int * attIndex, int attNum);
	AttIndexTab_SVM ChooseAttribute1(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL);
	// 建立树
	Tree_SVMNode_SVM* BuildTree_SVM(double ** sampleData, int row, int line, int * attIndex, int attNum/*,Tree_SVMNode_SVM * root*/,bool outPut);
	void deleteClassUnique_SVM(ClassUnique_SVM *classLick);
private:
	// 每个属性计算过程中的到得分裂点的值
	double m_splitPointValue;


public:
	Tree_SVMNode_SVM* BuildTree_SVM1(double ** sampleData, int row, int line,int **sampDataIndex,int m,int n,int ** attIndex, int attNum,int attL/*,Tree_SVMNode_SVM * root*/,bool outPut,int hight);
	//	Tree_SVMNode_SVM* BuildTree_SVM1(double ** sampleData, int row, int line,int **sampDataIndex,int m,int n,int * attIndex, int attNum/*,Tree_SVMNode_SVM * root*/,bool outPut,int hight);
	// 中序递归遍历
	void InOrder(Tree_SVMNode_SVM * root);
	// 移除树，
	void RemoveTree_SVM(Tree_SVMNode_SVM* root);
	// 判断剪枝，输入节点进行判断
	bool PostPrune(Tree_SVMNode_SVM* iNode);
	// 层序遍历
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
	// 用于计算树高
	int m_k;
	// 记录最佳索引
	int m_splitPointAttIndex;
public:
	// 计算被错分的个数
	int CalculateErrors(Tree_SVMNode_SVM* root, double ** SampleDatas, int m , int n,TuplesClassified_SVM* tuples,int tuplesN);
	// 输入树的根节点，分类后元组的总数，输出一个元组数组，目的是为了与样本数据做对比
	TuplesClassified_SVM * GetClassifiedTuples(Tree_SVMNode_SVM * root, int m);
	// 输入一个数组，将叶子节点的分类信息填入其中
	void traverse(Tree_SVMNode_SVM* root, TuplesClassified_SVM* T, int i);
	// 遍历数使用中序遍历方法
	void Traverse(Tree_SVMNode_SVM* root,double ** SampleDatas, int m , int n,TuplesClassified_SVM * tuples,int tuplesN,bool isReplaceTuples/*是否更新元组信息，不用于计算错误率*/);
private:
	// 在计算错误率中用于记录错分元组的个数
	int m_erroNum;
	// 最佳分裂值点
	int m_bestSplitPointValue;
public:
	// 获得树高值
	void getTree_SVMHeight(int h);
	// 计算模型的误差率
	double ErroRatioForModel(TuplesClassified_SVM * tuplesInfmation, int tuplesNum);
private:
	// 数组用于记录样本元组在样本数据中的索引以及元组所占的权重
	TuplesClassified_SVM *m_tuplesInformation;
	// 初始化记录元组信息的数组
	void TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/);
public:
	// 清除记录元组信息的数组
	void TuplesInformationDelete(TuplesClassified_SVM *tuples);
private:
	// 记录元组信息的数量
	int m_tuplesNum;
public:
	// 进行AdaBoosting提升
	//void AdaBoosting(TuplesClassified_SVM * tuples, int tuplesN, int k);
	//// 有放回抽样，返回样本
	//TuplesClassified_SVM* SamplingReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	// 获得整个样本数据
	double** GetSampleDatas(double** sampledates, int m , int n);
private:
	// 记录整个样本
	double** m_sampleDatas;
	double** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	// 样本数据行
	int m_sampleDatasRow;
	// 样本数据列
	int m_sampleDatasLine;
public:
	// 获得样本数据索引集
	int ** GetSampleDataIndex(TuplesClassified_SVM* tuples,int tuplesN);
	// 删除样本索引集
	void DeleteSampleDataIndex(TuplesClassified_SVM* tuples);
private:
	// 样本数据索引集行
	int m_tuplesRow;
	// 样本数据索引集列
	int m_tuplesLine;
	// 样本数据索引集行
	int m_sampleDataIndexRow;
	// 样本数据索引集列
	int m_sampleDataIndexLine;
public:
	// 获得属性集,attD表示属性集的维数
	int ** GetAttIndexList(int attNum, int attD);
private:
	// 属性集
	int** m_attListIndex;
	// 删除属性集
	void DeletAttIndexList(int ** attIndexList, int attM , int attN);
public:
	// 初始化样本数据，保存数据，属性集，等信息
	void SampleDataInitialization(double ** sampleData, int row, int line);
private:
	// 累计元组个数
	int m_tuplesI;
	// 轮盘赌函数
	int GetChromoRoulette(TuplesClassified_SVM* tuples, int tuplesN);
	// 删除新建的样本数据集
	void DeleteSamplingReplacement(TuplesClassified_SVM* tuples);
	// 提升算法迭代次数
	int m_adaBoostingK;
public:
	//// 保存从界面上获得的控制参数
	//void GetControlsParameter(int Tree_SVMHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath);
	// 对决策树EBP剪枝
	bool PostPruneEBP(Tree_SVMNode_SVM* root,TuplesClassified_SVM *tuples,int tuplesN);
private:
	// 计算阶乘
	int CalculateFactorial(int n);
	// 计算置信上限
	double CalculateConfidenceLLimit(int n, int e, double p);
	// 计算子树t下叶节点的预测错误样本率之和
	double ClaculateLeafPreErrorsADD(Tree_SVMNode_SVM* root);
	// 遍历子树叶子节点
	void TraverseChildTree_SVM(Tree_SVMNode_SVM * root, double LeafPreErrorsADD);
	// 计算子树叶子节点的预测错误率之和
	double m_leafPreErrorsADD;
	// 计算子树树高
	int GetChildTree_SVMHeight(Tree_SVMNode_SVM* root);
	// 记录子树高
	int m_childTree_SVMHeight;
	// 子树的叶子个数
	int m_childTree_SVMLeafs;
	// 子树的节点总数
	int m_childTree_SVMNode_SVMs;
	// 获得预测错分样本数
	double CalculatePreErrors(Tree_SVMNode_SVM* root,TuplesClassified_SVM * tuples,int tuplesN);
public:
	// 删除树结构
	void DeleteTree_SVM(Tree_SVMNode_SVM* root);
	// 获得分类规则
	int GetClassRules(Tree_SVMNode_SVM* root);
public:
	// 中序遍历获取规则集
	//bool InorderForRules(Tree_SVMNode_SVM* root,int tag/*记录当前点为左还是右*/,CString stRules/*记录节点之前的规则*/);
private:
	// 记录规则的变量，每次使用都要初始化
	//	CString m_strRules;
	// 使用栈记录节点的规则
	//CString* m_stackForNodeRules;
	// 标记栈顶
	int m_stackForNodeRulesTop;

public:
	// 测试精度
	//TuplesClassified_SVM* MeasuringAccuracyMatch(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
private:
	// 用中序遍历树的方法测试精度，被函数MeasuringAccuracy调用
	void InfoOrderForMeasuring(Tree_SVMNode_SVM* root,TuplesClassified_SVM* MeasuringTuples,int tuplesNum);
	// 测试元组
	TuplesClassified_SVM* m_measuringTuples;
	//	double MeasuringAccuracy(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
public:
	// 获得测试精度
	//double GetMeasuringAccuracy(int tuplesN,Tree_SVMNode_SVM * root,CString rulesPath);
	// 初始化测试数据
	void TestDataInitialization(double** testData , int row , int line);
	// 打印出元组信息
	//void PrintTuples(TuplesClassified_SVM * tuples, int tuplesN, CString path);
	// 打印样本数据
	///void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified_SVM* tuples, int tuplesN , CString path);
	//// 用于将抽样得到的样本元组替换原来的
	//void TuplesInfoReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	//void PrintSampledatsBySDataIndex(int ** S,int m,int n,CString path);
	// //打印样本数据
	void PrintSampdatas(double** S, int m , int n);
	// 所有叶子数据总和
	int m_allDatas;
	// 多数类总数
	int m_publics;
	// 错误总数
	int m_allErros;
private:
	// 按照树的叶子打乱元组列表的顺序
	void TuplesOrderByTree_SVM(TuplesClassified_SVM* tuples, int tuplesN);
public:
	Tree_SVMNode_SVM* BuildTree_SVM2(double ** sampleData, int row, int line,TuplesClassified_SVM *tuples,int tuplesN,int ** attIndex, int attNum,int attL,bool outPut,int hight,TuplesClassified_SVM *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/,int & progressI/*用于计算进度*/);
	Tree_SVMNode_SVM* BuildTree_SVM2(double ** sampleData, int row, int line,TuplesClassified_SVM *tuples,int tuplesN,int ** attIndex, int attNum,int attL,bool outPut,int hight,TuplesClassified_SVM *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/);
private:
	// 树叶子大小
	int m_leafsSize;
public:
	// 通过元组列表打印样本数据
	void PrintSampleDataByTuples(TuplesClassified_SVM* tuples, int tuplesN);
	// 获得决策树模型集
	Tree_SVMSelection_SVM* GetTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs, int Tree_SVMN);
	// 删除树模型选择集
	void DeleteTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs);
private:
	// 决策树模型集
	Tree_SVMSelection_SVM* m_Tree_SVMSelection_SVM;
public:
	//// 提取adaboosting之后的规则
	//int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum);
	//// 样本数据分割为训练集和测试集
	//void SampleDataDataPartition(double** sampleData, int row, int line);
	//// 产生不重复随机数
	//int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
private:
	//// 获得一定范围内不重复的随机数
	//int NonredundantRandom(int leftLimit, int rightLimit, int randomN);
	//// 需要的互相不重复随机数的个数
	//int m_randomNum;
	//// 记录随机数组
	//int* m_random;
	//// 样本率
	//double m_sampleDataRatio;
public:
	// 获得规则集
	//int GetintRuleSet(Tree_SVMSelection_SVM * Tree_SVMs, int Tree_SVMsN,CString rulesPath);
private:
	// 遍历树，获取规则集
	//bool InorderForRuleSet(Tree_SVMNode_SVM* root, int tag, double erroratio,CString stRules,int rulesN);
	// 判断条件的个数
	int m_ruleCount;
	// 记录最终得到的规则集
	//CString* m_ruleSet;
	// 计数器
	int m_ruleSetI;
	// 规则集文件路径
	//CString m_ruleFilePath;
	// 训练元组的个数
	int m_tringDataN;
	int* m_quede;

public:
	// 获得构建树的参数
	//	void SetParameter(int Tree_SVMHeight, int leafsSize, CString rulePath);
	//获得数据
	void GetData(double ** sampleData, int row, int line,int ** attIndex);
	//// 开始建立树
	//void doBuildTree_SVM(TuplesClassified_SVM *tuples,int tuplesN,int & progressI);
	//void doBuildTree_SVM(TuplesClassified_SVM *tuples,int tuplesN);
	// 获得树的根节点
	Tree_SVMNode_SVM* GetTree_SVMRoot(void);
private:
	// 记录树的根部节点
	Tree_SVMNode_SVM* m_Tree_SVMRoot;
public:
	// 测试
	bool tagT;
	// 参数归零
	void doVariableToZero(void);
	// 控制进度条
	//void SetProgressBar(CProgressCtrl*  progressBar);
private:
	// 进度条控制
	//CProgressCtrl* m_progressBar;
	// 为节点赋值
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
	// 所有叶子数据总和
	int m_allDatas;
	// 多数类总数
	int m_publics;
	// 错误总数
	int m_allErros;
	int m_erroNum;
private:
	// 用于计算树高
	int m_k;
	// 记录最佳索引
	int m_splitPointAttIndex;
public:

	TuplesClassified_SVM *m_tuplesInformation;
	// 初始化记录元组信息的数组
	void TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/);
public:
	// 清除记录元组信息的数组
	void TuplesInformationDelete(TuplesClassified_SVM *tuples);
private:
	// 记录元组信息的数量
	int m_tuplesNum;
public:
	// 进行AdaBoosting提升
	void doAdaBoosting(TuplesClassified_SVM * tuples, int tuplesN, int k,int & progressI);
	void doAdaBoosting_SVM_adsvm(TuplesClassified_SVM * tuples, int tuplesN, int k,int & progressI);
	// 有放回抽样，返回样本
	TuplesClassified_SVM* SamplingReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	TuplesClassified_SVM* SamplingReplacement_SVM(TuplesClassified_SVM* tuples, int tuplesN,double *targetclassId);
	// 获得整个样本数据
	double** GetSampleDatas(double** sampledates, int m , int n);
private:
	// 记录整个样本
	double** m_sampleDatas;
	double** m_testDatas;
	int m_testDatasRow;
	int m_testDatasLine;
	// 样本数据行
	int m_sampleDatasRow;
	// 样本数据列
	int m_sampleDatasLine;
public:
	// 获得样本数据索引集
	int ** GetSampleDataIndex(TuplesClassified_SVM* tuples,int tuplesN);
	// 删除样本索引集
	void DeleteSampleDataIndex(TuplesClassified_SVM* tuples);
private:
	// 样本数据索引集行
	int m_tuplesRow;
	// 样本数据索引集列
	int m_tuplesLine;
	// 样本数据索引集行
	int m_sampleDataIndexRow;
	// 样本数据索引集列
	int m_sampleDataIndexLine;
	// 获得属性集,attD表示属性集的维数
	int ** GetAttIndexList(int attNum, int attD);
	// 属性集
	int** m_attListIndex;
	// 删除属性集
	void DeletAttIndexList(int ** attIndexList, int attM , int attN);
public:
	// 初始化样本数据，保存数据，属性集，等信息
	void SampleDataInitialization(double ** sampleData, int row, int line);
	void SampleDataInitialization_SVM(double ** sampleData, int row, int line);

private:
	// 累计元组个数
	int m_tuplesI;
	// 轮盘赌函数
	int GetChromoRoulette(TuplesClassified_SVM* tuples, int tuplesN);
	// 删除新建的样本数据集
	void DeleteSamplingReplacement(TuplesClassified_SVM* tuples);
public:
	// 提升算法迭代次数
	int m_adaBoostingK;
	svm_parameter * m_svm_parameter;
public:
	// 保存从界面上获得的控制参数
	//void GetControlsParameter(int Tree_SVMHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath);
	//计算错误个数
	int CalculateErrors(Tree_SVMNode_SVM* root, double ** SampleDatas, int m , int n,TuplesClassified_SVM* tuples,int tuplesN);
	int CalculateErrors_SVM(svm_model* model,svm_problem* prob,double * targetClassid);
	//计算错误率
	double ErroRatioForModel(TuplesClassified_SVM * tuplesInfmation, int tuplesNum);
	double AdaBoosting_SVM_adsvm::ErroRatioForModel_SVM(TuplesClassified_SVM * tuplesInfmation, int tuplesNum);
private:

	int GetClassRules(Tree_SVMNode_SVM* root);

public:
	//// 中序遍历获取规则集
	//bool InorderForRules(Tree_SVMNode_SVM* root,int tag/*记录当前点为左还是右*/,CString stRules/*记录节点之前的规则*/);
private:
	// 记录规则的变量，每次使用都要初始化
	//CString m_strRules;
	// 使用栈记录节点的规则
	//CString* m_stackForNodeRules;
	// 标记栈顶
	int m_stackForNodeRulesTop;

public:
	// 测试精度
	//TuplesClassified_SVM* MeasuringAccuracyMatch(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
private:
	// 用中序遍历树的方法测试精度，被函数MeasuringAccuracy调用
	//void InfoOrderForMeasuring(Tree_SVMNode_SVM* root,TuplesClassified_SVM* MeasuringTuples,int tuplesNum);
	// 测试元组
	TuplesClassified_SVM* m_measuringTuples;
	//double MeasuringAccuracy(TuplesClassified_SVM* MeasuringTuples,int tuplesNum,Tree_SVMNode_SVM * root,CString rulesPath);
public:
	// 获得测试精度
	//double GetMeasuringAccuracy(int tuplesN,Tree_SVMNode_SVM * root,CString rulesPath);
	// 初始化测试数据
	void TestDataInitialization(double** testData , int row , int line);
	// 打印出元组信息
	//void PrintTuples(TuplesClassified_SVM * tuples, int tuplesN, CString path);
	// 打印样本数据
	//void PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified_SVM* tuples, int tuplesN , CString path);
	// 用于将抽样得到的样本元组替换原来的
	void TuplesInfoReplacement(TuplesClassified_SVM* tuples, int tuplesN);
	//void PrintSampledatsBySDataIndex(int ** S,int m,int n,CString path);
	// //打印样本数据
	void PrintSampdatas(double** S, int m , int n);


public:

	// q权重归一化
	void WeightNormalization(TuplesClassified_SVM* tuples, int tuplesN,double errorRatio);
	void ReplaceWeightOnTupleList(TuplesClassified_SVM* Di, int tuplesN);
	// 初始化元组列表权重
	void TuplesWeightInitialization(TuplesClassified_SVM* tuples, int tuplesN);

public:
	// 通过元组列表打印样本数据
	//void PrintSampleDataByTuples(TuplesClassified_SVM* tuples, int tuplesN,CString path);
	// 获得决策树模型集
	Tree_SVMSelection_SVM* GetTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs, int Tree_SVMN);
	// 删除树模型选择集
	void DeleteTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs);
private:
	// 决策树模型集
	Tree_SVMSelection_SVM* m_Tree_SVMSelection_SVM;
public://double ** sampleData, int row, int line, int * attIndex, int attNum
	// 提取adaboosting之后的规则//
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI);//有进度条
	int AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum);
	// 样本数据分割为训练集和测试集
	void SampleDataDataPartition(double** sampleData, int row, int line);
	// 产生不重复随机数
	int * GetNonredundantRandom(int leftLimit, int rightLimit,int randomN);
private:
	// 获得一定范围内不重复的随机数
	int NonredundantRandom(int leftLimit, int rightLimit, int randomN);
	// 需要的互相不重复随机数的个数
	int m_randomNum;
	// 记录随机数组
	int* m_random;
	// 样本率
	double m_sampleDataRatio;
public:
	// 获得规则集
	//int GetintRuleSet(Tree_SVMSelection_SVM * Tree_SVMs, int Tree_SVMsN,CString rulesPath);
private:
	// 遍历树，获取规则集
	//bool InorderForRuleSet(Tree_SVMNode_SVM* root, int tag, double erroratio,CString stRules,int rulesN);
	// 判断条件的个数
	int m_ruleCount;
	// 记录最终得到的规则集

	// 计数器
	int m_ruleSetI;

	// 训练元组的个数
	int m_tringDataN;
	int* m_quede;
	int m_leafsSize;

public:
	// 返回复合模型，该函数可以用来重载
	Tree_SVMSelection_SVM * GetCompoundModel(void);
	//获得数据
	void GetData(double ** sampleData, int row, int line,int ** attIndex);
	void run(int & progressI);
	void run_SVM(int & progressI);
	// 设置进度条
	//void SetProgressBar(CProgressCtrl* progressBar);
private:


	// 目标类
	double** m_target_classsId;
	// 记录svm的模型集
	// 记录svm的模型集
	SVM_Selection* m_SVMSelection;
public:
	// 获得纪录SVM的数组
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
	// 存储数据
	double** m_data;
	// 数据行
	int m_dataRow;
	// 数据列
	int m_dataLine;
	// 数据元组，记录元组信息
	TuplesClassified_SVM* m_data_Tuples;
public:
	// 执行分类
	//ClassifiedData_SVM* doClassify(CString * rules,int rulsRow);
	ClassifiedData_SVM* doClassify(Tree_SVMSelection_SVM * Tree_SVMSelection_SVM,int Tree_SVMSum);
	//	ClassifiedData_SVM* doClassify(Tree_SVMSelection_SVM * Tree_SVMSelection_SVM,int Tree_SVMSum,CString & Eror);
	ClassifiedData_SVM* doClassify();
	//预测单个数据
	//bool  DataClassification::Tree_Classfy_1(TreeNode* root,double * classifydata,int &classID);
private:
	// 对元组分类
	//TuplesClassified_SVM* TuplesClassify(TuplesClassified_SVM* Tuples,int tuplesNum,CString * rules,int rulsN);
	//获得单棵树规则
	///CString* GetRuleSet_SingleTree_SVM(Tree_SVMSelection_SVM * Tree_SVMs, int Tree_SVMi/*树的编号*/);
	//bool InorderForRuleSet(Tree_SVMNode_SVM* root, int tag, double erroratio,CString stRules,int rulesN);
	// 规则存储数组
	//CString* m_ruleSet;
	// 规则数
	int m_ruleSetI;
	int m_ruleCount;
private:
	// 加权投票表决
	int WeightedVoting(ClassifiedData_SVM dataWaitingVote);
	// 交换
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
	// 获取分类之后的数据的类别，按照数据的序号返回
	int* GetClassifiedID(void);
private:
	// 记录分类数据类
	ClassifiedData_SVM *m_classData;
public:
	// 使用SVM分类
	short* doClassify_SVM(SVM_Selection * svmselection,int svmnum,int &progressI/*控制进度条*/);
private:
	// 填充用于分类的数据
	DataFilling_adsvm* m_FilledData_SVM;
public:
	// 用于单个数据的分类
	short doClassify_SVM_Single(svm_node * pre,int data_line,SVM_Selection *svmselection,int svmNum);
	short doClassify_SVM_Single(double* data,int data_line,SVM_Selection *svmselection,int svmNum);
};



class GLC_Adaboosting_SVM
{
public:
	GLC_Adaboosting_SVM(void);
	~GLC_Adaboosting_SVM(void);
	// 训练adaSVM模型
	bool G_adaSVM_ExecutTraining(double** traindata,int traindatrow,int traindatline,int adboostingK/*=10*/);
private:
	// adaSvm模型
	SVM_Selection *m_svm_modles;
public:
	// 获得adaSVM模型
	SVM_Selection * G_adaSVM_GetModel(void);
	short * G_adaSVM_ExecutClassify(double **adata,int row,int line,int& progressI );
	short * G_adaSVM_ExecutClassify_Single(double **adata,int row,int line,int& progressI );
private:
	// 迭代次数
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
	// 执行分类
	short* GLCSVM_Excute_Classify(double* data, int data_row, int data_line);
	// 清除数据
	bool GLCSVM_Clear(void);
private:
	// svm对象
	GLC_Adaboosting_SVM m_Gsvm;
	// svm的个数
	int m_svmNum;
	// 记录分类之后的id
	short* m_classid;
	svm_parameter* m_svm_parameter;
public:
	void GLCSVM_Set_Parameter(svm_parameter* parameter);
};

void quicksort(TuplesClassified_SVM a[], int left, int right);
int partition(TuplesClassified_SVM data[],int lo,int hi);
void QuickSort(TuplesClassified_SVM a[], int lo, int hi);