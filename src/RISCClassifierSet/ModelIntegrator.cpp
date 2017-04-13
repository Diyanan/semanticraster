#include "StdAfx.h"
#include "ModelIntegrator.h"

#include <cmath>
#include <iostream>
#include <ctime>
using namespace std;


ModelIntegrator::ModelIntegrator(void)
	:
	 m_treeHeight(0)
	, m_tuplesInformation(NULL)
	, m_tuplesNum(0)
	, m_sampleDatas(0)
	, m_sampleDatasRow(0)
	, m_sampleDatasLine(0)
	, m_attListIndex(NULL)
	, m_adaBoostingK(0)
	, m_leafsSize(0)
	, m_treeSelection(NULL)
	, m_randomNum(0)
	, m_random(NULL)
	, m_sampleDataRatio(0)
	, m_ruleCount(0)
	
	, m_ruleSetI(0)
	
	, m_tringDataN(0)
	, m_quede(NULL)
	, m_testDetas(NULL)
	
	
	, m_progressITemp(0)
	 {
		//  m_progressI=
      }


ModelIntegrator::~ModelIntegrator(void)
{
}
void ModelIntegrator::AdaBoostingTrainingRoles(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI)
{//// 样本数据分割为训练集和测试集
	//SampleDataDataPartition(sampleData,row,line);
	////m_adaBoosting.TuplesInformationInitial();
	m_adaBoosting.SampleDataInitialization(sampleData,row,line);//初始化训练数据
	//初始化测试数据
	m_adaBoosting.GetControlsParameter(m_treeHeight,m_adaBoostingK,m_leafsSize,m_sampleDataRatio);
	m_adaBoosting.run(progressI);

}
//void ModelIntegrator::AdaBoostingTrainingRoles1(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI,CString & Eror)//无样本率
//{
//	//// 样本数据分割为训练集和测试集
//	//SampleDataDataPartition(sampleData,row,line);
//	////m_adaBoosting.TuplesInformationInitial();
//	m_adaBoosting.SampleDataInitialization(sampleData,row,line);//初始化训练数据
//	      //初始化测试数据
//
//	m_adaBoosting.SetProgressBar(m_progressBar);
//	m_adaBoosting.GetControlsParameter(m_treeHeight,m_adaBoostingK,m_leafsSize,m_sampleDataRatio,m_ruleFilePath);
//	m_adaBoosting.run(progressI);
//
//
//}
//void ModelIntegrator::OutputRuleSet(CString rulesPath)
//{
//	TreeSelection * trees=m_adaBoosting.GetCompoundModel();
//	int treesN=m_adaBoostingK;
//	m_ruleSet=GetRuleSet(trees,treesN);//分类之后的叶节点最多不要超过1000，超过改此处
//	
//	
//	CStdioFile fl;
//	fl.Open(rulesPath,CFile::modeCreate|CFile::modeWrite/*|CFile::typeText*//*|CFile::typeBinary*/);
//
//	CString temp,Entries,head1,head2,y,m,d,h,s,l;
//	CTime tm;
//	tm=CTime::GetCurrentTime();
//
//	y.Format(_T("%d"),tm.GetYear());
//	m.Format(_T("%d"),tm.GetMonth());
//	d.Format(_T("%d"),tm.GetDay());
//	h.Format(_T("%d"),tm.GetHour());
//	l.Format(_T("%d"),tm.GetMinute());
//	s.Format(_T("%d"),tm.GetSecond());
//	int mmm=1;
//	head1=_T("id=\"GLC\/1.0 ")+y+_T("\/")+m+_T("\/")+d+_T(" ")+h+_T(":")+l+_T(":")+s+_T("\"\r\n");
//	//head1.Format()//id="GLC/1.0 2013/5/7 18:09:01"
//	Entries.Format(_T("%d"),/*m_adaBoostingK*/mmm);
//	head2=_T("entries=\"")+Entries+_T("\"\r\n");//迭代次数
//	fl.WriteString(head1);
//	fl.WriteString(head2);
//	//doReorderToRuleSet(m_ruleSet,m_ruleSetI);
//	for (int i=0;i<m_ruleSetI;i++)
//	{
//		temp=m_ruleSet[i];
//		fl.WriteString(temp);
//	}
//	fl.Close();
//	m_ruleSetI=0;//用完归零
//
//}
//void ModelIntegrator::OutputRuleSet(CString rulesPath,int & ruleRow)
//{
//	
//	m_ruleSetI=0;//用完归零
//
//}
//CString* ModelIntegrator::GetRuleSet(TreeSelection * trees, int treesN)
//{
//	m_ruleSet=new CString[1000];//分类之后的叶节点最多不要超过1000，超过改此处
//	m_ruleSetI=0;
//
//	for (int i=0;i<treesN;i++)
//	{
//
//		int tempruleindex=m_ruleSetI;		//给树规则的开头留下位置
//		m_ruleSetI++;
//		double erroratio=trees[i].erroratio;
//		CString stRootRules=_T("");
//		TreeNode *root=trees[i].Tree;
//		InorderForRuleSet(root,0,erroratio,stRootRules,0);
//
//		CString rules,def,head;
//		CString stii;
//		stii.Format(_T("%d"),i);
//		rules.Format(_T("%d"),m_ruleCount);
//		if (m_ruleCount==1)
//		{
//			
//		}
//
//
//		def.Format(_T("%d"),root->publicClassID);//rules="17" default="2"
//		head=_T("rules=\"")+rules+_T("\" default=\"")+def+/*_T(" i=")+stii+*/_T("\"\r\n");
//		m_ruleSet[tempruleindex]=head;
//		m_ruleCount=0;//用完归零
//	}
//
//	//返回规则集文件
//	
//	//m_ruleSetI=0;//用完归零
//	CString* TT;
//	TT=m_ruleSet;
//	return TT;
//
//}
void ModelIntegrator::SampleDataDataPartition(double** sampleData, int row, int line)
{
	int tringDataN=0;int testDataN=0;
	tringDataN=(int)(row*m_sampleDataRatio)/100;
	testDataN=row-tringDataN;

	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	
	int seeds=(int)time(NULL);//int seeds=st.wMilliseconds+st.wSecond*60+st.wMinute*60;;
	

	srand(seeds);           //随机数的种子
	int * sampleDataIndex;
    sampleDataIndex=GetNonredundantRandom(0,row-1,tringDataN);
	int m=m_randomNum;    //用完归零
	
	double** sDatas;
	sDatas=new double* [tringDataN];  
	for (int i=0;i<tringDataN;i++)
	{
		sDatas[i]=new double [line];
	}
	
	//获得测试数据
	int t=0,s=0;
	double ** tDatas;
	tDatas=new double* [testDataN];

	for (int i=0;i<testDataN;i++)
	{
		tDatas[i]=new double [line];
	}
	//分割训练数据
	for (int i=0;i<tringDataN;i++)
	{
		int tempindex=sampleDataIndex[i];
		for (int j=0;j<line;j++)
		{
			sDatas[i][j]=sampleData[tempindex][j];
		}
	}
	//分割测试数据
	 t=0;
	 int si=0;
	for (int i=0;i<row;i++)
	{
		if (i!=sampleDataIndex[si])
		{
			for (int j=0;j<line;j++)
			{
            tDatas[t][j]=sampleData[i][j];
			}
			t++;
		}
		else
		{
			si++;
		}
		
	}

	/*for (int i=0;i<row;i++)
	{
		
		if (i==sampleDataIndex[t])
		{
			for (int j=0;j<line;j++)
			{
				sDatas[t][j]=sampleData[i][j];
			}
			t++;
		}
		else
		{
			for (int j=0;j<line;j++)
			{
				tDatas[s][j]=sampleData[i][j];
			}
			s++;
		}
	}*/

   /*int** spDataIndex;
   spDataIndex=new int* [row];  
   for (int i=0;i<row;i++)
   {
	   spDataIndex[i]=new int [2];
   }
   int sprow=0;
   for (int i=0;i<row;i++)
   {
	   spDataIndex[i][0]=i;
	   spDataIndex[i][1]=sampleData[i][line-1];
	   sprow++;
   }*/
   m_sampleDatas=sDatas;
   m_sampleDatasLine=line;
   m_sampleDatasRow=tringDataN;
   m_testDatas=tDatas;
   m_testDatasLine=line;
   m_testDatasRow=testDataN;

   /*TestDataInitialization(tDatas,testDataN,line); 
   SampleDataInitialization(sDatas,tringDataN,line);*/
}
//bool ModelIntegrator::InorderForRuleSet(TreeNode* root, int tag, double erroratio,CString stRules,int rulesN)
//{
//	if (root==NULL)
//	{
//		return false;
//	}
//	else
//	{
//
//		//访问根部节点的数据
//		//.......................
//		double splitPoint=root->splitVertex;//获得分裂点
//		int spIndex=root->attIndex;         //获得属性索引
//		spIndex=spIndex+1;                 //为了和GLC相互对应
//		CString stSplitPoint;
//		stSplitPoint.Format(_T("%f"),splitPoint);
//		CString stSplitInex=_T("");
//		stSplitInex.Format(_T("%d"),spIndex);
//		CString stSign=_T("");             //符号
//		CString strulesf=_T("");            //当前节点规则
//		CString stRuleNew=_T("");           //新的规则链，用于下次迭代
//		int rulesCount=0;
//
//		if (tag==1)//当前节点是左节点
//		{
//			stSign=_T("<=");
//		}
//		if (tag==2)
//		{
//			stSign=_T(">");
//		}
//
//		if (root->isLeaf)                  //如果是叶子节点，考虑输出规则
//		{
//
//			CString stClassID=_T("");    //类别
//			CString stCount=_T("");     //判断条件的个数
//			CString stCover=_T("");     //叶节点覆盖元组数
//			CString stCorrect=_T("");   //叶节点覆盖元组数被正确分类的个数
//			CString stPre=_T("");     //预测权重
//			CString st,rules,ed;
//			//count="7" cover="1.00000" correct="1.00000" pre="3.01771" class="6"
//			int clssId=root->ClassID;
//			stClassID.Format(_T("%d"),clssId);
//			//stCount.Format(_T("%d"),m_ruleCount);
//			stCount.Format(_T("%d"),rulesN);
//			double cover=root->allN;
//			stCover.Format(_T("%lf"),cover);
//			double correct=root->allN-root->errors;
//			stCorrect.Format(_T("%lf"),correct);
//			double acc=0,pre=0;
//			acc=correct/cover;
//			pre=acc*log(1/erroratio);
//			stPre.Format(_T("%lf"),pre);
//			st=_T("count=\"")+stCount+_T("\" cover=\"")+stCover+_T("\" correct=\"")+stCorrect+_T("\" pre=\"")
//			+stPre+_T("\" class=\"")+stClassID+_T("\"\r\n");		
//			rules=st+stRules+stSign+_T("\"\r\n");	
//
//			m_ruleSet[m_ruleSetI]=rules;
//			m_ruleSetI++;
//			
//	
//			m_ruleCount++;//计算规则的个数，也就是叶子节点的个数
//		}
//		else
//		{		
//
//			strulesf=_T("att=\"")+stSplitInex+_T("\" ")+_T("cut=\"")+stSplitPoint+_T("\" ")+
//				_T("result=\"");	//提取本节点的规则信息
//			if (tag==0)    //当前节点是根部节点
//			{
//				stRuleNew=strulesf;
//			}
//			else
//			{
//				stRuleNew=stRules+stSign+_T("\"\r\n")+strulesf; //stRules+stSign属于父节点中提取的规则 strulesf是本节点提取的规则信息，但没有符号
//				/*m_ruleSet[m_ruleSetI]=stRules+stSign+_T("\"\r\n");
//				m_ruleSetI++;*/
//			}
//
//			rulesCount=rulesN+1;
//		}
//		InorderForRuleSet(root->leftChild,1,erroratio,stRuleNew,rulesCount);//遍历的是左边子树
//		InorderForRuleSet(root->rightChild,2,erroratio,stRuleNew,rulesCount);//遍历的右面子树
//	}
//	return true;
//}
void ModelIntegrator::SampleDataInitialization(double ** sampleData, int row, int line)
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
	m_adaBoosting.GetData(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine,spDataIndex);
	m_adaBoosting.TuplesInformationInitial(row/*元组的个数*/,spDataIndex/*样本索引*/,row/*行*/,2/*列，一般用2*/);
	TuplesInformationInitial(row/*元组的个数*/,spDataIndex/*样本索引*/,row/*行*/,2/*列，一般用2*/);
	// Calculate::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
}
void ModelIntegrator::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
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
void ModelIntegrator::TestDataInitialization(double** testData , int row , int line)
{
	/*for (int i=0;i<row;i++)
	{
	for (int j=0;j<line;j++)
	{
	m_testDatas[i][j]
	}
	}*/
	m_testDatas=testData;
	m_testDatasRow=row;
	m_testDatasLine=line;
}
int * ModelIntegrator::GetNonredundantRandom(int leftLimit, int rightLimit,int randomN)
{
	int * r;
	r=new int[randomN];
	m_random=new int[randomN];
	m_randomNum=randomN;
	int tr;
	bool tag=true;
	tr=rand()%(rightLimit-leftLimit);
	for (int i=0;i<randomN;i++)
	{  
		int j=0; 

		tr=rand()%(rightLimit-leftLimit);
		for (j=0;j<i;j++)
		{
			if(tr==m_random[j])
			{
				i--;
				tag=false;
				break;
			}
			else
			{
				tag=true;
			}
		}
		if (tag)
		{
			m_random[i]=tr;
		}

	}
	//排序
	int temp=0;
	for (int i=0;i<randomN-1;i++)
	{
		for (int j=i+1;j<randomN;j++)
		{
			if (m_random[j]<m_random[i])
			{
				temp=m_random[j];m_random[j]=m_random[i];
				m_random[i]=temp;
			}
		}

	}
	return m_random;
}
////获取控制参数
//void ModelIntegrator::GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath)
//{
//	m_treeHeight=treeHeight;
//	m_adaBoostingK=adaBoostingK;
//	m_leafsSize=leafsSize;
//	m_sampleDataRatio=sampleDataRatio;
//	m_ruleFilePath=rulePath;
//}
void ModelIntegrator::GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio)//不用规则集文件
{
	m_treeHeight=treeHeight;
	m_adaBoostingK=adaBoostingK;
	m_leafsSize=leafsSize;
	m_sampleDataRatio=sampleDataRatio;
	
}
//
//double ModelIntegrator::MeasuringAccuracy(TuplesClassified* MeasuringTuples,int tuplesNum,CString * rules,int rulsN)
//{
//	int cnt=0;
//	TuplesClassified* tempMeasuringTuples=MeasuringAccuracyMatch(MeasuringTuples,tuplesNum,rules,rulsN);
//	CString path=_T("C:\\Data\\Mtt.txt");
//	//PrintTuples(tempMeasuringTuples,tuplesNum,path);
//	for (int i=0;i<tuplesNum;i++)
//	{
//		int index=tempMeasuringTuples[i].sampleDataIndex;
//		if (tempMeasuringTuples[i].tuplesClassID!=(int)m_testDatas[index][m_testDatasLine-1])
//		{
//			cnt++;
//		}
//	}
//	//PrintTuples(tempMeasuringTuples,tuplesNum,path);
//	double Accuracy=(tuplesNum-cnt)*1.0/tuplesNum;
//	return Accuracy;
//}


