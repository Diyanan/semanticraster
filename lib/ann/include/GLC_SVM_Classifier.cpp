
#include "StdAfx.h"

#include <string>
#include "GLC_SVM_Classifier.h"
#include <cmath>
#include <set>
#include "svm.h"
#include <ctime>





//struct svm_model
//{
//	svm_parameter param;	// parameter
//	int nr_class;		// number of classes, = 2 in regression/one class svm
//	int l;			// total #SV
//	svm_node **SV;		// SVs (SV[l])
//	double **sv_coef;	// coefficients for SVs in decision functions (sv_coef[k-1][l])
//	double *rho;		// constants in decision functions (rho[k*(k-1)/2])
//	double *probA;		// pariwise probability information
//	double *probB;
//
//	// for classification only
//
//	int *label;		// label of each class (label[k])
//	int *nSV;		// number of SVs for each class (nSV[k])
//	// nSV[0] + nSV[1] + ... + nSV[k-1] = l
//	// XXX
//	int free_sv;		// 1 if svm_model is created by svm_load_model
//	// 0 if svm_model is created by svm_train
//};

/*********************************************Tree_SVM********************************************************************************/




Tree_SVM::Tree_SVM(void)
	: m_Tree_SVMRoot(NULL)
	, tagT(false)
	//,m_progressBar(NULL)
{
	m_Dimension0=0;
	m_Dimension1=0;
	m_Dimension2=0;
	m_splitPointValue=0;
	m_Tree_SVMHeight=0;
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
	m_childTree_SVMHeight=0;
	m_childTree_SVMLeafs=0;
	m_childTree_SVMNode_SVMs=0;
	m_stackForNodeRulesTop=0;
	m_allDatas=0;
	m_publics=0;
	m_allErros=0;
	m_leafsSize=0;
	m_ruleSetI=0;
	m_tringDataN=0;








}


