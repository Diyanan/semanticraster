#include "StdAfx.h"
#include "AdaBoosting.h"
#include "Tree.h"
#include <cmath>
#include <set>
#include <ctime>

AdaBoosting::AdaBoosting(void)
	: m_treeHeight(0)
	, m_Dimension0(0)
	, m_Dimension1(0)
	, m_Dimension2(0)
	, m_Dimension(0)
	, m_NodeSize(0)
	, m_k(1)
	, m_splitPointAttIndex(0)
	, m_erroNum(0)
	
	, m_tuplesInformation(NULL)
	, m_tuplesNum(0)
	, m_sampleDatas(0)
	, m_sampleDatasRow(0)
	, m_sampleDatasLine(0)
	, m_tuplesRow(0)
	, m_tuplesLine(0)
	, m_sampleDataIndexRow(0)
	, m_sampleDataIndexLine(0)
	, m_attListIndex(NULL)
	, m_tuplesI(0)
	, m_adaBoostingK(0)

	
	, m_stackForNodeRulesTop(0)
	, m_measuringTuples(NULL)
	, m_allDatas(0)
	, m_publics(0)
	, m_allErros(0)
	, m_leafsSize(0)
	, m_treeSelection(NULL)
	, m_randomNum(0)
	, m_random(NULL)
	, m_sampleDataRatio(0)
	, m_ruleCount(0)
	
	, m_ruleSetI(0)	
	, m_tringDataN(0)
	, m_quede(NULL)
{
}


