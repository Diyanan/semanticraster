#include "StdAfx.h"
#include "Tree.h"
#include <cmath>
#include <set> 

#include <iostream>
using namespace std;

Tree::Tree(void)
	: m_treeRoot(NULL)
	, tagT(false)
	//,m_progressBar(NULL)
{
	m_Dimension0=0;
	m_Dimension1=0;
	m_Dimension2=0;
	m_splitPointValue=0;
	m_treeHeight=0;
	m_Dimension=0;
	m_NodeSize=0;
	m_k=0;
	m_splitPointAttIndex=0;
	m_erroNum=0;
	m_bestSplitPointValue=0;
	m_tuplesNum=0;
	m_testDatasRow=0;
	m_testDatasLine=0;
	m_sampleDatasRow=0;
	m_sampleDatasLine=0;
	m_tuplesRow=0;
    m_tuplesLine=0;
	m_sampleDataIndexRow=0;
	m_sampleDataIndexLine=0;
	m_tuplesI=0;
	m_adaBoostingK=0;
	m_leafPreErrorsADD=0;
	m_childTreeHeight=0;
	m_childTreeLeafs=0;
	m_childTreeNodes=0;
	m_stackForNodeRulesTop=0;
	m_allDatas=0;
	m_publics=0;
	m_allErros=0;
	m_leafsSize=0;
	m_ruleSetI=0;
	m_tringDataN=0;








}


Tree::~Tree(void)
{
}
double Tree::Info(statisticsTab* staTab, int staCount)
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
double*Tree::discreteData(AttValuesTab* AttValues, int AttValuesCount)//
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
double Tree::findDiscPoint(double * discPoitList, int discNum, AttValuesTab * attValueslist,int attValuesNum,bool Tag)
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
double Tree::findDiscPoint1(double * discPoitList, int discNum, AttValuesTab * attValueslist,int attValuesNum,int **attVlueList,int attN,int attL,bool Tag)
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
		AttValuesTab attValTabD1[1000],attValTabD2[1000];
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
		ClassUnique *L1,*r1,*L2,*r2;
		int m1=0,m2=0;             

		L1=getClassUnique(attValTabD1,D1);
		L2=getClassUnique(attValTabD2,D2);

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
	
		E1=Info(stTabD1,m1);
		E2=Info(stTabD2,m2);

		//******************************************************
		//***********��Ϣ����****************************
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
	m_splitPointAttIndex=minIdex;
	if (Tag)
	{
       return minInfo;
	}
	else
	return bestPoint;
	
}
ClassUnique * Tree::getClassUnique(AttValuesTab * attValues, int attNum) 
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


void Tree::deleteClassUnique(ClassUnique *classLick) 
{
	ClassUnique *L,*p,*q;
	L=classLick;
	p=L;
	q=L;
	while (p)
	{
		q=p;
		p=p->next;
		delete q;
	}
}

double Tree::InfoAtt(InfoAttTab * infoAttValues, int infoNum)
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

double Tree::Gain(AttValuesTab * attValues, int attNum)
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
double Tree::Gain1(AttValuesTab * attValues, int attNum,int **attVlueList,int attN,int attL)
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

	
	classLink=getClassUnique(values,n);
	staNum=classLink->cunt;
	
	staTab=AttValuesStatistics(classLink);
	
	inf=Info(staTab,staNum);
	
	discData=discreteData(values,n);
	attInf=findDiscPoint1(discData,n-1,values,n,attVlueList,attN,attL,true);
	
	ginInf=inf-attInf;  
	return ginInf;
}


statisticsTab* Tree::AttValuesStatistics(ClassUnique* uniqueValuesLink)
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


double Tree::GainRatio(AttValuesTab * attValues, int attNum)
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



int Tree::ChooseAttribute(double ** sampleData, int row, int line, int * attIndex, int attNum)
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
		int tempIndex=0;          //��ȡ�����е������ţ�Ϊ����S��ȡֵ
		tempIndex=attIndex[i];
		
		AttValuesTab attValues[1000];
		double infGain=0,infGainRatio=0;
		double splitPoint=0;
		int lSplit=0,rSplit=0;

		for (int j=0;j<row;j++)  //��õ�ǰ�����µ�����ֵ�����ClassID
		{
			attValues[j].AttValue=S[j][tempIndex];
			attValues[j].classID=S[j][line-1];
			attValues[j].AttValueIndex=tempIndex;
		}

		

		infGain=Gain(attValues,row);
		infGainRatio=GainRatio(attValues,row);


		attIndexTab[i].attIndex=tempIndex;
		attIndexTab[i].attGain=infGain;
		attIndexTab[i].attGainRatio=infGainRatio;
		attIndexTab[i].splittPoint=m_splitPointValue;

		infGainAll=infGainAll+infGain;//�ۼ���Ϣ���棬���ڼ���ƽ��ֵ

		
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
			//if (attIndexTab[i].attGain>=averageInfGain)
			{
				if (minIGRatio<attIndexTab[i].attGainRatio)
				{
					minIGRatio=attIndexTab[i].attGainRatio;
					minI=attIndexTab[i].attIndex;
					m_splitPointValue=attIndexTab[i].splittPoint;
				}

			}
		}
	}
	int r=0;
	r=minI;
	delete [] attIndexTab;
	return r;
}

AttIndexTab Tree::ChooseAttribute1(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL)
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
		minI=sameMinGRatioIndex[I];
	}

	AttIndexTab r;
	r.attIndex=minI;
	r.splittPoint=attIndexTab[minI].splittPoint;

	delete [] sameMinGRatioIndex;
	delete [] attIndexTab;
	return r;
}
vector<AttIndexTab> Tree::ChooseAttribute2(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL)
{
	double ** S;
	S=sampleData;
	AttIndexTab * attIndexTab;//���ڴ洢�ɹ��������Եļ�����Ϣ��
	attIndexTab=new AttIndexTab[attNum];//��̬����ע��ɾ��
	double infGainAll=0;//�ۼ���Ϣ���棬���ڼ���ƽ��ֵ
	double averageInfGain=0; //��ȡ����ƽ��ֵ
	double minIGRatio=0;   //����С������
	int minI=0;

	vector<AttIndexTab> results;

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

		results.push_back(attIndexTab[i]);


	}
	averageInfGain=infGainAll/attNum;//��ȡ����ƽ��ֵ

	//������lv���дӴ�С������
	AttIndexTab temp;
	for (int i=0;i<results.size()-1;i++)
	{
		for (int j=i+1;j<results.size();j++)
		{
			if (results.at(j).attGainRatio>results.at(i).attGainRatio)
			{
				temp=results.at(j);
				results.at(j)=results.at(i);
				results.at(i)=temp;
			}
		}		
	}

	//for (int i=0;i<AttValuesCount-1;i++)
	//{
	//	for (int j=i+1;j<AttValuesCount;j++)
	//	{
	//		if (AttValues[j].AttValue<AttValues[i].AttValue)
	//		{
	//			temp=AttValues[j];AttValues[j]=AttValues[i];
	//			AttValues[i]=temp;
	//		}
	//	}

	//}

	////������lv���дӴ�С������
	//AttIndexTab temp;
	//for (int i=0;i<attNum-1;i++)
	//{
	//	for (int j=i+1;j<attNum;j++)
	//	{
	//		if (attIndexTab[j].attGainRatio<attIndexTab[i].attGainRatio)
	//		{
	//			temp=attIndexTab[j];
	//			attIndexTab[j]=attIndexTab[i];
	//			attIndexTab[i]=temp;
	//		}
	//	}		
	//}
	//

	//for (int i=0;i<attNum;i++)
	//{
	//	if (i==0)
	//	{
	//		minIGRatio=attIndexTab[i].attGainRatio;
	//		minI=attIndexTab[i].attIndex;
	//	}
	//	else
	//	{
	//		if (attIndexTab[i].attGain>=averageInfGain)
	//		{
	//			if (minIGRatio<attIndexTab[i].attGainRatio)
	//			{
	//				minIGRatio=attIndexTab[i].attGainRatio;
	//				minI=attIndexTab[i].attIndex;

	//			}

	//		}
	//	}
	//}
	////��С��������û����ȵ�
	//int *sameMinGRatioIndex=new int[attNum];
	//int sameMinGRatioI=0;
	//for (int i=0;i<attNum;i++)
	//{
	//	if (minIGRatio==attIndexTab[i].attGainRatio)
	//	{
	//		sameMinGRatioIndex[sameMinGRatioI]=i;
	//		sameMinGRatioI++;
	//	}
	//}
	//if (sameMinGRatioI>1)
	//{
	//	int I=rand()%sameMinGRatioI;
	//	minI=sameMinGRatioIndex[I];
	//}

	//AttIndexTab r;
	//r.attIndex=minI;
	//r.splittPoint=attIndexTab[minI].splittPoint;

	//delete [] sameMinGRatioIndex;
	delete [] attIndexTab;
	return results;
}