Tree_SVM::~Tree_SVM(void)
{
}
double Tree_SVM::Info(statisticsTab_SVM* staTab, int staCount)
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
double*Tree_SVM::discreteData(AttValuesTab_SVM* AttValues, int AttValuesCount)//
{
	

	int m_discretePointsCount=AttValuesCount-1;//记录离散数据可能的分裂点的个数
	//对于属性数据进行排序，从小到大
	double min=0;
	AttValuesTab_SVM temp;
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
	//计算可能的分裂点
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
double Tree_SVM::findDiscPoint(double * discPoitList, int discNum, AttValuesTab_SVM * attValueslist,int attValuesNum,bool Tag)
{
	int allValues=0;
	allValues=attValuesNum;


    double minInfo=0;
	int minIdex=0;
	double I=0;
	for (int i=0;i<discNum;i++)
	{		
		double discPoint;//分割点
		int D1=0,D2=0;//记录分割点两边各自值得个数
		discPoint=discPoitList[i];
		AttValuesTab_SVM attValTabD1[10000],attValTabD2[10000];//用于记录两个分量的元组，最大值设置成了一千，不要提前计算数组的大小，节约时间
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
		//////////////////////测试/////////
		
		
		//*************计算分量的类别属性的熵值*****************
		double E1,E2;    //熵值
		ClassUnique_SVM *L1,*r1,*L2,*r2,*REC1,*REC2;//各分量类别属性的链表,统计唯一值
		int m1=0,m2=0;             //获得各分量类别属性唯一值得个数

		L1=getClassUnique_SVM(attValTabD1,D1);
		L2=getClassUnique_SVM(attValTabD2,D2);

		REC1=L1;
		REC2=L2;

		r1=L1;
		r2=L2;

		m1=L1->cunt;
		m2=L2->cunt;
		////////////////////////////////
		/*r1=r1->next;
		int A1=r1->cunt;
		int CL1=r1->classID;*/

		////////////////////////////////
		statisticsTab_SVM *stTabD1,*stTabD2;//统计表,用于熵计算
		stTabD1=new statisticsTab_SVM[m1];
		stTabD2=new statisticsTab_SVM[m2];

		//////////////////////////////////////////////////////////////////////////


		//统计表赋值
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
		deleteClassUnique_SVM(REC1);
		deleteClassUnique_SVM(REC2);
		
		E1=Info(stTabD1,m1);
		E2=Info(stTabD2,m2);
		delete [] stTabD1;
		delete [] stTabD2;

		//******************************************************
		//***********信息增益****************************
		double P1,P2;
		
		P1=D1*1.0/allValues*1.0;
		P2=D2*1.0/allValues*1.0;
		
		InfoAttTab_SVM infAtt[2];
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
       return minInfo;//返回找到合适分割点后的属性期望信息
	}
	else
	return bestPoint;//返回合适的分割点的数值
	
}
double Tree_SVM::findDiscPoint1(double * discPoitList, int discNum, AttValuesTab_SVM * attValueslist,int attValuesNum,int **attVlueList,int attN,int attL,bool Tag)
{
	int allValues=0;
	allValues=attValuesNum;

	
    double minInfo=0;
	int minIdex=0;
	double I=0;
	for (int i=0;i<discNum;i++)
	{		
		double discPoint;//分割点
		int D1=0,D2=0;//记录分割点两边各自值得个数
		discPoint=discPoitList[i];
		AttValuesTab_SVM attValTabD1[1000],attValTabD2[1000];//用于记录两个分量的元组，最大值设置成了一千，不要提前计算数组的大小，节约时间
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
		//////////////////////测试/////////
		
		
		//*************计算分量的类别属性的熵值*****************
		double E1,E2;    //熵值
		ClassUnique_SVM *L1,*r1,*L2,*r2;//各分量类别属性的链表,统计唯一值
		int m1=0,m2=0;             //获得各分量类别属性唯一值得个数

		L1=getClassUnique_SVM(attValTabD1,D1);
		L2=getClassUnique_SVM(attValTabD2,D2);

		r1=L1;
		r2=L2;

		m1=L1->cunt;
		m2=L2->cunt;
		////////////////////////////////
		/*r1=r1->next;
		int A1=r1->cunt;
		int CL1=r1->classID;*/

		////////////////////////////////
		statisticsTab_SVM *stTabD1,*stTabD2;//统计表,用于熵计算
		stTabD1=new statisticsTab_SVM[m1];
		stTabD2=new statisticsTab_SVM[m2];

		//////////////////////////////////////////////////////////////////////////


		//统计表赋值
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
		
		InfoAttTab_SVM infAtt[2];
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
       return minInfo;//返回找到合适分割点后的属性期望信息
	}
	else
	return bestPoint;//返回合适的分割点的数值
	
}
ClassUnique_SVM * Tree_SVM::getClassUnique_SVM(AttValuesTab_SVM * attValues, int attNum)  //注意最后还要编写销毁的函数
{
	if (attNum==0)
	{
	//	MessageBox(NULL,_T("错误！数据不能为空！"),_T("注意！"),MB_OK);
	}

	int n=0;
	n=attNum;
	ClassUnique_SVM *first,*r,*s,*p;
	//初始化一个链表
	first=new ClassUnique_SVM;
	first->next=NULL;
	r=first;
	p=first;
	
	int m=0;//用于记录实际使用节点的个数

	for (int i=0;i<attNum;i++)
	{
		if (i==0)//将列表初始化
		{   
			m++;//每新建一个节点m+1
			s=new ClassUnique_SVM;
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
				if (p->classID==attValues[i].classID)//如果列表节点的ID与当前的类别ID相等
				{
					p->cunt=p->cunt+1;
					p=first->next;
					break;//计数器加1，跳出循环进行下一个类别ＩＤ的对比
				}
				if (p==r)//如果P走到了链表尾部
				{	
					if (p->classID==attValues[i].classID)//如果列表节点的ID与当前的类别ID相等
					{
						p->cunt=p->cunt+1;
						p=first->next;
						break;//计数器加1，跳出循环进行下一个类别ＩＤ的对比
					}
					m++;//每新建一个节点m+1
					s=new ClassUnique_SVM;
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
	first->cunt=m;   //头指针的count记录类别的个数
	
	first->classID=n;//头指针的classID记录所有数据的个数
	return first;
}

//删除列表
void Tree_SVM::deleteClassUnique_SVM(ClassUnique_SVM *classLick) //清除唯一值链表
{
	ClassUnique_SVM *L,*p,*q;
	L=classLick;
	p=L;
	q=L;
	while (p)
	{
		q=p;//暂存被释放节点
		p=p->next;
		delete q;
	}
}
// 计算基于按A划分对D元组分类所需要的期望信息
double Tree_SVM::InfoAtt(InfoAttTab_SVM * infoAttValues, int infoNum)
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
// 计算获得信息增益
double Tree_SVM::Gain(AttValuesTab_SVM * attValues, int attNum)
{
	AttValuesTab_SVM * values;
	values=attValues;
	int n;
	n=attNum;
	ClassUnique_SVM * classLink;
	statisticsTab_SVM * staTab;
	double inf,attInf,ginInf,ginInfRatio;
	int staNum=0;
	double * discData;

	//统计获得类别属性唯一值的链表
	classLink=getClassUnique_SVM(values,n);
	staNum=classLink->cunt;
	//获得统计表
	staTab=AttValuesStatistics(classLink);
	deleteClassUnique_SVM(classLink);
	//计算类别属性的熵
	inf=Info(staTab,staNum);
	delete [] staTab;
	//离散属性数据
	discData=discreteData(values,n);
	attInf=findDiscPoint(discData,n-1,values,n,true);
	delete[] discData;
	//dis,f.sampleRows-1,c.m_AttValuesTab_SVMle,f.sampleRows,true
	ginInf=inf-attInf;   //获得信息增益
	//ginInfRatio=ginInf/inf;//获得信息增益率

	return ginInf;
}
double Tree_SVM::Gain1(AttValuesTab_SVM * attValues, int attNum,int **attVlueList,int attN,int attL)
{
	AttValuesTab_SVM * values;
	values=attValues;
	int n;
	n=attNum;
	ClassUnique_SVM * classLink;
	statisticsTab_SVM * staTab;
	double inf,attInf,ginInf,ginInfRatio;
	int staNum=0;
	double * discData;

	//统计获得类别属性唯一值的链表
	classLink=getClassUnique_SVM(values,n);
	staNum=classLink->cunt;
	//获得统计表
	staTab=AttValuesStatistics(classLink);
	//计算类别属性的熵
	inf=Info(staTab,staNum);
	//离散属性数据
	discData=discreteData(values,n);
	attInf=findDiscPoint1(discData,n-1,values,n,attVlueList,attN,attL,true);
	//dis,f.sampleRows-1,c.m_AttValuesTab_SVMle,f.sampleRows,true
	ginInf=inf-attInf;   //获得信息增益
	//ginInfRatio=ginInf/inf;//获得信息增益率
	return ginInf;
}

// 统计得到统计表
statisticsTab_SVM* Tree_SVM::AttValuesStatistics(ClassUnique_SVM* uniqueValuesLink)
{
	ClassUnique_SVM *p,*head;
	int n;
	int tempCount=0,tempAll=0;
	head=uniqueValuesLink;
	p=head;

	n=head->cunt;
	tempAll=head->classID;
	int Li=0,Lj=0;
	statisticsTab_SVM *stTabD;//统计表,用于熵计算
	stTabD=new statisticsTab_SVM[n];
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


// 获得信息增益量
double Tree_SVM::GainRatio(AttValuesTab_SVM * attValues, int attNum)
{
	AttValuesTab_SVM * values;
	values=attValues;
	int n;
	n=attNum;
	ClassUnique_SVM * classLink;
	statisticsTab_SVM * staTab;
	double inf,attInf,ginInf,ginInfRatio;
	int staNum=0;
	double * discData;

	//统计获得类别属性唯一值的链表
	classLink=getClassUnique_SVM(values,n);
	staNum=classLink->cunt;
	//获得统计表
	staTab=AttValuesStatistics(classLink);
	//计算类别属性的熵
	inf=Info(staTab,staNum);
	//离散属性数据
	discData=discreteData(values,n);
	attInf=findDiscPoint(discData,n-1,values,n,true);
	//dis,f.sampleRows-1,c.m_AttValuesTab_SVMle,f.sampleRows,true
	ginInf=inf-attInf;   //获得信息增益
	ginInfRatio=ginInf/inf;//获得信息增益率

	delete[] classLink;
	delete[] staTab;
	return ginInfRatio;

}


// 选择分裂属性，
//sampleData不一定是整体的样本数据，但必须是划分后样本的数据，其中ClassID必须保留的字段
//attIndex不包含ClassID字段，
int Tree_SVM::ChooseAttribute(double ** sampleData, int row, int line, int * attIndex, int attNum)
{
	double ** S;
	S=sampleData;
	AttIndexTab_SVM * attIndexTab_SVM;//用于存储可供分裂属性的计算信息，
	attIndexTab_SVM=new AttIndexTab_SVM[attNum];//动态建立注意删除
	double infGainAll=0;//累计信息增益，用于计算平均值
	double averageInfGain=0; //获取增益平均值
	double minIGRatio=0;   //求最小增益率
	int minI=0;

	for (int i=0;i<attNum;i++)//attNum可供分裂属性的个数
	{
		int tempIndex=0;          //获取索引中的索引号，为了在S中取值
		tempIndex=attIndex[i];
		//AttValuesTab_SVM * attValues; //建立当前属性表，包含ClassID,动态建立注意删除
		//attValues=new AttValuesTab_SVM[row];
		AttValuesTab_SVM attValues[1000];
		double infGain=0,infGainRatio=0;
		double splitPoint=0;
		int lSplit=0,rSplit=0;

		for (int j=0;j<row;j++)  //获得当前索引下的属性值表包含ClassID
		{
			attValues[j].AttValue=S[j][tempIndex];
			attValues[j].classID=S[j][line-1];
			attValues[j].AttValueIndex=tempIndex;
		}

		//计算信息增益

		infGain=Gain(attValues,row);
		infGainRatio=GainRatio(attValues,row);


		attIndexTab_SVM[i].attIndex=tempIndex;//记录索引
		attIndexTab_SVM[i].attGain=infGain;
		attIndexTab_SVM[i].attGainRatio=infGainRatio;
		attIndexTab_SVM[i].splittPoint=m_splitPointValue;

		infGainAll=infGainAll+infGain;//累计信息增益，用于计算平均值

		//delete [] attValues;
	}
	averageInfGain=infGainAll/attNum;//获取增益平均值

	for (int i=0;i<attNum;i++)
	{
		if (i==0)
		{
			minIGRatio=attIndexTab_SVM[i].attGainRatio;
			minI=attIndexTab_SVM[i].attIndex;
		}
		else
		{
			//if (AttIndexTab_SVM[i].attGain>=averageInfGain)
			{
				if (minIGRatio<attIndexTab_SVM[i].attGainRatio)
				{
					minIGRatio=attIndexTab_SVM[i].attGainRatio;
					minI=attIndexTab_SVM[i].attIndex;
					m_splitPointValue=attIndexTab_SVM[i].splittPoint;
				}

			}
		}
	}
	int r=0;
	r=minI;
	delete [] attIndexTab_SVM;
	return r;
}

AttIndexTab_SVM Tree_SVM::ChooseAttribute1(double ** sampleData, int row, int line, int ** attIndex, int attNum,int attL)
{
	double ** S;
	S=sampleData;
	AttIndexTab_SVM * attIndexTab_SVM;//用于存储可供分裂属性的计算信息，
	attIndexTab_SVM=new AttIndexTab_SVM[attNum];//动态建立注意删除
	double infGainAll=0;//累计信息增益，用于计算平均值
	double averageInfGain=0; //获取增益平均值
	double minIGRatio=0;   //求最小增益率
	int minI=0;

	for (int i=0;i<attNum;i++)//attNum可供分裂属性的个数
	{
		int tempIndex=0;          //获取索引中的索引号，为了在S中取值
		tempIndex=attIndex[i][0];
		//AttValuesTab_SVM * attValues; //建立当前属性表，包含ClassID,动态建立注意删除
		//attValues=new AttValuesTab_SVM[row];
		AttValuesTab_SVM attValues[10000];
		double infGain=0,infGainRatio=0;
		double splitPoint=0;
		int lSplit=0,rSplit=0;

		for (int j=0;j<row;j++)  //获得当前索引下的属性值表包含ClassID
		{
			attValues[j].AttValue=S[j][tempIndex];
			attValues[j].classID=S[j][line-1];
			attValues[j].AttValueIndex=tempIndex;
		}

		//计算信息增益

		infGain=Gain(attValues,row);
		infGainRatio=GainRatio(attValues,row);
		attIndex[i][1]=m_splitPointValue;

		attIndexTab_SVM[i].attIndex=tempIndex;//记录索引
		attIndexTab_SVM[i].attGain=infGain;
		attIndexTab_SVM[i].attGainRatio=infGainRatio;
		attIndexTab_SVM[i].splittPoint=m_splitPointValue;

		infGainAll=infGainAll+infGain;//累计信息增益，用于计算平均值

		//delete [] attValues;
	}
	averageInfGain=infGainAll/attNum;//获取增益平均值

	for (int i=0;i<attNum;i++)
	{
		if (i==0)
		{
			minIGRatio=attIndexTab_SVM[i].attGainRatio;
			minI=attIndexTab_SVM[i].attIndex;
		}
		else
		{
			if (attIndexTab_SVM[i].attGain>=averageInfGain)
			{
				if (minIGRatio<attIndexTab_SVM[i].attGainRatio)
				{
					minIGRatio=attIndexTab_SVM[i].attGainRatio;
					minI=attIndexTab_SVM[i].attIndex;
		
					//m_splitPointValue=AttIndexTab_SVM[i].splittPoint;
				}

			}
		}
	}
	//最小增益里有没有相等的
	int *sameMinGRatioIndex=new int[attNum];
	int sameMinGRatioI=0;
	for (int i=0;i<attNum;i++)
	{
		if (minIGRatio==attIndexTab_SVM[i].attGainRatio)
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

	AttIndexTab_SVM r;
	r.attIndex=minI;
	r.splittPoint=attIndexTab_SVM[minI].splittPoint;

	delete [] sameMinGRatioIndex;
	delete [] attIndexTab_SVM;
	return r;
}
Tree_SVMNode_SVM* Tree_SVM::BuildTree_SVM(double ** sampleData, int row, int line, int * attIndex, int attNum/*,Tree_SVMNode_SVM * root*/,bool outPut)
{
	double ** S=sampleData;
	int S_row=row;         //获得数据集的行
    int S_line=line;       //获得数据集的列
	int *attList=attIndex; //属性集合索引
	int attN=attNum;       //属性集合的个数
	Tree_SVMNode_SVM * Node;//创建一个节点作为根节点
	Node=new Tree_SVMNode_SVM;
	////Node=root;
	/////////////////////////
	//CString s1=_T("Create a new node,");
	//CString str;
	/////////////////////
	//判断D中的元组是否为同一类,这个方法貌似有点繁琐，
	AttValuesTab_SVM *D;
	D=new AttValuesTab_SVM[row];
	for (int i=0;i<row;i++)
	{
		D[i].classID=S[i][line-1];
		D[i].AttValue=S[i][0];
		D[i].AttValue=0;
	}
	ClassUnique_SVM * L,*p;
	L=getClassUnique_SVM(D,row);//获得唯一值链表,注意删除；
	p=L;
	int n=0;
	n=L->cunt;//获得数据集类别的个数

	//寻找多数类
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
	//记录多数类的个数，以及该分支训练数据的个数
	int publicClassN=0,allN=0;
	publicClassN=Max;
	allN=L->classID;
	allN=Max;
	Node->allN=allN;
	Node->publicClassN=publicClassN;
	Node->publicClassID=publicClassID;
	/////////////////////////////////////
	////CString temp;
	//temp.Format(_T("%d"),publicClassID);
	//CString s2=_T("its public class is ")+temp;
	//CString s3=_T("# ");
	//CString s4=_T("# ");
	//CString s5=_T("# ");
	////////////////////////////////////

	//如果样本为空则结束
	if (S_row==0)
	{
		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;
//		MessageBox(_T"样本为空，生成叶子节点！");
		/////////////////////////////
	/*	if (outPut)
		{
			CString tt;
			tt.Format(_T("%d"),publicClassID);
			s3=_T("The dataset is empty,generate a leaf,and the classID=");
			str=s1+s3+tt+_T("\r\n");
			file.Write(str,2*str.GetLength());
		}*/

		//////////////////////////////
		return Node;
		
	}

	if (n==1)  //如果D中的元组都是同一类C
	{
		Node->ClassID=L->next->classID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
	    Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;
		//MessageBox(_T("D中的元组都是同一类C，生成叶子节点！"));
		//////////////////////////////////////////////////////
		/*if (outPut)
		{
			CString tt;
			tt.Format(_T("%d"),Node->ClassID);
			s4=_T("The data are the same calss,so generat a leaf!and the classID=");
			str=s1+s4+tt+_T("\r\n");
			file.Write(str,2*str.GetLength());
		}*/
		
		/////////////////////////////////////////////////////
		return Node;//返回N作为叶子节点
	}
//-------------------(4)(5)--如果没有可分裂的条件属性----------
	if (attN==1)//如果attribute_list为空，
	{
		////寻找多数类
	 //  int i=0,Max=0,publicClassID=0,Num=0;
  //     while (p)
  //     {
		//   if (i=0)
		//   {
		//	   p=p->next;
		//	   i++;
		//   }
		//   else
		//   {
		//	   Num=p->cunTt;
		//	   if (Max<Num)
		//	   {
		//		   Max=Num;
		//		   publicClassID=p->classID;
		//	   }
		//	   p=p->next;
		//   }
  //     }
	   //返回N作为节点，标记为D中的多数类
	   Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
	   Node->isLeaf=true;//是叶子节点标记之
	   Node->leftChild=NULL;
	   Node->rightChild=NULL;
	   Node->splitVertex=0;
	 //  MessageBoxA(_T"attribute_list为空，生成叶子节点！");
	   ///////////////////////////////////
	 /*  if (outPut)
	   {
		   CString tt;
		   tt.Format(_T("%d"),Node->ClassID);
		   s5=_T("The attribute_list is empty,so generat a leaf!and the classID=");
		   str=s1+s5+tt+_T("\r\n");
		   file.Write(str,2*str.GetLength());
	   }*/

	   //////////////////////////////////////////////////////////////////////////
	   return Node;
	}
	deleteClassUnique_SVM(L);//清除唯一值链表
	//----------------------(6)(7)(8)(9)找出最好的Splitting_criterion-------------------------
    int splitting_criterionIndex=0;//最符合分裂的属性索引
	double splitPointValue=0;     //该属性下最佳的分裂点
	
	splitting_criterionIndex=ChooseAttribute(S,S_row,S_line,attList,attN);
	splitPointValue=m_splitPointValue;
	//////////////////////////////////////////////
	/*CString temp1,temp2,S6=_T(" ");
	temp1.Format(_T("%d"),splitting_criterionIndex);
	temp2.Format(_T("%4lf"),splitPointValue);*/
	
	Node->attIndex=splitting_criterionIndex;
	Node->ClassID=-1;//当节点不是叶子节点时它的类别用-1表示
	Node->isLeaf=false;//不是叶子节点
	Node->splitVertex=splitPointValue;//存储该属性下的分裂值
	 //从属性索引集合中删除当前的索引
	bool t=false;
	int /**newAttList*,*/newAttN=0;
	newAttN=attN-1;
	int newAttN1=newAttN;
	int newAttN2=newAttN;
	//newAttList=new int[newAttN];
	int newAttList[100];
	for (int i=0,j=0;i<newAttN;i++,j++)
	{
		
		if (attList[i]==splitting_criterionIndex)
		{
			j=j+1;
		}
		newAttList[i]=attList[j];
	}
	//delete [] attList;//释放原来属性索引集合数组的内存
	//----------------------------------------------------------------
	//将数据分割成两部分
	int D1=0,D2=0;
	for (int i=0;i<S_row;i++)
	{
		if (S[i][splitting_criterionIndex]<splitPointValue)
		{
			D1++;
		}
		else
		{
			D2++;
		}
	}

	double **S1,**S2;
	Tree_SVMNode_SVM *LNode,*RNode;
	int s1_m=0;
	int s2_m=0;
	if (D1!=0&&D2!=0)
	{
		S1=new double* [D1];//申请行的空间；
		for (int i=0;i<D1;i++)
		{
			S1[i]=new double [S_line];
		}
		S2=new double* [D2];//申请行的空间；
		for (int i=0;i<D2;i++)
		{
			S2[i]=new double [S_line];
		}
		//为分割后的数组赋值
		for (int i=0;i<S_row;i++)
		{
			if (S[i][splitting_criterionIndex]<splitPointValue)
			{
				for (int j=0;j<S_line;j++)
				{
					S1[s1_m][j]=S[i][j];					
				}
                 s1_m++;
			}
			else
			{
				for (int j=0;j<S_line;j++)
				{
                  S2[s2_m][j]=S[i][j];				  
				}	
				s2_m++;
			}
		}
	}
	else if (D1==0&&D2>0)
	{
		S2=new double* [D2];//申请行的空间；
		for (int i=0;i<D2;i++)
		{
			S2[i]=new double [S_line];
		}
		for (int i=0;i<S_row;i++)
		{
			for (int j=0;j<S_line;j++)
			{
            S2[i][j]=S[i][j];
			}
			//s2_m++;
			
		}
		
	}
	else if(D2==0&&D1>0)
	{
		S1=new double* [D1];    //申请行的空间；
		for (int i=0;i<D1;i++)
		{
			S1[i]=new double [S_line];
		}
		for (int i=0;i<S_row;i++)
		{
			for (int j=0;j<S_line;j++)
			{
				S1[i][j]=S[i][j];
			}
			//s1_m++;
		}
	}
	else
	{

	}

	//清除原先数据集的内存
	for (int i=0;i<S_row;i++)
	{
		delete []S[i];
	}
	delete []S;
	////////////////////////////////////////
	//CString S7,tempd1,tempd2;
	//tempd1.Format(_T("%d"),D1);
	//tempd2.Format(_T("%d"),D2);
	//S7=_T("The dataset was segregated tow patrs,Part1=")+tempd1+_T(",Part2=")+tempd2;
/////////////////////////////////////
	if (D1==0)  //如果Dj为空，加一个树叶到节点N，标记为D中的多数类
	{

		//建立叶子节点
		LNode=new Tree_SVMNode_SVM;//publicClassID
		LNode->ClassID=publicClassID;
		LNode->isLeaf=true;
		LNode->leftChild=NULL;
		LNode->rightChild=NULL;
		//将叶子节点接入
		Node->leftChild=LNode;
	}
	else
	{	
		LNode=BuildTree_SVM(S1,D1,S_line,newAttList,newAttN1,outPut);
		Node->leftChild=LNode;

	}
	if (D2==0)
	{
		//建立叶子节点
		RNode=new Tree_SVMNode_SVM;//publicClassID
		RNode->ClassID=publicClassID;
		RNode->isLeaf=true;
		RNode->leftChild=NULL;
		RNode->rightChild=NULL;
		//将叶子节点接入
		Node->rightChild=RNode;
	}
	else
	{
		
		RNode=BuildTree_SVM(S2,D2,S_line,newAttList,newAttN2,outPut);
		Node->rightChild=RNode;
	}

	delete []D;
	
	return Node;
}
Tree_SVMNode_SVM* Tree_SVM::BuildTree_SVM1(double ** sampleData, int row, int line,int **sampDataIndex,int m,int n,int ** attIndex, int attNum,int attL/*,Tree_SVMNode_SVM * root*/,bool outPut,int hight)
{
	double ** S;
	int ** SIndex=sampDataIndex;      //索引，记录选择的元组
	S=new double* [m];                //申请行的空间；
	for (int i=0;i<m;i++)
	{
		S[i]=new double [line];
	}
	for (int i=0;i<m;i++)
	{
		int t=SIndex[i][0];
		for (int j=0;j<line;j++)
		{
			S[i][j]=sampleData[t][j];
		}
	}
	int S_row=m;         //获得数据集的行
	int S_line=line;       //获得数据集的列
	int SIndex_row=m;         //获得数据集索引的行
	int SIndex_line=n;       //获得数据集索引的列
	int **attList=attIndex; //属性集合索引
	int attN=attNum;       //属性集合的个数
	int attL;
	double k=0;
	Tree_SVMNode_SVM * Node;//创建一个节点作为根节点
	Node=new Tree_SVMNode_SVM;
	m_NodeSize++;    //记录节点数
	m_Dimension2=m_NodeSize-m_Dimension0;//度2节点=总数-度1节点
	//Node=root;
	/////////////////////////
	//CString s1=_T("Create a new node,");
	//CString str;
	/////////////////////
	//判断D中的元组是否为同一类,这个方法貌似有点繁琐，
	AttValuesTab_SVM *D;
	D=new AttValuesTab_SVM[SIndex_row];
	int temprow=0;
	for (int i=0;i<SIndex_row;i++)
	{
		temprow=SIndex[i][0];
		D[i].classID=sampleData[temprow][line-1];
		D[i].AttValue=sampleData[temprow][0];    //可以不用
		D[i].AttValue=0;                         //可以不用
	}
	ClassUnique_SVM * L,*p;
	L=getClassUnique_SVM(D,SIndex_row);//获得唯一值链表,注意删除；
	p=L;
	int classN=0;
	classN=L->cunt;//获得数据集类别的个数

	//寻找多数类
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
	//记录多数类的个数，以及该分支训练数据的个数
	int publicClassN=0,allN=0;
	publicClassN=Max;
	allN=L->classID;
	//allN=Max;
	//Node->allN=allN;
	Node->allN=SIndex_row;  //获得节点元组个数
	Node->publicClassN=publicClassN;
	Node->publicClassID=publicClassID;
	Node->errors=Node->allN-Node->publicClassN;
	TuplesClassified_SVM *t; //获得元组信息
	t=new TuplesClassified_SVM[SIndex_row];
	for (int i=0;i<SIndex_row;i++)     //比较占内存可以考虑优化
	{	
		int ti=0;
		ti=t[i].sampleDataIndex;
		int classid=0;
		classid=sampleData[ti][line-1];
		if (classid!=publicClassID)  //如果不等于多数类在叶子节点上即被错分
		{

          t[i].err=1;              //被错分err赋值1

		}
		else
		{
		  t[i].err=0;            //分类正确确赋值为0
		}
		t[i].tuplesClassID=publicClassID;
		t[i].sampleDataIndex=SIndex[i][0];
		//int sampindex=SIndex[i][0];
		//t[i].sampleDataIndex=sampleData[sampindex][m_sampleDatasLine-1];
	}
	Node->tupInformation=t;
	
	///////////////////////////////////////
	//CString temp;
	//temp.Format(_T("%d"),publicClassID);
	//CString s2=_T("its public class is ")+temp;
	//CString s3=_T("# ");
	//CString s4=_T("# ");
	//CString s5=_T("# ");
	//////////////////////////////////////

	//如果样本为空则结束
	if (SIndex_row<=1)     //如果数据数量小于等于6
	{
		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;

		m_Dimension0++;
		//////////////////////////////
		return Node;

	}

	if (classN==1)  //如果D中的元组都是同一类C
	{
		Node->ClassID=L->next->classID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;
	
		m_Dimension0++;
		return Node;//返回N作为叶子节点
	}
	//-------------------(4)(5)--如果超出了给定树高----------
	//计算树高
	//core=core+temp*(log(temp)/log(2.0))
	
	k=log(m_Dimension2*1.0)/log(2.0);
	m_k=ceil(k)+1;//对k上取整,求得树高
	
	if (m_k>=hight)//如果超过树高，
	{
		
		Node->ClassID=publicClassID;//以类C标记，L是头节点，不记录类别信息，所以L的下一个节点才是真正记录类别信息的节点
		Node->isLeaf=true;//是叶子节点标记之
		Node->leftChild=NULL;
		Node->rightChild=NULL;
		Node->splitVertex=0;
		
		m_Dimension0++;
		return Node;
	}
	
	deleteClassUnique_SVM(L);//清除唯一值链表
	//----------------------(6)(7)(8)(9)找出最好的Splitting_criterion-------------------------
	int splitting_criterionIndex=0;//最符合分裂的属性索引
	double splitPointValue=0;     //该属性下最佳的分裂点
	AttIndexTab_SVM att;
	att=ChooseAttribute1(S,S_row,S_line,attList,attN,2);
	//splitting_criterionIndex=ChooseAttribute(S,S_row,S_line,attList,attN);
	splitting_criterionIndex=att.attIndex;
	splitPointValue=att.splittPoint/*attList[splitting_criterionIndex][1]*/;
	for (int i=0;i<S_row;i++)
	{
		delete []S[i];
	}
	delete []S;
	////////////////////////////////////////////////
	//CString temp1,temp2,S6=_T(" ");
	//temp1.Format(_T("%d"),splitting_criterionIndex);
	//temp2.Format(_T("%4lf"),splitPointValue);
	//
	////////////////////////////////////////
	Node->attIndex=splitting_criterionIndex;
	Node->ClassID=-1;//当节点不是叶子节点时它的类别用-1表示
	Node->isLeaf=false;//不是叶子节点
	Node->splitVertex=splitPointValue;//存储该属性下的分裂值
	//从属性索引集合中删除当前的索引
	//bool t=false;
	//----------------------------------------------------------------
	//将数据分割成两部分
	int D1=0,D2=0;
	for (int i=0;i<SIndex_row;i++)
	{
		int temprow=SIndex[i][0];
		if (sampleData[temprow][splitting_criterionIndex]<=splitPointValue)
		{
			D1++;
		}
		else
		{
			D2++;
		}
	}

	double **S1,**S2;
	int **S1_index,**S2_index;
	Tree_SVMNode_SVM *LNode,*RNode;
	int s1_m=0;
	int s2_m=0;
	if (D1!=0&&D2!=0)
	{
		S1_index=new int* [D1];//申请行的空间；
		for (int i=0;i<D1;i++)
		{
			S1_index[i]=new int [n];
		}
		S2_index=new int* [D2];//申请行的空间；
		for (int i=0;i<D2;i++)
		{
			S2_index[i]=new int [n];
		}
		//为分割后的数组赋值
		for (int i=0;i<SIndex_row;i++)
		{
			int temprow=SIndex[i][0];
			if (sampleData[temprow][splitting_criterionIndex]<=splitPointValue)
			{
				for (int j=0;j<SIndex_line;j++)
				{
					S1_index[s1_m][j]=SIndex[i][j];					
				}
				s1_m++;
			}
			else
			{
				for (int j=0;j<SIndex_line;j++)
				{
					S2_index[s2_m][j]=SIndex[i][j];				  
				}	
				s2_m++;
			}
		}
	}
	else if (D1==0&&D2>0)
	{
		S2_index=new int* [D2];//申请行的空间；
		for (int i=0;i<D2;i++)
		{
			S2_index[i]=new int [SIndex_line];
		}
		for (int i=0;i<SIndex_row;i++)
		{
			for (int j=0;j<SIndex_line;j++)
			{
				S2_index[i][j]=SIndex[i][j];
			}

		}

	}
	else if(D2==0&&D1>0)
	{
		S1_index=new int * [D1];    //申请行的空间；
		for (int i=0;i<D1;i++)
		{
			S1_index[i]=new int [SIndex_line];
		}
		for (int i=0;i<SIndex_row;i++)
		{
			for (int j=0;j<SIndex_line;j++)
			{
				S1_index[i][j]=SIndex[i][j];
			}
		}
	}
	else
	{
	}
	//清除原先数据集的内存
	for (int i=0;i<SIndex_row;i++)
	{
		delete []SIndex[i];
	}
	delete []SIndex;
	//////////////////////////////////////
	//CString S7,tempd1,tempd2;
	//tempd1.Format(_T("%d"),D1);
	//tempd2.Format(_T("%d"),D2);
	//S7=_T("The dataset was segregated tow patrs,Part1=")+tempd1+_T(",Part2=")+tempd2;
	/////////////////////////////////////
	if (D1==0)  //如果Dj为空，加一个树叶到节点N，标记为D中的多数类
	{

		LNode=new Tree_SVMNode_SVM;//publicClassID
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
		LNode=BuildTree_SVM1(sampleData,row,line,S1_index,D1,SIndex_line,attList,attN,2,outPut,hight);
		Node->leftChild=LNode;

	
		
	}
	if (D2==0)
	{
	
		//建立叶子节点
		RNode=new Tree_SVMNode_SVM;//publicClassID
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

		RNode=BuildTree_SVM1(sampleData,row,line,S2_index,D2,SIndex_line,attList,attN,2,outPut,hight);
		Node->rightChild=RNode;
		
	}
	
	delete []D;
	
	m_Tree_SVMHeight=m_k;
	return Node;
}


// 中序递归遍历
void Tree_SVM::InOrder(Tree_SVMNode_SVM * root)
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
			m_childTree_SVMLeafs++;
			m_allDatas=m_allDatas+root->allN;
		}
		m_childTree_SVMNode_SVMs++;
		InOrder(root->rightChild);
	}
}


// 移除树，
void Tree_SVM::RemoveTree_SVM(Tree_SVMNode_SVM* root)
{
	if (root!=NULL)
	{
		RemoveTree_SVM(root->leftChild);
		RemoveTree_SVM(root->rightChild);
		delete root;
	}
}


// 判断剪枝，输入节点进行判断
bool Tree_SVM::PostPrune(Tree_SVMNode_SVM* iNode)
{
	if (iNode->isLeaf)
	{
		return false;
	}
	Tree_SVMNode_SVM *L,*R;
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
			Tree_SVMNode_SVM * newNode;
			newNode=new Tree_SVMNode_SVM;
			newNode->isLeaf=true;
			//原节点的普遍结果即最后的结果属性
			newNode->ClassID=iNode->publicClassID;
			newNode->isLeaf=true;
			newNode->leftChild=NULL;
			newNode->rightChild=NULL;
		//	RemoveTree_SVM(iNode);
			delete iNode;
			iNode=newNode;
			return true;
		}
		return false;
	}
	return false;
}


// 层序遍历
void Tree_SVM::LeverOrder(Tree_SVMNode_SVM* root)
{
	//int front=0;
	//int rear=0;
	//if (root==NULL)
	//{
	//	return;
	//}
	//Tree_SVMNode_SVM* Q[1000];
	//Tree_SVMNode_SVM* q;
	//Q[++rear]=root;
	////CString classID,index,point,row;
	////CString att1,att2,att3;
	//double DPoint=0;
	//int ID,I;
	////CStdioFile f;
	////CFile f;
	////f.Open(_T("C:\\Data\\zyy.txt"),CFile::modeCreate|CFile::modeWrite/*|CFile::typeText*/);
	//while(front!=rear)
	//{
	//	q=Q[++front];
	//	//..................
	//		
	//	if (q->isLeaf)//如果是叶子节点
	//	{

	//		ID=q->ClassID;
	//		I=q->attIndex;
	//		DPoint=q->splitVertex;
	//		classID.Format(_T("%d"),ID);
	//		index.Format(_T("%d"),I);
	//		point.Format(_T("%.1lf"),DPoint);
	//		att1=_T("Leaf,ID=");
	//		att2=_T(",index=NULL");
	//		att3=_T(",point=NULL");
	//		row=att1+classID+att2+att3+_T("\r\n");		
	//		f.Write(row,2*row.GetAllocLength());
	//	}
	//	else
	//	{
	//		ID=q->ClassID;
	//		I=q->attIndex;
	//		DPoint=q->splitVertex;
	//		classID.Format(_T("%d"),ID);
	//		index.Format(_T("%d"),I);
	//		point.Format(_T("%.4lf"),DPoint);
	//		att1=_T("Node,ID=");
	//		att2=_T(",Index=");
	//		att3=_T(",Point=");
	//		row=att1+classID+att2+index+att3+point+_T("\r\n");
	//		f.Write(row,2*row.GetAllocLength());

	//	}
	//	
	//	if (q->leftChild!=NULL)
	//	{
	//		Q[++rear]=q->leftChild;
	//	}
	//	if (q->rightChild!=NULL)
	//	{
	//		Q[++rear]=q->rightChild;
	//	}
	//};
	///*f.Close();*/
}
// 计算被错分的个数
int Tree_SVM::CalculateErrors(Tree_SVMNode_SVM* root, double ** SampleDatas, int m , int n,TuplesClassified_SVM* tuples,int tuplesN)
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
  // Traverse(root,SampleDatas,m,n,tuples,tuplesN,false);
	
//	m_erroNum=0;  //用完归零m_tuplesI=0;	
	return erros;
}