AdaBoosting::~AdaBoosting(void)
{
}
TuplesClassified* AdaBoosting::SamplingReplacement(TuplesClassified* tuples, int tuplesN)
{
	//生成新的元组数组，用于记录新生成的样数据

	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	int seeds=(int)time(NULL);//st.wMilliseconds+st.wSecond*60+st.wMinute*60;
	srand(seeds);           //随机数的种子
	TuplesClassified* tempTuples;
	tempTuples=new TuplesClassified[tuplesN];
	int tempIndex=0;
	for (int i=0;i<tuplesN;i++)
	{	
		tempIndex=GetChromoRoulette(tuples,tuplesN);
		tempTuples[i].i=tuples[tempIndex].i;
		tempTuples[i].weight=tuples[tempIndex].weight;
		tempTuples[i].err=tuples[tempIndex].err;
		tempTuples[i].sampleDataIndex=tuples[tempIndex].sampleDataIndex;
		tempTuples[i].tuplesClassID=tuples[tempIndex].tuplesClassID;		
	}
	return tempTuples;
}
int AdaBoosting::CalculateErrors(TreeNode* root, double ** SampleDatas, int m , int n,TuplesClassified* tuples,int tuplesN)
{
	int erros=0;
	for (int i=0;i<tuplesN;i++)
	{
		if (tuples[i].err==1)
		{
			erros++;
		}

	}

	m_erroNum=0; m_tuplesI=0;m_allErros=0; //初始化
	
	return erros;
}
double AdaBoosting::ErroRatioForModel(TuplesClassified * tuplesInfmation, int tuplesNum)
{
	double errorM=0,temp;
	for (int i=0;i<tuplesNum;i++)
	{
		temp=tuplesInfmation[i].weight*tuplesInfmation[i].err;
		errorM=errorM+temp;
	}
	return errorM;
}
void AdaBoosting::doAdaBoosting(TuplesClassified * tuples, int tuplesN, int k,int & progressI)
{
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	int seeds=(int)time(NULL);//int seeds=st.wMilliseconds+st.wSecond*60+st.wMinute*60;
	srand(seeds);           //随机数的种子

	TuplesClassified **tuplesArr;       //用于记录每次分类之后的元组
	tuplesArr=new TuplesClassified *[k];
	TreeSelection *qualifiedTrees;         //用于记录复合条件的树
	qualifiedTrees=new TreeSelection [k]; 


	int txtnum=0;

	
	
	for (int i=0;i<k;i++)
	{
		Tree singalTree;       
		TuplesClassified * Di;	
		if(i==0)           
		 {
			 Di=new TuplesClassified[tuplesN];
			 for (int ii=0;ii<tuplesN;ii++)
			 {
				 Di[ii].err=m_tuplesInformation[ii].err;
				 Di[ii].i=m_tuplesInformation[ii].i;
				 Di[ii].sampleDataIndex=m_tuplesInformation[ii].sampleDataIndex;
				 Di[ii].tuplesClassID=m_tuplesInformation[ii].tuplesClassID;
				 Di[ii].weight=m_tuplesInformation[ii].weight;
			 }		
		
		 }
		 else
		 {
		 
			Di=SamplingReplacement(m_tuplesInformation/*初始样本元组*/,tuplesN);	
		 }

		 double ** sampleData;              
		 sampleData=singalTree.GetSampleDatas(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine);
	     int ** sampleDataIndex;               
	
		 sampleDataIndex=singalTree.GetSampleDataIndex(Di,tuplesN);

		 int ** attIndexList=m_attListIndex;                  
	
	                                      
		 m_Dimension2=0;m_k=0; m_NodeSize=0;m_Dimension0=0;
		 m_publics=0;m_tuplesI=0; m_allErros=0;m_erroNum=0;
	                                        
		
		 singalTree.GetData(sampleData,m_sampleDatasRow,m_sampleDatasLine,attIndexList);
		 singalTree.GetTreeParameter(m_treeHeight,m_leafsSize);//GetControlsParameter(m_treeHeight,m_adaBoostingK,m_leafsSize,m_sampleDataRatio);
  		 singalTree.doBuildTree(Di,tuplesN,progressI);
		 m_NodeSize=singalTree.m_NodeSize;     
		 if (m_NodeSize==0)
		 {


		 }
		 TreeNode* diTree=singalTree.GetTreeRoot();
		 if (m_NodeSize==0)
		 {


		 }
	
	
		 int errosNum=CalculateErrors(diTree/*数的根节点*/,sampleData,m_sampleDatasRow,m_sampleDatasLine,Di,tuplesN);  
		 double errorRatio=ErroRatioForModel(/*m_tuplesInformation*/Di,m_tuplesNum);                                    
		 while(errorRatio>0.5||errorRatio==0)
		 {
			 singalTree.DeleteTree(diTree);
			 TuplesWeightInitialization(m_tuplesInformation,tuplesN);
			 TuplesInformationDelete(Di);
			 if(i==0)          
			 {
				 Di=new TuplesClassified[tuplesN];
				 for (int ii=0;ii<tuplesN;ii++)
				 {
					 Di[ii].err=m_tuplesInformation[ii].err;
					 Di[ii].i=m_tuplesInformation[ii].i;
					 Di[ii].sampleDataIndex=m_tuplesInformation[ii].sampleDataIndex;
					 Di[ii].tuplesClassID=m_tuplesInformation[ii].tuplesClassID;
					 Di[ii].weight=m_tuplesInformation[ii].weight;
				 }  	
			 }
			 else
			 {
		 
				Di=SamplingReplacement(m_tuplesInformation/*初始样本元组*/,tuplesN);
		
			 }
			 singalTree.doVariableToZero();          
			 double ** sampleData;                  
			 sampleData=singalTree.GetSampleDatas(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine);


			 int ** sampleDataIndex;                
			 sampleDataIndex=singalTree.GetSampleDataIndex(Di,tuplesN);

			 int ** attIndexList=m_attListIndex;;                 



			 m_Dimension2=0;m_k=0; m_NodeSize=0;m_Dimension0=0;
			 m_publics=0;m_tuplesI=0; m_allErros=0;m_erroNum=0;

			 singalTree.doBuildTree(Di,tuplesN);
			 m_NodeSize=singalTree.m_NodeSize;     
			 diTree=singalTree.GetTreeRoot();

			 if (m_NodeSize==0)
			 {

			 }

  
			 errorRatio=ErroRatioForModel(/*m_tuplesInformation*/Di,m_tuplesNum); 
		 }
		double ttt=errorRatio;

		int caoerrors=0;
	
		for (int ii=0;ii<tuplesN;ii++)
		{
			if (Di[ii].err==1)
			{
				caoerrors++;
			}		 
		}

	
		 double replaceFactor=(1-errorRatio)/errorRatio;
		 double weightAdd=0;
		 for (int ii=0;ii<tuplesN;ii++)
		 {
			 if (Di[ii].err==1)
			 {
				 int index=Di[ii].i;
				 m_tuplesInformation[index].weight=Di[ii].weight*replaceFactor;
			 }
			 else
			 {
				 int index=Di[ii].i;
				 m_tuplesInformation[index].weight=Di[ii].weight;
			 }
		
		 }
		 for (int ii=0;ii<tuplesN;ii++)
		 {
			  weightAdd=weightAdd+m_tuplesInformation[ii].weight;
		 }

		 //归一化
		 for (int ii=0;ii<tuplesN;ii++)
		 {
			 m_tuplesInformation[ii].weight=m_tuplesInformation[ii].weight/weightAdd;
		 }
		 double caobi=0;  
		 for (int ii=0;ii<tuplesN;ii++)
		 {
			 caobi=m_tuplesInformation[ii].weight+caobi;
		 }
		 TuplesInformationDelete(Di); 
	 
		 qualifiedTrees[i].Tree=diTree;
		 qualifiedTrees[i].erroratio=errorRatio;
		 qualifiedTrees[i].leafs=m_NodeSize;//记录叶子树；
		 qualifiedTrees[i].publicClssId=diTree->publicClassID;

	
	  
	 }
	 delete [] m_tuplesInformation;
	 
	 double ticketWeight=0;
	 double treeErroratio=0;
	 double aa=0,bb=0;
	 int nodes=0;
	 for (int i=0;i<k;i++)
	 {
		 treeErroratio=qualifiedTrees[i].erroratio;
		 aa=1-treeErroratio;
		 bb=treeErroratio;
		 ticketWeight=log((aa/bb));
		 qualifiedTrees[i].weight=ticketWeight;

		 
	 }
	 m_treeSelection=qualifiedTrees;
 	
	
}

