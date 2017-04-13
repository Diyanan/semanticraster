
enum { RISC_CTree=1, RISC_SVM=2, RISC_ANN=3, RISC_KNN=4, RISC_Bayes=5 };
typedef struct GLCTreeParameter  
{
	int TreeHeight;
	int LeatfSize;
	
}TreeParameter;
struct glc_ann_parameter
{	
	int connectinRate;                  //链接参数
	float learningRate;             //学习率
	float desiredError;            //终止错误率
	int iterationBetweenRepots;       //报告间隔
	int maxepoches;                  //最大迭代次数
	int layes;                          //网络的层数
	int hidNum;                        //隐藏神经元的个数
	int inNum;	
	int outNum;                       //输出单元的个数
};
struct svm_parameter
{
	int svm_type;
	int kernel_type;
	int degree;	/* for poly */
	double gamma;	/* for poly/rbf/sigmoid */
	double coef0;	/* for poly/sigmoid */

	/* these are for training only */
	double cache_size; /* in MB */
	double eps;	/* stopping criteria */
	double C;	/* for C_SVC, EPSILON_SVR and NU_SVR */
	int nr_weight;		/* for C_SVC */
	int *weight_label;	/* for C_SVC */
	double* weight;		/* for C_SVC */
	double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
	double p;	/* for EPSILON_SVR */
	int shrinking;	/* use the shrinking heuristics */
	int probability; /* do probability estimates */
};

class RISC_Classifier
{
protected:
	
public:
	 RISC_Classifier(void);

	  virtual ~RISC_Classifier(void){}; //注意，最好定义此虚析构函数，够避免子类不能正常调用析构函数；如果定义为纯虚析构函数，则必须带定义体，因为子类隐含调用该析构函数。
	 //创建分类模型
	 int Create();
	 //从文件中获取样本
	 int LoardSampleFile(const char * samplesetFile);
	 int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	 //设置参数
	 void SetParameter(const char* parameter);
	 //训练样本
	 void ExcuteTrain();
	 //执行分类
	 int ExcuteClassify(double data[]);
	 //删除模型
	 int  Destroy();

	//virtual ~RISC_Classifier(void)=0{}; //注意，最好定义此虚析构函数，够避免子类不能正常调用析构函数；如果定义为纯虚析构函数，则必须带定义体，因为子类隐含调用该析构函数。
	////创建分类模型
	//virtual int Create()=0;
	////从文件中获取样本
	// int LoardSampleFile(const char * samplesetFile);
	// int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	////设置参数
	//virtual void SetParameter(const char* parameter)=0;
	////训练样本
	//virtual void ExcuteTrain()=0;
	////执行分类
	//virtual int ExcuteClassify(double data[])=0;
	////删除模型
	//virtual int  Destroy()=0;

	
};


class  RISC_Classifier_KNN :
	public RISC_Classifier
{
private:
	
public:
	RISC_Classifier_KNN(void);
	//~RISC_Classifier_KNN(void);
	 int Create();
	 //从文件中获取样本
	 //void LoardSampleFile(const char * samplesetFile);
	 //设置参数
	 void SetParameter(const char* parameter);
	 //训练样本
	 void ExcuteTrain();
	 //执行分类
	 int ExcuteClassify(double data[]);
	 //删除模型
	 int  Destroy();
	 int m_row;
	 int test(double data[]);
};
class RISC_Classifier_Bayes :
	public RISC_Classifier
{

private:
	

public:
	
	RISC_Classifier_Bayes(void);
	~RISC_Classifier_Bayes(void);
	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(const char* parameter);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();	
};
class RISC_Classifier_Tree :
	public RISC_Classifier
{
private: 
	
public:
	RISC_Classifier_Tree(void);
	~RISC_Classifier_Tree(void);

	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(int treeHeigh,int leefSize);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};
class RISC_Classifier_ANN :
	public RISC_Classifier
{
private: 
	
public:
	RISC_Classifier_ANN(void);
	~RISC_Classifier_ANN(void);
	/************************************************************************/
	/*                                                         */
	/************************************************************************/

	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(
		int inNum=14,                      //输入单元的个数，用于分类的特征个数
		int outNum=6,                      //输出单元的个数,输出类别的个数		
		int hidNum=8,                        //隐藏神经元的个数	，经验值，需要网格验证
		int layes=3,                          //网络的层数
		int connectinRate=1,              //链接参数
		float learningRate=0.007f,             //学习率
		float desiredError=0.0001f,          //终止错误率
		int iterationBetweenRepots=100,       //报告间隔
		int maxepoches=5000                  //最大迭代次数

		);       
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	int ExcuteClassify(float data[]);
	//删除模型
	int  Destroy();
};
class RISC_Classifier_SVM :
	public RISC_Classifier
{

public:
	RISC_Classifier_SVM(void);
	~RISC_Classifier_SVM(void);

	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(svm_parameter para_svm);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};
