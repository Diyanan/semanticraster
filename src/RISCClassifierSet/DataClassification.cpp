#include "StdAfx.h"
#include "DataClassification.h"
#include "ClassifiedData.h"

#include <cmath>
#include<iostream>
using namespace std;

typedef struct statistics
{ 
	int classId;     //��ֵ   
	double PreWeightSum;//Ȩ��

}GLC_adTree_Statistics;
DataClassification::DataClassification(void)
	: m_data(NULL)
	, m_dataRow(0)
	, m_dataLine(0)
	, m_data_Tuples(NULL)	
	, m_ruleSetI(0)
	, m_ruleCount(0)
	, m_classifideData(NULL)
	, m_classData(NULL)
{
	

}
DataClassification::DataClassification(double **data,int row,int line)
	: m_data(NULL)
	, m_dataRow(0)
	, m_dataLine(0)
	, m_data_Tuples(NULL)	
	, m_ruleSetI(0)
	, m_ruleCount(0)
	, m_classifideData(NULL)
	, m_classData(NULL)
{
	//��ʼ������������
	m_data=data;
	m_dataRow=row;
	m_dataLine=line;
	//��ʼ��������Ԫ��
	TuplesClassified* data_Tuples;
	data_Tuples=new TuplesClassified[row];
	for (int j=0;j<row;j++)
	{
		data_Tuples[j].i=j;
		data_Tuples[j].sampleDataIndex=j;
		data_Tuples[j].tuplesClassID=-1;
		data_Tuples[j].err=0;
		data_Tuples[j].weight=0;
		data_Tuples[j].clssified_data=NULL;
	}
	m_data_Tuples=data_Tuples;
	
}


DataClassification::~DataClassification(void)
{
	try
	{
		delete [] m_data_Tuples;
	}
	catch(std::exception ex)
	{
		throw ex.what();
	}
	
		

}


