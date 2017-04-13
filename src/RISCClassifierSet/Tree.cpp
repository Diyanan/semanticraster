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
		//***********信息增益****************************
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
		if (i==0)//将列表初始化
		{   
			m++;//每新建一个节点m+1
			s=new ClassUnique;
			s->classID=attValues[0].classID;	
			s->cunt=1;
			s->next=NULL;
			r->next=s;
			r=s;
		}
		else
		{
			p=first->next;//将P指向头指针
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

	//统计获得类别属性唯一值的链表
	classLink=getClassUnique(values,n);
	staNum=classLink->cunt;
	//获得统计表
	staTab=AttValuesStatistics(classLink);
	deleteClassUnique(classLink);
	//计算类别属性的熵
	inf=Info(staTab,staNum);
	delete [] staTab;
	//离散属性数据
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
	//统计表赋值


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

	//统计获得类别属性唯一值的链表
	classLink=getClassUnique(values,n);
	staNum=classLink->cunt;
	//获得统计表
	staTab=AttValuesStatistics(classLink);
	//计算类别属性的熵
	inf=Info(staTab,staNum);
	//离散属性数据
	discData=discreteData(values,n);
	attInf=findDiscPoint(discData,n-1,values,n,true);
	//dis,f.sampleRows-1,c.m_AttValuesTable,f.sampleRows,true
	ginInf=inf-attInf;   //获得信息增益
	ginInfRatio=ginInf/inf;//获得信息增益率

	delete[] classLink;
	delete[] staTab;
	return ginInfRatio;

}