void Tree::InOrder(TreeNode * root)
{
	if (root==NULL)
	{
		return;
	}
	else
	{
        InOrder(root->leftChild);
		//���ʸ����ڵ������
		//.......................
		if (root->isLeaf)
		{
			m_childTreeLeafs++;
			m_allDatas=m_allDatas+root->allN;
		}
		m_childTreeNodes++;
		InOrder(root->rightChild);
	}
}



void Tree::RemoveTree(TreeNode* root)
{
	if (root!=NULL)
	{
		RemoveTree(root->leftChild);
		RemoveTree(root->rightChild);
		delete root;
	}
}



bool Tree::PostPrune(TreeNode* iNode)
{
	if (iNode->isLeaf)
	{
		return false;
	}
	TreeNode *L,*R;
	bool leafTopL=true,leafTopR=true;
	//���������������Ҷ�ӽڵ㣬��ݹ�
	L=iNode->leftChild;
	R=iNode->rightChild;
	//
	if (!L->isLeaf)//�������������Ҷ�ӽڵ㣬��������
	{
		if (PostPrune(L))   //������Լ�֦��������ֵ
		{
			return true;			
		}
		leafTopL=false;
	}
	if (!R->isLeaf)//���������������Ҷ�ӽڵ㣬��������
	{
		if (PostPrune(R))   //������Լ�֦��������ֵ
		{
			return true;			
		}
		leafTopR=false;
	}
	if (leafTopL&&leafTopR)
	{
		//�����֦ǰ�Ĵ����ʣ�������=��ǰ�ڵ�ռ�����Ľ��/�˽ڵ�������֮��
		double littleN=iNode->allN-iNode->publicClassN;
		double N=iNode->allN;
		double erroBefore=littleN/N;
		double erroAfter;
		double errorNum;
		//��ô˽ڵ����з��ѷ�֧�Ĵ������ܺ�
		if (iNode->leftChild!=NULL&&iNode->rightChild!=NULL)
		{
			errorNum=(iNode->leftChild->allN-iNode->publicClassN)+(iNode->rightChild->allN-iNode->publicClassN);
		}
		else if (iNode->leftChild!=NULL&&iNode->rightChild==NULL)
		{
			errorNum=(iNode->leftChild->allN-iNode->publicClassN);
		}
		else
		{
			errorNum=(iNode->rightChild->allN-iNode->publicClassN);
		}
	
		//�����֦��Ĵ�����,������=��ֵ����*0.5+�����ܺͣ�/�˽ڵ�������
		erroAfter=(errorNum+0.5*2)/N;
		if (erroAfter>=erroBefore)
		{
			TreeNode * newNode;
			newNode=new TreeNode;
			newNode->isLeaf=true;
			//ԭ�ڵ���ձ��������Ľ������
			newNode->ClassID=iNode->publicClassID;
			newNode->isLeaf=true;
			newNode->leftChild=NULL;
			newNode->rightChild=NULL;
		//	RemoveTree(iNode);
			delete iNode;
			iNode=newNode;
			return true;
		}
		return false;
	}
	return false;
}



//void Tree::LeverOrder(TreeNode* root)
//{
//	int front=0;
//	int rear=0;
//	if (root==NULL)
//	{
//		return;
//	}
//	TreeNode* Q[1000];
//	TreeNode* q;
//	Q[++rear]=root;
//	CString classID,index,point,row;
//	CString att1,att2,att3;
//	double DPoint=0;
//	int ID,I;
//	//CStdioFile f;
//	//CFile f;
//	//f.Open(_T("C:\\Data\\zyy.txt"),CFile::modeCreate|CFile::modeWrite/*|CFile::typeText*/);
//	while(front!=rear)
//	{
//		q=Q[++front];
//		//..................
//			
//		if (q->isLeaf)//�����Ҷ�ӽڵ�
//		{
//
//			ID=q->ClassID;
//			I=q->attIndex;
//			DPoint=q->splitVertex;
//			classID.Format(_T("%d"),ID);
//			index.Format(_T("%d"),I);
//			point.Format(_T("%.1lf"),DPoint);
//			att1=_T("Leaf,ID=");
//			att2=_T(",index=NULL");
//			att3=_T(",point=NULL");
//			row=att1+classID+att2+att3+_T("\r\n");		
//			f.Write(row,2*row.GetAllocLength());
//		}
//		else
//		{
//			ID=q->ClassID;
//			I=q->attIndex;
//			DPoint=q->splitVertex;
//			classID.Format(_T("%d"),ID);
//			index.Format(_T("%d"),I);
//			point.Format(_T("%.4lf"),DPoint);
//			att1=_T("Node,ID=");
//			att2=_T(",Index=");
//			att3=_T(",Point=");
//			row=att1+classID+att2+index+att3+point+_T("\r\n");
//			f.Write(row,2*row.GetAllocLength());
//
//		}
//		
//		if (q->leftChild!=NULL)
//		{
//			Q[++rear]=q->leftChild;
//		}
//		if (q->rightChild!=NULL)
//		{
//			Q[++rear]=q->rightChild;
//		}
//	};
//	f.Close();
//}

int Tree::CalculateErrors(TreeNode* root, double ** SampleDatas, int m , int n,TuplesClassified* tuples,int tuplesN)
{
    int erros=0;
	for (int i=0;i<tuplesN;i++)
	{
		if (tuples[i].err==1)
		{
			erros++;
		}
		
	}

	m_erroNum=0; m_tuplesI=0;m_allErros=0; //��ʼ��
 
	return erros;
}



TuplesClassified * Tree::GetClassifiedTuples(TreeNode * root, int m)
{
	return NULL;
}



void Tree::traverse(TreeNode* root, TuplesClassified* T, int i)
{
}



void Tree::Traverse(TreeNode* root,double ** SampleDatas, int m , int n,TuplesClassified * tuples,int tuplesN,bool isReplaceTuples/*�Ƿ����Ԫ����Ϣ�������ڼ��������*/)
{	
	if (root==NULL)
	{
		return;
	}
	else
	{
		Traverse(root->leftChild,SampleDatas,m,n,tuples,tuplesN,isReplaceTuples);
		//���ʸ����ڵ������
		//.......................
		if (root->isLeaf)
		{		
			if (isReplaceTuples)     //��ȡ�ڵ�Ҷ�ӽڵ����ݣ�������ԭ����Ԫ���б�
			{

			int M=root->allN;  
			TuplesClassified *T;
			T=root->tupInformation;	
			int errosOnleaf,temp;  //��ʼ��
			temp=m_erroNum;
			
			for (int i=0;i<M;i++)
			{
				/*int index=T[i].sampleDataIndex;
				int newID=T[i].tuplesClassID;
				int oldID=SampleDatas[index][n-1];*/
			//	//*******************************
			////��ӡ���ձ���֤�㷨����ȷ��
			//	/*for (int j=0;j<n;j++)
			//	{
			//		CString temps,str;
			//		double tempx;
			//		tempx=SampleDatas[index][j];
			//		temps.Format(_T("%f"),tempx);
			//		str=temps+_T(",");
			//		file.Write(str,2*str.GetLength());
			//	}
			//	CString t;
			//	t=_T("\r\n");
			//	file.Write(t,2*t.GetLength());*/
			////***************************
				//m_tuplesInformation[m_tuplesI].sampleDataIndex=index;
				//m_tuplesInformation[m_tuplesI].tuplesClassID=oldID;
				//m_tuplesInformation[m_tuplesI].err=0;  //��ʵ������
				tuples[m_tuplesI].i=T[i].i;
				tuples[m_tuplesI].sampleDataIndex=T[i].sampleDataIndex;
				tuples[m_tuplesI].tuplesClassID=T[i].tuplesClassID;
				tuples[m_tuplesI].err=T[i].err; 
				tuples[m_tuplesI].weight=T[i].weight;//Ҫ��Ҫ����ڵ��ϼ�¼�Ķ�����������������Ҹо�����
				//��ʵ������
				//if (newID!=oldID)   //Ԫ�鱻���
				//{
				//	//m_tuplesInformation[m_tuplesI].err=1; //Ϊ��ֵ�Ԫ�������Err
				//	tuples[m_tuplesI].err=1; //Ϊ��ֵ�Ԫ�������Err
				//	m_erroNum++;    //��¼��ֵ�Ԫ�����
				//	//**************************
				//	/*for (int j=0;j<n;j++)
				//	{
				//		CString temps,str;
				//		double tempx;
				//		tempx=SampleDatas[index][j];
				//		temps.Format(_T("%f"),tempx);
				//		str=temps+_T(",");
				//		file.Write(str,2*str.GetLength());
				//		
				//	}
				//	CString t,t1;
				//	t1.Format(_T("%d"),newID);
				//	t=t1+_T("\r\n");
				//	file.Write(t,2*t.GetLength());*/
				//	//***********************************
				//
				//}
				m_tuplesI++;
			}
            errosOnleaf=m_erroNum-temp;
			}
			else
			{					
				m_publics=m_publics+root->publicClassN;
				m_allDatas=m_allDatas+root->allN;
			



			int M=root->allN;  
			TuplesClassified *T;
			T=root->tupInformation;	
			int errosOnleaf,temp;  //��ʼ��
			temp=m_erroNum;		
			for (int i=0;i<M;i++)
			{
				if (T[i].err==1)
				{
					m_allErros++;
				}	
			}
			}
			//root->errors=errosOnleaf;    //��ø�Ҷ�ӽڵ��´�ֵĸ���
			//root->errors=root->allN-root->publicClassN;
			//m_erroNum=m_erroNum+root->errors;
      

		}
		Traverse(root->rightChild,SampleDatas,m,n,tuples,tuplesN,isReplaceTuples);
	}
}