//// 获得测试精度
//double* ModelIntegrator::GetMeasuringAccuracy_separately(int tuplesN/*测试数据的个数*/)
//{
//
//	if (m_testDatas==NULL)
//	{
//		cout<<"ERROR:The test data hasn't been initialized,plese check!"<<endl;
//		return NULL;
//		//MessageBox(NULL,_T("The test data hasn't been initialized,plese check!"),_T("提示"),MB_OK);
//	}
//	//返回每棵树的测试精度
//	double *testAccy=new double[m_adaBoostingK];
//	
//	TreeSelection* trees=m_adaBoosting.GetCompoundModel();
//	
//	for (int i=0;i<m_adaBoostingK;i++)
//	{
//
//		//初始测试元组
//		TuplesClassified* MeasuringTuples;
//		MeasuringTuples=new TuplesClassified[tuplesN];
//		for (int j=0;j<tuplesN;j++)
//		{
//			MeasuringTuples[j].sampleDataIndex=j;
//			MeasuringTuples[j].tuplesClassID=m_testDatas[j][m_sampleDatasLine-1];
//			MeasuringTuples[j].err=0;
//			MeasuringTuples[j].weight=0;
//		}
//		//获得每棵树的规则
//		CString * rulesSingle=GetRuleSet_SingleTree(trees,i);
//	    
//
//		int ruels=trees[i].rules;//获得规则数目
//		   
//        double r=MeasuringAccuracy(MeasuringTuples,tuplesN,rulesSingle,m_ruleSetI);
//		trees[i].erroratio_test=r; //给树集挂上测试的正确率
//		testAccy[i]=r;
//		delete [] MeasuringTuples;
//	}
//	return testAccy;
//}
//