// 输入树的根节点，分类后元组的总数，输出一个元组数组，目的是为了与样本数据做对比
TuplesClassified_SVM * Tree_SVM::GetClassifiedTuples(Tree_SVMNode_SVM * root, int m)
{
	return NULL;
}


// 输入一个数组，将叶子节点的分类信息填入其中
void Tree_SVM::traverse(Tree_SVMNode_SVM* root, TuplesClassified_SVM* T, int i)
{
}


// 遍历数使用中序遍历方法，用于计算错误率
void Tree_SVM::Traverse(Tree_SVMNode_SVM* root,double ** SampleDatas, int m , int n,TuplesClassified_SVM * tuples,int tuplesN,bool isReplaceTuples/*是否更新元组信息，不用于计算错误率*/)
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
			TuplesClassified_SVM *T;
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
			TuplesClassified_SVM *T;
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


// 获得树高值
void Tree_SVM::getTree_SVMHeight(int h)
{
	m_Tree_SVMHeight=h;
}


// 计算模型的误差率
double Tree_SVM::ErroRatioForModel(TuplesClassified_SVM * tuplesInfmation, int tuplesNum)
{
	double errorM=0,temp;
	for (int i=0;i<tuplesNum;i++)
	{
		temp=tuplesInfmation[i].weight*tuplesInfmation[i].err;
		errorM=errorM+temp;
	}
	return errorM;
}


// 初始化记录元组信息的数组,用于第一次建立树之前，
void Tree_SVM::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
{
	
	m_tuplesInformation=new TuplesClassified_SVM[tuplesNum];
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


// 清除记录元组信息的数组，用于最后清理内存
void Tree_SVM::TuplesInformationDelete(TuplesClassified_SVM *tuples)
{
	delete [] tuples;
}






// 获得整个样本数据
double** Tree_SVM::GetSampleDatas(double** sampledates, int m , int n)
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


// 获得样本数据索引集
int** Tree_SVM::GetSampleDataIndex(TuplesClassified_SVM* tuples,int tuplesN)
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


// 删除样本索引集
void Tree_SVM::DeleteSampleDataIndex(TuplesClassified_SVM* tuples)
{
	delete [] tuples;
}


// 获得属性集,attD表示属性集的维数

int ** Tree_SVM::GetAttIndexList(int attNum, int attD/*默认2*/)
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


// 删除属性集
void Tree_SVM::DeletAttIndexList(int ** attIndexList, int attM , int attN)
{
}

//double ** sampleData, int row, int line,int **sampDataIndex,int m,int n,int ** attIndex, int attNum,int attL/*,Tree_SVMNode_SVM * root*/,bool outPut,int hight
// 初始化样本数据，保存数据，属性集，等信息
void Tree_SVM::SampleDataInitialization(double ** sampleData, int row, int line)
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
	// Tree_SVM::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
}


// 轮盘赌函数，参见一些遗传算法的资料,被SamplingReplacement（）调用
int Tree_SVM::GetChromoRoulette(TuplesClassified_SVM* tuples, int tuplesN)
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


// 删除新建的样本数据集
void Tree_SVM::DeleteSamplingReplacement(TuplesClassified_SVM* tuples)
{
	delete [] tuples;
}





// 对决策树EBP剪枝
bool Tree_SVM::PostPruneEBP(Tree_SVMNode_SVM* root,TuplesClassified_SVM *tuples,int tuplesN)
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
	int L=GetChildTree_SVMHeight(root->leftChild); //计算左子树树高
	int R=GetChildTree_SVMHeight(root->rightChild); //判断右面子树树高
	int C=0;
	if (L!=R)
	{
		C=(L>R ? L:R);
	}
	    //计算获得E3
	double E3=0;
	Tree_SVMNode_SVM * grafting;
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
		Tree_SVMNode_SVM  newNode;
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
		
		Tree_SVMNode_SVM * temp;
		temp=root;        //记录根节点的地址
		//DeleteTree_SVM(root); //移除子树树
		DeleteTree_SVM(root->leftChild);
		DeleteTree_SVM(root->rightChild);
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
			DeleteTree_SVM(root->rightChild);//先删除右子树
			Tree_SVMNode_SVM * temp;
		    temp=root->leftChild;
			Tree_SVMNode_SVM replaceNode;
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
			DeleteTree_SVM(root->leftChild);//先删除左子树
			Tree_SVMNode_SVM * temp;
			temp=root->rightChild;
			Tree_SVMNode_SVM replaceNode;
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


// 计算阶乘
int Tree_SVM::CalculateFactorial(int n)
{
	int f=1;
	for (int i=n;i>0;i--)
	{
		f=f*i;
	}
	return f;
}


// 计算置信上限
double Tree_SVM::CalculateConfidenceLLimit(int n/*样本数*/, int e/*实验的次数*/, double p/*每次实验错误的概率*/)
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


// 计算子树t下叶节点的预测错误样本率之和,被PostPruneEBP调用
double Tree_SVM::ClaculateLeafPreErrorsADD(Tree_SVMNode_SVM* root)
{
    double leafPreErrosADD;     //
	m_leafPreErrorsADD=0;       //初始化
	TraverseChildTree_SVM(root,0/*该参数没有用*/);//遍历叶子节点，并计算
	leafPreErrosADD=m_leafPreErrorsADD;
	return leafPreErrosADD;
}


// 遍历子树叶子节点,被ClaculateLeafPreErrorsADD调用
void Tree_SVM::TraverseChildTree_SVM(Tree_SVMNode_SVM * root, double LeafPreErrorsADD/*该参数没有用*/)
{
	if (root==NULL)
	{
		return;
	}
	else
	{
		TraverseChildTree_SVM(root->leftChild,LeafPreErrorsADD);
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
		TraverseChildTree_SVM(root->rightChild,LeafPreErrorsADD);
	}
}


// 计算子树树高
int Tree_SVM::GetChildTree_SVMHeight(Tree_SVMNode_SVM* root)
{
	m_childTree_SVMLeafs=0;
	m_childTree_SVMNode_SVMs=0;
	InOrder(root);
	int nodes=m_childTree_SVMNode_SVMs;
	int leafs=m_childTree_SVMLeafs;
	double k,height;
	int nodes2=nodes-leafs;
	k=log(nodes2*1.0)/log(2.0);
	height=ceil(k)+1;//对k上取整,求得树高

	return height;
}