void Tree::getTreeHeight(int h)
{
	m_treeHeight=h;
}



double Tree::ErroRatioForModel(TuplesClassified * tuplesInfmation, int tuplesNum)
{
	double errorM=0,temp;
	for (int i=0;i<tuplesNum;i++)
	{
		temp=tuplesInfmation[i].weight*tuplesInfmation[i].err;
		errorM=errorM+temp;
	}
	return errorM;
}



void Tree::TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/)
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



void Tree::TuplesInformationDelete(TuplesClassified *tuples)
{
	delete [] tuples;
}






double** Tree::GetSampleDatas(double** sampledates, int m , int n)
{
	double ** getSample;
	int SampleRow;
	int SampleLine;
	m_sampleDatas=sampledates;
	m_sampleDatasLine=n;
	m_sampleDatasRow=m;
	getSample=m_sampleDatas;
	SampleRow=m_sampleDatasRow;
	SampleLine=m_sampleDatasLine;
	return getSample;
}



int** Tree::GetSampleDataIndex(TuplesClassified* tuples,int tuplesN)
{
	m_sampleDataIndexRow=tuplesN;
	m_sampleDataIndexLine=2;

	int ** spDataIndex;
	spDataIndex=new int * [m_sampleDataIndexRow];  
	for (int i=0;i<m_sampleDataIndexRow;i++)
	{
		spDataIndex[i]=new int [2];
	}

	for (int i=0;i<m_sampleDatasRow;i++)
	{       
		   spDataIndex[i][0]=tuples[i].sampleDataIndex;
		   spDataIndex[i][1]=tuples[i].tuplesClassID;
	}
	return spDataIndex;
}



void Tree::DeleteSampleDataIndex(TuplesClassified* tuples)
{
	delete [] tuples;
}




int ** Tree::GetAttIndexList(int attNum, int attD/*Ĭ��2*/)
{
	int row=attNum;
	int** attListIndex;
	attListIndex=m_attListIndex;

	attListIndex=new int* [row];  
	for (int i=0;i<row;i++)
	{
		attListIndex[i]=new int [2];
	}

	return attListIndex;
}



void Tree::DeletAttIndexList(int ** attIndexList, int attM , int attN)
{
}


void Tree::SampleDataInitialization(double ** sampleData, int row, int line)
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
	TuplesInformationInitial(row/*Ԫ��ĸ���*/,spDataIndex/*��������*/,row/*��*/,2/*�У�һ����2*/);
	// Tree::TuplesInformationInitial(int tuplesNum/*Ԫ��ĸ���*/,int **sampDataIndex/*��������*/,int m/*��*/,int n/*�У�һ����2*/)
}



int Tree::GetChromoRoulette(TuplesClassified* tuples, int tuplesN)
{
	int indexChoosed=0;
	//����һ��0����Ӧ�������ܺ�֮��������������Ԫ��Ȩ�ر��淶������ȡ0��1	
	 int m=rand(); //�����������
	 double Slice=(double)std::rand()/(double)RAND_MAX;//����0��1�����
	 //�ۼ���Ӧ�Է����ĺ�
	 double FitnessSoFar=0;
	 for (int i=0;i<tuplesN;i++)
	 {
		 //�ۼ���Ӧ�Է���
		 FitnessSoFar=FitnessSoFar+tuples[i].weight;
		 //����ۼƷ��������������ѡ���ʱ������������
		 if (FitnessSoFar>=Slice)
		 {
          indexChoosed=i;
		  break;
		 }
	 }
	 //����ת��ѡ�����ĵĸ�������
	
	 //CString st,str;
	
	 //st.Format(_T("%d"),m);
	 //str=st+_T(",");
	 //file1.Write(str,2*str.GetLength());

  //   CString x=_T("\r\n");
  //   file1.Write(x,2*x.GetLength());
		 //}
	return indexChoosed;
}



void Tree::DeleteSamplingReplacement(TuplesClassified* tuples)
{
	delete [] tuples;
}



void Tree::GetTreeParameter(int treeHeight,int leafsSize)
{
	m_treeHeight=treeHeight;
	m_leafsSize=leafsSize;
}