//CString * ModelIntegrator::doReorderToRuleSet(CString * oldRuleSet, int rulesSetRow)
//{
//	CString * tempR,temp=_T("");
//	tempR=new CString[1000];              //暂时把规则的最多数定位1000，后面可以扩初
//	CString buffer=_T(""),stend=_T("\r");
//	int newrows=0;
//	for (int i=0;i<rulesSetRow;i++)
//	{
//		int m=0;
//		buffer=oldRuleSet[i];
//		int bufferL=buffer.GetAllocLength();
//		for (int j=0;j<bufferL;j++)
//		{ 
//			m++;
//		  if (buffer[j]==stend)
//			{
//				temp=buffer.Mid(j-m,m);
//				tempR[newrows]=temp;
//				newrows++;
//               m=0;
//			}
//		}
//	}
//	delete [] m_ruleSet;
//	m_ruleSet=tempR;
//	m_ruleSetI=newrows;
//	return NULL;
//}
//

//// 获得整个规则数组
//CString* ModelIntegrator::GetRuleSetAll(void)
//{
//	if (m_ruleSet==NULL)
//	{
//		MessageBox(NULL,_T("The ruleset is empty,plese check!"),_T("提示"),MB_OK);
//	}
//	return m_ruleSet;
//}


//// 获得单个树的规则
//CString* ModelIntegrator::GetRuleSet_SingleTree(TreeSelection * trees, int treei/*树的编号*/)
//{
//	     m_ruleSet=new CString[1000];//分类之后的叶节点最多不要超过1000，超过改此处
//	     m_ruleSetI=0;	m_ruleCount=0;
//        //遍历树，获取规则
//		double erroratio=trees[treei].erroratio;
//		CString stRootRules=_T("");
//		TreeNode *root=trees[treei].Tree;
//		InorderForRuleSet(root,0,erroratio,stRootRules,0);
//		trees[treei].rules=m_ruleCount;
//
//		CString pt=_T("C:\\Data\\testPP.txt");
//		//PrintRules(m_ruleSet,m_ruleSetI,pt);
//		//重新整理规则
//		CString* TT;
//		TT=new CString[1000];      //  设置最大容量，可扩充
//		int ttI=0;
//		for (int i=0;i<m_ruleCount;i++)
//		{    
//			CString temp=m_ruleSet[i];
//		    CString tempChang=temp;  
//			CString t=_T("\r\n");
//			int j=0,m=0;
//			for (j=0;j<temp.GetAllocLength();j++)
//			{
//				m++;
//				CString B=_T("\r"),C=_T("\n");
//				if (temp[j]==B)
//				{
//					tempChang=temp.Mid(j-m,m);
//					TT[ttI]=tempChang+_T("\r\n");
//					
//					ttI++;
//					m=0;
//				}
//				if (temp[j]==C)
//				{
//					m=0;
//				}
//			}
//		
//		}
//		
//		trees[treei].rulset=TT;   //给每棵树集挂上规则
//		trees[treei].ruleeSetRow=ttI;
//		delete [] m_ruleSet;
//		m_ruleSetI=ttI;
//		
//		return TT;
//		
//	
//}
//CString* ModelIntegrator::GetRuleSet_SingleTree(TreeSelection * trees, int treei/*树的编号*/,int & ruleCount)
//{
//	     m_ruleSet=new CString[1000];//分类之后的叶节点最多不要超过1000，超过改此处
//	     m_ruleSetI=0;	m_ruleCount=0;
//        //遍历树，获取规则
//		double erroratio=trees[treei].erroratio;
//		CString stRootRules=_T("");
//		TreeNode *root=trees[treei].Tree;
//		InorderForRuleSet(root,0,erroratio,stRootRules,0);
//		trees[treei].rules=m_ruleCount;
//		CString pt=_T("C:\\Data\\testPP.txt");
//		//PrintRules(m_ruleSet,m_ruleSetI,pt);
//		//重新整理规则
//		CString* TT;
//		TT=new CString[1000];      //  设置最大容量，可扩充
//		int ttI=0;
//		for (int i=0;i<m_ruleCount;i++)
//		{    
//			CString temp=m_ruleSet[i];
//		    CString tempChang=temp;  
//			CString t=_T("\r\n");
//			int j=0,m=0;
//			for (j=0;j<temp.GetAllocLength();j++)
//			{
//				m++;
//				CString B=_T("\r"),C=_T("\n");
//				if (temp[j]==B)
//				{
//					tempChang=temp.Mid(j-m,m);
//					TT[ttI]=tempChang/*+_T("\r\n")*/;					
//					ttI++;
//					m=0;
//				}
//				if (temp[j]==C)
//				{
//					m=0;
//				}
//			}
//	
//		}	
//		trees[treei].rulset=TT;   //给每棵树集挂上规则
//		trees[treei].ruleeSetRow=ttI;
//		delete [] m_ruleSet;
//		m_ruleSetI=ttI;
//		
//
//		return TT;
//		
//	
//}