// 获得预测错分样本数
double Tree_SVM::CalculatePreErrors(Tree_SVMNode_SVM* root,TuplesClassified_SVM * tuples,int tuplesN)
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


// 删除树结构
void Tree_SVM::DeleteTree_SVM(Tree_SVMNode_SVM* root)
{
	if (root!=NULL)
	{
		DeleteTree_SVM(root->leftChild);
		DeleteTree_SVM(root->rightChild);
		TuplesClassified_SVM * temp;
		temp=root->tupInformation;
		//delete  temp;
		delete root;
		
	}
}


// 获得分类规则
int Tree_SVM::GetClassRules(Tree_SVMNode_SVM* root)
{

	//m_strRules=_T("");//初始化规则
	////通过树高计算栈的大概空间
	////int nodes=pow(2,m_Tree_SVMHeight);
	////m_stackForNodeRules=new CString[nodes];
	////m_stackForNodeRulesTop=0;  //初始化栈顶
	//CString stRootRules=_T("");

 //   file2.Open(_T("C:\\Data\\rules1.rules"),CFile::modeCreate|CFile::modeWrite/*|CFile::typeText*//*|CFile::typeBinary*/);
	//InorderForRules(root,0,stRootRules);
	//delete [] m_stackForNodeRules;//清栈
	//m_stackForNodeRulesTop=0;     //归零
 //   file2.Close();
	return 0;
}






// 用中序遍历树的方法测试精度，被函数MeasuringAccuracy调用
void Tree_SVM::InfoOrderForMeasuring(Tree_SVMNode_SVM* root,TuplesClassified_SVM* MeasuringTuples,int tuplesNum)
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





// 初始化测试数据
void Tree_SVM::TestDataInitialization(double** testData , int row , int line)
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









// //打印样本数据
void Tree_SVM::PrintSampdatas(double** S, int m , int n)
{
}


// 按照树的叶子打乱元组列表的顺序
void Tree_SVM::TuplesOrderByTree_SVM(TuplesClassified_SVM* tuples, int tuplesN)
{

}




// 通过元组列表打印样本数据
void Tree_SVM::PrintSampleDataByTuples(TuplesClassified_SVM* tuples, int tuplesN)
{
	//CStdioFile fi;
	//fi.Open(path,CFile::modeCreate|CFile::modeWrite);
	//int sindex=0;
	//int classid=0;
	//int err=0;
	//double w=0;
	//double values=0;
	//CString stSindex,st=_T(""),ed=_T("\r\n"),stvalues,temp		;
	//TuplesClassified_SVM * Di=tuples;



	//for (int ii=0;ii<tuplesN;ii++)
	//{
	//	st=_T("");
	//	sindex=Di[ii].sampleDataIndex;
	//	for (int jj=0;jj<line;jj++)
	//	{
	//		values=sampdata[sindex][jj];

	//		if (jj!=line-1)
	//		{
	//			stvalues.Format(_T("%fl"),values);
	//			temp=stvalues+_T(",");
	//			st=st+temp;
	//		}
	//		else
	//		{
	//			stvalues.Format(_T("%d"),(int)values);
	//			temp=stvalues;
	//			st=st+temp;
	//		}		
	//	}
	//	st=st+ed;
	//	fi.WriteString(st);

	//}

	////TestDataInitialization(T,tuplesN,line);
	//fi.Close();
}


// 获得决策树模型集
Tree_SVMSelection_SVM* Tree_SVM::GetTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs, int Tree_SVMN/*参数无用*/)
{
	Tree_SVMSelection_SVM* selected;
	selected=m_Tree_SVMSelection_SVM;
	return selected;
}


// 删除树模型选择集
void Tree_SVM::DeleteTree_SVMSelection_SVM(Tree_SVMSelection_SVM* Tree_SVMSelection_SVMs)
{
	delete [] Tree_SVMSelection_SVMs;
}











//
//// 开始建立树
//void Tree_SVM::doBuildTree_SVM(TuplesClassified_SVM *tuples,int tuplesN,int & progressI)
//{
//    m_Tree_SVMRoot=BuildTree_SVM2(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine,tuples,tuplesN,m_attListIndex,m_sampleDatasLine-1,2,true,m_Tree_SVMHeight,tuples,tuplesN,progressI);
//}
//void Tree_SVM::doBuildTree_SVM(TuplesClassified_SVM *tuples,int tuplesN)
//{
//	m_Tree_SVMRoot=BuildTree_SVM2(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine,tuples,tuplesN,m_attListIndex,m_sampleDatasLine-1,2,true,m_Tree_SVMHeight,tuples,tuplesN);
//}

//// 获得树的根节点
//Tree_SVMNode_SVM* Tree_SVM::GetTree_SVMRoot(void)
//{
//	Tree_SVMNode_SVM* t=m_Tree_SVMRoot;
//	return t;
//}
void Tree_SVM::GetData(double ** sampleData, int row, int line,int ** attIndex)
{
	m_sampleDatas=sampleData;
	m_sampleDatasRow=row;
	m_sampleDatasLine=line;
	m_attListIndex=attIndex;
	
}


// 参数归零
void Tree_SVM::doVariableToZero(void)
{
	 m_Tree_SVMRoot=NULL;
	 tagT=false;
		m_Dimension0=0;
		m_Dimension1=0;
		m_Dimension2=0;
		m_splitPointValue=0;
		m_Tree_SVMHeight=0;
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
		m_childTree_SVMHeight=0;
		m_childTree_SVMLeafs=0;
		m_childTree_SVMNode_SVMs=0;
		m_stackForNodeRulesTop=0;
		m_allDatas=0;
		m_publics=0;
		m_allErros=0;
		m_leafsSize=0;
		m_ruleSetI=0;
		m_tringDataN=0;
}


//// 控制进度条
//void Tree_SVM::SetProgressBar(CProgressCtrl*  progressBar)
//{
//	m_progressBar=progressBar;
//
//}