bool Tree::PostPruneEBP(TreeNode* root,TuplesClassified *tuples,int tuplesN)
{
	//�ж��Ƿ���Ҷ�ӽڵ�
	if (root->isLeaf)      
	{
        return false;
	}
	PostPruneEBP(root->leftChild,tuples,tuplesN);
	PostPruneEBP(root->rightChild,tuples,tuplesN);
	//�������ĸ���
	int erros=0,covers=0;//erros����������ĸ�����covers��������
	covers=root->allN;
//	erros=CalculateErrors(root,m_sampleDatas/*��������*/,m_sampleDatasRow,m_sampleDatasLine,tuples, tuplesN);
	erros=root->errors;
	double p=erros/covers;  //����ĸ���
    //������������
	double U=CalculateConfidenceLLimit(covers,erros,p);
	//����Ҷ�ڵ��Ԥ����������,Ҷ�ڵ��Ԥ����������=�����Ҷ�ڵ��������*��Ҷ�ڵ��Ԥ����������U
	double preErrors/*Ԥ����������*/=0;
	preErrors=covers*U;//U�ǲ��Ǹ�Ҷ�ڵ��Ԥ���������ʣ�������Ϊ������


	//�ж��Ƿ��֦����μ�֦
	//��������t������Ҷ�ڵ�Ԥ����������֮�ͣ���ΪE1
	double E1=ClaculateLeafPreErrorsADD(root);
	//��������t����֦��Ҷ�ڵ����ʱ��Ԥ��������������ΪE2
	double E2=preErrors;//?
	//��������t������֦��Ԥ��������������ΪE3
	     //Ѱ������֧
	int L=GetChildTreeHeight(root->leftChild); //��������������
	int R=GetChildTreeHeight(root->rightChild); //�ж�������������
	int C=0;
	if (L!=R)
	{
		C=(L>R ? L:R);
	}
	    //������E3
	double E3=0;
	TreeNode * grafting;
	if (C==L&&C!=0)
	{
		E3=CalculatePreErrors(root->leftChild,tuples,tuplesN);
		grafting=root->leftChild;
	}
	if (C==R&&C!=0)
	{
		E3=CalculatePreErrors(root->rightChild,tuples,tuplesN);
		grafting=root->rightChild;
	}
    //�Ƚ�E1��E2��E3������
	double minE=0,a,b;
	a=(E2<E3 ? E2:E3);
	minE=(E1<a ? E1:a);
	//E1��Сʱ������֦
	if (minE==E1)  
	{
		return false;
	}
	//E2��Сʱ�����м�֦����һ��Ҷ�ڵ����t
	if (minE==E2)
	{
		TreeNode  newNode;
		newNode.allN=root->allN;
		newNode.attIndex=root->attIndex;
		newNode.ClassID=root->publicClassID;//��ԭ���ڵ�Ķ�������Ϊ�½ڵ�����
		newNode.errors=root->allN-root->publicClassN;//��ָ���=����-������ĸ���
		newNode.isLeaf=true;//��Ҷ�ӽڵ�ı�־
		newNode.leftChild=NULL;
		newNode.rightChild=NULL;
		newNode.publicClassID=root->publicClassID;
		newNode.publicClassN=root->publicClassN;
		newNode.splitVertex=root->splitVertex;//Ҷ�ӽڵ����Ϣ������ν����ʱ��ֵ�ˣ�ûӰ��
		newNode.tupInformation=root->tupInformation;//�����е���ң����һ��Ԫ����÷������Ƴ�����ʱ����ܵ�Ӱ�죬����û���Ƴ�Ԫ����Ϣ���������Ҫ�Ƴ������ԵøĽ�
		
		TreeNode * temp;
		temp=root;        //��¼���ڵ�ĵ�ַ
		//DeleteTree(root); //�Ƴ�������
		DeleteTree(root->leftChild);
		DeleteTree(root->rightChild);
		temp->allN=newNode.allN;
		temp->attIndex=newNode.attIndex;
		temp->ClassID=newNode.ClassID;
		temp->errors=newNode.errors;
		temp->isLeaf=newNode.isLeaf;
		temp->leftChild=newNode.leftChild;
		temp->rightChild=newNode.rightChild;
		temp->publicClassID=newNode.publicClassID;
		temp->publicClassN=newNode.publicClassN;
		temp->splitVertex=newNode.splitVertex;
		temp->tupInformation=newNode.tupInformation;
		
	}
	//E3��Сʱ�����á��޽ӡ�(grafting)���ԣ������������֦����t
	if (minE==E3)
	{
		//�жϼ޽ӵ���������
		if (C==L&&C!=0)
		{   
			DeleteTree(root->rightChild);//��ɾ��������
			TreeNode * temp;
		    temp=root->leftChild;
			TreeNode replaceNode;
			replaceNode.allN=temp->allN;
			replaceNode.attIndex=temp->attIndex;
			replaceNode.ClassID=temp->ClassID;
			replaceNode.errors=temp->errors;
			replaceNode.isLeaf=temp->isLeaf;
			replaceNode.leftChild=temp->leftChild;
			replaceNode.rightChild=temp->rightChild;
			replaceNode.publicClassID=temp->publicClassID;
			replaceNode.publicClassN=temp->publicClassN;
			replaceNode.splitVertex=temp->splitVertex;
			replaceNode.tupInformation=temp->tupInformation;

			root->allN=replaceNode.allN;
			root->attIndex=replaceNode.attIndex;
			root->ClassID=replaceNode.ClassID;
			root->errors=replaceNode.errors;
			root->isLeaf=replaceNode.isLeaf;
			root->leftChild=replaceNode.leftChild;
			root->rightChild=replaceNode.rightChild;
			root->publicClassID=replaceNode.publicClassID;
			root->publicClassN=replaceNode.publicClassN;
			root->splitVertex=replaceNode.splitVertex;
			root->tupInformation=replaceNode.tupInformation;

			delete temp;  //ɾ�����ӽڵ�
		
		}
		if (C==R&&C!=0)
		{
			DeleteTree(root->leftChild);//��ɾ��������
			TreeNode * temp;
			temp=root->rightChild;
			TreeNode replaceNode;
			replaceNode.allN=temp->allN;
			replaceNode.attIndex=temp->attIndex;
			replaceNode.ClassID=temp->ClassID;
			replaceNode.errors=temp->errors;
			replaceNode.isLeaf=temp->isLeaf;
			replaceNode.leftChild=temp->leftChild;
			replaceNode.rightChild=temp->rightChild;
			replaceNode.publicClassID=temp->publicClassID;
			replaceNode.publicClassN=temp->publicClassN;
			replaceNode.splitVertex=temp->splitVertex;
			replaceNode.tupInformation=temp->tupInformation;

			root->allN=replaceNode.allN;
			root->attIndex=replaceNode.attIndex;
			root->ClassID=replaceNode.ClassID;
			root->errors=replaceNode.errors;
			root->isLeaf=replaceNode.isLeaf;
			root->leftChild=replaceNode.leftChild;
			root->rightChild=replaceNode.rightChild;
			root->publicClassID=replaceNode.publicClassID;
			root->publicClassN=replaceNode.publicClassN;
			root->splitVertex=replaceNode.splitVertex;
			root->tupInformation=replaceNode.tupInformation;

			delete temp;  //ɾ���Һ��ӽڵ�
			
		}
		return true;
	}


	



	


}



int Tree::CalculateFactorial(int n)
{
	int f=1;
	for (int i=n;i>0;i--)
	{
		f=f*i;
	}
	return f;
}



double Tree::CalculateConfidenceLLimit(int n/*������*/, int e/*ʵ��Ĵ���*/, double p/*ÿ��ʵ�����ĸ���*/)
{
	double C=0/*���*/,CF=0;
	int k=0;
	for (k=0;k<=e;k++)
	{
		int denominator=0;
		int numerator=0;
		denominator=CalculateFactorial(n);
		numerator=CalculateFactorial(k)*CalculateFactorial(n-k);
		C=denominator*1.0/numerator*1.0;

		double P1=0,P2=0;
		P1=pow(p,k);
		P2=pow(p,n-k);

		double tempResult=0;
		tempResult=C*P1*P2;
		CF=CF+tempResult;
	}
	return CF;
}



double Tree::ClaculateLeafPreErrorsADD(TreeNode* root)
{
    double leafPreErrosADD;     //
	m_leafPreErrorsADD=0;       //��ʼ��
	TraverseChildTree(root,0/*�ò���û����*/);//����Ҷ�ӽڵ㣬������
	leafPreErrosADD=m_leafPreErrorsADD;
	return leafPreErrosADD;
}



void Tree::TraverseChildTree(TreeNode * root, double LeafPreErrorsADD/*�ò���û����*/)
{
	if (root==NULL)
	{
		return;
	}
	else
	{
		TraverseChildTree(root->leftChild,LeafPreErrorsADD);
		//���ʸ����ڵ������
		//.......................
		if (root->isLeaf)
		{
			//�������ĸ���
			int erros=0,covers=0;//erros����������ĸ�����covers��������
			covers=root->allN;
			erros=root->errors;
			double p=erros/covers;  //����ĸ���
			//������������
			double U=CalculateConfidenceLLimit(covers,erros,p);
			//����Ҷ�ڵ��Ԥ����������,Ҷ�ڵ��Ԥ����������=�����Ҷ�ڵ��������*��Ҷ�ڵ��Ԥ����������U
			double preErrors/*Ԥ����������*/=0;
			preErrors=covers*U;//U�ǲ��Ǹ�Ҷ�ڵ��Ԥ���������ʣ�������Ϊ������
		
		
			m_leafPreErrorsADD=m_leafPreErrorsADD+preErrors;
			
		}
		TraverseChildTree(root->rightChild,LeafPreErrorsADD);
	}
}



int Tree::GetChildTreeHeight(TreeNode* root)
{
	m_childTreeLeafs=0;
	m_childTreeNodes=0;
	InOrder(root);
	int nodes=m_childTreeNodes;
	int leafs=m_childTreeLeafs;
	double k,height;
	int nodes2=nodes-leafs;
	k=log(nodes2*1.0)/log(2.0);
	height=ceil(k)+1;//��k��ȡ��,�������

	return height;
}



double Tree::CalculatePreErrors(TreeNode* root,TuplesClassified * tuples,int tuplesN)
{
	//�������ĸ���
	int erros=0,covers=0;//erros����������ĸ�����covers��������
	covers=root->allN;
	erros=CalculateErrors(root,m_sampleDatas/*��������*/,m_sampleDatasRow,m_sampleDatasLine,tuples,tuplesN);
	double p=erros/covers;  //����ĸ���
	//������������
	double U=CalculateConfidenceLLimit(covers,erros,p);
	//����Ҷ�ڵ��Ԥ����������,Ҷ�ڵ��Ԥ����������=�����Ҷ�ڵ��������*��Ҷ�ڵ��Ԥ����������U
	double preErrors/*Ԥ����������*/=0;
	preErrors=covers*U;//U�ǲ��Ǹ�Ҷ�ڵ��Ԥ���������ʣ�������Ϊ������

	return preErrors;
}



void Tree::DeleteTree(TreeNode* root)
{
	if (root!=NULL)
	{
		DeleteTree(root->leftChild);
		DeleteTree(root->rightChild);
		TuplesClassified * temp;
		temp=root->tupInformation;
		//delete  temp;
		delete root;
		
	}
}



//int Tree::GetClassRules(TreeNode* root)
//{
//
//	m_strRules=_T("");//��ʼ������
//	
//	CString stRootRules=_T("");
//
//    file2.Open(_T("C:\\Data\\rules1.rules"),CFile::modeCreate|CFile::modeWrite/*|CFile::typeText*//*|CFile::typeBinary*/);
//	InorderForRules(root,0,stRootRules);
//	delete [] m_stackForNodeRules;//��ջ
//	m_stackForNodeRulesTop=0;     //����
//    file2.Close();
//	return 0;
//}



