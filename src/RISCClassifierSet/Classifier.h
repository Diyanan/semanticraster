
enum { RISC_CTree=1, RISC_SVM=2, RISC_ANN=3, RISC_KNN=4, RISC_Bayes=5 };
typedef struct GLCTreeParameter  
{
	int TreeHeight;
	int LeatfSize;
	
}TreeParameter;
struct glc_ann_parameter
{	
	int connectinRate;                  //���Ӳ���
	float learningRate;             //ѧϰ��
	float desiredError;            //��ֹ������
	int iterationBetweenRepots;       //������
	int maxepoches;                  //����������
	int layes;                          //����Ĳ���
	int hidNum;                        //������Ԫ�ĸ���
	int inNum;	
	int outNum;                       //�����Ԫ�ĸ���
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

	  virtual ~RISC_Classifier(void){}; //ע�⣬��ö�������������������������಻���������������������������Ϊ�������������������������壬��Ϊ�����������ø�����������
	 //��������ģ��
	 int Create();
	 //���ļ��л�ȡ����
	 int LoardSampleFile(const char * samplesetFile);
	 int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	 //���ò���
	 void SetParameter(const char* parameter);
	 //ѵ������
	 void ExcuteTrain();
	 //ִ�з���
	 int ExcuteClassify(double data[]);
	 //ɾ��ģ��
	 int  Destroy();

	//virtual ~RISC_Classifier(void)=0{}; //ע�⣬��ö�������������������������಻���������������������������Ϊ�������������������������壬��Ϊ�����������ø�����������
	////��������ģ��
	//virtual int Create()=0;
	////���ļ��л�ȡ����
	// int LoardSampleFile(const char * samplesetFile);
	// int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	////���ò���
	//virtual void SetParameter(const char* parameter)=0;
	////ѵ������
	//virtual void ExcuteTrain()=0;
	////ִ�з���
	//virtual int ExcuteClassify(double data[])=0;
	////ɾ��ģ��
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
	 //���ļ��л�ȡ����
	 //void LoardSampleFile(const char * samplesetFile);
	 //���ò���
	 void SetParameter(const char* parameter);
	 //ѵ������
	 void ExcuteTrain();
	 //ִ�з���
	 int ExcuteClassify(double data[]);
	 //ɾ��ģ��
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
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(const char* parameter);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
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
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(int treeHeigh,int leefSize);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
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
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(
		int inNum=14,                      //���뵥Ԫ�ĸ��������ڷ������������
		int outNum=6,                      //�����Ԫ�ĸ���,������ĸ���		
		int hidNum=8,                        //������Ԫ�ĸ���	������ֵ����Ҫ������֤
		int layes=3,                          //����Ĳ���
		int connectinRate=1,              //���Ӳ���
		float learningRate=0.007f,             //ѧϰ��
		float desiredError=0.0001f,          //��ֹ������
		int iterationBetweenRepots=100,       //������
		int maxepoches=5000                  //����������

		);       
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	int ExcuteClassify(float data[]);
	//ɾ��ģ��
	int  Destroy();
};
class RISC_Classifier_SVM :
	public RISC_Classifier
{

public:
	RISC_Classifier_SVM(void);
	~RISC_Classifier_SVM(void);

	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(svm_parameter para_svm);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
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
//	//���ò���	
//	void SetParameter(TreeParameter para_tree,glc_ann_parameter para_ann,svm_parameter para_svm);    
//	//�����ӷ�����	
//	void SetSubClassifiers(int *subClassifier,int nSubClassifier);
//	//���ò�����������
//	void SetTestData(double ** testData,int testDataRow,int testDataLine);
//	//ѵ������
//	void ExcuteTrain();
//	//ִ�з���
//	int ExcuteClassify(double data[]);
//	//ɾ��ģ��
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
	//���ò���	
	void SetParameter(TreeParameter para_tree,glc_ann_parameter para_ann,svm_parameter para_svm);    
	//�����ӷ�����	
	void SetSubClassifiers(int *subClassifier,int nSubClassifier);
	//���ò�����������
	void SetTestData(double ** testData,int testDataRow,int testDataLine);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
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
	//���ò���	
	void SetParameter(TreeParameter para_tree,glc_ann_parameter para_ann,svm_parameter para_svm);    
	//�����ӷ�����	
	void SetSubClassifiers(int *subClassifier,int nSubClassifier);
	//���ò�����������
	void SetTestData(double ** testData,int testDataRow,int testDataLine);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();
};
class RISC_Classifier_JC1 :
	public RISC_Classifier
{

public:
	RISC_Classifier_JC1(void);
	~RISC_Classifier_JC1(void);

	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);

	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();	
};
class RISC_Classifier_JC2 :
	public RISC_Classifier
{

public:
	RISC_Classifier_JC2(void);
	~RISC_Classifier_JC2(void);

	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);

	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
	int  Destroy();	
};
class RISC_Classifier_AdTree :
	public RISC_Classifier
{

public:
	RISC_Classifier_AdTree(void);
	~RISC_Classifier_AdTree(void);
	int Create();
	//���ļ��л�ȡ����
	//void LoardSampleFile(const char * samplesetFile);
	//���ò���
	void SetParameter(int treeHeigh/*����*/,int leefSize/*Ҷ��������������*/,int adboostingK/*Adboosting��������*/);
	//ѵ������
	void ExcuteTrain();
	//ִ�з���
	int ExcuteClassify(double data[]);
	//ɾ��ģ��
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
/* �������۽ӿ�                                                         */
/************************************************************************/
void SetClassfyTag(int *testTag,int nTest/*�������ݵĸ���*/,int classifierCount/*���÷�������������*/);                      //���÷��������ǩ
void AddClassifiedResult(short *classifiedID);     //��ӷ������ķ�����
int	CreateAccuracy();                              
int GetClassCount();                               //������ĸ���
int GetClassifierCount();                          //��÷������ĸ���
void GetClassifierCofusionMatrix_1D(int classifierIndex/*����������*/,int* confusionMatrix_1d);//�������������Ļ�������ת����һά���鵼��Ԫ�ظ���ΪnClass*nClass
double GetKappa();//���㿨��ϵ��


