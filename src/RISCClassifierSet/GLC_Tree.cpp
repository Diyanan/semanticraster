#include "StdAfx.h"
#include "GLC_Tree.h"
#include <cmath>
#include <set> 
//*********************//
#include <iostream>
using namespace std;


GLC_Tree::GLC_Tree(void)
	: m_NodeSize(0)
	, m_leafsSize(1)
	, tagT(true)
	, m_Dimension0(0)
	, m_Dimension2(0)
	, m_k(10)
	, m_splitPointValue(0)
	, m_treeHeight(0)
	, m_attListIndex(NULL)
	, m_spDataIndex(NULL)
	, m_initial_tuples(NULL)
	, m_classifiedID(NULL)
{
}


GLC_Tree::~GLC_Tree(void)
{
	delete[] m_classifiedID;
}
void GLC_Tree::DataFormat()
{

}
void GLC_Tree::SetParameter(TreeParameter treeparameter)
{
	m_leafsSize=treeparameter.LeatfSize;
	m_treeHeight=treeparameter.TreeHeight;
	
}
void GLC_Tree::SetData() //��÷�������
{
	//(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/,int & progressI/*���ڼ������*/)
	int n=m_data_line-1;
	int** attListIndex;               //�������Լ�
	attListIndex=new int* [n];  
	for (int i=0;i<n;i++)
	{
		attListIndex[i]=new int [2];
		attListIndex[i][0]=i;
		attListIndex[i][1]=0;

		//cout<<"-";
	}
	m_attListIndex=attListIndex;
	//������������
	int** spDataIndex;
	spDataIndex=new int* [m_data_row];  
	for (int i=0;i<m_data_row;i++)
	{
		spDataIndex[i]=new int [2];
		spDataIndex[i][0]=i;                     //Ԫ���������е�����
		spDataIndex[i][1]=m_data[i][m_data_line-1]; //��¼����Ԫ�������

		//cout<<"+";
	}
	m_spDataIndex=spDataIndex;
	//����Ԫ�鼯
    TuplesClassified *tuples=new TuplesClassified[m_data_row];
	int tuplesNum=m_data_row;
	for (int i=0;i<tuplesNum;i++)
	{
		tuples[i].i=i;
		tuples[i].err=0;
		tuples[i].tuplesClassID=spDataIndex[i][1];//n-1�����ݼ�¼�������ݵ������
		tuples[i].sampleDataIndex=spDataIndex[i][0];//0�����ݼ�¼���������е�����
		tuples[i].weight=1.0/(tuplesNum*1.0);
	}
	m_initial_tuples=tuples;
	//BuildTree2(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine,tuples,tuplesN,m_attListIndex,m_sampleDatasLine-1,2,true,m_treeHeight,tuples,tuplesN,progressI);
	
	//cout<<endl;
}
void GLC_Tree::ExecutTrain(int & progressi)                                     //ִ��ѵ��
{
	SetData();//��������	
	m_tree=Tree_Builde(m_data,m_data_row,m_data_line,m_initial_tuples,m_data_row,m_attListIndex,m_data_line-1,2,true,m_treeHeight,m_initial_tuples,m_data_row,progressi);
	//����ѵ��֮���õ�����
	delete [] m_initial_tuples;

	//cout<<progressi<<"%"<<endl;
	for (int i=0;i<m_data_row;i++)
	{
		if (i<m_data_line-1)
		{
			delete [] m_attListIndex[i];
		}
		delete [] m_spDataIndex[i];

		//cout<<".";
	}

	//cout<<endl;
	delete [] m_spDataIndex;
	delete [] m_attListIndex;
	

}
void GLC_Tree::GetModle(TreeNode * tree)                                        //���ģ��
{
	tree=m_tree;
}
TreeNode *  GLC_Tree::ReturnModle()                                        //���ģ��
{
	return m_tree;
}
void GLC_Tree::SetModle(TreeNode * tree) //����ģ�ͣ��������Ѿ����ɵ�ģ�������з���
{
	m_tree=tree;
}
short* GLC_Tree::ExecutClassify(double **data,int data_row,int data_line,int & progressi/*���ڶ����ݷ���ò�����0�����������һ�����ݵķ���ò�������Ϊ����֮������ֵ����ʼ����Ϊ-1*/)                                   //ִ�з���
{
	if (progressi!=-1)
	{
				//��������
	TuplesClassified *classTuples=new TuplesClassified[data_row];
	ClassifiedData * classData=new ClassifiedData[data_row];
	for (int t=0;t<data_row;t++)
	{
		classData[t].SetData(t,1);
		classTuples[t].err=-1;
		classTuples[t].i=t;
		classTuples[t].pre=0;
		classTuples[t].sampleDataIndex=t;
		classTuples[t].tuplesClassID=-1;
		classTuples[t].weight=-1;
		classTuples[t].clssified_data=&classData[t];
	}
	TuplesClassified *afterClassTuples=new TuplesClassified[data_row];
	TreeNode * tree=m_tree;
	
	int tuplesI=0;
	int &tI=tuplesI;
	int isRoot=0;//�����жϵݹ�ĵ�һ��
	int &root=isRoot;
	m_classifiedID=new short[data_row];
	Tree_Classfy(tree,classTuples,data_row,afterClassTuples,progressi,root,data);
	//************************************************************************//
	/*for (int i=0;i<data_row;i++)
	{
	cout<<afterClassTuples[i].tuplesClassID<<"  index="<<afterClassTuples[i].sampleDataIndex<<"  realValue="<<m_classifiedID[i]<<endl;

	}*/
	//************************************************************************//
	delete [] afterClassTuples;
	delete [] classTuples;
	delete [] classData;
	m_classifiedresultID=m_classifiedID;
	return m_classifiedID;
	}
	else
	{
		TreeNode * tree=m_tree;
		double* classifyData=data[0];
		Tree_Classfy_1(m_tree,classifyData,progressi);
		return NULL;
	}
	
}
short GLC_Tree::ExecutClassify(double *data)                                           //ִ�з���
{

	TreeNode * tree=m_tree;
	int classid=0;
	int & cid=classid;
	Tree_Classfy_1(m_tree,data,cid);
	return classid;
}

short *GLC_Tree::GetTest(double **data,int data_row,int data_line)                                      //��ò��Խ��
{
	int progressI=0;
	int &prI=progressI;

	if (data==NULL)
	{
		short* classsid=ExecutClassify(m_data,m_data_row,m_data_line,prI);
		return classsid;
	}
	
	short* classsid=ExecutClassify(data,data_row,data_line,prI);
	m_testclassID=classsid;
	return classsid;
}
void GLC_Tree::GetAccuracyEvaluation(void)                          //��þ�������
{

}


// ******************************************************************�������ṹ***********************************************************
TreeNode* GLC_Tree::Tree_Builde(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/,int & progressI/*���ڼ������*/)//ʹ����
{	
   
	//
	if (tuplesN==0)
	{
		//cout<<"�������ݲ���Ϊ�գ�"<<endl;
		return NULL;
	}
	//CString timestr;
	
	double ** S; 
	
	TuplesClassified * Tindex=tuples;
	S=new double* [tuplesN];                //�����еĿռ䣻

	for (int i=0;i<tuplesN;i++)
	{
		S[i]=new double [line];

		//cout<<"-";
	}
	for (int i=0;i<tuplesN;i++)
	{
	
		int t=Tindex[i].sampleDataIndex;
	
		for (int j=0;j<line;j++)
		{
			S[i][j]=sampleData[t][j];
		}

		//cout<<"+";
	}
	
	int S_row=tuplesN;         //������ݼ�����
	int S_line=line;       //������ݼ�����

	////����
	//cout<<"<--------------------------------->"<<"���ݸ���:"<<S_row<<"<---------------------------------------->"<<endl;
	//for (int i=0;i<S_row;i++)
	//{
	//	for (int j=0;j<S_line;j++)
	//	{
	//		cout<<S[i][j]<<",";
	//	}
	//	cout<<endl;
	//}
	//	/////////////

	int T_row=tuplesN;
	int **attList=attIndex; //���Լ�������
	int attN=attNum;       //���Լ��ϵĸ���
	double k=0;
	TreeNode * Node;//����һ���ڵ���Ϊ���ڵ�
	Node=new TreeNode;
	m_NodeSize++;    
	
	AttValuesTab *D;
	
	D=new AttValuesTab[T_row];
	int temprow=0;
	
    for (int i=0;i<T_row;i++)
	{
		
		temprow=Tindex[i].sampleDataIndex;
		D[i].classID=sampleData[temprow][line-1];
		D[i].AttValue=sampleData[temprow][0];   
		D[i].AttValue=0;    

		//cout<<"-";
	}
	ClassUnique * L,*p;

	L=getClassUnique(D,T_row);
	p=L;
	int classN=0;
	classN=L->cunt;

	
	int i=0,Max=0,publicClassID=0,Num=0;
	while (p)
	{
		if (i=0)
		{
			p=p->next;
			i++;
		}
		else
		{
			Num=p->cunt;
			if (Max<Num)
			{
				Max=Num;
				publicClassID=p->classID;
			}
			p=p->next;
		}

		//cout<<"+";
	}
	
	int publicClassN=0,allN=0;
	publicClassN=Max;
	allN=L->classID;

	Node->allN=T_row;  //��ýڵ�Ԫ�����
	Node->publicClassN=publicClassN;
	Node->publicClassID=publicClassID;

	if (T_row<=m_leafsSize)     //�����������С�ڵ���6
	{
	//	cout<<"m_leafsSize,������С"<<endl;
		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=publicClassID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;
		
		TuplesClassified *tOri;
		tOri=Node->tupInformation;		
		int OTindex=0;
		for (int i=0;i<Node->allN;i++)
		{
			OTindex=tOri[i].i;
			if (tOri[i].err==1)
			{
              tuplesOriginal[OTindex].err=1;
			}
			else
              tuplesOriginal[OTindex].err=0;
			
		}
		int cor=Node->publicClassN;
		int cov=Node->allN;
		double arc=(cor*1.0)/(cov*1.0);

		
		

		m_Dimension0++;
		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�

		//���Դ���
		if (m_NodeSize<=1)
		{

			//MessageBox(NULL,_T("The Nodesize=1!Empty!"),_T("��ʾ"),MB_OK);
		}

		progressI=progressI+Node->allN; //���ƽ�����
		/*m_progressBar->SetPos(progressI);                     
		Sleep(0.1); */
		return Node;

	}

	if (classN==1)  //���D�е�Ԫ�鶼��ͬһ��C
	{
		//cout<<"D�е�Ԫ�鶼��ͬһ��C"<<endl;
		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=L->next->classID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//����ԭʼ�б���Ϣ
		TuplesClassified *tOri;
		tOri=Node->tupInformation;		
		int OTindex=0;
		for (int i=0;i<Node->allN;i++)
		{
			OTindex=tOri[i].i;
			if (tOri[i].err==1)
			{
				tuplesOriginal[OTindex].err=1;
			}
			else
				tuplesOriginal[OTindex].err=0;

		}
	    
		m_Dimension0++;
		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�

		//���Դ���
		if (m_NodeSize<=1)
		{

			//MessageBox(NULL,_T("The Nodesize=1!Same!"),_T("��ʾ"),MB_OK);
		}
		//cout<<endl;
		//progressI=progressI+Node->allN; //���ƽ�����
		//m_progressBar->SetPos(progressI);                     
		//Sleep(0.1); 
		return Node;//����N��ΪҶ�ӽڵ�
	}
	

	k=log(m_Dimension2*1.0)/log(2.0);
	m_k=ceil(k)+1;//��k��ȡ��,�������
	
	if (m_k>=hight)//����������ߣ�
	{
		
		//cout<<"D�����������C"<<endl;
		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=publicClassID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//����ԭʼ�б���Ϣ
		TuplesClassified *tOri;
		tOri=Node->tupInformation;		
		int OTindex=0;
		for (int i=0;i<Node->allN;i++)
		{
			OTindex=tOri[i].i;
			if (tOri[i].err==1)
			{
				tuplesOriginal[OTindex].err=1;
			}
			else
				tuplesOriginal[OTindex].err=0;
		}
	
		m_Dimension0++;
		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�

		//���Դ���
		if (m_NodeSize<=1)
		{

			//MessageBox(NULL,_T("The Nodesize=1!Height!"),_T("��ʾ"),MB_OK);
		}
		//progressI=progressI+Node->allN; //���ƽ�����
		//m_progressBar->SetPos(progressI);                     
		//Sleep(0.1); 
		return Node;
	}
	
	
	int splitting_criterionIndex=0;
	double splitPointValue=0;     
	AttIndexTab att;
	att=ChooseAttribute1(S,S_row,S_line,attList,attN,2);
	double r=att.attGainRatio;
	splitting_criterionIndex=att.attIndex;
	splitPointValue=att.splittPoint;
	for (int i=0;i<S_row;i++)
	{
		delete []S[i];
	}
	delete []S;
	deleteClassUnique(L);

	Node->attIndex=splitting_criterionIndex;
	Node->ClassID=-1;//���ڵ㲻��Ҷ�ӽڵ�ʱ���������-1��ʾ
	Node->isLeaf=false;//����Ҷ�ӽڵ�
	Node->splitVertex=splitPointValue;//�洢�������µķ���ֵ
	
	//�����ݷָ��������
	int D1=0,D2=0;
	

	int **attlist1,**attlist2;
	double **S1,**S2;

	TuplesClassified *T1_index,*T2_index;
	TreeNode *LNode,*RNode;
	int s1_m=0;
	int s2_m=0;
	//�ָ�����
	T1_index=new TuplesClassified[T_row];//�����еĿռ�
	T2_index=new TuplesClassified[T_row];//�����еĿռ䣻
	//Ϊ�ָ������鸳ֵ
	for (int i=0;i<T_row;i++)
	{		
		int temprow=Tindex[i].sampleDataIndex;
		if (sampleData[temprow][splitting_criterionIndex]<=splitPointValue)
		{

			T1_index[s1_m].err=Tindex[i].err;
			T1_index[s1_m].i=Tindex[i].i;
			T1_index[s1_m].sampleDataIndex=Tindex[i].sampleDataIndex;
			T1_index[s1_m].tuplesClassID=Tindex[i].tuplesClassID;
			T1_index[s1_m].weight=Tindex[i].weight;
			s1_m++;
		}
		else
		{

			T2_index[s2_m].err=Tindex[i].err;
			T2_index[s2_m].i=Tindex[i].i;
			T2_index[s2_m].sampleDataIndex=Tindex[i].sampleDataIndex;
			T2_index[s2_m].tuplesClassID=Tindex[i].tuplesClassID;
			T2_index[s2_m].weight=Tindex[i].weight;
			s2_m++;
		}

		//cout<<"-";
	}
	D1=s1_m;D2=s2_m;
	
	
	if (D1!=0&&D2!=0)
	{
		
	
	}
	else if (D1==0&&D2>0)
	{
		
		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=publicClassID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//����ԭʼ�б���Ϣ
		TuplesClassified *tOri;
		tOri=Node->tupInformation;		
		int OTindex=0;
		for (int i=0;i<Node->allN;i++)
		{
			OTindex=tOri[i].i;
			if (tOri[i].err==1)
			{
				tuplesOriginal[OTindex].err=1;
			}
			else
				tuplesOriginal[OTindex].err=0;
		}

		m_Dimension0++;
		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�		
		return Node;

		//�����Դ���
		
		if (tagT)
		{
			//MessageBox(NULL,_T("The data1 is wrror!"),_T("��ʾ"),MB_OK);
			
			tagT=!tagT;
		}
		if (D1==0)
		{
			tagT=!tagT;
		}
		

	}
	else if(D2==0&&D1>0)
	{	
		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=publicClassID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//����ԭʼ�б���Ϣ
		TuplesClassified *tOri;
		tOri=Node->tupInformation;		
		int OTindex=0;
		for (int i=0;i<Node->allN;i++)
		{
			OTindex=tOri[i].i;
			if (tOri[i].err==1)
			{
				tuplesOriginal[OTindex].err=1;
			}
			else
				tuplesOriginal[OTindex].err=0;
		}

		m_Dimension0++;
		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�

		 
		return Node;

		if (tagT)
		{
			//MessageBox(NULL,_T("The data2 is wrror!"),_T("��ʾ"),MB_OK);
			
			tagT=!tagT;
		}
		if (D2==0)
		{
			tagT=!tagT;
		}

	}
	else
	{
	}
	//���ԭ�����ݼ����ڴ�
	//cout<<endl;
	
	if (D1==0)  //���DjΪ�գ���һ����Ҷ���ڵ�N�����ΪD�еĶ�����
	{
	
		//����Ҷ�ӽڵ�
		LNode=new TreeNode;//publicClassID
		m_NodeSize++;      //��¼�ڵ���
		LNode->ClassID=publicClassID;
		LNode->isLeaf=true;
		LNode->leftChild=NULL;
		LNode->rightChild=NULL;
		//
		Node->leftChild=NULL;
		m_Dimension0++;
	}
	else
	{
		LNode=Tree_Builde(sampleData,row,line,T1_index,D1,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON,progressI);
		Node->leftChild=LNode;


		
	}
	if (D2==0)
	{
		
		//����Ҷ�ӽڵ�
		RNode=new TreeNode;//publicClassID
		m_NodeSize++;      //��¼�ڵ���
		RNode->ClassID=publicClassID;
		RNode->isLeaf=true;
		RNode->leftChild=NULL;
		RNode->rightChild=NULL;

		//��Ҷ�ӽڵ����
		Node->rightChild=NULL;
		m_Dimension0++;
	}
	else
	{

		RNode=Tree_Builde(sampleData,row,line,T2_index,D2,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON,progressI);
		Node->rightChild=RNode;
		
	}
	
	delete []D;
	
	m_treeHeight=m_k;
	return Node;
	
}
ClassUnique * GLC_Tree::getClassUnique(AttValuesTab * attValues, int attNum) 
{
	if (attNum==0)
	{

	}

	int n=0;
	n=attNum;
	ClassUnique *first,*r,*s,*p;

	first=new ClassUnique;
	first->next=NULL;
	r=first;
	p=first;

	int m=0;

	for (int i=0;i<attNum;i++)
	{
		if (i==0)//���б��ʼ��
		{   
			m++;//ÿ�½�һ���ڵ�m+1
			s=new ClassUnique;
			s->classID=attValues[0].classID;	
			s->cunt=1;
			s->next=NULL;
			r->next=s;
			r=s;
		}
		else
		{
			p=first->next;//��Pָ��ͷָ��
			while(p)
			{
				if (p->classID==attValues[i].classID)
				{
					p->cunt=p->cunt+1;
					p=first->next;
					break;
				}
				if (p==r)
				{	
					if (p->classID==attValues[i].classID)
					{
						p->cunt=p->cunt+1;
						p=first->next;
						break;
					}
					m++;
					s=new ClassUnique;
					s->classID=attValues[i].classID;	
					s->cunt=1;
					s->next=NULL;
					r->next=s;
					r=s;
					p=first->next;
					break;
				}

				p=p->next;
			}
		}
	}
	first->cunt=m;   

	first->classID=n;
	return first;
}
void GLC_Tree::SetValuesForTreeNode(TreeNode* treeNode,int tuplesN,TuplesClassified *tuples,double ** sampleData,int sampleData_line,int publicClassID)
{
	int line=sampleData_line;
	TuplesClassified * Tindex=tuples;
	int T_row=tuplesN;
	TreeNode *Node=treeNode;
	//m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�

	//�ж�D�е�Ԫ���Ƿ�Ϊͬһ��,�������ò���е㷱����
	//AttValuesTab *D;
	////D=new AttValuesTab[SIndex_row];
	//D=new AttValuesTab[T_row];
	//int temprow=0;
	////for (int i=0;i<SIndex_row;i++)
	//for (int i=0;i<T_row;i++)
	//{
	//	//temprow=SIndex[i][0];
	//	temprow=Tindex[i].sampleDataIndex;
	//	D[i].classID=sampleData[temprow][line-1];
	//	D[i].AttValue=sampleData[temprow][0];    //���Բ���
	//	D[i].AttValue=0;                         //���Բ���
	//}
	//ClassUnique * L,*p;

	//L=getClassUnique(D,T_row);//���Ψһֵ����,ע��ɾ��
	//p=L;
	//int classN=0;
	//classN=L->cunt;//������ݼ����ĸ���

	////Ѱ�Ҷ�����
	//int i=0,Max=0,publicClassID=0,Num=0;
	//while (p)
	//{
	//	if (i=0)
	//	{
	//		p=p->next;
	//		i++;
	//	}
	//	else
	//	{
	//		Num=p->cunt;
	//		if (Max<Num)
	//		{
	//			Max=Num;
	//			publicClassID=p->classID;
	//		}
	//		p=p->next;
	//	}
	//}
	////��¼������ĸ������Լ��÷�֧ѵ�����ݵĸ���
	//int publicClassN=0,allN=0;
	//publicClassN=Max;
	//allN=L->classID;

	//Node->allN=T_row;  //��ýڵ�Ԫ�����
	//Node->publicClassN=publicClassN;
	//Node->publicClassID=publicClassID;
	////Node->errors=Node->allN-Node->publicClassN;   //�ⲿ��ֵ�������ã�����Ĵ�����������ѭ���õ��Ĵ�������һ������
	int errorcount=0;
	TuplesClassified *t; //���Ԫ����Ϣ
	t=new TuplesClassified[T_row];
	for (int i=0;i<T_row;i++)     //�Ƚ�ռ�ڴ���Կ����Ż�
	{	
		t[i].tuplesClassID=publicClassID;
		t[i].sampleDataIndex=Tindex[i].sampleDataIndex;
		t[i].err=Tindex[i].err;
		//t[i].tuplesClassID=Tindex[i].tuplesClassID;
		t[i].weight=Tindex[i].weight;
		t[i].i=Tindex[i].i;

		int ti=0;
		ti=t[i].sampleDataIndex;
		int classid=0;
		classid=sampleData[ti][line-1];
		if (classid!=publicClassID)  //��������ڶ�������Ҷ�ӽڵ��ϼ������
		{

			t[i].err=1;              //�����err��ֵ1
			errorcount++;

		}
		else
		{
			t[i].err=0;            //������ȷȷ��ֵΪ0
		}


	}
	Node->tupInformation=t;
	Node->errors=errorcount;    
}
AttIndexTab GLC_Tree::ChooseAttribute1(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL)
{
	double ** S;
	S=sampleData;
	AttIndexTab * attIndexTab;//���ڴ洢�ɹ��������Եļ�����Ϣ��
	attIndexTab=new AttIndexTab[attNum];//��̬����ע��ɾ��
	double infGainAll=0;//�ۼ���Ϣ���棬���ڼ���ƽ��ֵ
	double averageInfGain=0; //��ȡ����ƽ��ֵ
	double minIGRatio=0;   //����С������
	int minI=0;

	for (int i=0;i<attNum;i++)//attNum�ɹ��������Եĸ���
	{
		int tempIndex=0;          
		tempIndex=attIndex[i][0];

		AttValuesTab attValues[10000];
		double infGain=0,infGainRatio=0;
		double splitPoint=0;
		int lSplit=0,rSplit=0;

		for (int j=0;j<row;j++)  
		{
			attValues[j].AttValue=S[j][tempIndex];
			attValues[j].classID=S[j][line-1];
			attValues[j].AttValueIndex=tempIndex;
		}

		//������Ϣ����

		infGain=Gain(attValues,row);
		infGainRatio=GainRatio(attValues,row);
		attIndex[i][1]=m_splitPointValue;

		attIndexTab[i].attIndex=tempIndex;//��¼����
		attIndexTab[i].attGain=infGain;
		attIndexTab[i].attGainRatio=infGainRatio;
		attIndexTab[i].splittPoint=m_splitPointValue;

		infGainAll=infGainAll+infGain;


	}
	averageInfGain=infGainAll/attNum;//��ȡ����ƽ��ֵ

	for (int i=0;i<attNum;i++)
	{
		if (i==0)
		{
			minIGRatio=attIndexTab[i].attGainRatio;
			minI=attIndexTab[i].attIndex;
		}
		else
		{
			if (attIndexTab[i].attGain>=averageInfGain)
			{
				if (minIGRatio<attIndexTab[i].attGainRatio)
				{
					minIGRatio=attIndexTab[i].attGainRatio;
					minI=attIndexTab[i].attIndex;

				}

			}
		}
	}
	//��С��������û����ȵ�
	int *sameMinGRatioIndex=new int[attNum];
	int sameMinGRatioI=0;
	for (int i=0;i<attNum;i++)
	{
		if (minIGRatio==attIndexTab[i].attGainRatio)
		{
			sameMinGRatioIndex[sameMinGRatioI]=i;
			sameMinGRatioI++;
		}
	}
	if (sameMinGRatioI>1)
	{
		int I=rand()%sameMinGRatioI;
		//cout<<"���"<<I<<endl;
		minI=sameMinGRatioIndex[I];
	}

	AttIndexTab r;
	r.attIndex=minI;	
	r.splittPoint=attIndexTab[minI].splittPoint;

	delete [] sameMinGRatioIndex;
	delete [] attIndexTab;
	return r;
}
double GLC_Tree::Gain(AttValuesTab * attValues, int attNum)
{
	AttValuesTab * values;
	values=attValues;
	int n;
	n=attNum;
	ClassUnique * classLink;
	statisticsTab * staTab;
	double inf,attInf,ginInf,ginInfRatio;
	int staNum=0;
	double * discData;

	//ͳ�ƻ���������Ψһֵ������
	classLink=getClassUnique(values,n);
	staNum=classLink->cunt;
	//���ͳ�Ʊ�
	staTab=AttValuesStatistics(classLink);
	deleteClassUnique(classLink);
	//����������Ե���
	inf=Info(staTab,staNum);
	delete [] staTab;
	//��ɢ��������
	discData=discreteData(values,n);
	attInf=findDiscPoint(discData,n-1,values,n,true);
	delete[] discData;

	ginInf=inf-attInf;   

	return ginInf;
}
double GLC_Tree::GainRatio(AttValuesTab * attValues, int attNum)
{
	AttValuesTab * values;
	values=attValues;
	int n;
	n=attNum;
	ClassUnique * classLink;
	statisticsTab * staTab;
	double inf,attInf,ginInf,ginInfRatio;
	int staNum=0;
	double * discData;

	//ͳ�ƻ���������Ψһֵ������
	classLink=getClassUnique(values,n);
	staNum=classLink->cunt;
	//���ͳ�Ʊ�
	staTab=AttValuesStatistics(classLink);
	//����������Ե���
	inf=Info(staTab,staNum);
	//��ɢ��������
	discData=discreteData(values,n);
	attInf=findDiscPoint(discData,n-1,values,n,true);
	//dis,f.sampleRows-1,c.m_AttValuesTable,f.sampleRows,true
	ginInf=inf-attInf;   //�����Ϣ����
	ginInfRatio=ginInf/inf;//�����Ϣ������

	delete[] classLink;
	delete[] staTab;
	return ginInfRatio;

}
statisticsTab* GLC_Tree::AttValuesStatistics(ClassUnique* uniqueValuesLink)
{
	ClassUnique *p,*head;
	int n;
	int tempCount=0,tempAll=0;
	head=uniqueValuesLink;
	p=head;

	n=head->cunt;
	tempAll=head->classID;
	int Li=0,Lj=0;
	statisticsTab *stTabD;
	stTabD=new statisticsTab[n];
	//ͳ�Ʊ�ֵ


	while(p)
	{
		if (Lj>0)
		{
			stTabD[Li].valueID=p->classID;
			stTabD[Li].valueCount=p->cunt;
			tempCount=p->cunt;
			//tempAll=head->classID;
			stTabD[Li].Proportion=tempCount*1.0/tempAll*1.0;
			Li++;
		}
		Lj++;
		p=p->next;
	}


	return stTabD;
}
double*GLC_Tree::discreteData(AttValuesTab* AttValues, int AttValuesCount)//
{

	int m_discretePointsCount=AttValuesCount-1;

	double min=0;
	AttValuesTab temp;
	for (int i=0;i<AttValuesCount-1;i++)
	{
		for (int j=i+1;j<AttValuesCount;j++)
		{
			if (AttValues[j].AttValue<AttValues[i].AttValue)
			{
				temp=AttValues[j];AttValues[j]=AttValues[i];
				AttValues[i]=temp;
			}
		}

	}

	double * m_discretePoints;
	m_discretePoints=new double[m_discretePointsCount];
	double a=0,b=0;
	int disPC=0;
	for (int i=0;i<AttValuesCount-1;i++)
	{
		m_discretePoints[i]=(AttValues[i].AttValue+AttValues[i+1].AttValue)/2.0;
	}

	return m_discretePoints;
}
double GLC_Tree::findDiscPoint(double * discPoitList, int discNum, AttValuesTab * attValueslist,int attValuesNum,bool Tag)
{
	int allValues=0;
	allValues=attValuesNum;

	statisticsTab  stTab[2];
	double minInfo=0;
	int minIdex=0;
	double I=0;
	for (int i=0;i<discNum;i++)
	{		
		double discPoint;
		int D1=0,D2=0;
		discPoint=discPoitList[i];
		AttValuesTab attValTabD1[10000],attValTabD2[10000];
		for (int j=0;j<attValuesNum;j++)
		{
			if (attValueslist[j].AttValue<=discPoint)
			{
				attValTabD1[D1].AttValue=attValueslist[j].AttValue;
				attValTabD1[D1].AttValueIndex=attValueslist[j].AttValueIndex;
				attValTabD1[D1].classID=attValueslist[j].classID;
				D1++;
			}
			else
			{
				attValTabD2[D2].AttValue=attValueslist[j].AttValue;
				attValTabD2[D2].AttValueIndex=attValueslist[j].AttValueIndex;
				attValTabD2[D2].classID=attValueslist[j].classID;
				D2++;
			}
		}

		double E1,E2;    
		ClassUnique *L1,*r1,*L2,*r2,*REC1,*REC2;
		int m1=0,m2=0;            

		L1=getClassUnique(attValTabD1,D1);
		L2=getClassUnique(attValTabD2,D2);

		REC1=L1;
		REC2=L2;

		r1=L1;
		r2=L2;

		m1=L1->cunt;
		m2=L2->cunt;

		statisticsTab *stTabD1,*stTabD2;
		stTabD1=new statisticsTab[m1];
		stTabD2=new statisticsTab[m2];


		int tempCount=0,tempAll=0;
		int Li=0,Lj=0;

		while(r1)
		{
			if (Lj>0)
			{
				stTabD1[Li].valueID=r1->classID;
				stTabD1[Li].valueCount=r1->cunt;
				tempCount=r1->cunt;
				tempAll=D1;
				stTabD1[Li].Proportion=tempCount*1.0/tempAll*1.0;
				Li++;
			}
			Lj++;
			r1=r1->next;


		}
		Lj=0;Li=0;
		while (r2)
		{
			if (Lj>0)
			{
				stTabD2[Li].valueID=r2->classID;
				stTabD2[Li].valueCount=r2->cunt;
				tempCount=r2->cunt;
				tempAll=D2;
				stTabD2[Li].Proportion=tempCount*1.0/tempAll*1.0;
				Li++;
			}
			Lj++;
			r2=r2->next;
		}
		deleteClassUnique(REC1);
		deleteClassUnique(REC2);

		E1=Info(stTabD1,m1);
		E2=Info(stTabD2,m2);
		delete [] stTabD1;
		delete [] stTabD2;


		double P1,P2;

		P1=D1*1.0/allValues*1.0;
		P2=D2*1.0/allValues*1.0;

		InfoAttTab infAtt[2];
		infAtt[0].Info=E1;
		infAtt[0].Weight=P1;


		infAtt[1].Info=E2;
		infAtt[1].Weight=P2;

		I=InfoAtt(infAtt,2);

		if (i==0)
		{
			minInfo=I;
			minIdex=0;
		}
		else
		{
			if (minInfo>I)
			{
				minInfo=I;
				minIdex=i;
			}
		}



	}
	double bestPoint=discPoitList[minIdex];
	m_splitPointValue=bestPoint;

	if (Tag)
	{
		return minInfo;
	}
	else
		return bestPoint;

}
void GLC_Tree::deleteClassUnique(ClassUnique *classLick) 
{
	ClassUnique *L,*p,*q;
	L=classLick;
	p=L;
	q=L;
	while (p)
	{
		q=p;
		p=p->next;
//		q->dataIndex.clear();
		delete q;
	}
}
double GLC_Tree::InfoAtt(InfoAttTab * infoAttValues, int infoNum)
{
	double weight=0,inf=0,infAtt=0;
	for (int i=0;i<infoNum;i++)
	{
		weight=infoAttValues[i].Weight;
		inf=infoAttValues[i].Info;
		infAtt=infAtt+weight*inf;
	}

	return infAtt;
}
double GLC_Tree::Info(statisticsTab* staTab, int staCount)
{
	double core=0;
	double temp=staTab[0].Proportion;
	for (int i=0;i<staCount;i++)
	{
		if (staTab[i].Proportion==1||staTab[i].Proportion==0)
		{
			core=core+0;
		}
		else
		{
			temp=staTab[i].Proportion;
			core=core+temp*(log(temp)/log(2.0));
		}

	}
	double reault=0;
	reault=(-1)*core;

	return reault;
}
//*************************************************************************************************************************************
//***********************************************��������*****************************************************************//
//bool GLC_Tree::Tree_Classfy(TreeNode* root, double erroratio,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI,int tree_NO,int& isRoot)
//{
//	try
//	{
//		if (tuplesN>5000)
//		{
//			throw "GLC_Tree_Error5:����������Ԥ����Χ��5000������ֿ鴦��";
//			
//		}
//	}
//	catch (...)
//	{
//		return false;
//		
//	}	
//	if (root==NULL)
//	{
//		return false;
//	}
//	else
//	{
//		if (root->isLeaf)
//		{
//			double err=0;
//			double acc=0,pre=0;
//			err=root->errors;
//
//			double cover=root->allN;
//			double correct=cover-err;
//			acc=correct/cover;
//			pre=acc*log(1/erroratio);
//			root->pre=pre;
//
//			int classId;
//			classId=root->ClassID;
//			for (int i=0;i<tuplesN;i++)
//			{
//
//				//��ֵ
//				tuplesResult[tuplesResultI].err=tuples[i].err;
//				tuplesResult[tuplesResultI].i=tuples[i].i;
//				tuplesResult[tuplesResultI].pre=pre;
//				tuplesResult[tuplesResultI].sampleDataIndex=tuples[i].sampleDataIndex;
//				tuplesResult[tuplesResultI].tuplesClassID=classId;
//				tuplesResult[tuplesResultI].weight=tuples[i].weight;
//				tuplesResult[tuplesResultI].clssified_data=tuples[i].clssified_data;
//
//				
//				tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].pre=tuplesResult[tuplesResultI].pre;
//				tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].tuplesClassID=tuplesResult[tuplesResultI].tuplesClassID;
//				tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].isvotied=true;
//				if (tree_NO==0)
//				{
//					int& staI=tuplesResult[tuplesResultI].clssified_data->StatisticsI;
//					tuplesResult[tuplesResultI].clssified_data->Statistics[staI].pre=tuplesResult[tuplesResultI].pre;
//					tuplesResult[tuplesResultI].clssified_data->Statistics[staI].tuplesClassID=tuplesResult[tuplesResultI].tuplesClassID;
//					tuplesResult[tuplesResultI].clssified_data->Statistics[staI].isvotied=true;
//					staI++;
//				}
//				else
//				{
//					int staNum=tuplesResult[tuplesResultI].clssified_data->StatisticsI;
//					for (int k=0;k<staNum;k++)
//					{
//						if (tuplesResult[tuplesResultI].clssified_data->Statistics[k].tuplesClassID==tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].tuplesClassID)
//						{
//							tuplesResult[tuplesResultI].clssified_data->Statistics[k].pre=tuplesResult[tuplesResultI].clssified_data->Statistics[k].pre+tuplesResult[tuplesResultI].clssified_data->resultEveryT[tree_NO].pre;
//							//afterClassTuples[m].clssified_data->Statistics[k].pre=afterClassTuples[m].clssified_data->Statistics[k].tuplesClassID=afterClassTuples[m].clssified_data->resultEveryT[i].tuplesClassID;
//						}
//						else
//						{
//							int& staI=tuplesResult[tuplesResultI].clssified_data->StatisticsI;
//							tuplesResult[tuplesResultI].clssified_data->Statistics[staI].pre=tuplesResult[tuplesResultI].pre;
//							tuplesResult[tuplesResultI].clssified_data->Statistics[staI].tuplesClassID=tuplesResult[tuplesResultI].tuplesClassID;
//							tuplesResult[tuplesResultI].clssified_data->Statistics[staI].isvotied=true;
//							staI++;
//
//						}
//					}
//				}			
//
//				tuplesResultI++;
//
//			}
//			if (isRoot>0)
//			{
//				delete [] tuples;
//			}
//
//		}
//		else
//		{
//			;
//			int tuplesLN=0,tuplesRN=0;
//			int tuplesLi=0,tuplesRi=0;
//
//			double splitpoint=root->splitVertex;//��÷��ѵ�
//			int splitattIndex=root->attIndex;        //��÷�������
//
//			//�������������ݸ���
//
//			TuplesClassified *tuplesL=new TuplesClassified[tuplesN];
//			TuplesClassified *tuplesR=new TuplesClassified[tuplesN];
//
//			for (int i=0;i<tuplesN;i++)
//			{
//				int dataIndex=tuples[i].sampleDataIndex;
//				if (m_data[dataIndex][splitattIndex]<=splitpoint)
//				{
//					tuplesL[tuplesLi].err=tuples[i].err;
//					tuplesL[tuplesLi].i=tuples[i].i;
//					tuplesL[tuplesLi].pre=tuples[i].pre;
//					tuplesL[tuplesLi].sampleDataIndex=tuples[i].sampleDataIndex;
//					tuplesL[tuplesLi].tuplesClassID=tuples[i].tuplesClassID;
//					tuplesL[tuplesLi].weight=tuples[i].weight;
//					tuplesL[tuplesLi].clssified_data=tuples[i].clssified_data;
//					tuplesLi++;
//				}
//				else
//				{
//					tuplesR[tuplesRi].err=tuples[i].err;
//					tuplesR[tuplesRi].i=tuples[i].i;
//					tuplesR[tuplesRi].pre=tuples[i].pre;
//					tuplesR[tuplesRi].sampleDataIndex=tuples[i].sampleDataIndex;
//					tuplesR[tuplesRi].tuplesClassID=tuples[i].tuplesClassID;
//					tuplesR[tuplesRi].weight=tuples[i].weight;
//					tuplesR[tuplesRi].clssified_data=tuples[i].clssified_data;
//					tuplesRi++;
//				}
//			}
//			//���ԭ��������
//			if (isRoot>0)
//			{
//				delete [] tuples;
//			}
//			isRoot++;
//			if (tuplesLi!=0&&tuplesRi!=0)
//			{
//
//				Tree_Classfy(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI,tree_NO,isRoot);//���������������
//				Tree_Classfy(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI,tree_NO,isRoot);//��������������
//
//			}
//			if (tuplesLi==0&&tuplesRi!=0)
//			{
//				//��������
//
//
//				Tree_Classfy(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI,tree_NO,isRoot);//��������������
//
//			}
//			if (tuplesLi!=0&&tuplesRi==0)
//			{
//				//��������
//
//				Tree_Classfy(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI,tree_NO,isRoot);//���������������
//			}
//
//		}
//
//	}
//	return true;
//}