//bool Tree::InorderForRules(TreeNode* root,int tag/*��¼��ǰ��Ϊ������*/,CString stRules/*��¼�ڵ�֮ǰ�Ĺ���*/)
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
//		CString stSplitPoint;
//		stSplitPoint.Format(_T("%f"),splitPoint);
//		CString stSplitInex=_T("");
//		stSplitInex.Format(_T("%d"),spIndex);
//		CString stSign=_T("");             //����
//		CString strulesf=_T("");            //��ǰ�ڵ����
//        CString stRuleNew=_T("");           //�µĹ������������´ε���
//
//		if (tag==1)//��ǰ�ڵ�����ڵ�
//		{
//			stSign=_T("<");
//		}
//		if (tag==2)
//		{
//			stSign=_T(">=");
//		}
//
//		if (root->isLeaf)                  //�����Ҷ�ӽڵ㣬�����������
//		{
//
//			CString stClassID=_T("");
//			CString st,rules,ed;
//			int clssId=root->ClassID;
//			stClassID.Format(_T("%d"),clssId);
//
//			//st=_T("classID=")+stClassID+_T("\r\n");		
//			//rules=st+stRules+stSign+_T("\"\r\n");	
//			//file2.Write(rules,2*rules.GetLength());//�������
//
//			st=_T("classID=")+stClassID+_T("\r\n");		
//			rules=st+stRules+stSign+_T("\"\r\n");	
//			file2.WriteString(rules);//�������
//
//		}
//		else
//		{		
//			
//		   strulesf=_T("att=\"")+stSplitInex+_T("\" ")+_T("cut=\"")+stSplitPoint+_T("\" ")+
//				_T("result=\"");	//��ȡ���ڵ�Ĺ�����Ϣ
//		   if (tag==0)    //��ǰ�ڵ��Ǹ����ڵ�
//		   {
//			   stRuleNew=strulesf;
//		   }
//		   else
//		   {
//           stRuleNew=stRules+stSign+_T("\"\r\n")+strulesf; //stRules+stSign���ڸ��ڵ�����ȡ�Ĺ��� strulesf�Ǳ��ڵ���ȡ�Ĺ�����Ϣ����û�з���
//		   }
//			//if (InorderForRules(root->leftChild))//������������������
//			//{
//			//	stSign=_T("<");
//			//}
//			//if (InorderForRules(root->rightChild))//�����������������
//			//{
//			//	stSign=_T(">=");
//			//}	
//		}
//		InorderForRules(root->leftChild,1,stRuleNew);//���������������
//		InorderForRules(root->rightChild,2,stRuleNew);//��������������
//	}
//	return true;
//}


//// ���Ծ���,���ڻ�÷�����Ԫ�鼯����MeasuringAccuracy����
//TuplesClassified* Tree::MeasuringAccuracyMatch(TuplesClassified* MeasuringTuples,int tuplesNum,TreeNode * root,CString rulesPath)
//{
//	CString FileData; //����һ��CString������Ϊ������
//	int line=0,row=0; //������
//
//	CString stOld=_T("classID="),stNew=_T("classID=");
//	int tempTupleN=tuplesNum;
//	int classId;
//	//���ļ�����ȡ����
//	f.Open(rulesPath,CFile::modeReadWrite/*|CFile::typeText*/);
//	//��ͷ��ʼ��
//	f.ReadString(FileData);//��ȡ��һ�У��õ���ͷ
//	CString temp=_T("classID=");
//	int i=temp.GetLength();
//	int j=FileData.GetLength();
//	int m=j-i;
//	CString stClassId=FileData.Right(m);
//	_stscanf(stClassId,_T( "%d"),&classId);
//	//������ʼ��
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
//	while (f.ReadString(FileData))
//	{
//		if (FileData.Find(_T("classID="))>-1)//���������ͷ����֮ǰ�ķ�����д���
//		{
//			 //��֮ǰ�ķ�����д���
//			for (int i=0;i<tempN;i++)      
//             {
//				int tuplesIndex=tempcode[i];
//				MeasuringTuples[tuplesIndex].tuplesClassID=classId;
//			 }
//
//			//������ǰ�ı�ͷ���ݣ����ֳ�ʼ��
//			     //��ͷ��ʼ��
//			CString temp=_T("classID=");
//			int i=temp.GetLength();
//			int j=FileData.GetLength();
//			int m=j-i;
//		    CString stClassId=FileData.Right(m);
//			_stscanf(stClassId,_T( "%d"),&classId);
//			     //������ʼ��
//			tempcode[1000];       //��¼�ϴ�ѭ������¼������
//			newTempcode[1000];    //ѭ��������¼������
//			tempN=tuplesNum;      //��ʼ������������Ԫ�������ͬ
//			tempi=0;              //suoyi����������ʼΪ0
//			     //��ʼ�������������¼Ԫ�鰴˳���������
//			for (int ii=0;ii<tempN;ii++)
//			{
//				tempcode[i]=i;
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
//				 CString tempA=_T("<");
//				 CString tempB=_T(">=");
//				 //�����ݷ���
//				  int tmpi=0;        //���������������ݱ����ɷ������������ĸ���
//				 if (stRuleStr==tempA)  //С��
//				 {
//					 //�������������������������
//					 for (int i=0;i<tempN;i++)
//					 {
//						 int tuplesindex=tempcode[i];//����������¼��Ԫ������
//						 int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//ͨ��Ԫ��������ҳ��������ݵ�����
//						 if (m_testDatas[testindex][att]<cut) //�����������
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
//						 if (m_testDatas[testindex][att]<cut) //�����������
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
//
//
//	f.Close();
//	TuplesClassified *M=MeasuringTuples;
//
//	return M;
//}
//

// ������������ķ������Ծ��ȣ�������MeasuringAccuracy����
void Tree::InfoOrderForMeasuring(TreeNode* root,TuplesClassified* MeasuringTuples,int tuplesNum)
{
	//if (root==NULL)
	//{
	//	return ;
	//}
	//else
	//{

	//	//���ʸ����ڵ������
	//	//.......................
	//	double splitPoint=root->splitVertex;//��÷��ѵ�
	//	int spIndex=root->attIndex;         //�����������
	//	
	//	if (root->isLeaf)                  //�����Ҷ�ӽڵ㣬�����������
	//	{
	//		int clssId=root->ClassID;

	//	}
	//	else
	//	{		

	//	}
	//	}
	//	InorderForRules(root->leftChild,1,stRuleNew);//���������������
	//	InorderForRules(root->rightChild,2,stRuleNew);//��������������
	//}
	//return true;
}

//
//double Tree::MeasuringAccuracy(TuplesClassified* MeasuringTuples,int tuplesNum,TreeNode * root,CString rulesPath)
//{
//	int cnt=0;
//	TuplesClassified* tempMeasuringTuples=MeasuringAccuracyMatch(MeasuringTuples,tuplesNum,root,rulesPath);
//	for (int i=0;i<tuplesNum;i++)
//	{
//		int index=tempMeasuringTuples[i].sampleDataIndex;
//		if (tempMeasuringTuples[i].tuplesClassID!=(int)m_testDatas[index][m_testDatasLine-1])
//		{
//			cnt++;
//		}
//	}
//	double Accuracy=(tuplesNum-cnt)*1.0/tuplesNum;
//	return Accuracy;
//}
//
//

//double Tree::GetMeasuringAccuracy(int tuplesN,TreeNode * root,CString rulesPath)
//{
//	TuplesClassified* MeasuringTuples;
//	MeasuringTuples=new TuplesClassified[tuplesN];int j=0;
//	for (int i=0;i<tuplesN;i++)
//	{
//		MeasuringTuples[i].sampleDataIndex=i;
//		MeasuringTuples[i].tuplesClassID=m_testDatas[i][m_sampleDatasLine-1];
//		MeasuringTuples[i].err=0;
//		MeasuringTuples[i].weight=0;
//	}
//	double r=MeasuringAccuracy(MeasuringTuples,tuplesN,root,rulesPath);
//	return r;
//}


// ��ʼ����������
//void Tree::TestDataInitialization(double** testData , int row , int line)
//{
//	/*for (int i=0;i<row;i++)
//	{
//	for (int j=0;j<line;j++)
//	{
//	m_testDatas[i][j]
//	}
//	}*/
//	m_testDatas=testData;
//	m_testDatasRow=row;
//	m_testDatasLine=line;
//}