//// 打印出各个树的测试精度
//void ModelIntegrator::OuntPutMeasuringAccuracy_separately(CString path)
//{
//	double * accy;
//	accy=GetMeasuringAccuracy_separately(m_testDatasRow);
//	CStdioFile fl;
//	fl.Open(path,CFile::modeCreate|CFile::modeWrite/*|CFile::typeText*//*|CFile::typeBinary*/);
//	CString  s=_T("");
//	for (int i=0;i<m_adaBoostingK;i++)
//	{
//		double temp=accy[i];
//		CString stT,str,stI;
//		stT.Format(_T("%lf"),temp);
//		stI.Format(_T("%d",i));
//		str=_T("i=")+stI+_T(" testerrorratio=")+stT+_T("\r\n");
//		s=s+str;
//	}
//     fl.WriteString(s);
//}
//void ModelIntegrator::PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified* tuples, int tuplesN,CString path)
//{
//	CStdioFile fi;
//	fi.Open(path,CFile::modeCreate|CFile::modeWrite);
//	int sindex=0;
//	int classid=0;
//	int err=0;
//	double w=0;
//	double values=0;
//	CString stSindex,st=_T(""),ed=_T("\r\n"),stvalues,temp		;
//	TuplesClassified * Di=tuples;
//
//	for (int ii=0;ii<tuplesN;ii++)
//	{
//		st=_T("");
//		sindex=Di[ii].sampleDataIndex;
//		for (int jj=0;jj<line;jj++)
//		{
//			values=sampdata[sindex][jj];
//
//			if (jj!=line-1)
//			{
//				stvalues.Format(_T("%fl"),values);
//				temp=stvalues+_T(",");
//				st=st+temp;
//			}
//			else
//			{
//				stvalues.Format(_T("%d"),(int)values);
//				temp=stvalues;
//				st=st+temp;
//			}		
//		}
//		st=st+ed;
//		fi.WriteString(st);
//
//	}
//	;
//	fi.Close();
//}
//void ModelIntegrator::PrintTuples(TuplesClassified * tuples, int tuplesN, CString path)
//{
//	CStdioFile file2;
//	file2.Open(path,CFile::modeCreate|CFile::modeWrite);
//	int i=0;
//	int sindex=0;
//	int classid=0;
//	int err=0;
//	double w=0;
//	CString sti,stSindex,stClassid,stErr,stW,st,ed;
//	TuplesClassified * Di=tuples;	 
//
//	for (int ii=0;ii<tuplesN;ii++)
//	{
//		i=Di[ii].i;
//		sindex=Di[ii].sampleDataIndex;
//		classid=Di[ii].tuplesClassID;
//		err=Di[ii].err;
//		w=Di[ii].weight;
//
//
//		sti.Format(_T("%d"),i);
//		stSindex.Format( _T("%d"),sindex);
//		stClassid.Format( _T("%d"),classid);
//		stW.Format( _T("%lf"),w);
//		stErr.Format( _T("%d"),err);
//		ed=_T("\r\n");
//		st=_T("i=")+sti+_T("sindex=")+stSindex+_T(",classId=")+stClassid+_T(",err=")+
//			stErr+_T(",w=")+stW+ed;
//
//		file2.WriteString(st);
//	}
//	file2.Close();
//}
//
//
//void ModelIntegrator::PrintRules(CString * rules, int rulesN , CString path)
//{
//	CStdioFile f;
//	f.Open(path,CFile::modeCreate|CFile::modeWrite);
//	for (int i=0;i<rulesN;i++)
//	{
//		f.WriteString(rules[i]);
//		MessageBox(NULL,rules[i],_T("提示"),MB_OK);
//	}
//	f.Close();
//}