void AdaBoosting::TuplesInformationDelete(TuplesClassified *tuples)
{
	delete [] tuples;
}


int AdaBoosting::GetChromoRoulette(TuplesClassified* tuples, int tuplesN)
{
	int indexChoosed=0;
	
	int m=rand();
	double Slice=(double)std::rand()/(double)RAND_MAX;
	
	double FitnessSoFar=0;
	for (int i=0;i<tuplesN;i++)
	{
		
		FitnessSoFar=FitnessSoFar+tuples[i].weight;
		
		if (FitnessSoFar>=Slice)
		{
			indexChoosed=i;
			break;
		}
	}

	return indexChoosed;
}
void AdaBoosting::TuplesWeightInitialization(TuplesClassified* tuples, int tuplesN)
{
	for (int i=0;i<tuplesN;i++)
	{
		tuples[i].err=0;
		tuples[i].weight=1/(tuplesN*1.0);
	}
}


TreeSelection * AdaBoosting::GetCompoundModel(void)
{
	TreeSelection *m=m_treeSelection;
	return m;
}

void AdaBoosting::GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio)
{
	m_treeHeight=treeHeight;
	m_adaBoostingK=adaBoostingK;
	m_leafsSize=leafsSize;
	m_sampleDataRatio=sampleDataRatio;
	
}

void AdaBoosting::GetData(double ** sampleData, int row, int line,int ** attIndex)
{
	m_sampleDatas=sampleData;
	m_sampleDatasRow=row;
	m_sampleDatasLine=line;
	m_attListIndex=attIndex;

}
void AdaBoosting::SampleDataInitialization(double ** sampleData, int row, int line)
{
	m_sampleDatas=sampleData;
	m_sampleDatasRow=row;
	m_sampleDatasLine=line;

	int n=m_sampleDatasLine-1;//参与分列的属性个数
	int** attListIndex;               //建立属性集
	attListIndex=new int* [n];  
	for (int i=0;i<n;i++)
	{
		attListIndex[i]=new int [2];
	}
	for (int i=0;i<n;i++)
	{
		attListIndex[i][0]=i;
		attListIndex[i][1]=0;
	}
	m_attListIndex=attListIndex;


	int** spDataIndex;
	spDataIndex=new int* [row];  
	for (int i=0;i<row;i++)
	{
		spDataIndex[i]=new int [2];
	}

	for (int i=0;i<row;i++)
	{
		spDataIndex[i][0]=i;                     //元组在样本中的索引
		spDataIndex[i][1]=sampleData[i][line-1]; //记录的是元组的类别号
	}
	
	TuplesInformationInitial(row/*元组的个数*/,spDataIndex/*样本索引*/,row/*行*/,2/*列，一般用2*/);
	
}
void AdaBoosting::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
{

	m_tuplesInformation=new TuplesClassified[tuplesNum];
	m_tuplesNum=tuplesNum;
	for (int i=0;i<tuplesNum;i++)
	{
		m_tuplesInformation[i].i=i;
		m_tuplesInformation[i].err=0;
		m_tuplesInformation[i].tuplesClassID=sampDataIndex[i][n-1];//n-1列数据记录样本数据的类别标记
		m_tuplesInformation[i].sampleDataIndex=sampDataIndex[i][0];//0列数据记录样本数据中的索引
		m_tuplesInformation[i].weight=1.0/(tuplesNum*1.0);
	}
}
void AdaBoosting::TestDataInitialization(double** testData , int row , int line)
{

	m_testDatas=testData;
	m_testDatasRow=row;
	m_testDatasLine=line;
}
void AdaBoosting::run(int & progressI)
{

	doAdaBoosting(m_tuplesInformation,m_tuplesNum,m_adaBoostingK,progressI);
}
