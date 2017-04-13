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
{//// �������ݷָ�Ϊѵ�����Ͳ��Լ�
	//SampleDataDataPartition(sampleData,row,line);
	////m_adaBoosting.TuplesInformationInitial();
	m_adaBoosting.SampleDataInitialization(sampleData,row,line);//��ʼ��ѵ������
	//��ʼ����������
	m_adaBoosting.GetControlsParameter(m_treeHeight,m_adaBoostingK,m_leafsSize,m_sampleDataRatio);
	m_adaBoosting.run(progressI);

}
//void ModelIntegrator::AdaBoostingTrainingRoles1(double ** sampleData, int row, int line, int * attIndex, int attNum,int & progressI,CString & Eror)//��������
//{
//	//// �������ݷָ�Ϊѵ�����Ͳ��Լ�
//	//SampleDataDataPartition(sampleData,row,line);
//	////m_adaBoosting.TuplesInformationInitial();
//	m_adaBoosting.SampleDataInitialization(sampleData,row,line);//��ʼ��ѵ������
//	      //��ʼ����������
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
//	m_ruleSet=GetRuleSet(trees,treesN);//����֮���Ҷ�ڵ���಻Ҫ����1000�������Ĵ˴�
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
//	head2=_T("entries=\"")+Entries+_T("\"\r\n");//��������
//	fl.WriteString(head1);
//	fl.WriteString(head2);
//	//doReorderToRuleSet(m_ruleSet,m_ruleSetI);
//	for (int i=0;i<m_ruleSetI;i++)
//	{
//		temp=m_ruleSet[i];
//		fl.WriteString(temp);
//	}
//	fl.Close();
//	m_ruleSetI=0;//�������
//
//}
//void ModelIntegrator::OutputRuleSet(CString rulesPath,int & ruleRow)
//{
//	
//	m_ruleSetI=0;//�������
//
//}
//CString* ModelIntegrator::GetRuleSet(TreeSelection * trees, int treesN)
//{
//	m_ruleSet=new CString[1000];//����֮���Ҷ�ڵ���಻Ҫ����1000�������Ĵ˴�
//	m_ruleSetI=0;
//
//	for (int i=0;i<treesN;i++)
//	{
//
//		int tempruleindex=m_ruleSetI;		//��������Ŀ�ͷ����λ��
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
//		m_ruleCount=0;//�������
//	}
//
//	//���ع����ļ�
//	
//	//m_ruleSetI=0;//�������
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
	

	srand(seeds);           //�����������
	int * sampleDataIndex;
    sampleDataIndex=GetNonredundantRandom(0,row-1,tringDataN);
	int m=m_randomNum;    //�������
	
	double** sDatas;
	sDatas=new double* [tringDataN];  
	for (int i=0;i<tringDataN;i++)
	{
		sDatas[i]=new double [line];
	}
	
	//��ò�������
	int t=0,s=0;
	double ** tDatas;
	tDatas=new double* [testDataN];

	for (int i=0;i<testDataN;i++)
	{
		tDatas[i]=new double [line];
	}
	//�ָ�ѵ������
	for (int i=0;i<tringDataN;i++)
	{
		int tempindex=sampleDataIndex[i];
		for (int j=0;j<line;j++)
		{
			sDatas[i][j]=sampleData[tempindex][j];
		}
	}
	//�ָ��������
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
//		//���ʸ����ڵ������
//		//.......................
//		double splitPoint=root->splitVertex;//��÷��ѵ�
//		int spIndex=root->attIndex;         //�����������
//		spIndex=spIndex+1;                 //Ϊ�˺�GLC�໥��Ӧ
//		CString stSplitPoint;
//		stSplitPoint.Format(_T("%f"),splitPoint);
//		CString stSplitInex=_T("");
//		stSplitInex.Format(_T("%d"),spIndex);
//		CString stSign=_T("");             //����
//		CString strulesf=_T("");            //��ǰ�ڵ����
//		CString stRuleNew=_T("");           //�µĹ������������´ε���
//		int rulesCount=0;
//
//		if (tag==1)//��ǰ�ڵ�����ڵ�
//		{
//			stSign=_T("<=");
//		}
//		if (tag==2)
//		{
//			stSign=_T(">");
//		}
//
//		if (root->isLeaf)                  //�����Ҷ�ӽڵ㣬�����������
//		{
//
//			CString stClassID=_T("");    //���
//			CString stCount=_T("");     //�ж������ĸ���
//			CString stCover=_T("");     //Ҷ�ڵ㸲��Ԫ����
//			CString stCorrect=_T("");   //Ҷ�ڵ㸲��Ԫ��������ȷ����ĸ���
//			CString stPre=_T("");     //Ԥ��Ȩ��
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
//			m_ruleCount++;//�������ĸ�����Ҳ����Ҷ�ӽڵ�ĸ���
//		}
//		else
//		{		
//
//			strulesf=_T("att=\"")+stSplitInex+_T("\" ")+_T("cut=\"")+stSplitPoint+_T("\" ")+
//				_T("result=\"");	//��ȡ���ڵ�Ĺ�����Ϣ
//			if (tag==0)    //��ǰ�ڵ��Ǹ����ڵ�
//			{
//				stRuleNew=strulesf;
//			}
//			else
//			{
//				stRuleNew=stRules+stSign+_T("\"\r\n")+strulesf; //stRules+stSign���ڸ��ڵ�����ȡ�Ĺ��� strulesf�Ǳ��ڵ���ȡ�Ĺ�����Ϣ����û�з���
//				/*m_ruleSet[m_ruleSetI]=stRules+stSign+_T("\"\r\n");
//				m_ruleSetI++;*/
//			}
//
//			rulesCount=rulesN+1;
//		}
//		InorderForRuleSet(root->leftChild,1,erroratio,stRuleNew,rulesCount);//���������������
//		InorderForRuleSet(root->rightChild,2,erroratio,stRuleNew,rulesCount);//��������������
//	}
//	return true;
//}
void ModelIntegrator::SampleDataInitialization(double ** sampleData, int row, int line)
{
	m_sampleDatas=sampleData;
	m_sampleDatasRow=row;
	m_sampleDatasLine=line;

	int n=m_sampleDatasLine-1;//������е����Ը���
	int** attListIndex;               //�������Լ�
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
		spDataIndex[i][0]=i;                     //Ԫ���������е�����
		spDataIndex[i][1]=sampleData[i][line-1]; //��¼����Ԫ�������
	}
	m_adaBoosting.GetData(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine,spDataIndex);
	m_adaBoosting.TuplesInformationInitial(row/*Ԫ��ĸ���*/,spDataIndex/*��������*/,row/*��*/,2/*�У�һ����2*/);
	TuplesInformationInitial(row/*Ԫ��ĸ���*/,spDataIndex/*��������*/,row/*��*/,2/*�У�һ����2*/);
	// Calculate::TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/)
}
void ModelIntegrator::TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/)
{

	m_tuplesInformation=new TuplesClassified[tuplesNum];
	m_tuplesNum=tuplesNum;
	for (int i=0;i<tuplesNum;i++)
	{
		m_tuplesInformation[i].i=i;
		m_tuplesInformation[i].err=0;
		m_tuplesInformation[i].tuplesClassID=sampDataIndex[i][n-1];//n-1�����ݼ�¼�������ݵ������
		m_tuplesInformation[i].sampleDataIndex=sampDataIndex[i][0];//0�����ݼ�¼���������е�����
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
	//����
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
////��ȡ���Ʋ���
//void ModelIntegrator::GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath)
//{
//	m_treeHeight=treeHeight;
//	m_adaBoostingK=adaBoostingK;
//	m_leafsSize=leafsSize;
//	m_sampleDataRatio=sampleDataRatio;
//	m_ruleFilePath=rulePath;
//}
void ModelIntegrator::GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio)//���ù����ļ�
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


//// ��ò��Ծ���
//double* ModelIntegrator::GetMeasuringAccuracy_separately(int tuplesN/*�������ݵĸ���*/)
//{
//
//	if (m_testDatas==NULL)
//	{
//		cout<<"ERROR:The test data hasn't been initialized,plese check!"<<endl;
//		return NULL;
//		//MessageBox(NULL,_T("The test data hasn't been initialized,plese check!"),_T("��ʾ"),MB_OK);
//	}
//	//����ÿ�����Ĳ��Ծ���
//	double *testAccy=new double[m_adaBoostingK];
//	
//	TreeSelection* trees=m_adaBoosting.GetCompoundModel();
//	
//	for (int i=0;i<m_adaBoostingK;i++)
//	{
//
//		//��ʼ����Ԫ��
//		TuplesClassified* MeasuringTuples;
//		MeasuringTuples=new TuplesClassified[tuplesN];
//		for (int j=0;j<tuplesN;j++)
//		{
//			MeasuringTuples[j].sampleDataIndex=j;
//			MeasuringTuples[j].tuplesClassID=m_testDatas[j][m_sampleDatasLine-1];
//			MeasuringTuples[j].err=0;
//			MeasuringTuples[j].weight=0;
//		}
//		//���ÿ�����Ĺ���
//		CString * rulesSingle=GetRuleSet_SingleTree(trees,i);
//	    
//
//		int ruels=trees[i].rules;//��ù�����Ŀ
//		   
//        double r=MeasuringAccuracy(MeasuringTuples,tuplesN,rulesSingle,m_ruleSetI);
//		trees[i].erroratio_test=r; //���������ϲ��Ե���ȷ��
//		testAccy[i]=r;
//		delete [] MeasuringTuples;
//	}
//	return testAccy;
//}
//


//CString * ModelIntegrator::doReorderToRuleSet(CString * oldRuleSet, int rulesSetRow)
//{
//	CString * tempR,temp=_T("");
//	tempR=new CString[1000];              //��ʱ�ѹ�����������λ1000�������������
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

//// ���������������
//CString* ModelIntegrator::GetRuleSetAll(void)
//{
//	if (m_ruleSet==NULL)
//	{
//		MessageBox(NULL,_T("The ruleset is empty,plese check!"),_T("��ʾ"),MB_OK);
//	}
//	return m_ruleSet;
//}


//// ��õ������Ĺ���
//CString* ModelIntegrator::GetRuleSet_SingleTree(TreeSelection * trees, int treei/*���ı��*/)
//{
//	     m_ruleSet=new CString[1000];//����֮���Ҷ�ڵ���಻Ҫ����1000�������Ĵ˴�
//	     m_ruleSetI=0;	m_ruleCount=0;
//        //����������ȡ����
//		double erroratio=trees[treei].erroratio;
//		CString stRootRules=_T("");
//		TreeNode *root=trees[treei].Tree;
//		InorderForRuleSet(root,0,erroratio,stRootRules,0);
//		trees[treei].rules=m_ruleCount;
//
//		CString pt=_T("C:\\Data\\testPP.txt");
//		//PrintRules(m_ruleSet,m_ruleSetI,pt);
//		//�����������
//		CString* TT;
//		TT=new CString[1000];      //  �������������������
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
//		trees[treei].rulset=TT;   //��ÿ���������Ϲ���
//		trees[treei].ruleeSetRow=ttI;
//		delete [] m_ruleSet;
//		m_ruleSetI=ttI;
//		
//		return TT;
//		
//	
//}
//CString* ModelIntegrator::GetRuleSet_SingleTree(TreeSelection * trees, int treei/*���ı��*/,int & ruleCount)
//{
//	     m_ruleSet=new CString[1000];//����֮���Ҷ�ڵ���಻Ҫ����1000�������Ĵ˴�
//	     m_ruleSetI=0;	m_ruleCount=0;
//        //����������ȡ����
//		double erroratio=trees[treei].erroratio;
//		CString stRootRules=_T("");
//		TreeNode *root=trees[treei].Tree;
//		InorderForRuleSet(root,0,erroratio,stRootRules,0);
//		trees[treei].rules=m_ruleCount;
//		CString pt=_T("C:\\Data\\testPP.txt");
//		//PrintRules(m_ruleSet,m_ruleSetI,pt);
//		//�����������
//		CString* TT;
//		TT=new CString[1000];      //  �������������������
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
//		trees[treei].rulset=TT;   //��ÿ���������Ϲ���
//		trees[treei].ruleeSetRow=ttI;
//		delete [] m_ruleSet;
//		m_ruleSetI=ttI;
//		
//
//		return TT;
//		
//	
//}



//// ��ӡ���������Ĳ��Ծ���
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
//		MessageBox(NULL,rules[i],_T("��ʾ"),MB_OK);
//	}
//	f.Close();
//}


//// ����
//TuplesClassified* ModelIntegrator::Classify(TuplesClassified* T, int N , CString* rule,int ruleN,int &rightTuples/*��ȷ����ĸ���*/,int calssId)
//{
//
//	CString FileData=rule[0];//��ȡ��һ������	
//	TuplesClassified* testT=new TuplesClassified[N];
//	//��ȡ��������
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
//	//��ȡ����ֵ
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
//	//��ȡ�жϷ���
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
//	//�����ݷ���
//	int tmpi=0;        //���������������ݱ����ɷ������������ĸ���
//	if (stRuleStr==tempA)  //С��
//	{
//		//�������������������������
//		for (int i=0;i<N;i++)
//		{
//			
//			int testindex=T[i].sampleDataIndex;//ͨ��Ԫ��������ҳ��������ݵ�����
//			if (m_testDatas[testindex][att]<cut) //�����������
//			{
//				testT[tmpi]=T[i];   //���������¼����������������
//				tmpi++;
//			}
//		}
//
//	}
//	if (stRuleStr==tempB)  //����
//	{
//		//�������������������������
//		for (int i=0;i<N;i++)
//		{
//			
//
//			int testindex=T[i].sampleDataIndex;//ͨ��Ԫ��������ҳ��������ݵ�����
//			if (m_testDatas[testindex][att]<cut) //�����������
//			{
//				testT[tmpi]=T[i];   //���������¼����������������
//				
//				tmpi++;
//			}
//		}
//	}
//	
//	if (ruleN==1)
//	{
//		rightTuples=tmpi;//�ϸ���
//		for (int j=0;j<tmpi;j++)
//		{
//			testT[j].tuplesClassID=calssId;//�����
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
//// ���Ծ���,���ڻ�÷�����Ԫ�鼯����MeasuringAccuracy����
//TuplesClassified* ModelIntegrator::MeasuringAccuracyMatch(TuplesClassified* MeasuringTuples,int tuplesNum,CString * rules,int rulsN)
//{
//	CString FileData;int FileDatai=1; //����һ��CString������Ϊ������
//	int line=0,row=0; //������
//
//	CString stOld=_T("class=\""),stNew=_T("class=\"");
//	int tempTupleN=tuplesNum;
//	int classId;     //���
//
//	//��ͷ��ʼ��
//	FileData=rules[0];                           //��ȡ��һ�У��õ���ͷ
//	//��ʼ�����
//	CString temp=_T("class=\"");
//	int j=temp.GetLength();
//	int k=FileData.Find(_T("class=\""));
//	int m=k+j;
//	int n=FileData.Find(_T("\""),m);
//	CString stClassId=FileData.Mid(m,n);
//	_stscanf(stClassId,_T( "%d"),&classId);
//	
//	int tempcode[1000];       //��¼�ϴ�ѭ������¼������
//	int newTempcode[1000];    //ѭ��������¼������
//	int tempN=tuplesNum;      //��ʼ������������Ԫ�������ͬ
//	int tempi=0;              //suoyi����������ʼΪ0
//	      //��ʼ�������������¼Ԫ�鰴˳���������
//	for (int ii=0;ii<tempN;ii++)
//	{
//		tempcode[ii]=ii;
//	}
//
//    //ѭ����ȡ�ļ��е�����
//	while (FileDatai<rulsN)
//	{
//		FileData=rules[FileDatai++];               //��ȡһ������
//		if (FileData.Find(_T("class=\""))>-1)//���������ͷ����֮ǰ�ķ�����д���
//		{
//			 //��֮ǰ�ķ�����д���
//			for (int i=0;i<tempN;i++)      
//             {
//				int tuplesIndex=tempcode[i];
//				MeasuringTuples[tuplesIndex].tuplesClassID=classId;
//			 }
//
//			//������ǰ�ı�ͷ���ݣ����ֳ�ʼ��
//			     //��ͷ��ʼ��//��ʼ�����
//			CString temp=_T("class=\"");
//			int j=temp.GetLength();
//			int k=FileData.Find(_T("class=\""));
//			int m=k+j;
//			int n=FileData.Find(_T("\""),m);
//			CString stClassId=FileData.Mid(m,n);
//			_stscanf(stClassId,_T( "%d"),&classId);
//			     //������ʼ��
//			tempcode[1000];       //��¼�ϴ�ѭ������¼������
//			newTempcode[1000];    //ѭ��������¼������
//			tempN=tuplesNum;      //��ʼ������������Ԫ�������ͬ
//			tempi=0;              //suoyi����������ʼΪ0
//			     //��ʼ�������������¼Ԫ�鰴˳���������
//			for (int ii=0;ii<tempN;ii++)
//			{
//				tempcode[ii]=ii;
//			}
//		}
//		else
//		{
//				 //��ȡ��������
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
//				 //��ȡ����ֵ
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
//				 //��ȡ�жϷ���
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
//				 //�����ݷ���
//				  int tmpi=0;        //���������������ݱ����ɷ������������ĸ���
//				 if (stRuleStr==tempA)  //С��
//				 {
//					 //�������������������������
//					 for (int i=0;i<tempN;i++)
//					 {
//						 int tuplesindex=tempcode[i];//����������¼��Ԫ������
//						 int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//ͨ��Ԫ��������ҳ��������ݵ�����
//						 if (m_testDatas[testindex][att]<=cut) //�����������
//						 {
//							 newTempcode[tmpi]=tempcode[i];   //���������¼����������������
//							 tmpi++;
//						 }
//					 }
//
//				 }
//				 if (stRuleStr==tempB)  //����
//				 {
//					 //�������������������������
//					 for (int i=0;i<tempN;i++)
//					 {
//						 int tuplesindex=tempcode[i];//����������¼��Ԫ������
//						 int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//ͨ��Ԫ��������ҳ��������ݵ�����
//						 if (m_testDatas[testindex][att]>cut) //�����������
//						 {
//							 newTempcode[tmpi]=tempcode[i];   //���������¼����������������
//							 tmpi++;
//						 }
//					 }
//				 }	
//				 //��������
//				 tempN=tmpi;//����������
//				 tmpi=0;    //����������
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
//// ���ý�����
//void ModelIntegrator::SetProgressBar(CProgressCtrl* progressBar)
//{
//	m_progressBar=progressBar;
//
//}

////�������еؽ����򼯴洢�������У�
//CString* ModelIntegrator::GetRuleSet1by1(TreeSelection * trees/*���ɵ�����*/,int & ruleSetRow/*��������ù��򼯵�����*/)
//{
//	//����洢�ռ�,������
//	CString rulesSet[3000];
//	
//	int ruleRow=0;//���������������ͷ�͹���ͷ��count="3" cover="40.000000" correct="40.000000" pre="5.888878" class="10"
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

// �������
TreeSelection * ModelIntegrator::GetTreeSelection(int & treeSum)
{
	treeSum=m_adaBoostingK;
	return m_adaBoosting.GetCompoundModel();
}


// ��ȡ��������
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