//// 分类
//TuplesClassified* ModelIntegrator::Classify(TuplesClassified* T, int N , CString* rule,int ruleN,int &rightTuples/*正确分类的个数*/,int calssId)
//{
//
//	CString FileData=rule[0];//读取第一条规则	
//	TuplesClassified* testT=new TuplesClassified[N];
//	//读取属性索引
//	CString stAtt=_T("att=\"");
//	int i= FileData.Find(_T("att=\""));
//	int j=i+stAtt.GetLength();
//	int k=j;
//	int m=0;
//	CString t=_T("\"");
//	while(FileData[k]!=t)
//	{
//		m++;
//		k++;
//	};
//	CString attStr=FileData.Mid(j,m);
//	int att=0;
//	_stscanf(attStr,_T("%d"),&att);
//	//读取分裂值
//	CString stSplitPoint=_T("cut=\"");
//	i= FileData.Find(_T("cut=\""));
//	j=i+stSplitPoint.GetLength();
//	k=j;
//	m=0;
//	t=_T("\"");
//	while(FileData[k]!=t)
//	{
//		m++;
//		k++;
//	};
//	CString stSplitPointStr=FileData.Mid(j,m);
//	int cut=0;
//	_stscanf(stSplitPointStr,_T("%d"),&cut);
//	//读取判断符号
//	CString stRule=_T("result=\"");
//	i= FileData.Find(_T("result=\""));
//	j=i+stRule.GetLength();
//	k=j;
//	m=0;
//	t=_T("\"");
//	while(FileData[k]!=t)
//	{
//		m++;
//		k++;
//	};
//	CString stRuleStr=FileData.Mid(j,m);
//	CString tempA=_T("<");
//	CString tempB=_T(">=");
//	//对数据分类
//	int tmpi=0;        //计数器，最终数据被当成符合条件索引的个数
//	if (stRuleStr==tempA)  //小于
//	{
//		//对索引里的所有索引进行搜索
//		for (int i=0;i<N;i++)
//		{
//			
//			int testindex=T[i].sampleDataIndex;//通过元组的索引找出测试数据的索引
//			if (m_testDatas[testindex][att]<cut) //如果符合条件
//			{
//				testT[tmpi]=T[i];   //用新数组记录符合条件的索引；
//				tmpi++;
//			}
//		}
//
//	}
//	if (stRuleStr==tempB)  //大于
//	{
//		//对索引里的所有索引进行搜索
//		for (int i=0;i<N;i++)
//		{
//			
//
//			int testindex=T[i].sampleDataIndex;//通过元组的索引找出测试数据的索引
//			if (m_testDatas[testindex][att]<cut) //如果符合条件
//			{
//				testT[tmpi]=T[i];   //用新数组记录符合条件的索引；
//				
//				tmpi++;
//			}
//		}
//	}
//	
//	if (ruleN==1)
//	{
//		rightTuples=tmpi;//合格数
//		for (int j=0;j<tmpi;j++)
//		{
//			testT[j].tuplesClassID=calssId;//分类别
//		}
//		
//		return testT;
//	}
//	int newRuleN=ruleN-1;
//	CString * newRules=new CString[newRuleN];
//	for (int i=0;i<newRuleN;i++)
//	{
//		newRules[i]=rule[i+1];
//	}
//	delete [] T;
//	//delete [] rule;
//    return Classify(testT,tmpi,newRules,newRuleN,rightTuples,calssId);
//}
//// 测试精度,用于获得分类后的元组集，被MeasuringAccuracy调用
//TuplesClassified* ModelIntegrator::MeasuringAccuracyMatch(TuplesClassified* MeasuringTuples,int tuplesNum,CString * rules,int rulsN)
//{
//	CString FileData;int FileDatai=1; //定义一个CString变量作为缓冲区
//	int line=0,row=0; //行列数
//
//	CString stOld=_T("class=\""),stNew=_T("class=\"");
//	int tempTupleN=tuplesNum;
//	int classId;     //类别
//
//	//表头初始化
//	FileData=rules[0];                           //读取第一行，得到表头
//	//初始化类别
//	CString temp=_T("class=\"");
//	int j=temp.GetLength();
//	int k=FileData.Find(_T("class=\""));
//	int m=k+j;
//	int n=FileData.Find(_T("\""),m);
//	CString stClassId=FileData.Mid(m,n);
//	_stscanf(stClassId,_T( "%d"),&classId);
//	
//	int tempcode[1000];       //记录上次循环所记录的索引
//	int newTempcode[1000];    //循环后所记录的索引
//	int tempN=tuplesNum;      //初始的索引个数与元组个数相同
//	int tempi=0;              //suoyi计数器，初始为0
//	      //初始化的索引数组记录元组按顺序的索引号
//	for (int ii=0;ii<tempN;ii++)
//	{
//		tempcode[ii]=ii;
//	}
//
//    //循环读取文件中的数据
//	while (FileDatai<rulsN)
//	{
//		FileData=rules[FileDatai++];               //读取一个规则
//		if (FileData.Find(_T("class=\""))>-1)//如果读到表头，对之前的分类进行处理，
//		{
//			 //对之前的分类进行处理
//			for (int i=0;i<tempN;i++)      
//             {
//				int tuplesIndex=tempcode[i];
//				MeasuringTuples[tuplesIndex].tuplesClassID=classId;
//			 }
//
//			//读出当前的表头数据，各种初始化
//			     //表头初始化//初始化类别
//			CString temp=_T("class=\"");
//			int j=temp.GetLength();
//			int k=FileData.Find(_T("class=\""));
//			int m=k+j;
//			int n=FileData.Find(_T("\""),m);
//			CString stClassId=FileData.Mid(m,n);
//			_stscanf(stClassId,_T( "%d"),&classId);
//			     //索引初始化
//			tempcode[1000];       //记录上次循环所记录的索引
//			newTempcode[1000];    //循环后所记录的索引
//			tempN=tuplesNum;      //初始的索引个数与元组个数相同
//			tempi=0;              //suoyi计数器，初始为0
//			     //初始化的索引数组记录元组按顺序的索引号
//			for (int ii=0;ii<tempN;ii++)
//			{
//				tempcode[ii]=ii;
//			}
//		}
//		else
//		{
//				 //读取属性索引
//				 CString stAtt=_T("att=\"");
//				 int i= FileData.Find(_T("att=\""));
//				 int j=i+stAtt.GetLength();
//				 int k=j;
//				 int m=0;
//				 CString t=_T("\"");
//				 while(FileData[k]!=t)
//				 {
//					 m++;
//					 k++;
//				 };
//				 CString attStr=FileData.Mid(j,m);
//				 int att=0;
//				 _stscanf(attStr,_T("%d"),&att);
//				 //读取分裂值
//				 CString stSplitPoint=_T("cut=\"");
//				 i= FileData.Find(_T("cut=\""));
//				 j=i+stSplitPoint.GetLength();
//				 k=j;
//				 m=0;
//				 t=_T("\"");
//				 while(FileData[k]!=t)
//				 {
//					 m++;
//					 k++;
//				 };
//				 CString stSplitPointStr=FileData.Mid(j,m);
//				 int cut=0;
//				 _stscanf(stSplitPointStr,_T("%d"),&cut);
//				 //读取判断符号
//				 CString stRule=_T("result=\"");
//				 i= FileData.Find(_T("result=\""));
//				 j=i+stRule.GetLength();
//				 k=j;
//				 m=0;
//				 t=_T("\"");
//				 while(FileData[k]!=t)
//				 {
//					 m++;
//					 k++;
//				 };
//				 CString stRuleStr=FileData.Mid(j,m);
//				 CString tempA=_T("<=");
//				 CString tempB=_T(">");
//				 //对数据分类
//				  int tmpi=0;        //计数器，最终数据被当成符合条件索引的个数
//				 if (stRuleStr==tempA)  //小于
//				 {
//					 //对索引里的所有索引进行搜索
//					 for (int i=0;i<tempN;i++)
//					 {
//						 int tuplesindex=tempcode[i];//读出索引记录的元组索引
//						 int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//通过元组的索引找出测试数据的索引
//						 if (m_testDatas[testindex][att]<=cut) //如果符合条件
//						 {
//							 newTempcode[tmpi]=tempcode[i];   //用新数组记录符合条件的索引；
//							 tmpi++;
//						 }
//					 }
//
//				 }
//				 if (stRuleStr==tempB)  //大于
//				 {
//					 //对索引里的所有索引进行搜索
//					 for (int i=0;i<tempN;i++)
//					 {
//						 int tuplesindex=tempcode[i];//读出索引记录的元组索引
//						 int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//通过元组的索引找出测试数据的索引
//						 if (m_testDatas[testindex][att]>cut) //如果符合条件
//						 {
//							 newTempcode[tmpi]=tempcode[i];   //用新数组记录符合条件的索引；
//							 tmpi++;
//						 }
//					 }
//				 }	
//				 //更新数据
//				 tempN=tmpi;//更新索引数
//				 tmpi=0;    //计数器归零
//				 for (int i=0;i<tempN;i++)
//				 {
//					 tempcode[i]=newTempcode[i];
//				 }
//		}
//	}
////	TuplesClassified *M=MeasuringTuples;
//
////	return M;
//	return MeasuringTuples;
//}
//
//
//// 设置进度条
//void ModelIntegrator::SetProgressBar(CProgressCtrl* progressBar)
//{
//	m_progressBar=progressBar;
//
//}