// 为节点赋值
void Tree_SVM::SetValuesForTree_SVMNode_SVM(Tree_SVMNode_SVM* treeNode,int tuplesN,TuplesClassified_SVM *tuples,double ** sampleData,int sampleData_line,int publicClassID)
{
	//int line=sampleData_line;
	//TuplesClassified_SVM * Tindex=tuples;
	//int T_row=tuplesN;
	//Tree_SVMNode_SVM * nod=Tree_SVMNode_SVM;

	int line=sampleData_line;
	TuplesClassified_SVM * Tindex=tuples;
	int T_row=tuplesN;
	Tree_SVMNode_SVM  * Node=treeNode;
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
	TuplesClassified_SVM *t; //获得元组信息
	t=new TuplesClassified_SVM[T_row];
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



/*********************************************AdaBoosting_SVM_adsvm********************************************************************************/






AdaBoosting_SVM_adsvm::AdaBoosting_SVM_adsvm(void)
	: m_Tree_SVMHeight(0)
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

//	, m_strRules(_T(""))
	//, m_stackForNodeRules(NULL)
	, m_stackForNodeRulesTop(0)
	, m_measuringTuples(NULL)
	, m_allDatas(0)
	, m_publics(0)
	, m_allErros(0)
	, m_leafsSize(0)
	, m_Tree_SVMSelection_SVM(NULL)
	, m_randomNum(0)
	, m_random(NULL)
	, m_sampleDataRatio(0)
	, m_ruleCount(0)
	//, m_ruleSet(NULL)
	, m_ruleSetI(0)
	//, m_ruleFilePath(_T(""))
	, m_tringDataN(0)
	, m_quede(NULL)
//	, progressBar(NULL)
	//, m_progressBar(NULL)
//	, m_progressI(0)
, m_target_classsId(NULL)
, m_SVMSelection(NULL)
{
}


AdaBoosting_SVM_adsvm::~AdaBoosting_SVM_adsvm(void)
{
}
TuplesClassified_SVM* AdaBoosting_SVM_adsvm::SamplingReplacement(TuplesClassified_SVM* tuples, int tuplesN)
{
	//生成新的元组数组，用于记录新生成的样数据
	srand((int)time(NULL));           //随机数的种子
	TuplesClassified_SVM* tempTuples;
	tempTuples=new TuplesClassified_SVM[tuplesN];
	int tempIndex=0;
	for (int i=0;i<tuplesN;i++)
	{
		
		tempIndex=GetChromoRoulette(tuples,tuplesN);
		tempTuples[i].i=tuples[tempIndex].i;
		tempTuples[i].weight=tuples[tempIndex].weight;
		tempTuples[i].err=tuples[tempIndex].err;
		tempTuples[i].sampleDataIndex=tuples[tempIndex].sampleDataIndex;
		tempTuples[i].tuplesClassID=tuples[tempIndex].tuplesClassID;
		/*CString st,str;

		st.Format(_T("%d"),tempIndex);
		str=st+_T(",");
		file1.Write(str,2*str.GetLength());

		 CString x=_T("\r\n");
	    file1.Write(x,2*x.GetLength());*/
		//}
	}
	return tempTuples;
}
TuplesClassified_SVM* AdaBoosting_SVM_adsvm::SamplingReplacement_SVM(TuplesClassified_SVM* tuples, int tuplesN,double *targetclassId)
{
	//生成新的元组数组，用于记录新生成的样数据
	srand((int)time(NULL));           //随机数的种子
	TuplesClassified_SVM* tempTuples;
	tempTuples=new TuplesClassified_SVM[tuplesN];
	int tempIndex=0;
	for (int i=0;i<tuplesN;i++)
	{
		
		tempIndex=GetChromoRoulette(tuples,tuplesN);
		tempTuples[i].i=tuples[tempIndex].i;
		tempTuples[i].weight=tuples[tempIndex].weight;
		tempTuples[i].err=tuples[tempIndex].err;
		int sampledataindex;
		tempTuples[i].sampleDataIndex=sampledataindex=tuples[tempIndex].sampleDataIndex;
		tempTuples[i].tuplesClassID=tuples[tempIndex].tuplesClassID;
		tempTuples[i].data=m_sampleDatas[sampledataindex];//将元组与数据关联
		targetclassId[i]=tempTuples[i].tuplesClassID;
	}
	return tempTuples;
}
int AdaBoosting_SVM_adsvm::CalculateErrors(Tree_SVMNode_SVM* root, double ** SampleDatas, int m , int n,TuplesClassified_SVM* tuples,int tuplesN)
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
	// Traverse(root,SampleDatas,m,n,tuples,tuplesN,false);

	//	m_erroNum=0;  //用完归零m_tuplesI=0;	
	return erros;
}
int AdaBoosting_SVM_adsvm::CalculateErrors_SVM(svm_model* model,svm_problem* prob,double * targetClassid)
{
	int erros=0;
	int N=prob->l;
	svm_node ** nodes=prob->x;
	for (int i=0;i<N;i++)
	{
		svm_node * nod=nodes[i];
		double temp=targetClassid[i];
		double rpeClassid=svm_predict(model,nod);
		if (temp!=rpeClassid)
		{
			erros++;
		}

	}


	return erros;
}
double AdaBoosting_SVM_adsvm::ErroRatioForModel(TuplesClassified_SVM * tuplesInfmation, int tuplesNum)
{
	double errorM=0,temp;
	for (int i=0;i<tuplesNum;i++)
	{
		temp=tuplesInfmation[i].weight*tuplesInfmation[i].err;
		errorM=errorM+temp;
	}
	return errorM;
}
double AdaBoosting_SVM_adsvm::ErroRatioForModel_SVM(TuplesClassified_SVM * tuplesInfmation, int tuplesNum)
{
	//做二折交叉
	double errorM=0,temp;
	for (int i=0;i<tuplesNum;i++)
	{
		temp=tuplesInfmation[i].weight*tuplesInfmation[i].err;
		errorM=errorM+temp;
	}
	return errorM;
}
double ** getdatabytuples_SVM(TuplesClassified_SVM *tuples,int row,int* radomIndex,int radomN)
{

	double ** sampledata;               //建立属性集
	sampledata=new double* [radomN];
	int radom_i=0;
	for (int i=0;i<row;i++)
	{
		if (i==radomIndex[radom_i])
		{
			sampledata[radom_i]=tuples[i].data;
		    tuples[i].data=NULL;        
			radom_i++;
		}
		   //用完断开
	}
	return  sampledata;
	/////////////////////////////////////////////////////
	//double ** sampledata;               //建立属性集
	//sampledata=new double* [row];  
	//for (int i=0;i<row;i++)
	//{
	//	sampledata[i]=tuples[i].data;
	//	tuples[i].data=NULL;           //用完断开
	//}
	//return  sampledata;
	
}
//不重复随机数
int * AdaBoosting_SVM_adsvm::GetNonredundantRandom(int leftLimit, int rightLimit,int randomN)
{
	int * r;
	r=new int[randomN];
	int *m_random=new int[randomN];
	int m_randomNum=randomN;
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
void AdaBoosting_SVM_adsvm::doAdaBoosting(TuplesClassified_SVM * tuples, int tuplesN, int k,int & progressI)
{
//	srand((int)time(NULL));           //随机数的种子
//	TuplesClassified_SVM **tuplesArr;       //用于记录每次分类之后的元组
//	tuplesArr=new TuplesClassified_SVM *[k];
//	Tree_SVMSelection_SVM *qualifiedTree_SVMs;         //用于记录复合条件的树
//	qualifiedTree_SVMs=new Tree_SVMSelection_SVM [k]; 
//
//
//	int txtnum=0;
//	/*CString a=_T("C:\\Data\\D"),b,c=_T(".txt");
//	CString a1=_T("C:\\Data\\m_errorsize.txt");
//	b.Format(_T("%d"),txtnum);*/
//	/*CStdioFile caoF;
//	caoF.Open(a1,CFile::modeCreate|CFile::modeWrite);*/
//	//初始元组列表
////	m_tuplesInformation=tuples;
//	//训练复合模型
//	for (int i=0;i<k;i++)
//	{
//
//		/*b.Format(_T("%d"),txtnum);
//		CString st=a+b+c;	 
//		txtnum++;*/
//
//	  Tree_SVM singalTree_SVM;       //单独建立一棵树
// 	  TuplesClassified_SVM * Di;	
//	 if(i==0)           //如果是第一次循环使用初始化的元组信息表
//	 {
//		 Di=new TuplesClassified_SVM[tuplesN];
//		 for (int ii=0;ii<tuplesN;ii++)
//		 {
//			 Di[ii].err=m_tuplesInformation[ii].err;
//			 Di[ii].i=m_tuplesInformation[ii].i;
//			 Di[ii].sampleDataIndex=m_tuplesInformation[ii].sampleDataIndex;
//			 Di[ii].tuplesClassID=m_tuplesInformation[ii].tuplesClassID;
//			 Di[ii].weight=m_tuplesInformation[ii].weight;
//		 }		
//		
//	 }
//	 else
//	 {
//		 //根据元组的权重，从D中有放回采样，得到Di
//        Di=SamplingReplacement(m_tuplesInformation/*初始样本元组*/,tuplesN);//有放回抽样，返回样本	
//	 }
//	/* CString sapt=_T("C:\\Data\\testSampdata")+b+_T(".txt");
//	 PrintSampleDataByTuples(Di,tuplesN,sapt);*/
//	  double ** sampleData;                  //获得样本数据
//	 //sampleData=singalTree_SVM.GetSampleDatas(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine);
//	  sampleData=singalTree_SVM.GetSampleDatas(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine);
//
//	  
//	 int ** sampleDataIndex;                //获得样本数据集索引
//	// sampleDataIndex=singalTree_SVM.GetSampleDataIndex(Di,tuplesN);//m_tuplesInformation
//	 sampleDataIndex=singalTree_SVM.GetSampleDataIndex(Di,tuplesN);//m_tuplesInformation
//
//	 int ** attIndexList=m_attListIndex;                   //获得属性集
//	 //attIndexList=singalTree_SVM.GetAttIndexList(tuplesN,2);
//	                                        //初始化一些参数
//	/* PrintTuples(Di,tuplesN,st);*/
//	 m_Dimension2=0;m_k=0; m_NodeSize=0;m_Dimension0=0;
//	 m_publics=0;m_tuplesI=0; m_allErros=0;m_erroNum=0;
//	                                        //建立树
//	// singalTree_SVM.SetProgressBar(m_progressBar);//设置进度条控制
//	 singalTree_SVM.GetData(sampleData,m_sampleDatasRow,m_sampleDatasLine,attIndexList);
//	// singalTree_SVM.GetControlsParameter(m_Tree_SVMHeight,m_adaBoostingK,m_leafsSize,m_sampleDataRatio,m_ruleFilePath);
//  	 singalTree_SVM.doBuildTree_SVM(Di,tuplesN,progressI);
//	 m_NodeSize=singalTree_SVM.m_NodeSize;     //记录节点数
//	/* if (m_NodeSize==0)
//	 {
//
//		 MessageBox(NULL,_T("The Nodesize=0!FUCK outFront!"),_T("提示"),MB_OK);
//	 }*/
//	 Tree_SVMNode_SVM* diTree_SVM=singalTree_SVM.GetTree_SVMRoot();
//	 /*if (m_NodeSize==0)
//	 {
//
//		 MessageBox(NULL,_T("The Nodesize=0!FUCK outBack!"),_T("提示"),MB_OK);
//	 }*/
//	
//	
//	 int errosNum=CalculateErrors(diTree_SVM/*数的根节点*/,sampleData,m_sampleDatasRow,m_sampleDatasLine,Di,tuplesN);   //计算错分的个数；
//	 double errorRatio=ErroRatioForModel(/*m_tuplesInformation*/Di,m_tuplesNum);                                    //计算错误率
//	 while(errorRatio>0.5||errorRatio==0)
//	 {
//    singalTree_SVM.DeleteTree_SVM(diTree_SVM);//删除不合格的树；
//	TuplesWeightInitialization(m_tuplesInformation,tuplesN);//重新将权重初始化为1/d
//	TuplesInformationDelete(Di);//原先产生的样本无效删了去
//	 if(i==0)           //如果是第一次循环使用初始化的元组信息表
//	 {
//		 Di=new TuplesClassified_SVM[tuplesN];
//		 for (int ii=0;ii<tuplesN;ii++)
//		 {
//			 Di[ii].err=m_tuplesInformation[ii].err;
//			 Di[ii].i=m_tuplesInformation[ii].i;
//			 Di[ii].sampleDataIndex=m_tuplesInformation[ii].sampleDataIndex;
//			 Di[ii].tuplesClassID=m_tuplesInformation[ii].tuplesClassID;
//			 Di[ii].weight=m_tuplesInformation[ii].weight;
//		 }  	
//	 }
//	 else
//	 {
//		 //根据元组的权重，从D中有放回采样，得到Di
//        Di=SamplingReplacement(m_tuplesInformation/*初始样本元组*/,tuplesN);//有放回抽样，返回样本
//		
//	 }
//	 singalTree_SVM.doVariableToZero();          //参数归零
//	 double ** sampleData;                  //获得样本数据
//	 sampleData=singalTree_SVM.GetSampleDatas(m_sampleDatas,m_sampleDatasRow,m_sampleDatasLine);
//
//
//	 int ** sampleDataIndex;                //获得样本数据集索引
//	 sampleDataIndex=singalTree_SVM.GetSampleDataIndex(Di,tuplesN);//m_tuplesInformation
//
//	 int ** attIndexList=m_attListIndex;;                   //获得属性集
//	 
//	 //初始化一些参数
//	 //初始化一些参数
//
//	 m_Dimension2=0;m_k=0; m_NodeSize=0;m_Dimension0=0;
//	 m_publics=0;m_tuplesI=0; m_allErros=0;m_erroNum=0;
//	 //建立树
//	// singalTree_SVM.m_NodeSize=0;  //将树
//	 //Tree_SVM newSingalTree_SVM;
//	 singalTree_SVM.doBuildTree_SVM(Di,tuplesN);
//	 	 m_NodeSize=singalTree_SVM.m_NodeSize;     //记录节点数
//	 diTree_SVM=singalTree_SVM.GetTree_SVMRoot();
//
//	 //if (m_NodeSize==0)
//	 //{
//		// MessageBox(NULL,_T("The Nodesize=0!FUCK!"),_T("提示"),MB_OK);
//	 //}
//	 //                                       //初始化一些参数
//                                        	
//  
//	 errorRatio=ErroRatioForModel(/*m_tuplesInformation*/Di,m_tuplesNum); 
//	 }
//	double ttt=errorRatio;
//
//	int caoerrors=0;
//	//CString stCaoErrors,stc;
//	for (int ii=0;ii<tuplesN;ii++)
//	{
//		if (Di[ii].err==1)
//		{
//			caoerrors++;
//		}		 
//	}
//
//	
//	 double replaceFactor=(1-errorRatio)/errorRatio;//更换因子
//	 double weightAdd=0;
//	 for (int ii=0;ii<tuplesN;ii++)
//	 {
//		 if (Di[ii].err==1)
//		 {
//			 int index=Di[ii].i;
//			 m_tuplesInformation[index].weight=Di[ii].weight*replaceFactor;
//		 }
//		 else
//		 {
//			 int index=Di[ii].i;
//			 m_tuplesInformation[index].weight=Di[ii].weight;
//		 }
//		
//	 }
//	 for (int ii=0;ii<tuplesN;ii++)
//	 {
//		  weightAdd=weightAdd+m_tuplesInformation[ii].weight;
//	 }
//
//	 //归一化
//	 for (int ii=0;ii<tuplesN;ii++)
//	 {
//		 m_tuplesInformation[ii].weight=m_tuplesInformation[ii].weight/weightAdd;
//	 }
//	 double caobi=0;  //测试归一化是否正确
//	 for (int ii=0;ii<tuplesN;ii++)
//	 {
//		 caobi=m_tuplesInformation[ii].weight+caobi;
//	 }
//     TuplesInformationDelete(Di); //清除元组信息列表
//	 //记录合格树
//	 qualifiedTree_SVMs[i].Tree_SVM=diTree_SVM;
//	 qualifiedTree_SVMs[i].erroratio=errorRatio;
//	 qualifiedTree_SVMs[i].leafs=m_NodeSize;//记录叶子树；
//	 qualifiedTree_SVMs[i].publicClssId=diTree_SVM->publicClassID;
//
//	/* CString zyjw=_T("C:\\Data\\m_tuples.txt");
//	 PrintTuples(m_tuplesInformation,tuplesN,zyjw);*/
//	  
//	 }
//	 delete [] m_tuplesInformation;//使用完了删除了去
//	 //构造符合模型
//	 double ticketWeight=0;//分类器的投票权重
//	 double Tree_SVMErroratio=0;//分类器的错误率
//	 double aa=0,bb=0;
//	 int nodes=0;
//	 for (int i=0;i<k;i++)
//	 {
//		 Tree_SVMErroratio=qualifiedTree_SVMs[i].erroratio;
//		 aa=1-Tree_SVMErroratio;
//		 bb=Tree_SVMErroratio;
//		 ticketWeight=log((aa/bb));
//		 qualifiedTree_SVMs[i].weight=ticketWeight;
//
//		 /*CString stTree_SVMErroratio,stTweight,stTree_SVMNo,sts,stnode;
//		 stTree_SVMErroratio.Format(_T("%lf"),bb);
//		 stTweight.Format(_T("%lf"),ticketWeight);
//		 stTree_SVMNo.Format(_T("%d"),i);
//		 nodes=qualifiedTree_SVMs[i].leafs;
//		 stnode.Format(_T("%d"),nodes);
//		 sts=_T("Tree_SVMNo=")+stTree_SVMNo+_T(",ticketWeight=")+stTweight+_T(",Tree_SVMErroratio=")+stTree_SVMErroratio+_T(",Nodes=")+stnode+_T("\r\n");
//		 caoF.WriteString(sts);*/
//	 }
//	 m_Tree_SVMSelection_SVM=qualifiedTree_SVMs;//记录到决策树集中。
//
// 	//caoF.Close();
	
}
void AdaBoosting_SVM_adsvm::doAdaBoosting_SVM_adsvm(TuplesClassified_SVM * tuples, int tuplesN, int k,int & progressI)
{
	srand((int)time(NULL));           //随机数的种子
	TuplesClassified_SVM **tuplesArr;       //用于记录每次分类之后的元组
	tuplesArr=new TuplesClassified_SVM *[k];
	Tree_SVMSelection_SVM *qualifiedTree_SVMs;         //用于记录复合条件的树
	qualifiedTree_SVMs=new Tree_SVMSelection_SVM [k]; 

	SVM_Selection* qualifiedSVM;             //用于记录复合条件的SVM
	qualifiedSVM=new SVM_Selection[k];

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//进度条
	extern CProgressCtrl* p_global_Progress;
	extern CStatic *p_global_Percent;
	extern CStatic* p_global_jdmc;
	extern int sum_train_data;
	extern int train_k;
	int sum_train = sum_train_data * train_k;
	float float_sum_train = (float)sum_train;
	float decimals_train = sum_train / 100.00;
	//int multiple_train = sum_train - remainder_train;
	//int partial_train = multiple_train / 100;
	int train_quzheng ;
	p_global_jdmc->SetWindowTextW(_T("执行训练:"));
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int txtnum=0;
	double *targetClassId=new double[tuplesN];
	for (int i=0;i<k;i++)
	{	    
 	 TuplesClassified_SVM * Di;	
	 if(i==0)           //如果是第一次循环使用初始化的元组信息表,复制过来，而不是直接用指针，
	 {
		 Di=new TuplesClassified_SVM[tuplesN];
		 for (int ii=0;ii<tuplesN;ii++)
		 {
			 Di[ii].err=m_tuplesInformation[ii].err;
			 Di[ii].i=m_tuplesInformation[ii].i;
			 Di[ii].sampleDataIndex=m_tuplesInformation[ii].sampleDataIndex;
			 Di[ii].tuplesClassID=m_tuplesInformation[ii].tuplesClassID;
			 Di[ii].weight=m_tuplesInformation[ii].weight;
			 Di[ii].data=m_tuplesInformation[ii].data;
			 //初始化目标数组
			 targetClassId[ii]= Di[ii].tuplesClassID;
		 }			
	 }
	 else
	 {
		 //根据元组的权重，从D中有放回采样，得到Di
        Di=SamplingReplacement_SVM(m_tuplesInformation/*初始样本元组*/,tuplesN,targetClassId/*初始化目标数组*/);//有放回抽样，返回样本	
	 }

	  int * radom=GetNonredundantRandom(0,tuplesN,tuplesN/2);
	  int radomI=0;
	  int * radom_supplementaryset=new int[tuplesN-tuplesN/2];
	  int radom_supplementaryset_i=0;
	  double *targetClassId_train=new double[tuplesN/2];
	  double *targetClassId_test=new double[tuplesN-tuplesN/2];
	  for (int m=0;m<tuplesN;m++)
	  {
		  if (m==radom[radomI])
		  {
			  targetClassId_train[radomI]=Di[m].tuplesClassID;
			  radomI++;
			  ////////////////////////////////////////////////////////进度条
			  progressI++;
			  train_quzheng = (int)(progressI / decimals_train);
			  p_global_Progress->SetPos(train_quzheng);
			  CString str;
			  str.Format(_T("%d%%"),train_quzheng);
			  p_global_Percent->SetWindowTextW(str);


			  if (progressI == sum_train)
			  {
				  Sleep(1000);
				  p_global_Progress->SetPos(100);
				  Sleep(1000);
				  p_global_Percent->SetWindowTextW(_T("100% 完成！"));
				  Sleep(100);
			  }
		  }
		  else
		  {
		  targetClassId_test[radom_supplementaryset_i]=Di[m].tuplesClassID;
		  radom_supplementaryset[radom_supplementaryset_i]=m;
		  radom_supplementaryset_i++;
		  ////////////////////////////////////////////////////////进度条
		  progressI++;
		  train_quzheng = (int)(progressI / decimals_train);
		  p_global_Progress->SetPos(train_quzheng);
		  CString str;
		  str.Format(_T("%d%%"),train_quzheng);
		  p_global_Percent->SetWindowTextW(str);


		  if (progressI == sum_train)
		  {
			  Sleep(1000);
			  p_global_Progress->SetPos(100);
			  Sleep(1000);
			  p_global_Percent->SetWindowTextW(_T("100% 完成！"));
			  Sleep(100);
		  }
		  }
	  }
	  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	                          //获得样本训练数据
	  double ** sampleData;                 
	  sampleData=getdatabytuples_SVM(Di,tuplesN,radom,tuplesN/2);
	  int sampleData_row=tuplesN/2;
	  int sampleData_line=m_sampleDatasLine;
	                         //获得样本测试数据
	  double ** testData;                 
	  testData=getdatabytuples_SVM(Di,tuplesN,radom_supplementaryset,tuplesN-tuplesN/2);
	  int testData_row=tuplesN-tuplesN/2;
	  int testData_line=m_sampleDatasLine;

	  //                        //设置参数(参数在page1.cpp里面有设置)
	  svm_parameter parameter;	 
	  parameter.svm_type =C_SVC; 
	  parameter.kernel_type = RBF; 
	  parameter.degree = 3; 
	  parameter.gamma = 0.00001/*1.0/m_sampleDataLine*/; 
	  parameter.coef0 = 0; 
	  parameter.nu = 0.5;  
	  parameter.cache_size = 100; 
	  parameter.C = 32; 
	  parameter.eps = 1e-5; 
	  parameter.p = 0.1;
	  parameter.shrinking = 1; 
	  parameter.probability = 0; 
	  parameter.nr_weight = 0; 
	  parameter.weight_label = NULL; 
	  parameter.weight = NULL;


	 /* svm_parameter parameter;	 
	  parameter.svm_type =m_svm_parameter->svm_type; 
	  parameter.kernel_type =m_svm_parameter->kernel_type; 
	  parameter.degree = m_svm_parameter->degree; 
	  parameter.gamma =m_svm_parameter->gamma; 
	  parameter.coef0 = m_svm_parameter->coef0; 
	  parameter.nu =m_svm_parameter->nu;  
	  parameter.cache_size = m_svm_parameter->cache_size; 
	  parameter.C = m_svm_parameter->C; 
	  parameter.eps =m_svm_parameter->eps; 
	  parameter.p = m_svm_parameter->p;
	  parameter.shrinking =m_svm_parameter->shrinking; 
	  parameter.probability =m_svm_parameter->probability; 
	  parameter.nr_weight = m_svm_parameter->nr_weight; 
	  parameter.weight_label = m_svm_parameter->weight_label; 
	  parameter.weight = m_svm_parameter->weight;*/
	
	  
	                        //填充训练数据为SVM使用的格式
	  DataFilling_adsvm data(sampleData,sampleData_row,sampleData_line-1);//之所以减去1是因为最后一列不需要	
	  svm_problem *problem=&data.GetSVMproblem(targetClassId_train);
	  svm_node **nod_data=problem->x;
	                          //训练模型
	  svm_model * model;
	  model=svm_train(problem,&parameter);
	                        //填充测试数据为SVM使用的格式
	  DataFilling_adsvm data_test(testData,testData_row,testData_line-1);//之所以减去1是因为最后一列不需要	
	  svm_problem *testproblem=&data_test.GetSVMproblem(targetClassId_test);	  
	                         //测试，计算错误率
	  svm_node **nod_test=testproblem->x;
	  
	  int errors=0;
	  radomI=0;
	  radom_supplementaryset_i=0;
	  for (int m=0;m<tuplesN;m++)
	  { 
		  if (m==radom[radomI])
		  {
              svm_node * pre=nod_data[radomI];
			  double tempPre=svm_predict(model,pre);
			  if (tempPre!=Di[m].tuplesClassID)
			  {
				  Di[m].err=1;
				  errors++;
			  }
			  else
			  {
				  Di[m].err=0;
				  //errors++;
			  }
			  radomI++;
		  }
		  else
		  {
			  svm_node * pre=nod_test[radom_supplementaryset_i];
			  double tempPre=svm_predict(model,pre);
			  if (tempPre!=Di[m].tuplesClassID)
			  {
				  Di[m].err=1;
				  errors++;
			  }
			  else
			  {
				  Di[m].err=0;
				  //errors++;
			  }
			  radom_supplementaryset_i++;
		  }		 
	  }
	 
	 double errorRatio=ErroRatioForModel_SVM(/*m_tuplesInformation*/Di,m_tuplesNum);                                    //计算错误率
	 while(errorRatio>0.5||errorRatio==0)
	 {
	   // svm_destroy_model(model);//删除不合格的SVM；
	    TuplesWeightInitialization(m_tuplesInformation,tuplesN);//重新将权重初始化为1/d
	    TuplesInformationDelete(Di);//原先产生的样本无效删了去
	       if(i==0)           //如果是第一次循环使用初始化的元组信息表
	       {
		     Di=new TuplesClassified_SVM[tuplesN];
		     for (int ii=0;ii<tuplesN;ii++)
		     {
			 Di[ii].err=m_tuplesInformation[ii].err;
			 Di[ii].i=m_tuplesInformation[ii].i;
			 Di[ii].sampleDataIndex=m_tuplesInformation[ii].sampleDataIndex;
			 Di[ii].tuplesClassID=m_tuplesInformation[ii].tuplesClassID;
			 Di[ii].weight=m_tuplesInformation[ii].weight;
			 Di[ii].data=m_tuplesInformation[ii].data;
			 //初始化目标数组
			 targetClassId[ii]= Di[ii].tuplesClassID;
		     }  	
	    }
	 else
	 {
		 //根据元组的权重，从D中有放回采样，得到Di
        Di=SamplingReplacement_SVM(m_tuplesInformation/*初始样本元组*/,tuplesN,targetClassId/*初始化目标数组*/);//有放回抽样，返回样本
		
	 }
	 //将数据分为训练集和测试集
	 int * radom=GetNonredundantRandom(0,tuplesN,tuplesN/2);
	 int radomI=0;
	 int * radom_supplementaryset=new int[tuplesN-tuplesN/2];
	 int radom_supplementaryset_i=0;
	 double *targetClassId_train=new double[tuplesN/2];
	 double *targetClassId_test=new double[tuplesN-tuplesN/2];
	 for (int m=0;m<tuplesN;m++)
	 {
		 if (m==radom[radomI])
		 {
			 targetClassId_train[radomI]=Di[m].tuplesClassID;
			 radomI++;
		 }
		 else
		 {
			 targetClassId_test[radom_supplementaryset_i]=Di[m].tuplesClassID;
			 radom_supplementaryset[radom_supplementaryset_i]=m;
			 radom_supplementaryset_i++;
		 }
	 }

	 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //获得样本训练数据
	 double ** sampleData;                 
	 sampleData=getdatabytuples_SVM(Di,tuplesN,radom,tuplesN/2);
	 int sampleData_row=tuplesN/2;
	 int sampleData_line=m_sampleDatasLine;
	 //获得样本测试数据
	 double ** testData;                 
	 testData=getdatabytuples_SVM(Di,tuplesN,radom_supplementaryset,tuplesN-tuplesN/2);
	 int testData_row=tuplesN-tuplesN/2;
	 int testData_line=m_sampleDatasLine;
	 //设置参数(参数在page1.cpp里面有设置)
	 svm_parameter parameter;	 
	 parameter.svm_type =C_SVC; 
	 parameter.kernel_type = RBF; 
	 parameter.degree = 3; 
	 parameter.gamma = 0.00001/*1.0/m_sampleDataLine*/; 
	 parameter.coef0 = 0; 
	 parameter.nu = 0.5;  
	 parameter.cache_size = 100; 
	 parameter.C = 32; 
	 parameter.eps = 1e-5; 
	 parameter.p = 0.1;
	 parameter.shrinking = 1; 
	 parameter.probability = 0; 
	 parameter.nr_weight = 0; 
	 parameter.weight_label = NULL; 
	 parameter.weight = NULL;
	 //填充训练数据为SVM使用的格式
	 DataFilling_adsvm data(sampleData,sampleData_row,sampleData_line-1);//之所以减去1是因为最后一列不需要	
	 svm_problem *problem=&data.GetSVMproblem(targetClassId_train);
	 svm_node **nod_data=problem->x;
	 //训练模型
	 svm_model * model;
	 model=svm_train(problem,&parameter);
	 //填充测试数据为SVM使用的格式
	 DataFilling_adsvm data_test(testData,testData_row,testData_line-1);//之所以减去1是因为最后一列不需要	
	 svm_problem *testproblem=&data_test.GetSVMproblem(targetClassId_test);	  
	 //测试，计算错误率
	 svm_node **nod_test=testproblem->x;	 
	 int errors=0;
	 radomI=0;
	 radom_supplementaryset_i=0;
	 for (int m=0;m<tuplesN;m++)
	 { 
		 if (m==radom[radomI])
		 {
			 svm_node * pre=nod_data[radomI];
			 double tempPre=svm_predict(model,pre);
			 if (tempPre!=Di[m].tuplesClassID)
			 {
				 Di[m].err=1;
				 errors++;
			 }
			 else
			 {
				 Di[m].err=0;
				 //errors++;
			 }
			 radomI++;
		 }
		 else
		 {
			 svm_node * pre=nod_test[radom_supplementaryset_i];
			 double tempPre=svm_predict(model,pre);
			 if (tempPre!=Di[m].tuplesClassID)
			 {
				 Di[m].err=1;
				 errors++;
			 }
			 else
			 {
				 Di[m].err=0;
			 }
			 radom_supplementaryset_i++;
		 }		 
	 }
	  errorRatio=ErroRatioForModel_SVM(/*m_tuplesInformation*/Di,m_tuplesNum);                                    //计算错误率
	}
	double ttt=errorRatio;

	int caoerrors=0;
	//CString stCaoErrors,stc;
	for (int ii=0;ii<tuplesN;ii++)
	{
		if (Di[ii].err==1)
		{
			caoerrors++;
		}		 
	}

	
	 double replaceFactor=(1-errorRatio)/errorRatio;//更换因子
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
	 double caobi=0;  //测试归一化是否正确
	 for (int ii=0;ii<tuplesN;ii++)
	 {
		 caobi=m_tuplesInformation[ii].weight+caobi;
	 }
     TuplesInformationDelete(Di); //清除元组信息列表
	 //记录合格SVM
	 qualifiedSVM[i].model=model;
	 qualifiedSVM[i].erroratio=errorRatio;
	 qualifiedSVM[i].weight=-1;   //权重初始化为-1
	 
	 }
	 delete [] m_tuplesInformation;//使用完了删除了去
	 //构造符合模型
	 double ticketWeight=0;//分类器的投票权重
	 double SVMErroratio=0;//分类器的错误率
	 double aa=0,bb=0;
	 int nodes=0;
	 for (int i=0;i<k;i++)
	 {
		 SVMErroratio=qualifiedSVM[i].erroratio;
		 aa=1-SVMErroratio;
		 bb=SVMErroratio;
		 ticketWeight=log((aa/bb));
		 qualifiedSVM[i].weight=ticketWeight;		 
	 }
	 delete [] targetClassId;
	 m_SVMSelection=qualifiedSVM;//记录到模型集中。

 	//caoF.Close();
	
}


void AdaBoosting_SVM_adsvm::TuplesInformationDelete(TuplesClassified_SVM *tuples)
{
	delete [] tuples;
}

// 轮盘赌函数，参见一些遗传算法的资料,被SamplingReplacement（）调用
int AdaBoosting_SVM_adsvm::GetChromoRoulette(TuplesClassified_SVM* tuples, int tuplesN)
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
void AdaBoosting_SVM_adsvm::TuplesWeightInitialization(TuplesClassified_SVM* tuples, int tuplesN)
{
	for (int i=0;i<tuplesN;i++)
	{
		tuples[i].err=0;
		tuples[i].weight=1/(tuplesN*1.0);
	}
}

// 返回复合模型，该函数可以用来重载
Tree_SVMSelection_SVM * AdaBoosting_SVM_adsvm::GetCompoundModel(void)
{
	Tree_SVMSelection_SVM *m=m_Tree_SVMSelection_SVM;
	return m;
}
//// 保存从界面上获得的控制参数
//void AdaBoosting::GetControlsParameter(int Tree_SVMHeight, int adaBoostingK,int leafsSize,double sampleDataRatio,CString rulePath)
//{
//	m_Tree_SVMHeight=Tree_SVMHeight;
//	m_adaBoostingK=adaBoostingK;
//	m_leafsSize=leafsSize;
//	m_sampleDataRatio=sampleDataRatio;
//	m_ruleFilePath=rulePath;
//}

void AdaBoosting_SVM_adsvm::GetData(double ** sampleData, int row, int line,int ** attIndex)
{
	m_sampleDatas=sampleData;
	m_sampleDatasRow=row;
	m_sampleDatasLine=line;
	m_attListIndex=attIndex;

}
void AdaBoosting_SVM_adsvm::SampleDataInitialization_SVM(double ** sampleData, int row, int line)
{
	m_sampleDatas=sampleData;
	m_sampleDatasRow=row;
	m_sampleDatasLine=line;
	int tuplesNum=row;

	int n=m_sampleDatasLine-1;//参与分列的属性个数
	int** attListIndex;               //建立属性集
	attListIndex=new int* [n];  
	for (int i=0;i<n;i++)
	{
		attListIndex[i]=new int [2];
		attListIndex[i][0]=i;
		attListIndex[i][1]=0;
	}
	m_attListIndex=attListIndex;
	//处理数据索引,初始化元组，目标类

	m_tuplesInformation=new TuplesClassified_SVM[tuplesNum];
	int** spDataIndex;
	spDataIndex=new int* [row];  
	for (int i=0;i<row;i++)
	{
		spDataIndex[i]=new int [2];
		spDataIndex[i][0]=i;                     //元组在样本中的索引
		spDataIndex[i][1]=sampleData[i][line-1]; //记录的是元组的类别号

		//初始化元组
		m_tuplesInformation[i].i=i;
		m_tuplesInformation[i].err=0;
		m_tuplesInformation[i].tuplesClassID=sampleData[i][line-1];
		m_tuplesInformation[i].sampleDataIndex=spDataIndex[i][0];//0列数据记录样本数据中的索引
		m_tuplesInformation[i].weight=1.0/(tuplesNum*1.0);
		m_tuplesInformation[i].data=sampleData[i];

	}
	m_tuplesNum=tuplesNum;
	////TuplesInformationInitial(row/*元组的个数*/,spDataIndex/*样本索引*/,row/*行*/,2/*列，一般用2*/);
	//m_tuplesInformation=new TuplesClassified_SVM[tuplesNum];
	//m_tuplesNum=tuplesNum;
	//for (int i=0;i<tuplesNum;i++)
	//{
	//	m_tuplesInformation[i].i=i;
	//	m_tuplesInformation[i].err=0;
	//	m_tuplesInformation[i].tuplesClassID=sampDataIndex[i][n-1];//n-1列数据记录样本数据的类别标记
	//	m_tuplesInformation[i].sampleDataIndex=sampDataIndex[i][0];//0列数据记录样本数据中的索引
	//	m_tuplesInformation[i].weight=1.0/(tuplesNum*1.0);
	//}
}
void AdaBoosting_SVM_adsvm::SampleDataInitialization(double ** sampleData, int row, int line)
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
	//TuplesInformationInitial(row/*元组的个数*/,spDataIndex/*样本索引*/,row/*行*/,2/*列，一般用2*/);
	// Calculate::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
}
void AdaBoosting_SVM_adsvm::TuplesInformationInitial(int tuplesNum/*元组的个数*/,int **sampDataIndex/*样本索引*/,int m/*行*/,int n/*列，一般用2*/)
{

	m_tuplesInformation=new TuplesClassified_SVM[tuplesNum];
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
void AdaBoosting_SVM_adsvm::TestDataInitialization(double** testData , int row , int line)
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
void AdaBoosting_SVM_adsvm::run(int & progressI)
{

	doAdaBoosting(m_tuplesInformation,m_tuplesNum,m_adaBoostingK,progressI);
}
void AdaBoosting_SVM_adsvm::run_SVM(int & progressI)
{

	doAdaBoosting_SVM_adsvm(m_tuplesInformation,m_tuplesNum,m_adaBoostingK,progressI);
}





// 获得纪录SVM的数组
SVM_Selection* AdaBoosting_SVM_adsvm::GetSVM_Selection(int& SVM_NUM)
{
	SVM_NUM=m_adaBoostingK;
	return m_SVMSelection;
}



/*********************************************GLC_AdaBoosting_SVM********************************************************************************/
GLC_Adaboosting_SVM::GLC_Adaboosting_SVM(void)
	: m_svm_modles(NULL)
	, m_adaboostingK(0)
	, m_svm_parameter(NULL)
{
}


GLC_Adaboosting_SVM::~GLC_Adaboosting_SVM(void)
{
}


// 训练adaSVM模型
bool GLC_Adaboosting_SVM::G_adaSVM_ExecutTraining(double** traindata,int traindatrow,int traindatline,int adboostingK)
{
	AdaBoosting_SVM_adsvm adaboosting;
	adaboosting.SampleDataInitialization_SVM(traindata,traindatrow,traindatline);
	int modles_NUM=0;
	int & m=modles_NUM;
	//CString pass=_T("");//无用代码，又懒得去改就这样
	//adaboosting.GetControlsParameter(7,adboostingK,1,50,pass);
	//m_adaboostingK=adboostingK;
	adaboosting.m_adaBoostingK=adboostingK;
	adaboosting.m_svm_parameter=m_svm_parameter;
	adaboosting.run_SVM(m);
	
	m_svm_modles=adaboosting.GetSVM_Selection(m);
	
	if (m_svm_modles!=NULL)
	{
		return true;
	}
	return false;
}


// 获得adaSVM模型
SVM_Selection * GLC_Adaboosting_SVM::G_adaSVM_GetModel(void)
{
	if (m_svm_modles!=NULL)
	{
		return m_svm_modles;
	}
	return NULL;
}

short *GLC_Adaboosting_SVM::G_adaSVM_ExecutClassify_Single(double **adata,int row,int line,int& progressI )
{
	short *classid=new short[row];
	DataClassification_SVM data;
	for (int i=0;i<row;i++)
	{
		//	classid[i]=data.doClassify_SVM_Single(adata[i],line,m_svm_modles,m_adaboostingK);
		progressI++;
	}	
	return classid;

}
short * GLC_Adaboosting_SVM::G_adaSVM_ExecutClassify(double **adata,int row,int line,int& progressI )
{
	DataClassification_SVM data(adata,row,line,1/*1表示用SVM分类*/);
	try
	{
		if (m_svm_modles==NULL)
		{
			throw "错误：未能生成模型（m_svm_modles==NULL）！";
		}
	}
	catch (...)
	{
		return NULL;
	}

	short *classtmepId=data.doClassify_SVM(m_svm_modles,m_adaboostingK,progressI);
	return classtmepId;



}

/**************************************DataFilling_adsvm*******************************************************/

DataFilling_adsvm::DataFilling_adsvm(double** data, int row , int line/*数据的行和列*/)
	: m_SVMnode(NULL)
	, m_SVMnode_row(0)
	, m_SVMnode_line(0)
	, m_sampleClassId(NULL)
	, m_svmnode_forone(NULL)
{
	/*m_SVMnode=*/SetSVMNodes(data,row,line);
}


DataFilling_adsvm::~DataFilling_adsvm(void)
{
	
}


// 获取SVM_node得到一个样本的特征
svm_node* DataFilling_adsvm::SetSVM_node(double *characteristic,int characteristic_Num)
{
	if (characteristic==NULL)
	{
		return NULL;
	}
	svm_node* svmnode=new svm_node[characteristic_Num+1];
	
	int svmindex=0;	
	for (int i=0;i<characteristic_Num;i++)
	{
		if (characteristic[i]!=0)
		{			
			svmnode[svmindex].index=svmindex+1;
			svmnode[svmindex].value=characteristic[i];
			svmindex++;
		}
		/*svmnode[i]=new svm_node[]
		svmnode[i].index=i+1;
		svmnode[i].value=characteristic[i];*/
	}
	/*for (int i=0;i<characteristic_Num;i++)
	{
		svmnode[i].index=0;
		svmnode[i].value=characteristic[i];
	}*/
	
	/*if (svmindex!=characteristic_Num)
	{
		MessageBox(NULL,_T("出现零值"),_T("注意！"),MB_OK);
	}*/
	/*for (int i=svmindex;i<characteristic_Num;i++)
	{
	svmnode[svmindex].index=-1;
	svmnode[svmindex].value=NULL;
	}*/
	svmnode[svmindex].index=-1;
	svmnode[svmindex].value=NULL;	
	m_svmnode_forone=svmnode;//记录地址用于删除；
	/*const int a=svmindex;*/
	
	

	return NULL;

}


// 获得整个样本的特征
svm_node** DataFilling_adsvm::SetSVMNodes(double** data, int row , int line)
{
	if (data==NULL)
	{
		return NULL;
	}
	svm_node** svmnodes;
	svmnodes=new svm_node*[row];
	for (int i=0;i<row;i++)
	{
		svm_node* tempsvmnode;		
		double *tempdata=data[i];
		SetSVM_node(tempdata,line);
		tempsvmnode=m_svmnode_forone;/*=SetSVM_node(tempdata,line)*/;
		svmnodes[i]=tempsvmnode;			
	}
	m_SVMnode_row=row;
	m_SVMnode_line=line;
	m_SVMnode=svmnodes;
	return svmnodes;
}
svm_node**DataFilling_adsvm::GetSVMNodes()
{
	if (m_SVMnode==NULL)
	{
		return NULL;
	}
	return m_SVMnode;
}

// 存储本次参加运算的所有样本（数据集）
svm_problem DataFilling_adsvm::GetSVMproblem(double* SampleClassId)
{
	if (m_SVMnode==NULL)
	{
       return svm_problem();
	}
	svm_problem problem;
	problem.l=m_SVMnode_row;
	problem.x=m_SVMnode;
	problem.y=SampleClassId;	
	return problem;
	
}


// 设置SVM参数
void DataFilling_adsvm::SetSVMparameter(	int svm_type,//SVM类型，见前enum
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
                                  )
{
	m_svm_parameter.C=C;
	m_svm_parameter.cache_size=cache_size;
	m_svm_parameter.coef0=coef0;
	m_svm_parameter.degree=degree;
	m_svm_parameter.eps=eps;
	m_svm_parameter.gamma=gamma;
	m_svm_parameter.kernel_type=kernel_type;
	m_svm_parameter.nr_weight=nr_weight;
	m_svm_parameter.nu=nu;
	m_svm_parameter.p=p;
	m_svm_parameter.probability=probability;
	m_svm_parameter.shrinking=shrinking;
	m_svm_parameter.svm_type=svm_type;
	
}


// 获得参数
svm_parameter DataFilling_adsvm::GetSVMparameter(void)
{
	return m_svm_parameter;
}

//获得样本数据的目标类别
double * DataFilling_adsvm::GetSampleTargetClassId(void)
{
	return m_sampleClassId;
}


// 清除数据
void DataFilling_adsvm::DataCleaner(void)
{

	for (int i=0;i<m_SVMnode_row;i++)
	{		
		/*for (int j=0;j<m_SVMnode_line-1;j++)
		{
			m_SVMnode[i][j].index=0;
			m_SVMnode[i][j].value=0;
		}*/
		delete [] m_SVMnode[i];
	}
	delete [] m_SVMnode;
}

/*********************************ClassifiedData_SVM***********************************************************/

ClassifiedData_SVM::ClassifiedData_SVM()
	: m_ClassifiedData_SVM(NULL)

{
	StatisticsI=0;
}


ClassifiedData_SVM::~ClassifiedData_SVM(void)
{
	delete [] resultEveryT;
}


void ClassifiedData_SVM::SetData(int index/*元组的索引*/,int classT/*分类的次数*/)
{
	i=index;
	DataIndex=i;
	classificationTimes=classT;
	resultEveryT=new ClassifiedResutEveryTime_SVM_SVM[classificationTimes];
	for (int i=0;i<classificationTimes;i++)
	{
		resultEveryT[i].tuplesClassID=-1;//初始化，未分类之前为-1
		resultEveryT[i].pre=-1;
	}
}




/*********************************DataClassification_SVM***********************************************************/
DataClassification_SVM::DataClassification_SVM()
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
DataClassification_SVM::DataClassification_SVM(double **data,int row,int line)
	: m_data(NULL)
	, m_dataRow(0)
	, m_dataLine(0)
	, m_data_Tuples(NULL)
	
	, m_ruleSetI(0)
	, m_ruleCount(0)
	, m_classifideData(NULL)
	, m_classData(NULL)
{
	//初始化待分类数据
	m_data=data;
	m_dataRow=row;
	m_dataLine=line;
	//初始分类数据元组
	TuplesClassified_SVM* data_Tuples;
	data_Tuples=new TuplesClassified_SVM[row];
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
DataClassification_SVM::DataClassification_SVM(double **data,int row,int line,int classifyType)
	: m_data(NULL)
	, m_dataRow(0)
	, m_dataLine(0)
	, m_data_Tuples(NULL)
	
	, m_ruleSetI(0)
	, m_ruleCount(0)
	, m_classifideData(NULL)
	, m_classData(NULL)
{
	//初始化待分类数据
	m_data=data;
	m_dataRow=row;
	m_dataLine=line;
	//初始分类数据元组
	switch (classifyType)
	{
	case 0:                                         //用C5.0分类
		{
			//初始分类数据元组
			TuplesClassified_SVM* data_Tuples;
			data_Tuples=new TuplesClassified_SVM[row];
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
	case 1:                                          //SVM分类
		{
			m_FilledData_SVM=new DataFilling_adsvm(m_data,m_dataRow,m_dataLine);
		}

	}


}
DataClassification_SVM::~DataClassification_SVM(void)
{
	delete [] m_data_Tuples;
}
ClassifiedData_SVM  * DataClassification_SVM::doClassify(Tree_SVMSelection_SVM * Tree_SVMSelection_SVM,int Tree_SVMSum)
{
	//初始化分类数据，这里是一个类
	ClassifiedData_SVM * classData=new ClassifiedData_SVM[m_dataRow];
	for (int i=0;i<m_dataRow;i++)
	{
		classData[i].SetData(i,Tree_SVMSum);
	}
	//开始每次循环
	/*TuplesClassified_SVM classTuples[1000];
	TuplesClassified_SVM afterClassTuples[1000];*/

	
	
	TuplesClassified_SVM *classTuples=new TuplesClassified_SVM[m_dataRow];
	for (int t=0;t<m_dataRow;t++)
	{
		classTuples[t].err=m_data_Tuples[t].err;
		classTuples[t].i=m_data_Tuples[t].i;
		classTuples[t].pre=m_data_Tuples[t].pre;
		classTuples[t].sampleDataIndex=m_data_Tuples[t].sampleDataIndex;
		classTuples[t].tuplesClassID=m_data_Tuples[t].tuplesClassID;
		classTuples[t].weight=m_data_Tuples[t].weight;
		classTuples[t].clssified_data=&classData[t];
	}
  
	for (int i=0;i<Tree_SVMSum;i++)
	{
		TuplesClassified_SVM *afterClassTuples=new TuplesClassified_SVM[m_dataRow];
		Tree_SVMNode_SVM * Tree_SVM=Tree_SVMSelection_SVM[i].Tree_SVM;
		double errorRatio=Tree_SVMSelection_SVM[i].erroratio;
		int tuplesI=0;
		int &tI=tuplesI;
		int isRoot=0;//参数判断递归的第一层
		int &root=isRoot;
		ClassfyWithTree_SVM2(Tree_SVM,errorRatio,classTuples,m_dataRow,afterClassTuples,tI,i,root);




        delete [] afterClassTuples;
	}
	

	//投票表决
	for (int index=0;index<m_dataRow;index++)
	{
		int classNum;//类别数
		classNum=classData[index].StatisticsI;
		if (classNum==0)
		{
			//MessageBox(NULL,_T("类别错误，无法投票！classNum==0"),_T("注意！"),MB_OK);
		}
		if (classNum==1)
		{
			classData[index].classId=classData[index].Statistics[0].tuplesClassID;
		}
		else
		{
			double maxPre=-1;
			int maxPreI=-1;
			int temp[30],tempI=0;
			for (int i=0;i<classNum;i++)
			{
				if (maxPre<=classData[index].Statistics[i].pre)
				{
				    
					maxPre=classData[index].Statistics[i].pre;
					maxPreI=i;
			
					if (maxPre==classData[index].Statistics[i].pre)
					{
						temp[tempI]=i;
						tempI++;
					}
				}

			}
			if (tempI!=0)
			{
				maxPreI=rand()%tempI;
			}
			if (maxPre==-1&&maxPreI==-1)
			{
				//MessageBox(NULL,_T("类别错误，无法投票！maxPre==-1&&maxPreI==-1"),_T("注意！"),MB_OK);
			}
			classData[index].classId=classData[index].Statistics[maxPreI].tuplesClassID;
			

		}
	}

	m_classifideData=classData;
	return classData;
}
ClassifiedData_SVM* DataClassification_SVM::doClassify()
{
	return NULL;
}
int DataClassification_SVM::WeightedVoting(ClassifiedData_SVM dataWaitingVote)
{

	int k=dataWaitingVote.classificationTimes;
	ResutEveryTime_SVM TempSave;
	ResutEveryTime_SVM *dataInfo=new ResutEveryTime_SVM[k];
	
	for (int i=0;i<k;i++)
	{
		dataInfo[i].pre=dataWaitingVote.resultEveryT[i].pre;
		dataInfo[i].tuplesClassID=dataWaitingVote.resultEveryT[i].tuplesClassID;
		dataInfo[i].isProseed=true;
	}
	ResutEveryTime_SVM statistics[30];int statisticsI=0;
	int noRroseed=0;
	ResutEveryTime_SVM temp[30];int tempI=0;
	ResutEveryTime_SVM head;
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
	//判断最大值
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

void DataClassification_SVM::Swap(ClassifiedResutEveryTime_SVM_SVM  * a , ClassifiedResutEveryTime_SVM_SVM  * b)
{
	/*ClassifiedResutEveryTime_SVM_SVM TempSave;

		TempSave.pre=*a.pre;

		*a=*b;
		*b=TempSave;*/
	
}

bool DataClassification_SVM::ClassfyWithTree_SVM(Tree_SVMNode_SVM* root, double erroratio,TuplesClassified_SVM *tuples,int tuplesN,TuplesClassified_SVM *tuplesResult,int & tuplesResultI)
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
        TuplesClassified_SVM *tuplesL,*tuplesR;
		int tuplesLN=0,tuplesRN=0;
		int tuplesLi=0,tuplesRi=0;
			
		double splitpoint=root->splitVertex;//获得分裂点
		int splitattIndex=root->attIndex;        //获得分裂属性

		//计算两部分数据个数
		for (int i=0;i<tuplesN;i++)
		{
		//

	

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
			//建立数组
			tuplesL=new TuplesClassified_SVM[tuplesLN];
			tuplesR=new TuplesClassified_SVM[tuplesRN];
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
			ClassfyWithTree_SVM(root->leftChild,erroratio,tuplesL,tuplesLN,tuplesResult,tuplesResultI);//遍历的是左边子树
			ClassfyWithTree_SVM(root->rightChild,erroratio,tuplesR,tuplesRN,tuplesResult,tuplesResultI);//遍历的右面子树

		}
		if (tuplesLN==0&&tuplesRN!=0)
		{
			//建立数组
		
			tuplesR=new TuplesClassified_SVM[tuplesRN];
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
			
			ClassfyWithTree_SVM(root->rightChild,erroratio,tuplesR,tuplesRN,tuplesResult,tuplesResultI);//遍历的右面子树

		}
		if (tuplesLN!=0&&tuplesRN==0)
		{
			//建立数组
			tuplesL=new TuplesClassified_SVM[tuplesLN];
		
		
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
			ClassfyWithTree_SVM(root->leftChild,erroratio,tuplesL,tuplesLN,tuplesResult,tuplesResultI);//遍历的是左边子树
		}
	
		}
		
	}
	return true;
}
bool DataClassification_SVM::ClassfyWithTree_SVM1(Tree_SVMNode_SVM* root, double erroratio,TuplesClassified_SVM *tuples,int tuplesN,TuplesClassified_SVM *tuplesResult,int & tuplesResultI)
{
	if (tuplesN>5000)
	{
		//MessageBox(NULL,_T("数据量超出预定范围（5000）！请分块处理！"),_T("注意！"),MB_OK);
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

				//赋值
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

			double splitpoint=root->splitVertex;//获得分裂点
			int splitattIndex=root->attIndex;        //获得分裂属性

			//计算两部分数据个数
			
			TuplesClassified_SVM *tuplesL=new TuplesClassified_SVM[tuplesN];
			TuplesClassified_SVM *tuplesR=new TuplesClassified_SVM[tuplesN];

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
				ClassfyWithTree_SVM1(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI);//遍历的是左边子树
				ClassfyWithTree_SVM1(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI);//遍历的右面子树

			}
			if (tuplesLi==0&&tuplesRi!=0)
			{
				//建立数组

			
				delete [] tuples;

				ClassfyWithTree_SVM1(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI);//遍历的右面子树

			}
			if (tuplesLi!=0&&tuplesRi==0)
			{
				//建立数组
		
				delete [] tuples;
				ClassfyWithTree_SVM1(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI);//遍历的是左边子树
			}

		}

	}
	return true;
}
bool DataClassification_SVM::ClassfyWithTree_SVM2(Tree_SVMNode_SVM* root, double erroratio,TuplesClassified_SVM *tuples,int tuplesN,TuplesClassified_SVM *tuplesResult,int & tuplesResultI,int Tree_SVM_NO,int& isRoot)
{
	if (tuplesN>5000)
	{
	//	MessageBox(NULL,_T("数据量超出预定范围（5000）！请分块处理！"),_T("注意！"),MB_OK);
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

				//赋值
				tuplesResult[tuplesResultI].err=tuples[i].err;
				tuplesResult[tuplesResultI].i=tuples[i].i;
				tuplesResult[tuplesResultI].pre=pre;
				tuplesResult[tuplesResultI].sampleDataIndex=tuples[i].sampleDataIndex;
				tuplesResult[tuplesResultI].tuplesClassID=classId;
				tuplesResult[tuplesResultI].weight=tuples[i].weight;
				tuplesResult[tuplesResultI].clssified_data=tuples[i].clssified_data;

		
				tuplesResult[tuplesResultI].clssified_data->resultEveryT[Tree_SVM_NO].pre=tuplesResult[tuplesResultI].pre;
				tuplesResult[tuplesResultI].clssified_data->resultEveryT[Tree_SVM_NO].tuplesClassID=tuplesResult[tuplesResultI].tuplesClassID;
				tuplesResult[tuplesResultI].clssified_data->resultEveryT[Tree_SVM_NO].isvotied=true;
				if (Tree_SVM_NO==0)
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
						if (tuplesResult[tuplesResultI].clssified_data->Statistics[k].tuplesClassID==tuplesResult[tuplesResultI].clssified_data->resultEveryT[Tree_SVM_NO].tuplesClassID)
						{
							tuplesResult[tuplesResultI].clssified_data->Statistics[k].pre=tuplesResult[tuplesResultI].clssified_data->Statistics[k].pre+tuplesResult[tuplesResultI].clssified_data->resultEveryT[Tree_SVM_NO].pre;
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

			double splitpoint=root->splitVertex;//获得分裂点
			int splitattIndex=root->attIndex;        //获得分裂属性

			//计算两部分数据个数

			TuplesClassified_SVM *tuplesL=new TuplesClassified_SVM[tuplesN];
			TuplesClassified_SVM *tuplesR=new TuplesClassified_SVM[tuplesN];

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
            //清除原来的数据
				if (isRoot>0)
				{
					delete [] tuples;
				}
				isRoot++;
			if (tuplesLi!=0&&tuplesRi!=0)
			{

				ClassfyWithTree_SVM2(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI,Tree_SVM_NO,isRoot);//遍历的是左边子树
				ClassfyWithTree_SVM2(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI,Tree_SVM_NO,isRoot);//遍历的右面子树

			}
			if (tuplesLi==0&&tuplesRi!=0)
			{
				//建立数组


				ClassfyWithTree_SVM2(root->rightChild,erroratio,tuplesR,tuplesRi,tuplesResult,tuplesResultI,Tree_SVM_NO,isRoot);//遍历的右面子树

			}
			if (tuplesLi!=0&&tuplesRi==0)
			{
				//建立数组
				
				ClassfyWithTree_SVM2(root->leftChild,erroratio,tuplesL,tuplesLi,tuplesResult,tuplesResultI,Tree_SVM_NO,isRoot);//遍历的是左边子树
			}

		}

	}
	return true;
}
void quicksort(TuplesClassified_SVM a[], int left, int right){
	if(left >= right) return;
	int i = left,j = right-1;
	//int key = a[left];//取首元素为关键值
	int key = a[(left + right) / 2].i;
	while(i < j)//1
	{
		while(i < j && a[j].i >= key)//这里不能使用>=，有可能导致前半部分二次排序不起作用
			j--;
		while(i < j && a[i].i < key)
			i++;
		//swap
		if (i < j){
			TuplesClassified_SVM temp;
			//swap(a[i],a[j]);
			temp=a[i];
			a[i]=a[j];
			a[j]=temp;
			
			i++; j--;//缩小区间
		}
		if(i >= j ) break;//do nothing
	}
	quicksort(a,left,j);//取j不取i，因为后走的i有可能超过了j
	quicksort(a,j+1,right);
}