int Tree::ChooseAttribute(double ** sampleData, int row, int line, int * attIndex, int attNum)
{
	double ** S;
	S=sampleData;
	AttIndexTab * attIndexTab;//用于存储可供分裂属性的计算信息，
	attIndexTab=new AttIndexTab[attNum];//动态建立注意删除
	double infGainAll=0;//累计信息增益，用于计算平均值
	double averageInfGain=0; //获取增益平均值
	double minIGRatio=0;   //求最小增益率
	int minI=0;

	for (int i=0;i<attNum;i++)//attNum可供分裂属性的个数
	{
		int tempIndex=0;          //获取索引中的索引号，为了在S中取值
		tempIndex=attIndex[i];
		
		AttValuesTab attValues[1000];
		double infGain=0,infGainRatio=0;
		double splitPoint=0;
		int lSplit=0,rSplit=0;

		for (int j=0;j<row;j++)  //获得当前索引下的属性值表包含ClassID
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

		infGainAll=infGainAll+infGain;//累计信息增益，用于计算平均值

		
	}
	averageInfGain=infGainAll/attNum;//获取增益平均值

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
	AttIndexTab * attIndexTab;//用于存储可供分裂属性的计算信息，
	attIndexTab=new AttIndexTab[attNum];//动态建立注意删除
	double infGainAll=0;//累计信息增益，用于计算平均值
	double averageInfGain=0; //获取增益平均值
	double minIGRatio=0;   //求最小增益率
	int minI=0;

	for (int i=0;i<attNum;i++)//attNum可供分裂属性的个数
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

		//计算信息增益

		infGain=Gain(attValues,row);
		infGainRatio=GainRatio(attValues,row);
		attIndex[i][1]=m_splitPointValue;

		attIndexTab[i].attIndex=tempIndex;//记录索引
		attIndexTab[i].attGain=infGain;
		attIndexTab[i].attGainRatio=infGainRatio;
		attIndexTab[i].splittPoint=m_splitPointValue;

		infGainAll=infGainAll+infGain;

		
	}
	averageInfGain=infGainAll/attNum;//获取增益平均值

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
	//最小增益里有没有相等的
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
	AttIndexTab * attIndexTab;//用于存储可供分裂属性的计算信息，
	attIndexTab=new AttIndexTab[attNum];//动态建立注意删除
	double infGainAll=0;//累计信息增益，用于计算平均值
	double averageInfGain=0; //获取增益平均值
	double minIGRatio=0;   //求最小增益率
	int minI=0;

	vector<AttIndexTab> results;

	for (int i=0;i<attNum;i++)//attNum可供分裂属性的个数
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

		//计算信息增益

		infGain=Gain(attValues,row);
		infGainRatio=GainRatio(attValues,row);
		attIndex[i][1]=m_splitPointValue;

		attIndexTab[i].attIndex=tempIndex;//记录索引
		attIndexTab[i].attGain=infGain;
		attIndexTab[i].attGainRatio=infGainRatio;
		attIndexTab[i].splittPoint=m_splitPointValue;

		infGainAll=infGainAll+infGain;

		results.push_back(attIndexTab[i]);


	}
	averageInfGain=infGainAll/attNum;//获取增益平均值

	//对增益lv进行从大到小的排序
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

	////对增益lv进行从大到小的排序
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
	////最小增益里有没有相等的
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
		//访问根部节点的数据
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
	//遍历子树，如果非叶子节点，则递归
	L=iNode->leftChild;
	R=iNode->rightChild;
	//
	if (!L->isLeaf)//如果左子树不是叶子节点，继续遍历
	{
		if (PostPrune(L))   //如果可以剪枝，返回真值
		{
			return true;			
		}
		leafTopL=false;
	}
	if (!R->isLeaf)//如果右面子树不是叶子节点，继续遍历
	{
		if (PostPrune(R))   //如果可以剪枝，返回真值
		{
			return true;			
		}
		leafTopR=false;
	}
	if (leafTopL&&leafTopR)
	{
		//计算剪枝前的错误率，错误率=当前节点占少数的结果/此节点结果总数之和
		double littleN=iNode->allN-iNode->publicClassN;
		double N=iNode->allN;
		double erroBefore=littleN/N;
		double erroAfter;
		double errorNum;
		//获得此节点所有分裂分支的错误数总和
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
	
		//计算剪枝后的错误率,错误率=（值分量*0.5+错误总和）/此节点结果数和
		erroAfter=(errorNum+0.5*2)/N;
		if (erroAfter>=erroBefore)
		{
			TreeNode * newNode;
			newNode=new TreeNode;
			newNode->isLeaf=true;
			//原节点的普遍结果即最后的结果属性
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
//		if (q->isLeaf)//如果是叶子节点
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

	m_erroNum=0; m_tuplesI=0;m_allErros=0; //初始化
 
	return erros;
}



TuplesClassified * Tree::GetClassifiedTuples(TreeNode * root, int m)
{
	return NULL;
}



void Tree::traverse(TreeNode* root, TuplesClassified* T, int i)
{
}



void Tree::Traverse(TreeNode* root,double ** SampleDatas, int m , int n,TuplesClassified * tuples,int tuplesN,bool isReplaceTuples/*是否更新元组信息，不用于计算错误率*/)
{	
	if (root==NULL)
	{
		return;
	}
	else
	{
		Traverse(root->leftChild,SampleDatas,m,n,tuples,tuplesN,isReplaceTuples);
		//访问根部节点的数据
		//.......................
		if (root->isLeaf)
		{		
			if (isReplaceTuples)     //读取节点叶子节点数据，并更新原来的元组列表
			{

			int M=root->allN;  
			TuplesClassified *T;
			T=root->tupInformation;	
			int errosOnleaf,temp;  //初始化
			temp=m_erroNum;
			
			for (int i=0;i<M;i++)
			{
				/*int index=T[i].sampleDataIndex;
				int newID=T[i].tuplesClassID;
				int oldID=SampleDatas[index][n-1];*/
			//	//*******************************
			////打印对照表验证算法的真确性
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
				//m_tuplesInformation[m_tuplesI].err=0;  //其实不用这
				tuples[m_tuplesI].i=T[i].i;
				tuples[m_tuplesI].sampleDataIndex=T[i].sampleDataIndex;
				tuples[m_tuplesI].tuplesClassID=T[i].tuplesClassID;
				tuples[m_tuplesI].err=T[i].err; 
				tuples[m_tuplesI].weight=T[i].weight;//要不要清除节点上记录的东东？视情况而定，我感觉可以
				//其实不用这
				//if (newID!=oldID)   //元组被错分
				//{
				//	//m_tuplesInformation[m_tuplesI].err=1; //为错分的元组计算其Err
				//	tuples[m_tuplesI].err=1; //为错分的元组计算其Err
				//	m_erroNum++;    //记录错分的元组个数
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
			int errosOnleaf,temp;  //初始化
			temp=m_erroNum;		
			for (int i=0;i<M;i++)
			{
				if (T[i].err==1)
				{
					m_allErros++;
				}	
			}
			}
			//root->errors=errosOnleaf;    //求得该叶子节点下错分的个数
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



void Tree::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
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




int ** Tree::GetAttIndexList(int attNum, int attD/*默认2*/)
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
	// Tree::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
}



int Tree::GetChromoRoulette(TuplesClassified* tuples, int tuplesN)
{
	int indexChoosed=0;
	//产生一个0到适应新评分总和之间的随机数，由于元组权重被规范，所以取0到1	
	 int m=rand(); //随机生成整数
	 double Slice=(double)std::rand()/(double)RAND_MAX;//生成0到1随机数
	 //累计适应性分数的和
	 double FitnessSoFar=0;
	 for (int i=0;i<tuplesN;i++)
	 {
		 //累计适应性分数
		 FitnessSoFar=FitnessSoFar+tuples[i].weight;
		 //如果累计分数大于随机数，选择此时的样本（基因）
		 if (FitnessSoFar>=Slice)
		 {
          indexChoosed=i;
		  break;
		 }
	 }
	 //返回转盘选出来的的个体样本
	
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
	//判断是否是叶子节点
	if (root->isLeaf)      
	{
        return false;
	}
	PostPruneEBP(root->leftChild,tuples,tuplesN);
	PostPruneEBP(root->rightChild,tuples,tuplesN);
	//计算错误的概率
	int erros=0,covers=0;//erros被错分样本的个数，covers样本总数
	covers=root->allN;
//	erros=CalculateErrors(root,m_sampleDatas/*样本数据*/,m_sampleDatasRow,m_sampleDatasLine,tuples, tuplesN);
	erros=root->errors;
	double p=erros/covers;  //错误的概率
    //计算置信上限
	double U=CalculateConfidenceLLimit(covers,erros,p);
	//计算叶节点的预测错分样本数,叶节点的预测错分样本数=到达该叶节点的样本数*该叶节点的预测错分样本率U
	double preErrors/*预测错分样本数*/=0;
	preErrors=covers*U;//U是不是该叶节点的预测错分样本率？暂且认为这样，


	//判断是否剪枝集如何剪枝
	//计算子树t的所有叶节点预测错分样本数之和，记为E1
	double E1=ClaculateLeafPreErrorsADD(root);
	//计算子树t被剪枝以叶节点代替时的预测错分样本数，记为E2
	double E2=preErrors;//?
	//计算子树t的最大分枝的预测错分样本数，记为E3
	     //寻找最大分支
	int L=GetChildTreeHeight(root->leftChild); //计算左子树树高
	int R=GetChildTreeHeight(root->rightChild); //判断右面子树树高
	int C=0;
	if (L!=R)
	{
		C=(L>R ? L:R);
	}
	    //计算获得E3
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
    //比较E1，E2，E3，如下
	double minE=0,a,b;
	a=(E2<E3 ? E2:E3);
	minE=(E1<a ? E1:a);
	//E1最小时，不剪枝
	if (minE==E1)  
	{
		return false;
	}
	//E2最小时，进行剪枝，以一个叶节点代替t
	if (minE==E2)
	{
		TreeNode  newNode;
		newNode.allN=root->allN;
		newNode.attIndex=root->attIndex;
		newNode.ClassID=root->publicClassID;//用原来节点的多数类作为新节点的类别
		newNode.errors=root->allN-root->publicClassN;//错分个数=总数-多数类的个数
		newNode.isLeaf=true;//是叶子节点的标志
		newNode.leftChild=NULL;
		newNode.rightChild=NULL;
		newNode.publicClassID=root->publicClassID;
		newNode.publicClassN=root->publicClassN;
		newNode.splitVertex=root->splitVertex;//叶子节点该信息很无所谓，暂时赋值了，没影响
		newNode.tupInformation=root->tupInformation;//这里有点混乱，检查一下元组的用法，在移除树的时候会受到影响，索性没有移除元组信息，但最后还是要移除，所以得改进
		
		TreeNode * temp;
		temp=root;        //记录根节点的地址
		//DeleteTree(root); //移除子树树
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
	//E3最小时，采用“嫁接”(grafting)策略，即用这个最大分枝代替t
	if (minE==E3)
	{
		//判断嫁接的是左还是右
		if (C==L&&C!=0)
		{   
			DeleteTree(root->rightChild);//先删除右子树
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

			delete temp;  //删除左孩子节点
		
		}
		if (C==R&&C!=0)
		{
			DeleteTree(root->leftChild);//先删除左子树
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

			delete temp;  //删除右孩子节点
			
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



double Tree::CalculateConfidenceLLimit(int n/*样本数*/, int e/*实验的次数*/, double p/*每次实验错误的概率*/)
{
	double C=0/*组合*/,CF=0;
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
	m_leafPreErrorsADD=0;       //初始化
	TraverseChildTree(root,0/*该参数没有用*/);//遍历叶子节点，并计算
	leafPreErrosADD=m_leafPreErrorsADD;
	return leafPreErrosADD;
}



void Tree::TraverseChildTree(TreeNode * root, double LeafPreErrorsADD/*该参数没有用*/)
{
	if (root==NULL)
	{
		return;
	}
	else
	{
		TraverseChildTree(root->leftChild,LeafPreErrorsADD);
		//访问根部节点的数据
		//.......................
		if (root->isLeaf)
		{
			//计算错误的概率
			int erros=0,covers=0;//erros被错分样本的个数，covers样本总数
			covers=root->allN;
			erros=root->errors;
			double p=erros/covers;  //错误的概率
			//计算置信上限
			double U=CalculateConfidenceLLimit(covers,erros,p);
			//计算叶节点的预测错分样本数,叶节点的预测错分样本数=到达该叶节点的样本数*该叶节点的预测错分样本率U
			double preErrors/*预测错分样本数*/=0;
			preErrors=covers*U;//U是不是该叶节点的预测错分样本率？暂且认为这样，
		
		
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
	height=ceil(k)+1;//对k上取整,求得树高

	return height;
}



double Tree::CalculatePreErrors(TreeNode* root,TuplesClassified * tuples,int tuplesN)
{
	//计算错误的概率
	int erros=0,covers=0;//erros被错分样本的个数，covers样本总数
	covers=root->allN;
	erros=CalculateErrors(root,m_sampleDatas/*样本数据*/,m_sampleDatasRow,m_sampleDatasLine,tuples,tuplesN);
	double p=erros/covers;  //错误的概率
	//计算置信上限
	double U=CalculateConfidenceLLimit(covers,erros,p);
	//计算叶节点的预测错分样本数,叶节点的预测错分样本数=到达该叶节点的样本数*该叶节点的预测错分样本率U
	double preErrors/*预测错分样本数*/=0;
	preErrors=covers*U;//U是不是该叶节点的预测错分样本率？暂且认为这样，

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
//	m_strRules=_T("");//初始化规则
//	
//	CString stRootRules=_T("");
//
//    file2.Open(_T("C:\\Data\\rules1.rules"),CFile::modeCreate|CFile::modeWrite/*|CFile::typeText*//*|CFile::typeBinary*/);
//	InorderForRules(root,0,stRootRules);
//	delete [] m_stackForNodeRules;//清栈
//	m_stackForNodeRulesTop=0;     //归零
//    file2.Close();
//	return 0;
//}



//bool Tree::InorderForRules(TreeNode* root,int tag/*记录当前点为左还是右*/,CString stRules/*记录节点之前的规则*/)
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
//		CString stSplitPoint;
//		stSplitPoint.Format(_T("%f"),splitPoint);
//		CString stSplitInex=_T("");
//		stSplitInex.Format(_T("%d"),spIndex);
//		CString stSign=_T("");             //符号
//		CString strulesf=_T("");            //当前节点规则
//        CString stRuleNew=_T("");           //新的规则链，用于下次迭代
//
//		if (tag==1)//当前节点是左节点
//		{
//			stSign=_T("<");
//		}
//		if (tag==2)
//		{
//			stSign=_T(">=");
//		}
//
//		if (root->isLeaf)                  //如果是叶子节点，考虑输出规则
//		{
//
//			CString stClassID=_T("");
//			CString st,rules,ed;
//			int clssId=root->ClassID;
//			stClassID.Format(_T("%d"),clssId);
//
//			//st=_T("classID=")+stClassID+_T("\r\n");		
//			//rules=st+stRules+stSign+_T("\"\r\n");	
//			//file2.Write(rules,2*rules.GetLength());//输出规则
//
//			st=_T("classID=")+stClassID+_T("\r\n");		
//			rules=st+stRules+stSign+_T("\"\r\n");	
//			file2.WriteString(rules);//输出规则
//
//		}
//		else
//		{		
//			
//		   strulesf=_T("att=\"")+stSplitInex+_T("\" ")+_T("cut=\"")+stSplitPoint+_T("\" ")+
//				_T("result=\"");	//提取本节点的规则信息
//		   if (tag==0)    //当前节点是根部节点
//		   {
//			   stRuleNew=strulesf;
//		   }
//		   else
//		   {
//           stRuleNew=stRules+stSign+_T("\"\r\n")+strulesf; //stRules+stSign属于父节点中提取的规则 strulesf是本节点提取的规则信息，但没有符号
//		   }
//			//if (InorderForRules(root->leftChild))//如果遍历的是左边子树
//			//{
//			//	stSign=_T("<");
//			//}
//			//if (InorderForRules(root->rightChild))//如果遍历的是右子树
//			//{
//			//	stSign=_T(">=");
//			//}	
//		}
//		InorderForRules(root->leftChild,1,stRuleNew);//遍历的是左边子树
//		InorderForRules(root->rightChild,2,stRuleNew);//遍历的右面子树
//	}
//	return true;
//}


//// 测试精度,用于获得分类后的元组集，被MeasuringAccuracy调用
//TuplesClassified* Tree::MeasuringAccuracyMatch(TuplesClassified* MeasuringTuples,int tuplesNum,TreeNode * root,CString rulesPath)
//{
//	CString FileData; //定义一个CString变量作为缓冲区
//	int line=0,row=0; //行列数
//
//	CString stOld=_T("classID="),stNew=_T("classID=");
//	int tempTupleN=tuplesNum;
//	int classId;
//	//打开文件，读取数据
//	f.Open(rulesPath,CFile::modeReadWrite/*|CFile::typeText*/);
//	//表头初始化
//	f.ReadString(FileData);//读取第一行，得到表头
//	CString temp=_T("classID=");
//	int i=temp.GetLength();
//	int j=FileData.GetLength();
//	int m=j-i;
//	CString stClassId=FileData.Right(m);
//	_stscanf(stClassId,_T( "%d"),&classId);
//	//索引初始化
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
//	while (f.ReadString(FileData))
//	{
//		if (FileData.Find(_T("classID="))>-1)//如果读到表头，对之前的分类进行处理，
//		{
//			 //对之前的分类进行处理
//			for (int i=0;i<tempN;i++)      
//             {
//				int tuplesIndex=tempcode[i];
//				MeasuringTuples[tuplesIndex].tuplesClassID=classId;
//			 }
//
//			//读出当前的表头数据，各种初始化
//			     //表头初始化
//			CString temp=_T("classID=");
//			int i=temp.GetLength();
//			int j=FileData.GetLength();
//			int m=j-i;
//		    CString stClassId=FileData.Right(m);
//			_stscanf(stClassId,_T( "%d"),&classId);
//			     //索引初始化
//			tempcode[1000];       //记录上次循环所记录的索引
//			newTempcode[1000];    //循环后所记录的索引
//			tempN=tuplesNum;      //初始的索引个数与元组个数相同
//			tempi=0;              //suoyi计数器，初始为0
//			     //初始化的索引数组记录元组按顺序的索引号
//			for (int ii=0;ii<tempN;ii++)
//			{
//				tempcode[i]=i;
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
//				 CString tempA=_T("<");
//				 CString tempB=_T(">=");
//				 //对数据分类
//				  int tmpi=0;        //计数器，最终数据被当成符合条件索引的个数
//				 if (stRuleStr==tempA)  //小于
//				 {
//					 //对索引里的所有索引进行搜索
//					 for (int i=0;i<tempN;i++)
//					 {
//						 int tuplesindex=tempcode[i];//读出索引记录的元组索引
//						 int testindex=MeasuringTuples[tuplesindex].sampleDataIndex;//通过元组的索引找出测试数据的索引
//						 if (m_testDatas[testindex][att]<cut) //如果符合条件
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
//						 if (m_testDatas[testindex][att]<cut) //如果符合条件
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
//
//
//	f.Close();
//	TuplesClassified *M=MeasuringTuples;
//
//	return M;
//}
//

// 用中序遍历树的方法测试精度，被函数MeasuringAccuracy调用
void Tree::InfoOrderForMeasuring(TreeNode* root,TuplesClassified* MeasuringTuples,int tuplesNum)
{
	//if (root==NULL)
	//{
	//	return ;
	//}
	//else
	//{

	//	//访问根部节点的数据
	//	//.......................
	//	double splitPoint=root->splitVertex;//获得分裂点
	//	int spIndex=root->attIndex;         //获得属性索引
	//	
	//	if (root->isLeaf)                  //如果是叶子节点，考虑输出规则
	//	{
	//		int clssId=root->ClassID;

	//	}
	//	else
	//	{		

	//	}
	//	}
	//	InorderForRules(root->leftChild,1,stRuleNew);//遍历的是左边子树
	//	InorderForRules(root->rightChild,2,stRuleNew);//遍历的右面子树
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


// 初始化测试数据
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


// 打印出元组信息
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


//// //打印样本数据
//void Tree::PrintSampdatas(double** S, int m , int n)
//{
//}


// 按照树的叶子打乱元组列表的顺序
void Tree::TuplesOrderByTree(TuplesClassified* tuples, int tuplesN)
{

}

//TreeNode* Tree::BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/,int & progressI/*用于计算进度*/,CString & Eror)
//{	
//	if (tuplesN==0)
//	{
//		
//		Eror=_T("错误！数据不能为空！");
//		return NULL;
//	}
//	CString timestr;
//	
//	double ** S; 
//	
//	TuplesClassified * Tindex=tuples;
//	S=new double* [tuplesN];                //申请行的空间；
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
//		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
//		Node->isLeaf=true;//是叶子节点标记之
//		Node->leftChild=NULL;
//		Node->rightChild=NULL;
//		Node->splitVertex=0;
//		//更新原始列表信息
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
//		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点
//
//		//测试代码
//		if (m_NodeSize<=1)
//		{
//
//			 Eror=_T("The Nodesize=1(Leaf)");
//			
//
//		}
//
//		progressI=progressI+Node->allN; //控制进度条
//		/*m_progressBar->SetPos(progressI);                     
//		Sleep(0.1); */
//		return Node;
//
//	}
//
//	if (classN==1)  //如果D中的元组都是同一类C
//	{
//
//		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);
//
//		Node->ClassID=L->next->classID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
//		Node->isLeaf=true;//是叶子节点标记之
//		Node->leftChild=NULL;
//		Node->rightChild=NULL;
//		Node->splitVertex=0;
//
//		//更新原始列表信息
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
//		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点
//
//		
//		if (m_NodeSize<=1)
//		{
//             Eror=_T("The Nodesize=1(Same)");
//			 	
//			
//		}
//		
//		return Node;//返回N作为叶子节点
//	}
//	
//
//	k=log(m_Dimension2*1.0)/log(2.0);
//	m_k=ceil(k)+1;
//	
//	if (m_k>=hight)//如果超过树高，
//	{
//		
//
//		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);
//
//		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
//		Node->isLeaf=true;//是叶子节点标记之
//		Node->leftChild=NULL;
//		Node->rightChild=NULL;
//		Node->splitVertex=0;
//
//		//更新原始列表信息
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
//		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点
//
//		//测试代码
//		if (m_NodeSize<=1)
//		{
//
//			 Eror=_T("The Nodesize=1(Height)");
//			 return NULL;
//		}
//		//progressI=progressI+Node->allN; //控制进度条
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
//	Node->ClassID=-1;//当节点不是叶子节点时它的类别用-1表示
//	Node->isLeaf=false;//不是叶子节点
//	Node->splitVertex=splitPointValue;//存储该属性下的分裂值
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
//	//分割数据
//	T1_index=new TuplesClassified[T_row];//申请行的空间
//	T2_index=new TuplesClassified[T_row];//申请行的空间；
//	//为分割后的数组赋值
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
//	//清除原先数据集的内存
//
//	
//	if (D1==0)  //如果Dj为空，加一个树叶到节点N，标记为D中的多数类
//	{
//	
//		//建立叶子节点
//		LNode=new TreeNode;//publicClassID
//		m_NodeSize++;      //记录节点数
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
//		//建立叶子节点
//		RNode=new TreeNode;//publicClassID
//		m_NodeSize++;      //记录节点数
//		RNode->ClassID=publicClassID;
//		RNode->isLeaf=true;
//		RNode->leftChild=NULL;
//		RNode->rightChild=NULL;
//
//		//将叶子节点接入
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
TreeNode* Tree::BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/,int & progressI/*用于计算进度*/)//使用中
{	
	if (tuplesN==0)
	{
		//MessageBox(NULL,_T("错误！数据不能为空！"),_T("注意！"),MB_OK);
		return NULL;
	}
	//CString timestr;
	
	double ** S; 
	
	TuplesClassified * Tindex=tuples;
	S=new double* [tuplesN];                //申请行的空间；

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
	
	int S_row=tuplesN;         //获得数据集的行
	int S_line=line;       //获得数据集的列

	int T_row=tuplesN;
	int **attList=attIndex; //属性集合索引
	int attN=attNum;       //属性集合的个数
	double k=0;
	TreeNode * Node;//创建一个节点作为根节点
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

	Node->allN=T_row;  //获得节点元组个数
	Node->publicClassN=publicClassN;
	Node->publicClassID=publicClassID;

	if (T_row<=m_leafsSize)     //如果数据数量小于等于6
	{
		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点

		//测试代码
		if (m_NodeSize<=1)
		{
			std::cout<<"ERROR:The Nodesize=1!Empty!"<<endl;
		}

		progressI=progressI+Node->allN; //控制进度条
		/*m_progressBar->SetPos(progressI);                     
		Sleep(0.1); */
		return Node;

	}

	if (classN==1)  //如果D中的元组都是同一类C
	{

		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=L->next->classID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//更新原始列表信息
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点

		//测试代码
		if (m_NodeSize<=1)
		{
			std::cout<<"ERROR:The Nodesize=1!Empty!"<<endl;
		
		}
		//progressI=progressI+Node->allN; //控制进度条
		//m_progressBar->SetPos(progressI);                     
		//Sleep(0.1); 
		return Node;//返回N作为叶子节点
	}
	

	k=log(m_Dimension2*1.0)/log(2.0);
	m_k=ceil(k)+1;//对k上取整,求得树高
	
	if (m_k>=hight)//如果超过树高，
	{
		

		SetValuesForTreeNode(Node,tuplesN,tuples,sampleData,line,Node->publicClassID);

		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//更新原始列表信息
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点

		//测试代码
		if (m_NodeSize<=1)
		{
			std::cout<<"ERROR:he Nodesize=1!Height!"<<endl;
		}
		//progressI=progressI+Node->allN; //控制进度条
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
	Node->ClassID=-1;//当节点不是叶子节点时它的类别用-1表示
	Node->isLeaf=false;//不是叶子节点
	Node->splitVertex=splitPointValue;//存储该属性下的分裂值
	
	//将数据分割成两部分
	int D1=0,D2=0;
	

	int **attlist1,**attlist2;
	double **S1,**S2;

	TuplesClassified *T1_index,*T2_index;
	TreeNode *LNode,*RNode;
	int s1_m=0;
	int s2_m=0;
	//分割数据
	T1_index=new TuplesClassified[T_row];//申请行的空间
	T2_index=new TuplesClassified[T_row];//申请行的空间；
	//为分割后的数组赋值
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

		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//更新原始列表信息
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点		
		return Node;

		//对属性处理

		if (tagT)
		{
			//MessageBox(NULL,_T("The data1 is wrror!"),_T("提示"),MB_OK);

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

		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//更新原始列表信息
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点


		return Node;

		if (tagT)
		{
			//MessageBox(NULL,_T("The data2 is wrror!"),_T("提示"),MB_OK);

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
	//清除原先数据集的内存



	
	if (D1==0)  //如果Dj为空，加一个树叶到节点N，标记为D中的多数类
	{
	
		//建立叶子节点
		LNode=new TreeNode;//publicClassID
		m_NodeSize++;      //记录节点数
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
		
		//建立叶子节点
		RNode=new TreeNode;//publicClassID
		m_NodeSize++;      //记录节点数
		RNode->ClassID=publicClassID;
		RNode->isLeaf=true;
		RNode->leftChild=NULL;
		RNode->rightChild=NULL;

		//将叶子节点接入
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

TreeNode* Tree::BuildTree2(double ** sampleData, int row, int line,TuplesClassified *tuples,int tuplesN,int ** attIndex, int attNum,int attL/*,TreeNode * root*/,bool outPut,int hight,TuplesClassified *tuplesOriginal/*入树的初始元组列表*/,int tuplesON/*初始元组列表个数*/)
{
	

	
	double ** S;   
	
	TuplesClassified * Tindex=tuples;
	S=new double* [tuplesN];              //申请行的空间；
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
	int S_row=tuplesN;         //获得数据集的行
	int S_line=line;       

	int T_row=tuplesN;
	int **attList=attIndex; 
	int attN=attNum;      
	double k=0;
	TreeNode * Node;//创建一个节点作为根节点
	Node=new TreeNode;
	m_NodeSize++;    
	
	AttValuesTab *D;
	
	D=new AttValuesTab[T_row];
	int temprow=0;
	
    for (int i=0;i<T_row;i++)
	{
		
		temprow=Tindex[i].sampleDataIndex;
		D[i].classID=sampleData[temprow][line-1];
		D[i].AttValue=sampleData[temprow][0];    //可以不用
		D[i].AttValue=0;                         //可以不用
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

	Node->allN=T_row;  //获得节点元组个数
	Node->publicClassN=publicClassN;
	Node->publicClassID=publicClassID;
	
	int errorcount=0;
	TuplesClassified *t; //获得元组信息
	t=new TuplesClassified[T_row];
	for (int i=0;i<T_row;i++)     //比较占内存可以考虑优化
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
		if (classid!=publicClassID)  //如果不等于多数类在叶子节点上即被错分
		{

          t[i].err=1;              //被错分err赋值1
		  errorcount++;

		}
		else
		{
		  t[i].err=0;            //分类正确确赋值为0
		}


	}
	Node->tupInformation=t;
	Node->errors=errorcount;    

	
	if (T_row<=m_leafsSize)    
	{
		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;
		//更新原始列表信息
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点

		//测试代码
		if (m_NodeSize<=1)
		{
			std::cout<<"ERROR:The Nodesize=1!Empty!"<<endl;
		
		}


		return Node;

	}

	if (classN==1)  
	{
		Node->ClassID=L->next->classID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//更新原始列表信息
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点

		//测试代码
		if (m_NodeSize<=1)
		{
			std::cout<<"ERROR:The Nodesize=1!Same!"<<endl;
		}

		return Node;//返回N作为叶子节点
	}
	

	k=log(m_Dimension2*1.0)/log(2.0);
	m_k=ceil(k)+1;//对k上取整,求得树高
	
	if (m_k>=hight)//如果超过树高，
	{
		
		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		//更新原始列表信息
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
		m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点

		//测试代码
		if (m_NodeSize<=1)
		{
			std::cout<<"ERROR:he Nodesize=1!Height!"<<endl;
		}
		return Node;
	}
	

	int splitting_criterionIndex=0;//最符合分裂的属性索引
	double splitPointValue=0;     //该属性下最佳的分裂点
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
	Node->ClassID=-1;//当节点不是叶子节点时它的类别用-1表示
	Node->isLeaf=false;//不是叶子节点
	Node->splitVertex=splitPointValue;//存储该属性下的分裂值
	
	
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
		
		 T1_index=new TuplesClassified[D1];//申请行的空间；
		
		 T2_index=new TuplesClassified[D2];//申请行的空间；
		
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
		
		T2_index=new TuplesClassified[D2];//申请行的空间；

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
		//S1_index=new int * [D1];    //申请行的空间；
		T1_index=new TuplesClassified[D1];  //申请行的空间；
	
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
	
	if (D1==0)  //如果Dj为空，加一个树叶到节点N，标记为D中的多数类
	{
	
		//建立叶子节点
		LNode=new TreeNode;//publicClassID
		m_NodeSize++;      //记录节点数
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
		
		//建立叶子节点
		RNode=new TreeNode;//publicClassID
		m_NodeSize++;      //记录节点数
		RNode->ClassID=publicClassID;
		RNode->isLeaf=true;
		RNode->leftChild=NULL;
		RNode->rightChild=NULL;
		//将叶子节点接入
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



TreeSelection* Tree::GetTreeSelection(TreeSelection* treeSelections, int treeN/*参数无用*/)
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
//		rules.Format(_T("%d"),m_ruleCount);
//	
//		def.Format(_T("%d"),root->publicClassID);//rules="17" default="2"
//		head=_T("rules=\"")+rules+_T("\" default=\"")+def+_T("\"\r\n");
//		m_ruleSet[tempruleindex]=head;
//		m_ruleCount=0;//用完归零
// 	}
//   
//	//输出规则集文件
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
//	head2=_T("entries=\"")+Entries+_T("\"\r\n");//迭代次数
//    fl.WriteString(head1);
//	fl.WriteString(head2);
//
//	for (int i=0;i<m_ruleSetI;i++)
//	{
//		temp=m_ruleSet[i];
//		fl.WriteString(temp);
//	}
//	fl.Close();
//    m_ruleSetI=0;//用完归零
//	
//	return 0;
//}


// 遍历树，获取规则集
//bool Tree::InorderForRuleSet(TreeNode* root, int tag, double erroratio,CString stRules,int rulesN)
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
//			stSign=_T("<");
//		}
//		if (tag==2)
//		{
//			stSign=_T(">=");
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
//
//
//			st=_T("count=\"")+stCount+_T("\" cover=\"")+stCover+_T("\" correct=\"")+stCorrect+_T("\" pre=\"")
//				+stPre+_T("\" class=")+stClassID+_T("\r\n");		
//			rules=st+stRules+stSign+_T("\"\r\n");	
//			//file2.WriteString(rules);//输出规则
//			m_ruleSet[m_ruleSetI]=rules;
//			m_ruleSetI++;
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
//			}
//			
//			rulesCount=rulesN+1;
//		}
//		InorderForRuleSet(root->leftChild,1,erroratio,stRuleNew,rulesCount);//遍历的是左边子树
//		InorderForRuleSet(root->rightChild,2,erroratio,stRuleNew,rulesCount);//遍历的右面子树
//	}
//	return true;
//}


// 获得构建树的参数
//void Tree::SetParameter(int treeHeight, int leafsSize, CString rulePath)
//{
//	m_treeHeight=treeHeight;
//	
//	m_leafsSize=leafsSize;
//
//	m_ruleFilePath=rulePath;
//}
//

// 开始建立树
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


// 参数归零
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


//// 控制进度条
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
	//m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点

	//判断D中的元组是否为同一类,这个方法貌似有点繁琐，
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
	//	D[i].AttValue=sampleData[temprow][0];    //可以不用
	//	D[i].AttValue=0;                         //可以不用
	//}
	//ClassUnique * L,*p;

	//L=getClassUnique(D,T_row);//获得唯一值链表,注意删除
	//p=L;
	//int classN=0;
	//classN=L->cunt;//获得数据集类别的个数

	////寻找多数类
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
	////记录多数类的个数，以及该分支训练数据的个数
	//int publicClassN=0,allN=0;
	//publicClassN=Max;
	//allN=L->classID;

	//Node->allN=T_row;  //获得节点元组个数
	//Node->publicClassN=publicClassN;
	//Node->publicClassID=publicClassID;
	////Node->errors=Node->allN-Node->publicClassN;   //这部分值得再推敲，算出的错误数与下列循环得到的错误数不一致郁闷
	int errorcount=0;
	TuplesClassified *t; //获得元组信息
	t=new TuplesClassified[T_row];
	for (int i=0;i<T_row;i++)     //比较占内存可以考虑优化
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
		if (classid!=publicClassID)  //如果不等于多数类在叶子节点上即被错分
		{

			t[i].err=1;              //被错分err赋值1
			errorcount++;

		}
		else
		{
			t[i].err=0;            //分类正确确赋值为0
		}


	}
	Node->tupInformation=t;
	Node->errors=errorcount;    
}