//// ִ�з���
//ClassifiedData* DataClassification::doClassify(CString * rules,int rulsRow)
//{
//
//	return NULL;
//}
//ClassifiedData  * DataClassification::doClassify(TreeSelection * treeSelection,int treeSum)
//{
//	//��ʼ���������ݣ�������һ����
//	ClassifiedData * classData=new ClassifiedData[m_dataRow];        //***************************�����Ż��ĵط�****************************
//
//	try
//	{
//		for (int i=0;i<m_dataRow;i++)
//		{
//			classData[i].SetData(i,treeSum);
//		}
//	}
//	catch(std::exception ex)
//	{
//		cout<<"����classData[i].SetData(i,treeSum)λ��DataClassification::doClassify"<<endl;
//		
//		return NULL;
//	}
//	
//	
//	
//	TuplesClassified *classTuples=new TuplesClassified[m_dataRow];
//	for (int t=0;t<m_dataRow;t++)
//	{
//		classTuples[t].err=m_data_Tuples[t].err;
//		classTuples[t].i=m_data_Tuples[t].i;
//		classTuples[t].pre=m_data_Tuples[t].pre;
//		classTuples[t].sampleDataIndex=m_data_Tuples[t].sampleDataIndex;
//		classTuples[t].tuplesClassID=m_data_Tuples[t].tuplesClassID;
//		classTuples[t].weight=m_data_Tuples[t].weight;
//		classTuples[t].clssified_data=&classData[t];
//	}
//  
//	for (int i=0;i<treeSum;i++)
//	{
//		TRY 
//		{
//			TuplesClassified *afterClassTuples=new TuplesClassified[m_dataRow];
//			TreeNode * tree=treeSelection[i].Tree;
//			double errorRatio=treeSelection[i].erroratio;
//			int tuplesI=0;
//			int &tI=tuplesI;
//			int isRoot=0;//�����жϵݹ�ĵ�һ��
//			int &root=isRoot;
//			ClassfyWithTree2(tree,errorRatio,classTuples,m_dataRow,afterClassTuples,tI,i,root);
//
//		
//			delete [] afterClassTuples;
//		}
//		CATCH (CMemoryException, e)
//		{
//			MessageBox(NULL,_T("����ClassfyWithTree2(tree,errorRatio,classTuples,m_dataRow,afterClassTuples,tI,i,root)λ��DataClassification::doClassify"),_T("����"),MB_OK);
//			return NULL;
//			
//		}
//		END_CATCH
//		
//	}
//	
//	TRY 
//	{
//		//ͶƱ���
//		for (int index=0;index<m_dataRow;index++)
//		{
//			int classNum;//�����
//			classNum=classData[index].StatisticsI;
//			if (classNum==0)
//			{
//				MessageBox(NULL,_T("�������޷�ͶƱ��classNum==0"),_T("ע�⣡"),MB_OK);
//			}
//			if (classNum==1)
//			{
//				classData[index].classId=classData[index].Statistics[0].tuplesClassID;
//			}
//			else
//			{
//				double maxPre=-1;
//				int maxPreI=-1;
//				int temp[30],tempI=0;
//				for (int i=0;i<classNum;i++)
//				{
//					if (maxPre<=classData[index].Statistics[i].pre)
//					{
//
//						maxPre=classData[index].Statistics[i].pre;
//						maxPreI=i;
//
//						if (maxPre==classData[index].Statistics[i].pre)
//						{
//							temp[tempI]=i;
//							tempI++;
//						}
//					}
//
//				}
//				if (tempI!=0)
//				{
//					maxPreI=rand()%tempI;
//				}
//				if (maxPre==-1&&maxPreI==-1)
//				{
//					MessageBox(NULL,_T("�������޷�ͶƱ��maxPre==-1&&maxPreI==-1"),_T("ע�⣡"),MB_OK);
//				}
//				classData[index].classId=classData[index].Statistics[maxPreI].tuplesClassID;
//
//
//			}
//		}
//
//	}
//	CATCH (CMemoryException, e)
//	{
//		MessageBox(NULL,_T("����ͶƱ�����λ��DataClassification::doClassify"),_T("����"),MB_OK);
//		return NULL;
//	}
//	END_CATCH
//	
//
//	m_classifideData=classData;
//	TRY 
//	{
//		delete [] classTuples;
//	}
//	CATCH (CMemoryException, e)
//	{
//		MessageBox(NULL,_T("����elete [] classTuplesλ��DataClassification::doClassify"),_T("����"),MB_OK);
//		return NULL;
//	}
//	END_CATCH
//	
//	return classData;
//}
//ClassifiedData  * DataClassification::doClassify(TreeSelection * treeSelection,int treeSum,CString & Eror)
//{
//	//��ʼ���������ݣ�������һ����
//	ClassifiedData * classData=new ClassifiedData[m_dataRow];
//	for (int i=0;i<m_dataRow;i++)
//	{
//		classData[i].SetData(i,treeSum);
//	}
//	//��ʼÿ��ѭ��
//	/*TuplesClassified classTuples[1000];
//	TuplesClassified afterClassTuples[1000];*/
//
//	
//	
//	TuplesClassified *classTuples=new TuplesClassified[m_dataRow];
//	for (int t=0;t<m_dataRow;t++)
//	{
//		classTuples[t].err=m_data_Tuples[t].err;
//		classTuples[t].i=m_data_Tuples[t].i;
//		classTuples[t].pre=m_data_Tuples[t].pre;
//		classTuples[t].sampleDataIndex=m_data_Tuples[t].sampleDataIndex;
//		classTuples[t].tuplesClassID=m_data_Tuples[t].tuplesClassID;
//		classTuples[t].weight=m_data_Tuples[t].weight;
//		classTuples[t].clssified_data=&classData[t];
//	}
//  
//	for (int i=0;i<treeSum;i++)
//	{
//		TuplesClassified *afterClassTuples=new TuplesClassified[m_dataRow];
//		TreeNode * tree=treeSelection[i].Tree;
//		double errorRatio=treeSelection[i].erroratio;
//		int tuplesI=0;
//		int &tI=tuplesI;
//		int isRoot=0;//�����жϵݹ�ĵ�һ��
//		int &root=isRoot;
//		ClassfyWithTree2(tree,errorRatio,classTuples,m_dataRow,afterClassTuples,tI,i,root);
//
//
//
//		
//		CString pat=_T("C:\\Data\\classfy_data.data");
//        delete [] afterClassTuples;
//		Tree temp;
//		temp.DeleteTree(tree);
//	}
//	
//
//	//ͶƱ���
//	for (int index=0;index<m_dataRow;index++)
//	{
//		int classNum;//�����
//		classNum=classData[index].StatisticsI;
//		if (classNum==0)
//		{
//			//MessageBox(NULL,_T("�������޷�ͶƱ��classNum==0"),_T("ע�⣡"),MB_OK);
//			Eror=_T("�������޷�ͶƱ��classNum==0");
//			return NULL;
//		}
//		if (classNum==1)
//		{
//			classData[index].classId=classData[index].Statistics[0].tuplesClassID;
//		}
//		else
//		{
//			double maxPre=-1;
//			int maxPreI=-1;
//			int temp[30],tempI=0;
//			for (int i=0;i<classNum;i++)
//			{
//				if (maxPre<=classData[index].Statistics[i].pre)
//				{
//				    
//					maxPre=classData[index].Statistics[i].pre;
//					maxPreI=i;
//			
//					if (maxPre==classData[index].Statistics[i].pre)
//					{
//						temp[tempI]=i;
//						tempI++;
//					}
//				}
//
//			}
//			if (tempI!=0)
//			{
//				maxPreI=rand()%tempI;
//			}
//			if (maxPre==-1&&maxPreI==-1)
//			{
//				//MessageBox(NULL,_T("�������޷�ͶƱ��maxPre==-1&&maxPreI==-1"),_T("ע�⣡"),MB_OK);
//				Eror=_T("�������޷�ͶƱ��classNum==0");
//				return NULL;
//
//			}
//			classData[index].classId=classData[index].Statistics[maxPreI].tuplesClassID;
//			
//
//		}
//	}
//
//	m_classifideData=classData;
//	return classData;
//}
//ClassifiedData* DataClassification::doClassify()
//{
//	return NULL;
//}
//Ԥ�ⵥ������
bool  DataClassification::Tree_Classfy_1(TreeNode* root,double * classifydata,int &classID)
{
	try
	{
		if (root==NULL)
		{
			return false;
		}
		if (root->isLeaf==true)
		{
			classID=root->ClassID;
			return true;

		}
		double splitValue=root->splitVertex;
		int splitAttIndex=root->attIndex;
		double tempAttdata=classifydata[splitAttIndex];
		if (tempAttdata<=splitValue)
		{
			Tree_Classfy_1(root->leftChild,classifydata,classID);
		}
		else
		{
			Tree_Classfy_1(root->rightChild,classifydata,classID);
		}	
	}
	catch (...)
	{
		return false;

	}


	return true;
}