////逐行逐行地将规则集存储于数组中，
//CString* ModelIntegrator::GetRuleSet1by1(TreeSelection * trees/*生成的树集*/,int & ruleSetRow/*计数器获得规则集的行数*/)
//{
//	//计算存储空间,并申请
//	CString rulesSet[3000];
//	
//	int ruleRow=0;//规则计数器包括树头和规则头如count="3" cover="40.000000" correct="40.000000" pre="5.888878" class="10"
//	for (int i=0;i<m_adaBoostingK;i++)
//	{
//		CString * singleTreeRule;
//		singleTreeRule=GetRuleSet_SingleTree(trees,i);
//		int rules=trees[i].rules;
//		int def=trees[i].publicClssId;
//		CString stRules,stDefult,stHead;
//		stRules.Format(_T("%d"),rules);
//		stDefult.Format(_T("%d"),def);
//	
//		stHead=_T("rules=\"")+stRules+_T("\" default=\"")+stDefult+_T("\"\r\n");
//
//		rulesSet[ruleRow]=stHead;
//		ruleRow++;
//
//		for(int j=0;j<trees[i].ruleeSetRow;j++)
//		{
//			rulesSet[ruleRow]=singleTreeRule[j];
//			ruleRow++;
//		}
//		delete [] singleTreeRule;
//	}
//	CString * rulesSetReturn=new CString[ruleRow];
//	for (int i=0;i<ruleRow;i++)
//	{
//		rulesSetReturn[i]=rulesSet[i];
//	}
//	ruleSetRow=ruleRow;
//	return rulesSetReturn;
//}
//

// 获得树集
TreeSelection * ModelIntegrator::GetTreeSelection(int & treeSum)
{
	treeSum=m_adaBoostingK;
	return m_adaBoosting.GetCompoundModel();
}


// 获取测试数据
double** ModelIntegrator::GetTestData(int& testDataRow,int & testDataLine)
{
	double ** testdata;
	testdata=m_testDatas;
	testDataRow=m_testDatasRow;
	testDataLine=m_testDatasLine;
	return testdata;
}


double** ModelIntegrator::GetSampleData(void)
{
	return m_sampleDatas;
}