// ��ӡ��Ԫ����Ϣ
//void Tree::PrintTuples(TuplesClassified * tuples, int tuplesN, CString path)
//{
//	CStdioFile file2;
//    file2.Open(path,CFile::modeCreate|CFile::modeWrite);
//	int i=0;
//	int sindex=0;
//	int classid=0;
//	int err=0;
//	double w=0;
//    CString sti,stSindex,stClassid,stErr,stW,st,ed;
//	TuplesClassified * Di=tuples;	 
//
//		for (int ii=0;ii<tuplesN;ii++)
//		{
//			i=Di[ii].i;
//			sindex=Di[ii].sampleDataIndex;
//			classid=Di[ii].tuplesClassID;
//			err=Di[ii].err;
//			w=Di[ii].weight;
//
//			
//			sti.Format(_T("%d"),i);
//			stSindex.Format( _T("%d"),sindex);
//			stClassid.Format( _T("%d"),classid);
//			stW.Format( _T("%lf"),w);
//			stErr.Format( _T("%d"),err);
//            ed=_T("\r\n");
//			st=_T("i=")+sti+_T("sindex=")+stSindex+_T(",classId=")+stClassid+_T(",err=")+
//				stErr+_T(",w=")+stW+ed;
//			
//			file2.WriteString(st);
//		}
//	file2.Close();
//}
//


//void Tree::PrintSampdatasByIndex(double ** sampdata, int row , int line , TuplesClassified* tuples, int tuplesN , CString path)
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
//	
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
//				 stvalues.Format(_T("%fl"),values);
//                 temp=stvalues+_T(",");
//				 st=st+temp;
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
//	
//	
//	fi.Close();
//}
//



//void Tree::PrintSampledatsBySDataIndex(int ** S,int m,int n,CString path)
//{
//
//	CStdioFile fi;
//	fi.Open(path,CFile::modeCreate|CFile::modeWrite);
//	int sindex=0;
//	int classid=0;
//	int err=0;
//	double w=0;
//	double values=0;
//	CString stSindex,st=_T(""),ed=_T("\r\n"),stvalues,temp		;
//		 
//
//	for (int ii=0;ii<m;ii++)
//	{
//		st=_T("");
//		sindex=S[ii][0];
//		for (int jj=0;jj<m_sampleDatasLine;jj++)
//		{
//			values=m_sampleDatas[sindex][jj];
//
//			if (jj!=m_sampleDatasLine-1)
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
//
//		}
//		st=st+ed;
//		fi.WriteString(st);
//
//	}
//	fi.Close();
//}


//// //��ӡ��������
//void Tree::PrintSampdatas(double** S, int m , int n)
//{
//}


// ��������Ҷ�Ӵ���Ԫ���б��˳��
void Tree::TuplesOrderByTree(TuplesClassified* tuples, int tuplesN)
{

}