// ��Ԫ�����
//TuplesClassified* DataClassification::TuplesClassify(TuplesClassified* Tuples,int tuplesNum,CString * rules,int rulsN)
//{
//	TuplesClassified* MeasuringTuples=new TuplesClassified[tuplesNum];
//	for (int i=0;i<tuplesNum;i++)
//	{
//		MeasuringTuples[i].err=Tuples[i].err;
//		MeasuringTuples[i].i=Tuples[i].i;
//		MeasuringTuples[i].pre=Tuples[i].pre;
//		MeasuringTuples[i].sampleDataIndex=Tuples[i].sampleDataIndex;
//		MeasuringTuples[i].tuplesClassID=Tuples[i].tuplesClassID;
//		MeasuringTuples[i].weight=Tuples[i].weight;
//	}
//	CString FileData;int FileDatai=1; //����һ��CString������Ϊ������
//	int line=0,row=0; //������
//
//	CString stOld=_T("class=\""),stNew=_T("class=\"");
//	int tempTupleN=tuplesNum;
//	int classId;     //���
//	double pre;      //Ԥ��Ȩ��
//	/*
//	CString temp=_T("class=\"");
//	int j=temp.GetLength();
//	int k=FileData.Find(_T("class=\""));
//	int m=k+j;
//	int n=FileData.Find(_T("\""),m);
//	CString stClassId=FileData.Mid(m,n);
//	_stscanf(stClassId,_T( "%d"),&classId);
//	*/
//	//��ͷ��ʼ��
//	FileData=rules[0];                           //��ȡ��һ�У��õ���ͷ
//	//��ʼ�����
//	CString temp=_T("class=\"");
//	int j=temp.GetLength();
//	int k=FileData.Find(_T("class=\""));
//	int m=k+j;
//	int n=FileData.Find(_T("\""),m);
//	CString stClassId=FileData.Mid(m,n-1);
//	_stscanf(stClassId,_T( "%d"),&classId);
//	//��ʼ��Ԥ��Ȩ��
//	temp=_T("pre=\"");
//	j=temp.GetLength();
//	k=FileData.Find(_T("pre=\""));
//	m=k+j;
//	n=FileData.Find(_T("\""),m);
//	CString stPre=FileData.Mid(m,n);
//	_stscanf(stPre,_T( "%lf"),&pre);
//	
//	int tempcode[1000];       //��¼�ϴ�ѭ������¼������
//	int newTempcode[1000];    //ѭ��������¼������
//	int tempN=tuplesNum;      //��ʼ������������Ԫ�������ͬ
//	int tempi=0;              //suoyi����������ʼΪ0
//	//��ʼ�������������¼Ԫ�鰴˳���������
//	for (int ii=0;ii<tempN;ii++)
//	{
//		tempcode[ii]=ii;
//	}
//
//	//ѭ����ȡ�ļ��е�����
//	while (FileDatai<rulsN)
//	{
//		FileData=rules[FileDatai++];               //��ȡһ������
//		if (FileData.Find(_T("class=\""))>-1)//���������ͷ����֮ǰ�ķ�����д���
//		{
//			//��֮ǰ�ķ�����д���
//			for (int i=0;i<tempN;i++)      
//			{
//				int tuplesIndex=tempcode[i];
//				MeasuringTuples[tuplesIndex].tuplesClassID=classId;
//				MeasuringTuples[tuplesIndex].pre=pre;
//			}
//
//			//������ǰ�ı�ͷ���ݣ����ֳ�ʼ��
//			//��ͷ��ʼ��
//			//��ʼ�����
//			CString temp=_T("class=\"");
//			int j=temp.GetLength();
//			int k=FileData.Find(_T("class=\""));
//			int m=k+j;
//			int n=FileData.Find(_T("\""),m);
//			CString stClassId=FileData.Mid(m,n);
//			_stscanf(stClassId,_T( "%d"),&classId);
//			//��ʼ��Ԥ��Ȩ��
//			temp=_T("pre=\"");
//			j=temp.GetLength();
//			k=FileData.Find(_T("pre=\""));
//			m=k+j;
//			n=FileData.Find(_T("\""),m);
//			CString stPre=FileData.Mid(m,n);
//			_stscanf(stPre,_T( "%lf"),&pre);
//
//			//������ʼ��
//			tempcode[1000];       //��¼�ϴ�ѭ������¼������
//			newTempcode[1000];    //ѭ��������¼������
//			tempN=tuplesNum;      //��ʼ������������Ԫ�������ͬ
//			tempi=0;              //suoyi����������ʼΪ0
//			//��ʼ�������������¼Ԫ�鰴˳���������
//			for (int ii=0;ii<tempN;ii++)
//			{
//				tempcode[ii]=ii;
//			}
//		}
//		else
//		{
//			//��ȡ��������
//			CString stAtt=_T("att=\"");
//			int i= FileData.Find(_T("att=\""));
//			int j=i+stAtt.GetLength();
//			int k=j;
//			int m=0;
//			CString t=_T("\"");
//			while(FileData[k]!=t)
//			{
//				m++;
//				k++;
//			};
//			CString attStr=FileData.Mid(j,m);
//			int att=0;
//			_stscanf(attStr,_T("%d"),&att);
//			att=att-1;
//			//��ȡ����ֵ
//			CString stSplitPoint=_T("cut=\"");
//			i= FileData.Find(_T("cut=\""));
//			j=i+stSplitPoint.GetLength();
//			k=j;
//			m=0;
//			t=_T("\"");
//			while(FileData[k]!=t)
//			{
//				m++;
//				k++;
//			};
//			CString stSplitPointStr=FileData.Mid(j,m);
//			double  cut=0;
//			_stscanf(stSplitPointStr,_T("%lf"),&cut);
//			//��ȡ�жϷ���
//			CString stRule=_T("result=\"");
//			i= FileData.Find(_T("result=\""));
//			j=i+stRule.GetLength();
//			k=j;
//			m=0;
//			t=_T("\"");
//			while(FileData[k]!=t)
//			{
//				m++;
//				k++;
//			};
//			CString stRuleStr=FileData.Mid(j,m);
//			CString tempA=_T("<=");
//			CString tempB=_T(">");
//			//�����ݷ���
//			int tmpi=0;        //���������������ݱ����ɷ������������ĸ���
//			if (stRuleStr==tempA)  //С��
//			{
//				//�������������������������
//				for (int i=0;i<tempN;i++)
//				{
//					int tuplesindex=tempcode[i];//����������¼��Ԫ������
//					int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//ͨ��Ԫ��������ҳ��������ݵ�����
//					if (m_data[testindex][att]<=cut) //�����������
//					{
//						newTempcode[tmpi]=tempcode[i];   //���������¼����������������
//						tmpi++;
//					}
//				}
//
//			}
//			if (stRuleStr==tempB)  //����
//			{
//				//�������������������������
//				for (int i=0;i<tempN;i++)
//				{
//					int tuplesindex=tempcode[i];//����������¼��Ԫ������
//					int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//ͨ��Ԫ��������ҳ��������ݵ�����
//					if (m_data[testindex][att]>cut) //�����������
//					{
//						newTempcode[tmpi]=tempcode[i];   //���������¼����������������
//						tmpi++;
//					}
//				}
//			}	
//			//��������
//			tempN=tmpi;//����������
//			tmpi=0;    //����������
//			for (int i=0;i<tempN;i++)
//			{
//				tempcode[i]=newTempcode[i];
//			}
//		}
//	}
//	//	TuplesClassified *M=MeasuringTuples;
//
//	//	return M;
//	
//	return MeasuringTuples;
//}
////��õ���������
//CString* DataClassification::GetRuleSet_SingleTree(TreeSelection * trees, int treei/*���ı��*/)
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
//		/*	int l=temp.Find(_T("count=\""));
//			if (l>0&&i>0)
//			{
//				CString frent=temp.Left(l);
//				TT[ttI]=frent;
//				ttI++;
//				CString after=temp.Right(l);
//				ttI++;
//
//			}*/
//		}
//		
//		trees[treei].rulset=TT;   //��ÿ���������Ϲ���
//		trees[treei].ruleeSetRow=ttI;
//		delete [] m_ruleSet;
//		m_ruleSetI=ttI;
//		//att="0" cut="381.000000" result="<"count="2" cover="70.000000" correct="70.000000" pre="3.864600" class="2"
//	  //  PrintRules(TT,ttI,pt);
//
//		return TT;
//		
//	
//}
//bool DataClassification::InorderForRuleSet(TreeNode* root, int tag, double erroratio,CString stRules,int rulesN)
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
//
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