//class RISC_Classifier_VRPM :
//	public RISC_Classifier
//{
//private:
//	
//public:
//	RISC_Classifier_VRPM(void);
//	~RISC_Classifier_VRPM(void);
//	int Create();
//	//设置参数	
//	void SetParameter(TreeParameter para_tree,glc_ann_parameter para_ann,svm_parameter para_svm);    
//	//设置子分类器	
//	void SetSubClassifiers(int *subClassifier,int nSubClassifier);
//	//设置测试样本数据
//	void SetTestData(double ** testData,int testDataRow,int testDataLine);
//	//训练样本
//	void ExcuteTrain();
//	//执行分类
//	int ExcuteClassify(double data[]);
//	//删除模型
//	int  Destroy();
//};

class RISC_Classifier_INT1 :
	public RISC_Classifier
{
private:

public:
	RISC_Classifier_INT1(void);
	~RISC_Classifier_INT1(void);
	int Create();
	//设置参数	
	void SetParameter(TreeParameter para_tree,glc_ann_parameter para_ann,svm_parameter para_svm);    
	//设置子分类器	
	void SetSubClassifiers(int *subClassifier,int nSubClassifier);
	//设置测试样本数据
	void SetTestData(double ** testData,int testDataRow,int testDataLine);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};
class  RISC_Classifier_INT2 :
	public RISC_Classifier
{
private:
	
public:
	RISC_Classifier_INT2(void);
	~RISC_Classifier_INT2(void);
	int Create();
	//设置参数	
	void SetParameter(TreeParameter para_tree,glc_ann_parameter para_ann,svm_parameter para_svm);    
	//设置子分类器	
	void SetSubClassifiers(int *subClassifier,int nSubClassifier);
	//设置测试样本数据
	void SetTestData(double ** testData,int testDataRow,int testDataLine);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};
class RISC_Classifier_JC1 :
	public RISC_Classifier
{

public:
	RISC_Classifier_JC1(void);
	~RISC_Classifier_JC1(void);

	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);

	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();	
};
class RISC_Classifier_JC2 :
	public RISC_Classifier
{

public:
	RISC_Classifier_JC2(void);
	~RISC_Classifier_JC2(void);

	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);

	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();	
};
class RISC_Classifier_AdTree :
	public RISC_Classifier
{

public:
	RISC_Classifier_AdTree(void);
	~RISC_Classifier_AdTree(void);
	int Create();
	//从文件中获取样本
	//void LoardSampleFile(const char * samplesetFile);
	//设置参数
	void SetParameter(int treeHeigh/*树高*/,int leefSize/*叶子容纳样本个数*/,int adboostingK/*Adboosting迭代次数*/);
	//训练样本
	void ExcuteTrain();
	//执行分类
	int ExcuteClassify(double data[]);
	//删除模型
	int  Destroy();
};




RISC_Classifier_ANN * NewClassifier_ANN();
RISC_Classifier_Bayes * NewClassifier_Bayes();
RISC_Classifier_INT1 * NewClassifier_INT1();
RISC_Classifier_INT2 * NewClassifier_INT2();
RISC_Classifier_Tree *NewClassifier_Tree();
RISC_Classifier_KNN  *NewClassifier_KNN();
RISC_Classifier_SVM  *NewClassifier_SVM();
RISC_Classifier_JC1  *NewClassifier_JC1();
RISC_Classifier_JC2  *NewClassifier_JC2();
RISC_Classifier_AdTree  *NewClassifier_AdTree();


int DestroyModel(RISC_Classifier * classifier);


/************************************************************************/
/* 精度评价接口                                                         */
/************************************************************************/
void SetClassfyTag(int *testTag,int nTest/*测试数据的个数*/,int classifierCount/*设置分类器的最多个数*/);                      //设置分类的类别标签
void AddClassifiedResult(short *classifiedID);     //添加分类器的分类结果
int	CreateAccuracy();                              
int GetClassCount();                               //获得类别的个数
int GetClassifierCount();                          //获得分类器的个数
void GetClassifierCofusionMatrix_1D(int classifierIndex/*分类器索引*/,int* confusionMatrix_1d);//将当个分类器的混淆矩阵转换成一维数组导出元素个数为nClass*nClass
double GetKappa();//计算卡巴系数