//TreeNode* Tree::BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/,int & progressI/*���ڼ������*/,CString & Eror)
//{	
//	if (tuplesN==0)
//	{
//		
//		Eror=_T("�������ݲ���Ϊ�գ�");
//		return NULL;
//	}
//	CString timestr;
//	
//	double ** S; 
//	
//	TuplesClassified * Tindex=tuples;
//	S=new double* [tuplesN];                //�����еĿռ䣻
//
//	for (int i=0;i<tuplesN;i++)
//	{
//		S[i]=new double [line];
//	}
//	for (int i=0;i<tuplesN;i++)
//	{
//	
//		int t=Tindex[i].sampleDataIndex;
//	
//		for (int j=0;j<line;j++)
//		{
//			S[i][j]=sampleData[t][j];
//		}
//	}
//	
//	int S_row=tuplesN;         
//	int S_line=line;     
//
//	int T_row=tuplesN;
//	int **attList=attIndex; 
//	int attN=attNum;      
//	int attL;
//	double k=0;
//	TreeNode * Node;
//	Node=new TreeNode;
//	m_NodeSize++;    
//	
//	AttValuesTab *D;
//	
//	D=new AttValuesTab[T_row];
//	int temprow=0;
//	
//    for (int i=0;i<T_row;i++)
//	{
//		
//		temprow=Tindex[i].sampleDataIndex;
//		D[i].classID=sampleData[temprow][line-1];
//		D[i].AttValue=sampleData[temprow][0];    
//		D[i].AttValue=0;                         
//	}
//	ClassUnique * L,*p;
//
//	L=getClassUnique(D,T_row);
//	p=L;
//	int classN=0;
//	classN=L->cunt;
//
//	
//	int i=0,Max=0,publicClassID=0,Num=0;
//	while (p)
//	{
//		if (i=0)
//		{
//			p=p->next;
//			i++;
//		}
//		else
//		{
//			Num=p->cunt;
//			if (Max<Num)
//			{
//				Max=Num;
//				publicClassID=p->classID;
//			}
//			p=p->next;
//		}
//	}
//	
//	int publicClassN=0,allN=0;
//	publicClassN=Max;
//	allN=L->classID;
//
//	Node->allN=T_row;  
//	Node->publicClassN=publicClassN;
//	Node->publicClassID=publicClassID;
//	
//	if (T_row<=m_leafsSize)     
//	{
//		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);
//
//		Node->ClassID=publicClassID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
//		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
//		Node->leftChild=NULL;
//		Node->rightChild=NULL;
//		Node->splitVertex=0;
//		//����ԭʼ�б���Ϣ
//		TuplesClassified *tOri;
//		tOri=Node->tupInformation;		
//		int OTindex=0;
//		for (int i=0;i<Node->allN;i++)
//		{
//			OTindex=tOri[i].i;
//			if (tOri[i].err==1)
//			{
//              tuplesOriginal[OTindex].err=1;
//			}
//			else
//              tuplesOriginal[OTindex].err=0;
//			
//		}
//		int cor=Node->publicClassN;
//		int cov=Node->allN;
//		double arc=(cor*1.0)/(cov*1.0);
//
//		
//		
//
//		m_Dimension0++;
//		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�
//
//		//���Դ���
//		if (m_NodeSize<=1)
//		{
//
//			 Eror=_T("The Nodesize=1(Leaf)");
//			
//
//		}
//
//		progressI=progressI+Node->allN; //���ƽ�����
//		/*m_progressBar->SetPos(progressI);                     
//		Sleep(0.1); */
//		return Node;
//
//	}
//
//	if (classN==1)  //���D�е�Ԫ�鶼��ͬһ��C
//	{
//
//		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);
//
//		Node->ClassID=L->next->classID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
//		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
//		Node->leftChild=NULL;
//		Node->rightChild=NULL;
//		Node->splitVertex=0;
//
//		//����ԭʼ�б���Ϣ
//		TuplesClassified *tOri;
//		tOri=Node->tupInformation;		
//		int OTindex=0;
//		for (int i=0;i<Node->allN;i++)
//		{
//			OTindex=tOri[i].i;
//			if (tOri[i].err==1)
//			{
//				tuplesOriginal[OTindex].err=1;
//			}
//			else
//				tuplesOriginal[OTindex].err=0;
//
//		}
//	    
//		m_Dimension0++;
//		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�
//
//		
//		if (m_NodeSize<=1)
//		{
//             Eror=_T("The Nodesize=1(Same)");
//			 	
//			
//		}
//		
//		return Node;//����N��ΪҶ�ӽڵ�
//	}
//	
//
//	k=log(m_Dimension2*1.0)/log(2.0);
//	m_k=ceil(k)+1;
//	
//	if (m_k>=hight)//����������ߣ�
//	{
//		
//
//		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);
//
//		Node->ClassID=publicClassID;//����C��ǣ�L��ͷ�ڵ㣬����¼�����Ϣ������L����һ���ڵ����������¼�����Ϣ�Ľڵ�
//		Node->isLeaf=true;//��Ҷ�ӽڵ���֮
//		Node->leftChild=NULL;
//		Node->rightChild=NULL;
//		Node->splitVertex=0;
//
//		//����ԭʼ�б���Ϣ
//		TuplesClassified *tOri;
//		tOri=Node->tupInformation;		
//		int OTindex=0;
//		for (int i=0;i<Node->allN;i++)
//		{
//			OTindex=tOri[i].i;
//			if (tOri[i].err==1)
//			{
//				tuplesOriginal[OTindex].err=1;
//			}
//			else
//				tuplesOriginal[OTindex].err=0;
//
//		}
//	
//		m_Dimension0++;
//		m_Dimension2=m_NodeSize-m_Dimension0;//��2�ڵ�=����-��1�ڵ�
//
//		//���Դ���
//		if (m_NodeSize<=1)
//		{
//
//			 Eror=_T("The Nodesize=1(Height)");
//			 return NULL;
//		}
//		//progressI=progressI+Node->allN; //���ƽ�����
//		//m_progressBar->SetPos(progressI);                     
//		//Sleep(0.1); 
//		return Node;
//	}
//	
//	
//	int splitting_criterionIndex=0;
//	double splitPointValue=0;     
//	AttIndexTab att;
//	att=ChooseAttribute1(S,S_row,S_line,attList,attN,2);
//	double r=att.attGainRatio;
//	splitting_criterionIndex=att.attIndex;
//	splitPointValue=att.splittPoint;
//	for (int i=0;i<S_row;i++)
//	{
//		delete []S[i];
//	}
//	delete []S;
//	deleteClassUnique(L);
//
//	Node->attIndex=splitting_criterionIndex;
//	Node->ClassID=-1;//���ڵ㲻��Ҷ�ӽڵ�ʱ���������-1��ʾ
//	Node->isLeaf=false;//����Ҷ�ӽڵ�
//	Node->splitVertex=splitPointValue;//�洢�������µķ���ֵ
//	
//	int D1=0,D2=0;
//	
//
//	int **attlist1,**attlist2;
//	double **S1,**S2;
//
//	TuplesClassified *T1_index,*T2_index;
//	TreeNode *LNode,*RNode;
//	int s1_m=0;
//	int s2_m=0;
//	//�ָ�����
//	T1_index=new TuplesClassified[T_row];//�����еĿռ�
//	T2_index=new TuplesClassified[T_row];//�����еĿռ䣻
//	//Ϊ�ָ������鸳ֵ
//	for (int i=0;i<T_row;i++)
//	{		
//		int temprow=Tindex[i].sampleDataIndex;
//		if (sampleData[temprow][splitting_criterionIndex]<=splitPointValue)
//		{
//
//			T1_index[s1_m].err=Tindex[i].err;
//			T1_index[s1_m].i=Tindex[i].i;
//			T1_index[s1_m].sampleDataIndex=Tindex[i].sampleDataIndex;
//			T1_index[s1_m].tuplesClassID=Tindex[i].tuplesClassID;
//			T1_index[s1_m].weight=Tindex[i].weight;
//			s1_m++;
//		}
//		else
//		{
//
//			T2_index[s2_m].err=Tindex[i].err;
//			T2_index[s2_m].i=Tindex[i].i;
//			T2_index[s2_m].sampleDataIndex=Tindex[i].sampleDataIndex;
//			T2_index[s2_m].tuplesClassID=Tindex[i].tuplesClassID;
//			T2_index[s2_m].weight=Tindex[i].weight;
//			s2_m++;
//		}
//	}
//	D1=s1_m;D2=s2_m;
//	
//	
//	if (D1!=0&&D2!=0)
//	{
//		
//	
//	}
//	else if (D1==0&&D2>0)
//	{
//		
//		
//		
//		
//		if (tagT)
//		{
//			
//			 Eror=_T("The data1 is wrror!");
//			
//			tagT=!tagT;
//		}
//		if (D1==0)
//		{
//			tagT=!tagT;
//		}
//		
//
//	}
//	else if(D2==0&&D1>0)
//	{		
//
//		if (tagT)
//		{
//			
//			 Eror=_T("The data1 is wrror!");
//			tagT=!tagT;
//		}
//		if (D2==0)
//		{
//			tagT=!tagT;
//		}
//
//	}
//	else
//	{
//	}
//	//���ԭ�����ݼ����ڴ�
//
//	
//	if (D1==0)  //���DjΪ�գ���һ����Ҷ���ڵ�N�����ΪD�еĶ�����
//	{
//	
//		//����Ҷ�ӽڵ�
//		LNode=new TreeNode;//publicClassID
//		m_NodeSize++;      //��¼�ڵ���
//		LNode->ClassID=publicClassID;
//		LNode->isLeaf=true;
//		LNode->leftChild=NULL;
//		LNode->rightChild=NULL;
//		//
//		Node->leftChild=NULL;
//		m_Dimension0++;
//	}
//	else
//	{
//		LNode=BuildTree2(sampleData,row,line,T1_index,D1,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON,progressI, Eror);
//		Node->leftChild=LNode;
//
//
//		
//	}
//	if (D2==0)
//	{
//		
//		//����Ҷ�ӽڵ�
//		RNode=new TreeNode;//publicClassID
//		m_NodeSize++;      //��¼�ڵ���
//		RNode->ClassID=publicClassID;
//		RNode->isLeaf=true;
//		RNode->leftChild=NULL;
//		RNode->rightChild=NULL;
//
//		//��Ҷ�ӽڵ����
//		Node->rightChild=NULL;
//		m_Dimension0++;
//	}
//	else
//	{
//
//		RNode=BuildTree2(sampleData,row,line,T2_index,D2,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON,progressI, Eror);
//		Node->rightChild=RNode;
//		
//	}
//	
//	delete []D;
//	
//	m_treeHeight=m_k;
//	return Node;
//}
TreeNode* Tree::BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/,int & progressI/*���ڼ������*/)//ʹ����
{	
	if (tuplesN==0)
	{
		//MessageBox(NULL,_T("�������ݲ���Ϊ�գ�"),_T("ע�⣡"),MB_OK);
		return NULL;
	}
	//CString timestr;
	
	double ** S; 
	
	TuplesClassified * Tindex=tuples;
	S=new double* [tuplesN];                //�����еĿռ䣻

	for (int i=0;i<tuplesN;i++)
	{
		S[i]=new double [line];
	}
	for (int i=0;i<tuplesN;i++)
	{
	
		int t=Tindex[i].sampleDataIndex;
	
		for (int j=0;j<line;j++)
		{
			S[i][j]=sampleData[t][j];
		}
	}
	
	int S_row=tuplesN;         //������ݼ�����
	int S_line=line;       //������ݼ�����

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
	}
	
	int publicClassN=0,allN=0;
	publicClassN=Max;
	allN=L->classID;

	Node->allN=T_row;  //��ýڵ�Ԫ�����
	Node->publicClassN=publicClassN;
	Node->publicClassID=publicClassID;

	if (T_row<=m_leafsSize)     //�����������С�ڵ���6
	{
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
			std::cout<<"ERROR:The Nodesize=1!Empty!"<<endl;
		}

		progressI=progressI+Node->allN; //���ƽ�����
		/*m_progressBar->SetPos(progressI);                     
		Sleep(0.1); */
		return Node;

	}

	if (classN==1)  //���D�е�Ԫ�鶼��ͬһ��C
	{

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
			std::cout<<"ERROR:The Nodesize=1!Empty!"<<endl;
		
		}
		//progressI=progressI+Node->allN; //���ƽ�����
		//m_progressBar->SetPos(progressI);                     
		//Sleep(0.1); 
		return Node;//����N��ΪҶ�ӽڵ�
	}
	

	k=log(m_Dimension2*1.0)/log(2.0);
	m_k=ceil(k)+1;//��k��ȡ��,�������
	
	if (m_k>=hight)//����������ߣ�
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

		//���Դ���
		if (m_NodeSize<=1)
		{
			std::cout<<"ERROR:he Nodesize=1!Height!"<<endl;
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
		LNode=BuildTree2(sampleData,row,line,T1_index,D1,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON,progressI);
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

		RNode=BuildTree2(sampleData,row,line,T2_index,D2,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON,progressI);
		Node->rightChild=RNode;
		
	}
	
	delete []D;
	
	m_treeHeight=m_k;
	return Node;
}