// ��ȨͶƱ���
int DataClassification::WeightedVoting(ClassifiedData dataWaitingVote)
{

	int k=dataWaitingVote.classificationTimes;
	ResutEveryTime TempSave;
	ResutEveryTime *dataInfo=new ResutEveryTime[k];
	
	for (int i=0;i<k;i++)
	{
		dataInfo[i].pre=dataWaitingVote.resultEveryT[i].pre;
		dataInfo[i].tuplesClassID=dataWaitingVote.resultEveryT[i].tuplesClassID;
		dataInfo[i].isProseed=true;
	}
	ResutEveryTime statistics[30];int statisticsI=0;
	int noRroseed=0;
	ResutEveryTime temp[30];int tempI=0;
	ResutEveryTime head;
	while(noRroseed<k)
	{
	     tempI=0;
		for (int i=0;i<k;i++)
		{
			if (dataInfo[i].isProseed==true)
			{
				head=dataInfo[i];
				temp[tempI]=dataInfo[i];tempI++;
				dataInfo[i].isProseed=false;
				noRroseed++;
				continue;
			}
			if (head.tuplesClassID==dataInfo[i].tuplesClassID)
			{
				temp[tempI]=dataInfo[i];tempI++;
				dataInfo[i].isProseed=false;
				noRroseed++;
			}
		}
		int classId=-1;
		double pre=0;
		for (int i=0;i<tempI;i++)
		{
			pre=pre+temp[i].pre;
			classId=temp[i].tuplesClassID;
		}
		statistics[statisticsI].pre=pre;
		statistics[statisticsI].tuplesClassID=classId;
		statisticsI++;
	}
	int maxClassID=0;
	double maxPre=0;
	//�ж����ֵ
	for (int i=0;i<statisticsI;i++)
	{
		if (i==0)
		{
			maxClassID=statistics[i].tuplesClassID;
			maxPre=statistics[i].pre;
			continue;
		}
		if (maxPre<statistics[i].pre)
		{
			maxPre=statistics[i].pre;
			maxClassID=maxClassID;
		}
	}
	dataWaitingVote.classId=maxClassID;
	/*CString path=_T("C:\\Data\\classify_vote.data");
	Print(path,dataWaitingVote);*/
	delete [] dataInfo;
	return maxClassID;
}

//int DataClassification::WeightedVoting(ClassifiedData dataWaitingVote)
//{
//
//	int k=dataWaitingVote.classificationTimes;
//	ResutEveryTime TempSave;
//	ResutEveryTime *dataInfo=new ResutEveryTime[k];
//	
//	for (int i=0;i<k;i++)
//	{
//		dataInfo[i].pre=dataWaitingVote.resultEveryT[i].pre;
//		dataInfo[i].tuplesClassID=dataWaitingVote.resultEveryT[i].tuplesClassID;
//		dataInfo[i].isProseed=true;
//	}
//	ResutEveryTime statistics[30];int statisticsI=0;
//	int noRroseed=0;
//	ResutEveryTime temp[30];int tempI=0;
//	ResutEveryTime head;
//	while(noRroseed<k)
//	{
//	     tempI=0;
//		for (int i=0;i<k;i++)
//		{
//			if (dataInfo[i].isProseed==true)
//			{
//				head=dataInfo[i];
//				temp[tempI]=dataInfo[i];tempI++;
//				dataInfo[i].isProseed=false;
//				noRroseed++;
//				continue;
//			}
//			if (head.tuplesClassID==dataInfo[i].tuplesClassID)
//			{
//				temp[tempI]=dataInfo[i];tempI++;
//				dataInfo[i].isProseed=false;
//				noRroseed++;
//			}
//		}
//		int classId=-1;
//		double pre=0;
//		for (int i=0;i<tempI;i++)
//		{
//			pre=pre+temp[i].pre;
//			classId=temp[i].tuplesClassID;
//		}
//		statistics[statisticsI].pre=pre;
//		statistics[statisticsI].tuplesClassID=classId;
//		statisticsI++;
//	}
//	int maxClassID=0;
//	double maxPre=0;
//	//�ж����ֵ
//	for (int i=0;i<statisticsI;i++)
//	{
//		if (i==0)
//		{
//			maxClassID=statistics[i].tuplesClassID;
//			maxPre=statistics[i].pre;
//			continue;
//		}
//		if (maxPre<statistics[i].pre)
//		{
//			maxPre=statistics[i].pre;
//			maxClassID=maxClassID;
//		}
//	}
//	dataWaitingVote.classId=maxClassID;
//	/*CString path=_T("C:\\Data\\classify_vote.data");
//	Print(path,dataWaitingVote);*/
//	delete [] dataInfo;
//	return maxClassID;
//}