int partition(TuplesClassified_SVM data[],int lo,int hi)
{
	int i,j;
	TuplesClassified_SVM t;
	int key = data[hi].i;   //还是以最后一个元素作为哨兵，即主元元素
	i = lo-1;
	for (j =lo;j<=hi;j++)
		if(data[j].i<key)
		{
			i++;
			t = data[j];
			data[j] = data[i];
			data[i] = t;
		}
		data[hi] = data[i+1];  //先,data[i+1]赋给data[hi]
		data[i+1] = data[key];       //后，把事先保存的key值，即data[hi]赋给data[i+1]
		//不可调换这俩条语句的顺序。
		return i+1;
}
void QuickSort(TuplesClassified_SVM a[], int lo, int hi)
{
	if (lo<hi)
	{
		int k = partition(a, lo, hi);
		QuickSort(a, lo, k-1);
		QuickSort(a, k+1, hi);
	}
} 







// 获取分类之后的数据的类别，按照数据的序号返回
int* DataClassification_SVM::GetClassifiedID(void)
{
	int *classifiedID=new int[m_dataRow];
	if (m_classifideData==NULL)
	{
		//MessageBox(NULL,_T("数据未分类，不可使用！"),_T("注意！"),MB_OK);
		return NULL;
	}
	else
	{
        for (int i=0;i<m_dataRow;i++)
        {
			classifiedID[i]=m_classifideData[i].classId;
        }
	}
	return classifiedID;
}