TreeNode* Tree::BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*�����ĳ�ʼԪ���б�*/,int tuplesON/*��ʼԪ���б����*/)
{
	

	
	double ** S;   
	
	TuplesClassified * Tindex=tuples;
	S=new double* [tuplesN];              //�����еĿռ䣻
	for (int i=0;i<tuplesN;i++)
	{
		S[i]=new double [line];
	}
	for (int i=0;i<tuplesN;i++)
	{
	
		int t=Tindex[i].sampleDataIndex;
	
		for (int j=0;j<line;j++)
		{
			S[i][j]=sampleData[t][j];
		}
	}
	int S_row=tuplesN;         //������ݼ�����
	int S_line=line;       

	int T_row=tuplesN;
	int **attList=attIndex; 
	int attN=attNum;      
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
		D[i].AttValue=sampleData[temprow][0];    //���Բ���
		D[i].AttValue=0;                         //���Բ���
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
	}
	
	int publicClassN=0,allN=0;
	publicClassN=Max;
	allN=L->classID;

	Node->allN=T_row;  //��ýڵ�Ԫ�����
	Node->publicClassN=publicClassN;
	Node->publicClassID=publicClassID;
	
	int errorcount=0;
	TuplesClassified *t; //���Ԫ����Ϣ
	t=new TuplesClassified[T_row];
	for (int i=0;i<T_row;i++)     //�Ƚ�ռ�ڴ���Կ����Ż�
	{	
		t[i].tuplesClassID=publicClassID;
		t[i].sampleDataIndex=Tindex[i].sampleDataIndex;
		t[i].err=Tindex[i].err;
		
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

	
	if (T_row<=m_leafsSize)    
	{
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
			std::cout<<"ERROR:The Nodesize=1!Empty!"<<endl;
		
		}


		return Node;

	}

	if (classN==1)  
	{
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
			std::cout<<"ERROR:The Nodesize=1!Same!"<<endl;
		}

		return Node;//����N��ΪҶ�ӽڵ�
	}
	

	k=log(m_Dimension2*1.0)/log(2.0);
	m_k=ceil(k)+1;//��k��ȡ��,�������
	
	if (m_k>=hight)//����������ߣ�
	{
		
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
			std::cout<<"ERROR:he Nodesize=1!Height!"<<endl;
		}
		return Node;
	}
	

	int splitting_criterionIndex=0;//����Ϸ��ѵ���������
	double splitPointValue=0;     //����������ѵķ��ѵ�
	AttIndexTab att;
	att=ChooseAttribute1(S,S_row,S_line,attList,attN,2);
	double r=att.attGainRatio;
	splitting_criterionIndex=att.attIndex;
	splitPointValue=att.splittPoint/*attList[splitting_criterionIndex][1]*/;
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
	
	
	int D1=0,D2=0;
	
	for (int i=0;i<T_row;i++)
	{
	
		int temprow=Tindex[i].sampleDataIndex;
		if (sampleData[temprow][splitting_criterionIndex]<=splitPointValue)
		{
			D1++;
		}
		else
		{
			D2++;
		}
	}

	int **attlist1,**attlist2;
	double **S1,**S2;

	TuplesClassified *T1_index,*T2_index;
	TreeNode *LNode,*RNode;
	int s1_m=0;
	int s2_m=0;
	if (D1!=0&&D2!=0)
	{
		
		 T1_index=new TuplesClassified[D1];//�����еĿռ䣻
		
		 T2_index=new TuplesClassified[D2];//�����еĿռ䣻
		
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
		}
	}
	else if (D1==0&&D2>0)
	{
		
		T2_index=new TuplesClassified[D2];//�����еĿռ䣻

		for (int i=0;i<T_row;i++)
		{
			
			T2_index[i].err=Tindex[i].err;
			T2_index[i].i=Tindex[i].i;
			T2_index[i].sampleDataIndex=Tindex[i].sampleDataIndex;
			T2_index[i].tuplesClassID=Tindex[i].tuplesClassID;
			T2_index[i].weight=Tindex[i].weight;

		}
		
		
		if (tagT)
		{
			
			tagT=!tagT;
		}
		if (D1==0)
		{
			tagT=!tagT;
		}
		

	}
	else if(D2==0&&D1>0)
	{
		//S1_index=new int * [D1];    //�����еĿռ䣻
		T1_index=new TuplesClassified[D1];  //�����еĿռ䣻
	
		for (int i=0;i<T_row;i++)
		{
		

			T1_index[i].err=Tindex[i].err;
			T1_index[i].i=Tindex[i].i;
			T1_index[i].sampleDataIndex=Tindex[i].sampleDataIndex;
			T1_index[i].tuplesClassID=Tindex[i].tuplesClassID;
			T1_index[i].weight=Tindex[i].weight;

		}

		if (tagT)
		{
			
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
		LNode=BuildTree2(sampleData,row,line,T1_index,D1,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON);
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

		RNode=BuildTree2(sampleData,row,line,T2_index,D2,attList,attN,2,outPut,hight,tuplesOriginal,tuplesON);
		Node->rightChild=RNode;
		
	}
	
	delete []D;
	
	m_treeHeight=m_k;
	return Node;
}



//
//void Tree::PrintSampleDataByTuples(TuplesClassified* tuples, int tuplesN)
//{
//	
//}



TreeSelection* Tree::GetTreeSelection(TreeSelection* treeSelections, int treeN/*��������*/)
{
	TreeSelection* selected;
	selected=m_treeSelection;
	return selected;
}



void Tree::DeleteTreeSelection(TreeSelection* treeSelections)
{
	delete [] treeSelections;
}






//
//int Tree::GetintRuleSet(TreeSelection * trees, int treesN,CString rulesPath)
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
//		rules.Format(_T("%d"),m_ruleCount);
//	
//		def.Format(_T("%d"),root->publicClassID);//rules="17" default="2"
//		head=_T("rules=\"")+rules+_T("\" default=\"")+def+_T("\"\r\n");
//		m_ruleSet[tempruleindex]=head;
//		m_ruleCount=0;//�������
// 	}
//   
//	//��������ļ�
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
//
//	head1=_T("id=\"GLC\/1.0 ")+y+_T("\/")+m+_T("\/")+d+_T(" ")+h+_T(":")+l+_T(":")+s+_T("\"\r\n");
//	//head1.Format()//id="GLC/1.0 2013/5/7 18:09:01"
//	Entries.Format(_T("%d"),m_adaBoostingK);
//	head2=_T("entries=\"")+Entries+_T("\"\r\n");//��������
//    fl.WriteString(head1);
//	fl.WriteString(head2);
//
//	for (int i=0;i<m_ruleSetI;i++)
//	{
//		temp=m_ruleSet[i];
//		fl.WriteString(temp);
//	}
//	fl.Close();
//    m_ruleSetI=0;//�������
//	
//	return 0;
//}


// ����������ȡ����
//bool Tree::InorderForRuleSet(TreeNode* root, int tag, double erroratio,CString stRules,int rulesN)
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
//			stSign=_T("<");
//		}
//		if (tag==2)
//		{
//			stSign=_T(">=");
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
//
//
//			st=_T("count=\"")+stCount+_T("\" cover=\"")+stCover+_T("\" correct=\"")+stCorrect+_T("\" pre=\"")
//				+stPre+_T("\" class=")+stClassID+_T("\r\n");		
//			rules=st+stRules+stSign+_T("\"\r\n");	
//			//file2.WriteString(rules);//�������
//			m_ruleSet[m_ruleSetI]=rules;
//			m_ruleSetI++;
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
//			}
//			
//			rulesCount=rulesN+1;
//		}
//		InorderForRuleSet(root->leftChild,1,erroratio,stRuleNew,rulesCount);//���������������
//		InorderForRuleSet(root->rightChild,2,erroratio,stRuleNew,rulesCount);//��������������
//	}
//	return true;
//}


// ��ù������Ĳ���
//void Tree::SetParameter(int treeHeight, int leafsSize, CString rulePath)
//{
//	m_treeHeight=treeHeight;
//	
//	m_leafsSize=leafsSize;
//
//	m_ruleFilePath=rulePath;
//}
//

// ��ʼ������
void Tree::doBuildTree(TuplesClassified *tuples,int tuplesN,int & progressI)
{
    m_treeRoot=BuildTree2(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine,tuples,tuplesN,m_attListIndex,m_sampleDatasLine-1,2,true,m_treeHeight,tuples,tuplesN,progressI);
}
void Tree::doBuildTree(TuplesClassified *tuples,int tuplesN)
{
	m_treeRoot=BuildTree2(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine,tuples,tuplesN,m_attListIndex,m_sampleDatasLine-1,2,true,m_treeHeight,tuples,tuplesN);
}


TreeNode* Tree::GetTreeRoot(void)
{
	TreeNode* t=m_treeRoot;
	return t;
}
void Tree::GetData(double ** sampleData, int row, int line,int ** attIndex)
{
	m_sampleDatas=sampleData;
	m_sampleDatasRow=row;
	m_sampleDatasLine=line;
	m_attListIndex=attIndex;
	
}


// ��������
void Tree::doVariableToZero(void)
{
	 m_treeRoot=NULL;
	 tagT=false;
		m_Dimension0=0;
		m_Dimension1=0;
		m_Dimension2=0;
		m_splitPointValue=0;
		m_treeHeight=0;
		m_Dimension=0;
		m_NodeSize=0;
		m_k=0;
		m_splitPointAttIndex=0;
		m_erroNum=0;
		m_bestSplitPointValue=0;
		m_tuplesNum=0;
		m_testDatasRow=0;
		m_testDatasLine=0;
		m_sampleDatasRow=0;
		m_sampleDatasLine=0;
		m_tuplesRow=0;
		m_tuplesLine=0;
		m_sampleDataIndexRow=0;
		m_sampleDataIndexLine=0;
		m_tuplesI=0;
		m_adaBoostingK=0;
		m_leafPreErrorsADD=0;
		m_childTreeHeight=0;
		m_childTreeLeafs=0;
		m_childTreeNodes=0;
		m_stackForNodeRulesTop=0;
		m_allDatas=0;
		m_publics=0;
		m_allErros=0;
		m_leafsSize=0;
		m_ruleSetI=0;
		m_tringDataN=0;
}


//// ���ƽ�����
//void Tree::SetProgressBar(CProgressCtrl*  progressBar)
//{
//	m_progressBar=progressBar;
//
//}



void Tree::SetValuesForTreeNode(TreeNode* treeNode,int tuplesN,TuplesClassified *tuples,double ** sampleData,int sampleData_line,int publicClassID)
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