// ����
void DataClassification::Swap(ClassifiedResutEveryTime  * a , ClassifiedResutEveryTime  * b)
{
	/*ClassifiedResutEveryTime TempSave;

		TempSave.pre=*a.pre;

		*a=*b;
		*b=TempSave;*/
	
}
//����Ԫ���ӡ����
//void DataClassification::PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified* tuples, int tuplesN,CString path)
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
//void DataClassification::PrintTuples(TuplesClassified * tuples, int tuplesN, CString path)
//{
//	CStdioFile file2;
//	file2.Open(path,CFile::modeCreate|CFile::modeWrite);
//	int i=0;
//	int sindex=0;
//	int classid=0;
//	int err=0;
//	double w=0;
//	double pre=0;
//	CString sti,stSindex,stClassid,stErr,stW,stPre,st,ed;
//	TuplesClassified * Di=tuples;	 
//
//	for (int ii=0;ii<tuplesN;ii++)
//	{
//		i=Di[ii].i;
//		sindex=Di[ii].sampleDataIndex;
//		classid=Di[ii].tuplesClassID;
//		err=Di[ii].err;
//		w=Di[ii].weight;
//		pre=Di[ii].pre;
//
//		sti.Format(_T("%d"),i);
//		stSindex.Format( _T("%d"),sindex);
//		stClassid.Format( _T("%d"),classid);
//		stPre.Format(_T("%f"),pre);
//		stW.Format( _T("%lf"),w);
//		stErr.Format( _T("%d"),err);
//		ed=_T("\r\n");
//		st=_T("i=")+sti+_T("sindex=")+stSindex+_T(",classId=")+stClassid+_T(",pte=")+stPre+_T(",err=")+
//			stErr+_T(",w=")+stW+ed;
//
//		file2.WriteString(st);
//	}
//	file2.Close();
//}
bool DataClassification::ClassfyWithTree(TreeNode* root, double erroratio,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI)
{
	if (root==NULL)
	{
		return false;
	}
	else
	{
		if (root->isLeaf)
		{
			double err=0;
			double acc=0,pre=0;
			err=root->errors;
			
			double cover=root->allN;
            double correct=cover-err;
			acc=correct/cover;
			pre=acc*log(1/erroratio);
			root->pre=pre;
			
			int classId;
			classId=root->ClassID;
			for (int i=0;i<tuplesN;i++)
			{
				
				
				tuplesResult[tuplesResultI].err=tuples[i].err;
				tuplesResult[tuplesResultI].i=tuples[i].i;
				tuplesResult[tuplesResultI].pre=pre;
				tuplesResult[tuplesResultI].sampleDataIndex=tuples[i].sampleDataIndex;
				tuplesResult[tuplesResultI].tuplesClassID=classId;
				tuplesResult[tuplesResultI].weight=tuples[i].weight;
				tuplesResult[tuplesResultI].clssified_data=tuples[i].clssified_data;
				tuplesResultI++;
				
			}
			delete [] tuples;
		}
		else
		{
        TuplesClassified *tuplesL,*tuplesR;
		int tuplesLN=0,tuplesRN=0;
		int tuplesLi=0,tuplesRi=0;
			
		double splitpoint=root->splitVertex;//��÷��ѵ�
		int splitattIndex=root->attIndex;        //��÷�������

		//�������������ݸ���
		for (int i=0;i<tuplesN;i++)
		{
			
	

			int dataIndex=tuples[i].sampleDataIndex;
			if (m_data[dataIndex][splitattIndex]<=splitpoint)
			{
				
				tuplesLN++;
			}
			else
			{
				
				tuplesRN++;
			}
		}
		if (tuplesLN!=0&&tuplesRN!=0)
		{
			//��������
			tuplesL=new TuplesClassified[tuplesLN];
			tuplesR=new TuplesClassified[tuplesRN];
			//
			for (int i=0;i<tuplesN;i++)
			{
				int dataIndex=tuples[i].sampleDataIndex;
				if (m_data[dataIndex][splitattIndex]<=splitpoint)
				{
					tuplesL[tuplesLi].err=tuples[i].err;
					tuplesL[tuplesLi].i=tuples[i].i;
					tuplesL[tuplesLi].pre=tuples[i].pre;
					tuplesL[tuplesLi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesL[tuplesLi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesL[tuplesLi].weight=tuples[i].weight;
					tuplesL[tuplesLi].clssified_data=tuples[i].clssified_data;
					tuplesLi++;
				}
				else
				{
					tuplesR[tuplesRi].err=tuples[i].err;
					tuplesR[tuplesRi].i=tuples[i].i;
					tuplesR[tuplesRi].pre=tuples[i].pre;
					tuplesR[tuplesRi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesR[tuplesRi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesR[tuplesRi].weight=tuples[i].weight;
					tuplesR[tuplesRi].clssified_data=tuples[i].clssified_data;
					tuplesRi++;
				}
			}
			delete [] tuples;
			ClassfyWithTree(root->leftChild,erroratio,tuplesL,tuplesLN,tuplesResult,tuplesResultI);//���������������
			ClassfyWithTree(root->rightChild,erroratio,tuplesR,tuplesRN,tuplesResult,tuplesResultI);//��������������

		}
		if (tuplesLN==0&&tuplesRN!=0)
		{
			//��������
		
			tuplesR=new TuplesClassified[tuplesRN];
			//
			for (int i=0;i<tuplesN;i++)
			{
				    int dataIndex=tuples[i].sampleDataIndex;
				
					tuplesR[tuplesRi].err=tuples[i].err;
					tuplesR[tuplesRi].i=tuples[i].i;
					tuplesR[tuplesRi].pre=tuples[i].pre;
					tuplesR[tuplesRi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesR[tuplesRi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesR[tuplesRi].weight=tuples[i].weight;
					tuplesR[tuplesRi].clssified_data=tuples[i].clssified_data;
					tuplesRi++;
		
			}
			delete [] tuples;
			
			ClassfyWithTree(root->rightChild,erroratio,tuplesR,tuplesRN,tuplesResult,tuplesResultI);//��������������

		}
		if (tuplesLN!=0&&tuplesRN==0)
		{
			//��������
			tuplesL=new TuplesClassified[tuplesLN];
		
		
			for (int i=0;i<tuplesN;i++)
			{
			        int dataIndex=tuples[i].sampleDataIndex;
				
					tuplesL[tuplesLi].err=tuples[i].err;
					tuplesL[tuplesLi].i=tuples[i].i;
					tuplesL[tuplesLi].pre=tuples[i].pre;
					tuplesL[tuplesLi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesL[tuplesLi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesL[tuplesLi].weight=tuples[i].weight;
					tuplesL[tuplesLi].clssified_data=tuples[i].clssified_data;
					tuplesLi++;
				
			
			}
			delete [] tuples;
			ClassfyWithTree(root->leftChild,erroratio,tuplesL,tuplesLN,tuplesResult,tuplesResultI);//���������������
		}
	
		}
		
	}
	return true;
}
bool DataClassification::ClassfyWithTree1(TreeNode* root, double erroratio,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI)
{
	if (tuplesN>5000)
	{
		cout<<"ע��:����������Ԥ����Χ��5000������ֿ鴦��"<<endl;
		return false;
	}
	if (root==NULL)
	{
		return false;
	}
	else
	{
		if (root->isLeaf)
		{
			double err=0;
			double acc=0,pre=0;
			err=root->errors;

			double cover=root->allN;
			double correct=cover-err;
			acc=correct/cover;
			pre=acc*log(1/erroratio);
			root->pre=pre;

			int classId;
			classId=root->ClassID;
			for (int i=0;i<tuplesN;i++)
			{

				//��ֵ
				tuplesResult[tuplesResultI].err=tuples[i].err;
				tuplesResult[tuplesResultI].i=tuples[i].i;
				tuplesResult[tuplesResultI].pre=pre;
				tuplesResult[tuplesResultI].sampleDataIndex=tuples[i].sampleDataIndex;
				tuplesResult[tuplesResultI].tuplesClassID=classId;
				tuplesResult[tuplesResultI].weight=tuples[i].weight;
				tuplesResult[tuplesResultI].clssified_data=tuples[i].clssified_data;
				tuplesResultI++;

			}
			delete [] tuples;
		}
		else
		{
			 ;
			int tuplesLN=0,tuplesRN=0;
			int tuplesLi=0,tuplesRi=0;

			double splitpoint=root->splitVertex;//��÷��ѵ�
			int splitattIndex=root->attIndex;        //��÷�������

			//�������������ݸ���
			
			TuplesClassified *tuplesL=new TuplesClassified[tuplesN];
			TuplesClassified *tuplesR=new TuplesClassified[tuplesN];

			for (int i=0;i<tuplesN;i++)
			{
				int dataIndex=tuples[i].sampleDataIndex;
				if (m_data[dataIndex][splitattIndex]<=splitpoint)
				{
					tuplesL[tuplesLi].err=tuples[i].err;
					tuplesL[tuplesLi].i=tuples[i].i;
					tuplesL[tuplesLi].pre=tuples[i].pre;
					tuplesL[tuplesLi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesL[tuplesLi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesL[tuplesLi].weight=tuples[i].weight;
					tuplesL[tuplesLi].clssified_data=tuples[i].clssified_data;
					tuplesLi++;
				}
				else
				{
					tuplesR[tuplesRi].err=tuples[i].err;
					tuplesR[tuplesRi].i=tuples[i].i;
					tuplesR[tuplesRi].pre=tuples[i].pre;
					tuplesR[tuplesRi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesR[tuplesRi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesR[tuplesRi].weight=tuples[i].weight;
					tuplesR[tuplesRi].clssified_data=tuples[i].clssified_data;
					tuplesRi++;
				}
			}
			if (tuplesLi!=0&&tuplesRi!=0)
			{


				delete [] tuples;
				ClassfyWithTree1(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI);//���������������
				ClassfyWithTree1(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI);//��������������

			}
			if (tuplesLi==0&&tuplesRi!=0)
			{
				//��������

			
				delete [] tuples;

				ClassfyWithTree1(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI);//��������������

			}
			if (tuplesLi!=0&&tuplesRi==0)
			{
				//��������
		
				delete [] tuples;
				ClassfyWithTree1(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI);//���������������
			}

		}

	}
	return true;
}
bool DataClassification::ClassfyWithTree2(TreeNode* root, double erroratio,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI,int tree_NO,int& isRoot)
{
	if (tuplesN>5000)
	{
		cout<<"ע��:����������Ԥ����Χ��5000������ֿ鴦��"<<endl;
		return false;
	}
	if (root==NULL)
	{
		return false;
	}
	else
	{
		if (root->isLeaf)
		{
			double err=0;
			double acc=0,pre=0;
			err=root->errors;

			double cover=root->allN;
			double correct=cover-err;
			acc=correct/cover;
			pre=acc*log(1/erroratio);
			root->pre=pre;

			int classId;
			classId=root->ClassID;
			for (int i=0;i<tuplesN;i++)
			{

				//��ֵ
				tuplesResult[tuplesResultI].err=tuples[i].err;
				tuplesResult[tuplesResultI].i=tuples[i].i;
				tuplesResult[tuplesResultI].pre=pre;
				tuplesResult[tuplesResultI].sampleDataIndex=tuples[i].sampleDataIndex;
				tuplesResult[tuplesResultI].tuplesClassID=classId;
				tuplesResult[tuplesResultI].weight=tuples[i].weight;
				tuplesResult[tuplesResultI].clssified_data=tuples[i].clssified_data;

				//	if (i==0)
				//	{
				//		int& staI=afterClassTuples[m].clssified_data->StatisticsI;
				//		afterClassTuples[m].clssified_data->Statistics[staI].pre=afterClassTuples[m].pre;
				//		afterClassTuples[m].clssified_data->Statistics[staI].tuplesClassID=afterClassTuples[m].tuplesClassID;
				//		afterClassTuples[m].clssified_data->Statistics[staI].isvotied=true;
				//		staI++;
				//	}

				//�����ж�����,�޸ķ������ݵĲ���
				tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].pre=tuplesResult[tuplesResultI].pre;
				tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].tuplesClassID=tuplesResult[tuplesResultI].tuplesClassID;
				tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].isvotied=true;
				if (tree_NO==0)
				{
					int& staI=tuplesResult[tuplesResultI].clssified_data->StatisticsI;
					tuplesResult[tuplesResultI].clssified_data->Statistics[staI].pre=tuplesResult[tuplesResultI].pre;
					tuplesResult[tuplesResultI].clssified_data->Statistics[staI].tuplesClassID=tuplesResult[tuplesResultI].tuplesClassID;
					tuplesResult[tuplesResultI].clssified_data->Statistics[staI].isvotied=true;
					staI++;
				}
				else
				{
					int staNum=tuplesResult[tuplesResultI].clssified_data->StatisticsI;
					for (int k=0;k<staNum;k++)
					{
						if (tuplesResult[tuplesResultI].clssified_data->Statistics[k].tuplesClassID==tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].tuplesClassID)
						{
							tuplesResult[tuplesResultI].clssified_data->Statistics[k].pre=tuplesResult[tuplesResultI].clssified_data->Statistics[k].pre+tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].pre;
							//afterClassTuples[m].clssified_data->Statistics[k].pre=afterClassTuples[m].clssified_data->Statistics[k].tuplesClassID=afterClassTuples[m].clssified_data->resultEveryT[i].tuplesClassID;
						}
						else
						{
							int& staI=tuplesResult[tuplesResultI].clssified_data->StatisticsI;
							tuplesResult[tuplesResultI].clssified_data->Statistics[staI].pre=tuplesResult[tuplesResultI].pre;
							tuplesResult[tuplesResultI].clssified_data->Statistics[staI].tuplesClassID=tuplesResult[tuplesResultI].tuplesClassID;
							tuplesResult[tuplesResultI].clssified_data->Statistics[staI].isvotied=true;
							staI++;

						}
					}
				}
				//�޸���ϣ��Ͽ�����
			//	tuplesResult[tuplesResultI].clssified_data=NULL;
    //     	for (int m=0;m<m_dataRow;m++)
				//{
				//	afterClassTuples[m].clssified_data->resultEveryT[tree_NO].pre=afterClassTuples[m].pre;
				//	afterClassTuples[m].clssified_data->resultEveryT[tree_NO].tuplesClassID=afterClassTuples[m].tuplesClassID;
				//	afterClassTuples[m].clssified_data->resultEveryT[tree_NO].isvotied=true;

				//	if (i==0)
				//	{
				//		int& staI=afterClassTuples[m].clssified_data->StatisticsI;
				//		afterClassTuples[m].clssified_data->Statistics[staI].pre=afterClassTuples[m].pre;
				//		afterClassTuples[m].clssified_data->Statistics[staI].tuplesClassID=afterClassTuples[m].tuplesClassID;
				//		afterClassTuples[m].clssified_data->Statistics[staI].isvotied=true;
				//		staI++;
				//	}
				//	else
				//	{
				//		int staNum=afterClassTuples[m].clssified_data->StatisticsI;
				//		for (int k=0;k<staNum;k++)
				//		{
				//			if (afterClassTuples[m].clssified_data->Statistics[k].tuplesClassID==afterClassTuples[m].clssified_data->resultEveryT[i].tuplesClassID)
				//			{
				//				afterClassTuples[m].clssified_data->Statistics[k].pre=afterClassTuples[m].clssified_data->Statistics[k].pre+afterClassTuples[m].clssified_data->resultEveryT[i].pre;
				//				//afterClassTuples[m].clssified_data->Statistics[k].pre=afterClassTuples[m].clssified_data->Statistics[k].tuplesClassID=afterClassTuples[m].clssified_data->resultEveryT[i].tuplesClassID;
				//			}
				//			else
				//			{
				//				int& staI=afterClassTuples[m].clssified_data->StatisticsI;
				//				afterClassTuples[m].clssified_data->Statistics[staI].pre=afterClassTuples[m].pre;
				//				afterClassTuples[m].clssified_data->Statistics[staI].tuplesClassID=afterClassTuples[m].tuplesClassID;
				//				afterClassTuples[m].clssified_data->Statistics[staI].isvotied=true;
				//				staI++;

				//			}
				//		}
				//	}

				tuplesResultI++;

			}
			if (isRoot>0)
			{
				delete [] tuples;
			}
			
		}
		else
		{
			;
			int tuplesLN=0,tuplesRN=0;
			int tuplesLi=0,tuplesRi=0;

			double splitpoint=root->splitVertex;//��÷��ѵ�
			int splitattIndex=root->attIndex;        //��÷�������

			//�������������ݸ���

			TuplesClassified *tuplesL=new TuplesClassified[tuplesN];
			TuplesClassified *tuplesR=new TuplesClassified[tuplesN];

			for (int i=0;i<tuplesN;i++)
			{
				int dataIndex=tuples[i].sampleDataIndex;
				if (m_data[dataIndex][splitattIndex]<=splitpoint)
				{
					tuplesL[tuplesLi].err=tuples[i].err;
					tuplesL[tuplesLi].i=tuples[i].i;
					tuplesL[tuplesLi].pre=tuples[i].pre;
					tuplesL[tuplesLi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesL[tuplesLi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesL[tuplesLi].weight=tuples[i].weight;
					tuplesL[tuplesLi].clssified_data=tuples[i].clssified_data;
					tuplesLi++;
				}
				else
				{
					tuplesR[tuplesRi].err=tuples[i].err;
					tuplesR[tuplesRi].i=tuples[i].i;
					tuplesR[tuplesRi].pre=tuples[i].pre;
					tuplesR[tuplesRi].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesR[tuplesRi].tuplesClassID=tuples[i].tuplesClassID;
					tuplesR[tuplesRi].weight=tuples[i].weight;
					tuplesR[tuplesRi].clssified_data=tuples[i].clssified_data;
					tuplesRi++;
				}
			}
            //���ԭ��������
				if (isRoot>0)
				{
					delete [] tuples;
				}
				isRoot++;
			if (tuplesLi!=0&&tuplesRi!=0)
			{

				ClassfyWithTree2(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI,tree_NO,isRoot);//���������������
				ClassfyWithTree2(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI,tree_NO,isRoot);//��������������

			}
			if (tuplesLi==0&&tuplesRi!=0)
			{
				//��������


				ClassfyWithTree2(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI,tree_NO,isRoot);//��������������

			}
			if (tuplesLi!=0&&tuplesRi==0)
			{
				//��������
				
				ClassfyWithTree2(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI,tree_NO,isRoot);//���������������
			}

		}

	}
	return true;
}
void quicksort(TuplesClassified a[], int left, int right){
	if(left >= right) return;
	int i = left,j = right-1;
	//int key = a[left];//ȡ��Ԫ��Ϊ�ؼ�ֵ
	int key = a[(left + right) / 2].i;
	while(i < j)//1
	{
		while(i < j && a[j].i >= key)//���ﲻ��ʹ��>=���п��ܵ���ǰ�벿�ֶ�������������
			j--;
		while(i < j && a[i].i < key)
			i++;
		//swap
		if (i < j){
			TuplesClassified temp;
			//swap(a[i],a[j]);
			temp=a[i];
			a[i]=a[j];
			a[j]=temp;
			
			i++; j--;//��С����
		}
		if(i >= j ) break;//do nothing
	}
	quicksort(a,left,j);//ȡj��ȡi����Ϊ���ߵ�i�п��ܳ�����j
	quicksort(a,j+1,right);
}

int partition(TuplesClassified data[],int lo,int hi)
{
	int i,j;
	TuplesClassified t;
	int key = data[hi].i;   //���������һ��Ԫ����Ϊ�ڱ�������ԪԪ��
	i = lo-1;
	for (j =lo;j<=hi;j++)
		if(data[j].i<key)
		{
			i++;
			t = data[j];
			data[j] = data[i];
			data[i] = t;
		}
		data[hi] = data[i+1];  //��,data[i+1]����data[hi]
		data[i+1] = data[key];       //�󣬰����ȱ����keyֵ����data[hi]����data[i+1]
		//���ɵ�������������˳��
		return i+1;
}
void QuickSort(TuplesClassified a[], int lo, int hi)
{
	if (lo<hi)
	{
		int k = partition(a, lo, hi);
		QuickSort(a, lo, k-1);
		QuickSort(a, k+1, hi);
	}
} 




//void DataClassification::Print(CString path,ClassifiedData data)
//{
//	CStdioFile file2;
//	file2.Open(path,CFile::modeCreate|CFile::modeWrite);
//	CString stData=_T("");
//	CString stI,classID,stPre;
//	double pre;
//	int ID;
//	
//	for (int i=0;i<10;i++)
//	{
//		pre=data.resultEveryT[i].pre;
//		ID=data.resultEveryT[i].tuplesClassID;
//		stPre.Format(_T("%f"),pre);
//		classID.Format(_T("%d"),ID);
//		stI.Format(_T("%d"),i);
//		stData=_T("I=")+stI+_T(",classID=")+classID+_T(",pre=")+stPre;
//		file2.WriteString(stData);
//
//	}
//	file2.Close();
//}


// ��ȡ����֮������ݵ���𣬰������ݵ���ŷ���
short* DataClassification::GetClassifiedID(void)
{
	short *classifiedID=new short[m_dataRow];
	if (m_classifideData==NULL)
	{
		cout<<"ע��:����δ���࣬����ʹ�ã�"<<endl;
		return NULL;
	}
	else
	{
        for (int i=0;i<m_dataRow;i++)
        {
			classifiedID[i]=m_classifideData[i].classId;
			m_classifideData[i].Delete();
			m_classifideData[i].resultEveryT=NULL;
		   
			//m_classifideData[i].
        }
	}
	try
	{
		 delete [] m_classifideData;
	}
	catch (std::exception ex)
	{
		throw ex;
		return NULL;
	}

	return classifiedID;
}


// �������
void DataClassification::Delete(void)
{
	try
	{
		for (int i=0;i<m_dataRow;i++)
		{
			delete [] m_data[i];
		}
		delete []m_data;
	}
	catch (std::exception* e)
	{
		cout<<"����:delete []m_dataλ��DataClassification::~DataClassification(void)"<<endl;
		throw e;
	}

}


// ִ�з���
short DataClassification::doClassifySingle(double* data,TreeSelection *treeSelection,int treenum)
{
	try
	{
		  GLC_adTree_Statistics* sta=new GLC_adTree_Statistics[30];//����������������ó�30��
			   int staI=0;
			   double max_weight_add=0;
			   int max_weight_staI=0;
				for (int i=0;i<treenum;i++)
					{	
						TreeNode * tree=treeSelection[i].Tree;
						int classid=0;
						int & cid=classid;
						Tree_Classfy_1(tree,data,cid);
						if (i==0)//��ʼ��
						{							
							sta[staI].classId=classid;
							sta[staI].PreWeightSum=max_weight_add=treeSelection[i].weight;
							max_weight_staI=staI;
							staI++;
							continue;
						}
						bool notselected=true;//�ж�һ������Ƿ�����ӵ�ͳ�������У�
						for (int j=0;j<staI;j++)
						{
							if (classid==sta[j].classId)
							{
								sta[j].PreWeightSum=sta[j].PreWeightSum+treeSelection[i].weight;								
								notselected=false;
								break;
							}				
						}
						if (notselected)
						{
							sta[staI].classId=classid;
							sta[staI].PreWeightSum=treeSelection[i].weight;
							staI++;
						}
					}
				
				if (staI==1)
				{
					short classId=(short)sta[0].classId;
					delete [] sta;
					return classId;
				}
				for (int i=0;i<staI;i++)
				{
					if (sta[i].PreWeightSum>max_weight_add)
					{
						max_weight_add=sta[i].PreWeightSum;
						max_weight_staI=i;
					}
				}
				/*		short testa[30];
				for (int i=0;i<staI;i++)
				{
				testa[i]=sta[i].classId;
				}*/
				short classid=(short)sta[max_weight_staI].classId;
				delete [] sta;
				return classid;	
	}
	catch (std::exception* e)
	{
		cout<<"����1:ʹ�������������������ܳ���Ԥ��ֵ30!"<<endl;
		return 0;
	}
	
}