// 使用SVM分类
short* DataClassification_SVM::doClassify_SVM(SVM_Selection * svmselection,int svmnum,int &progressI/*控制进度条*/)
{	
	svm_node** tempnodes=m_FilledData_SVM->GetSVMNodes();
	short * classID=new short[m_dataRow];

	for(int i=0;i<m_dataRow;i++)
	{
		double temp[20][3]={{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},
		                     {-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0}
		                    };                                                               //0类别，1，权重，2，标记                                                                                  	
		int maxweight=-1,max_ID=-1;
		int n=20;
		int tempN=1;
		int tempi=0;
		maxweight=-1;max_ID=-1;
		svm_node * classifynode=tempnodes[i];
		
		for (int j=0;j<svmnum;j++)
		{			
			svm_model * mode=svmselection[j].model;
			double ID=svm_predict(mode,classifynode);
			if (i==0)              //初始化
			{
				temp[j][0]=ID;
				temp[j][1]=svmselection[j].weight;
				temp[j][2]=1;
				continue;
			}
			for (int k=0;k<n;k++)
			{
				if (ID==temp[k][0])
				{
					temp[k][0]=ID;
					temp[k][1]=temp[k][1]+svmselection[j].weight;
					temp[k][2]=1;
					if (maxweight<temp[k][1])
					{
						maxweight=temp[k][1];
						max_ID=temp[k][0];
					}
					break;
				}
				else if (ID!=temp[k][0]&&temp[k][2]!=1)
				{
					temp[k][0]=ID;
					temp[k][1]=svmselection[j].weight;
					temp[k][2]=1;
					tempN++;
					if (maxweight<temp[k][1])
					{
						maxweight=temp[k][1];
						max_ID=temp[k][0];
					}
					break;
				}
		     }
	   }
		if (tempN==1)
		{
			classID[i]=temp[0][0];
		}	
		if (tempN!=1)
		{
			//MessageBox(NULL,_T("kan"),_T("kan"),MB_OK);
			classID[i]=max_ID;
		}
		progressI++;//处理完一个数据进度加1；

		////////////////////////////////////////////////////////////////进度条
		//外部 全局变量 用于进度条控制
		extern CProgressCtrl* p_global_Progress;
		extern CStatic* p_global_Percent;
		extern CStatic* p_global_jdmc;
		int raster_percent = 0;
		p_global_jdmc->SetWindowText(_T("执行分类:"));

		int remainder = m_dataRow % 99;
		int multiple = m_dataRow - remainder;
		int partial = multiple / 99;
		if(progressI % partial == 0)
		{
			raster_percent++;
			p_global_Progress->SetPos(raster_percent);
			CString str;
			str.Format(_T("%d%%"),raster_percent);
			p_global_Percent->SetWindowTextW(str);
		}
		if (progressI == m_dataRow || progressI > 100*partial)
		{
			p_global_Progress->SetPos(100);
			p_global_Percent->SetWindowTextW(_T("100% 完成！"));
		}
	}
	return classID;
}
// 用于单个数据的分类
short DataClassification_SVM::doClassify_SVM_Single(double* data,int data_line,SVM_Selection *svmselection,int svmNum)
{
	//数据格式转换
	int svmindex=0;
	svm_node * pre=new svm_node[data_line+1];
	for (int i=0;i<data_line;i++)
	{
		if (data[i]!=0)
		{
			pre[i].index=i+1;
			pre[i].value=data[i];
			svmindex++;
		}
	}
	pre[svmindex].index=-1;
	pre[svmindex].value=NULL;
	
	try 
	{
		//执行adSVM 分类
		GLC_adSVM_Statistics* sta=new GLC_adSVM_Statistics[30];//分类的类别数最大设置成30；
		int staI=0;
		double max_weight_add=0;
		int max_weight_staI=0;
		for (int i=0;i<svmNum;i++)
		{
			svm_model *svmmodle=svmselection[i].model;
			int tempPre=(int)svm_predict(svmmodle,pre);
			if (i==0) //初始化
			{
				sta[staI].classId=tempPre;
				sta[staI].PreWeightSum=max_weight_add=svmselection[i].weight;
				max_weight_staI=staI;
				staI++;
				continue;
			}
			bool notselected=true;//判断一个类别是否已添加到统计数组中；
			for (int j=0;j<staI;j++)
			{
				if (tempPre==sta[j].classId)
				{
					sta[j].PreWeightSum=sta[j].PreWeightSum+svmselection[i].weight;								
					notselected=false;
					break;
				}				
			}
			if (notselected)
			{
				sta[staI].classId=tempPre;
				sta[staI].PreWeightSum=svmselection[i].weight;
				staI++;
			}
		}
		
		if (staI==1)
		{
			short classId=(short)sta[0].classId;
			delete [] pre;
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
		short classid=(short)sta[max_weight_staI].classId;
		
		delete [] pre;
		delete [] sta;
		return classid;
	}
	catch (...)
	{
	
		return 0;		
	}

}


// 用于单个数据的分类
short DataClassification_SVM::doClassify_SVM_Single(svm_node * pre,int data_line,SVM_Selection *svmselection,int svmNum)
{
	//数据格式转换	
	try
	{
		//执行adSVM 分类
		GLC_adSVM_Statistics* sta=new GLC_adSVM_Statistics[30];//分类的类别数最大设置成30；
		int staI=0;
		double max_weight_add=0;
		int max_weight_staI=0;
		for (int i=0;i<svmNum;i++)
		{
			svm_model *svmmodle=svmselection[i].model;
			int tempPre=(int)svm_predict(svmmodle,pre);
			if (i==0)                                         //初始化
			{
				sta[staI].classId=tempPre;
				sta[staI].PreWeightSum=max_weight_add=svmselection[i].weight;
				max_weight_staI=staI;
				staI++;
				continue;
			}
			bool notselected=true;                           //判断一个类别是否已添加到统计数组中；
			for (int j=0;j<staI;j++)
			{
				if (tempPre==sta[j].classId)
				{
					sta[j].PreWeightSum=sta[j].PreWeightSum+svmselection[i].weight;								
					notselected=false;
					break;
				}				
			}
			if (notselected)
			{
				sta[staI].classId=tempPre;
				sta[staI].PreWeightSum=svmselection[i].weight;
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
		short classid=(short)sta[max_weight_staI].classId;		
		delete [] sta;
		return classid;
	}
	catch (...)
	{
		
		return 0;		
	}

}

/******************************************GLC_SVM_Classify**************************************************************/
GLC_SVM_Classify::GLC_SVM_Classify(void)
	: m_svmNum(0)
	, m_classid(NULL)
	, m_svm_parameter(NULL)
{
}


GLC_SVM_Classify::~GLC_SVM_Classify(void)
{
}
// 执行训练
bool GLC_SVM_Classify::GLCSVM_Execute_Train(double** data, int data_row, int data_line,int adboostingK)
{
	m_svmNum=adboostingK;
	try 
	{
		//执行训练
		if (m_Gsvm.G_adaSVM_ExecutTraining(data,data_row,data_line,adboostingK))
		{
			return true;
		}
		else
		{
			throw "Error1:训练错误!";
			return false;
		}
	}
	catch (...)
	{
		throw "Error2:训练过程中出现内存错误!";
		return false;		
	}
	


}
bool GLC_SVM_Classify::GLCSVM_Execute_Train(double*data, int data_row, int data_line,int adboostingK)
{
	m_svmNum=adboostingK;
	try
	{
		//一位数组转换成二维数组
		double ** traindata=new double *[data_row];
		int tempindex=0;
		for (int i=0;i<data_row;i++)
		{
			for (int j=0;j<data_line;j++)
			{
				traindata[i][j]=data[tempindex];
				tempindex;
			}
		}
		//执行训练；

		m_Gsvm.G_adaSVM_ExecutTraining(traindata,data_row,data_line,adboostingK);
		//删除数组
		for (int i=0;i<data_row;i++)
		{
			delete [] traindata[i];
		}
		delete [] traindata;


		return true;
	}
	catch (...)
	{
		throw "Error2:训练过程中出现内存错误!";
		return false;		
	}
	


}
// 执行分类
short* GLC_SVM_Classify::GLCSVM_Excute_Classify(double* data, int data_row, int data_line)
{
	try 
	{
		/////////////////////////////////////////////////////////////////////////
		//外部 全局变量 用于进度条控制
		extern CProgressCtrl* p_global_Progress;
		extern CStatic* p_global_Percent;
		extern CStatic* p_global_jdmc;
		int raster_percent = 0;
		int progress=0;
		p_global_jdmc->SetWindowText(_T("执行分类:"));
		/////////////////////////////////////////////////////////////////////////
		int dataindex=0;
		short * classid=new short[data_row];		
		SVM_Selection *svmmodel=m_Gsvm.G_adaSVM_GetModel();
		DataClassification_SVM classification;
		double * classifydata;
		for (int i=0;i<data_row;i++)
		{


			/*svm_node* svmdata=new svm_node[raster_band+1];
			int svmindex=0;			
			for (int j=0;j<raster_band;j++)
			{
				if (rasterdata_B[rasterdataindex]!=0)
				{
					svmdata[svmindex].index=j+1;
					svmdata[svmindex].value=rasterdata_B[rasterdataindex];
					svmindex++;
				}		
				rasterdataindex++;
			}
			svmdata[svmindex].index=-1;
			svmdata[svmindex].value=NULL;
			short pixel_classid=svm_set.at(max_pre_i).ExecutClassify(svmdata);
			classid1[i]=pixel_classid;*/


			int svmindex=0;
			svm_node * pre=new svm_node[data_line+1];
			for (int j=0;j<data_line;j++)
			{
				if (data[j]!=0)
				{
					pre[svmindex].index=svmindex+1;
					pre[svmindex].value=data[i*data_line+j];
					svmindex++;
				}
			}
			pre[svmindex].index=-1;
			pre[svmindex].value=NULL;
			classid[i]=classification.doClassify_SVM_Single(pre,data_line,svmmodel,m_svmNum);
			delete[] pre;
			
			///////////////////////////////////////////////////////////////////////////////////////
			//另一种方法 取整数 将多余的先减去
			progress++;
			int remainder = data_row % 100;
			int multiple = data_row - remainder;
			int partial = multiple / 100;
			if(progress % partial == 0)
			{
				raster_percent++;
				p_global_Progress->SetPos(raster_percent);
				CString str;
				str.Format(_T("%d%%"),raster_percent);
				p_global_Percent->SetWindowTextW(str);
			}
			if (progress == data_row || progress > 100*partial)
			{
				p_global_Progress->SetPos(100);
				p_global_Percent->SetWindowTextW(_T("100% 完成！"));
			}
			///////////////////////////////////////////////////////////////////////////////////////
		}
		m_classid=classid;
		return classid;
	}
	catch (...)
	{
		throw "Error5:分类错误！";
		return NULL;
	}



}
// 清除数据
bool GLC_SVM_Classify::GLCSVM_Clear(void)
{
	if (m_classid==NULL)
	{
		return false;
	}
	else
	{
		delete [] m_classid;
		return true;
	}

}


void GLC_SVM_Classify::GLCSVM_Set_Parameter(svm_parameter* parameter)
{
	m_svm_parameter=parameter;
}


