#include "StdAfx.h"
#include "DataFilling.h"


DataFilling::DataFilling(double** data, int row , int line/*数据的行和列*/)
	: m_SVMnode(NULL)
	, m_SVMnode_row(0)
	, m_SVMnode_line(0)
	, m_sampleClassId(NULL)
	, m_svmnode_forone(NULL)
{
	/*m_SVMnode=*/SetSVMNodes(data,row,line);
}


DataFilling::~DataFilling(void)
{
	
}


// 获取SVM_node得到一个样本的特征
svm_node* DataFilling::SetSVM_node(double *characteristic,int characteristic_Num)
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
svm_node** DataFilling::SetSVMNodes(double** data, int row , int line)
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
svm_node**DataFilling::GetSVMNodes()
{
	if (m_SVMnode==NULL)
	{
		return NULL;
	}
	return m_SVMnode;
}

// 存储本次参加运算的所有样本（数据集）
svm_problem DataFilling::GetSVMproblem(double* SampleClassId)
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
void DataFilling::SetSVMparameter(	int svm_type,//SVM类型，见前enum
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
svm_parameter DataFilling::GetSVMparameter(void)
{
	return m_svm_parameter;
}

//获得样本数据的目标类别
double * DataFilling::GetSampleTargetClassId(void)
{
	return m_sampleClassId;
}


// 清除数据
void DataFilling::DataCleaner(void)
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