bool GLC_Tree::Tree_Classfy(TreeNode* root,TuplesClassified *tuples,int tuplesN,TuplesClassified *tuplesResult,int & tuplesResultI,int& isRoot,double **data)
{
	try
	{
		if (tuplesN>5000)
		{
		throw "GLC_Tree_Error5:����������Ԥ����Χ��5000������ֿ鴦��";
		}
		if (root==NULL)
		{
			return false;
		}
		else
		{
			if (root->isLeaf)
			{

				int classId=root->ClassID;
				for (int i=0;i<tuplesN;i++)
				{

					//��ֵ
					tuplesResult[tuplesResultI].i=tuples[i].i;
					tuplesResult[tuplesResultI].sampleDataIndex=tuples[i].sampleDataIndex;
					tuplesResult[tuplesResultI].tuplesClassID=classId;
					tuplesResult[tuplesResultI].clssified_data=tuples[i].clssified_data;	

					int tempi=tuplesResult[tuplesResultI].sampleDataIndex;
					m_classifiedID[tempi]=root->ClassID;
					tuplesResultI++;
					//tuplesResultI++;

				}
				if (isRoot>0)
				{
					delete [] tuples;
				}

			}
			else
			{

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
					if (data[dataIndex][splitattIndex]<=splitpoint)
					{

						tuplesL[tuplesLi].i=tuples[i].i;
						tuplesL[tuplesLi].sampleDataIndex=tuples[i].sampleDataIndex;
						tuplesL[tuplesLi].tuplesClassID=tuples[i].tuplesClassID;
						tuplesL[tuplesLi].clssified_data=tuples[i].clssified_data;
						tuplesLi++;
					}
					else
					{
						tuplesR[tuplesRi].i=tuples[i].i;
						tuplesR[tuplesRi].sampleDataIndex=tuples[i].sampleDataIndex;
						tuplesR[tuplesRi].tuplesClassID=tuples[i].tuplesClassID;
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

					Tree_Classfy(root->leftChild,tuplesL,tuplesLi,tuplesResult,tuplesResultI,isRoot,data);//���������������
					Tree_Classfy(root->rightChild,tuplesR,tuplesRi,tuplesResult,tuplesResultI,isRoot,data);//��������������

				}
				if (tuplesLi==0&&tuplesRi!=0)
				{

					Tree_Classfy(root->rightChild,tuplesR,tuplesRi,tuplesResult,tuplesResultI,isRoot,data);//��������������

				}
				if (tuplesLi!=0&&tuplesRi==0)
				{
					Tree_Classfy(root->leftChild,tuplesL,tuplesLi,tuplesResult,tuplesResultI,isRoot,data);//���������������
				}

			}

		}

	}
	catch (...)
	{
		return false;

	}	

	return true;
}
bool GLC_Tree::Tree_Classfy_1(TreeNode* root,double * classifydata,int &classID)
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
		//cout<<"*";
	}
	catch (...)
	{
		return false;
		
	}
	//cout<<endl;
	
	
	return true;
}
bool GLC_Tree::DestroyModel()   //ɾ��ģ��
{
	if (m_tree!=NULL)
	{
		DeleteTree(m_tree);
		return true;
	}
	return false;
}
void GLC_Tree::DeleteTree(TreeNode* root)
{
	if (root!=NULL)
	{
		TreeNode *a ,*b;
		a=root->leftChild;
		b=root->rightChild;
		/*TuplesClassified * temp=NULL;
		temp=root->tupInformation;
		delete [] temp;*/
		root->tupInformation=NULL;
		delete root;
		DeleteTree(a);
		DeleteTree(b);
			
	}
	
}