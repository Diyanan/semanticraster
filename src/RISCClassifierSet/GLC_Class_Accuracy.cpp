#include "StdAfx.h"
#include "GLC_Class_Accuracy.h"
//*********************************************//
#include <iostream>
//using namespace std;
//*********************************************//


GLC_Class_Accuracy::GLC_Class_Accuracy(void)
{
}


GLC_Class_Accuracy::~GLC_Class_Accuracy(void)
{
}
ClassUnique1 * GLC_Class_Accuracy::Acry_Get_ClassUnique(AttValuesTab * attValues, int attNum)  //注意最后还要编写销毁的函数
{
	if (attNum==0)
	{
		//	MessageBox(NULL,_T("错误！数据不能为空！"),_T("注意！"),MB_OK);
		return NULL;
	}
	int n=0;
	n=attNum;
	ClassUnique1 *first,*r,*s,*p;
	//初始化一个链表
	first=new ClassUnique1;
	first->next=NULL;
	r=first;
	p=first;

	int m=0;//用于记录实际使用节点的个数

	for (int i=0;i<attNum;i++)
	{
		if (i==0)//将列表初始化
		{   
			m++;//每新建一个节点m+1
			s=new ClassUnique1;
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
					s=new ClassUnique1;
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
ClassUnique1 * GLC_Class_Accuracy::Acry_Get_ClassUnique(AttValuesTab * attValues, int attNum,short**classfiedID,int nClassifier)  //注意最后还要编写销毁的函数
{
	if (attNum==0)
	{
		//	MessageBox(NULL,_T("错误！数据不能为空！"),_T("注意！"),MB_OK);
		return NULL;
	}
	int n=0;
	n=attNum;
	ClassUnique1 *first,*r,*s,*p;
	//初始化一个链表
	first=new ClassUnique1;
	first->next=NULL;
	r=first;
	p=first;

	int m=0;//用于记录实际使用节点的个数
	//int sum=0;
	for (int i=0;i<attNum;i++)
	{
		if (i==0)//将列表初始化
		{   
			m++;//每新建一个节点m+1
			s=new ClassUnique1;
			s->classID=attValues[0].classID;	
			s->cunt=1;
			s->dataIndex.push_back(i);
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
					p->dataIndex.push_back(i);					
					p=first->next;
					break;//计数器加1，跳出循环进行下一个类别ＩＤ的对比
				}
				if (p==r)//如果P走到了链表尾部
				{	
					if (p->classID==attValues[i].classID)//如果列表节点的ID与当前的类别ID相等
					{
						p->cunt=p->cunt+1;
						p->dataIndex.push_back(i);
						p=first->next;
						break;//计数器加1，跳出循环进行下一个类别ＩＤ的对比
					}
					m++;//每新建一个节点m+1
					s=new ClassUnique1;
					s->classID=attValues[i].classID;	
					s->cunt=1;
					s->dataIndex.push_back(i);
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
	////////////////////////////////通过链表获得混淆矩阵///////////////////////////////////////////////
	//建立各个分类器的混淆矩阵,并初始化
	ClassUnique1 * mixp=first->next;
	m_confusionMatrix=new ConfusionMatrix[nClassifier];
	for (int i=0;i<nClassifier;i++)
	{
		m_confusionMatrix[i].nClass=first->cunt;
		int ** tempMatirx=new int *[first->cunt+1];
		for (int j=0;j<first->cunt+1;j++)
		{
			tempMatirx[j]=new int [first->cunt+1];
			for (int k=0;k<first->cunt+1;k++)
			{
				tempMatirx[j][k]=0;
			}
		}
		m_confusionMatrix[i].matrix=tempMatirx;
		m_confusionMatrix[i].classifierID=i;
	}
	//填充混淆矩阵
	int sum=0;
	int nclassI=0;
	//cout<<"各个类别的个数："<<endl;
	while (mixp)
	{
		int dataNum=mixp->dataIndex.size();
		//cout<<"classid="<<mixp->classID<<":"<<mixp->cunt<<", ";
		for (int i=0;i<dataNum;i++)
		{
			int tempindex=mixp->dataIndex.at(i);
			for (int j=0;j<nClassifier;j++)
			{
				int targetClassID=attValues[tempindex].classID;
				int targetClassID_index=targetClassID-1;
				int classifiedClassID=classfiedID[j][tempindex];
				int classifiedClassID_index=classifiedClassID-1;
				//通过targetClassID_index和classifiedClassID_index寻找混淆矩阵中的位置
				m_confusionMatrix[j].matrix[classifiedClassID_index][targetClassID_index]++;
			}
		}
		mixp=mixp->next;
		nclassI++;
		//if (i==0)    //避开头节点
		//{
		//	p=p->next;
		//	i++;
		//}
		//else
		//{
		//	int tempnum=p->cunt;
		//	int tempclassid_i=p->classID-1;
		//	num_in_evryclass[tempclassid_i]=tempnum;			
		//	p=p->next;
		//	class_num_i++;
		//	i++;
		//}
	};
	//cout<<endl;
	for (int nclassf=0;nclassf<nClassifier;nclassf++)
	{
		for (int i=0;i<first->cunt;i++)
		{
			int sumi=0,sumj=0;
			for (int j=0;j<first->cunt;j++)
			{
				sumi=sumi+m_confusionMatrix[nclassf].matrix[i][j];
				sumj=sumj+m_confusionMatrix[nclassf].matrix[j][i];
			}
			m_confusionMatrix[nclassf].matrix[i][first->cunt]=sumi;
			m_confusionMatrix[nclassf].matrix[first->cunt][i]=sumj;
		}
		m_confusionMatrix[nclassf].matrix[first->cunt][first->cunt]=first->classID;
	}
	
	

	//cout<<"测试总个数是："<<sum<<endl<<"nclassi="<<nclassI<<endl;
	return first;
}

void  GLC_Class_Accuracy::Acry_Set_ClassAcryMatrix(int*data_tag/*类标签*/,int data_row/*数据行数*/,short **class_id/*分类之后的类标号*/,int& class_num/*类个数*/,int classifier_num/*分类器个数*/)
{
	//通过类标签设置参数用于统计类别的个数和各个类的元组数，链表的第一项除外
	AttValuesTab * attValues=new AttValuesTab[data_row];
	for (int i=0;i<data_row;i++)
	{
		attValues[i].AttValue=-1;
		attValues[i].AttValueIndex=0;
		attValues[i].classID=data_tag[i];
	}
	ClassUnique1 * classUnique_linck=Acry_Get_ClassUnique(attValues,data_row);
	class_num=classUnique_linck->cunt;//获得类别的个数
	//建立一维数组，从链表中读取各类的元组个数
	int * num_in_evryclass=new int[class_num];
	int class_num_i=0;
	ClassUnique1 * p=classUnique_linck;
	int i=0;
	while (p)
	{
		if (i==0)    //避开头节点
		{
			p=p->next;
			i++;
		}
		else
		{
			int tempnum=p->cunt;
			int tempclassid_i=p->classID-1;
			num_in_evryclass[tempclassid_i]=tempnum;			
			p=p->next;
			class_num_i++;
			i++;
		}
	};
	//************************************************************//
	/*cout<<"<-----------------Acry_Set_ClassAcryMatrix: num_in_evryclass------------------------>"<<endl;
	for (int i=0;i<class_num;i++)
	{
		cout<<"class"<<i<<":"<<num_in_evryclass[i]<<" ";
		if (i==2)
		{
			cout<<endl;
		}

	}
	cout<<endl;*/

	//************************************************************//
	
	//建立二维数组，用于记录各个类别分错的个数
	int ** errornum_in_evryclass=new int *[classifier_num];
	for (int i=0;i<classifier_num;i++)
	{
		errornum_in_evryclass[i]=new int [class_num];
		//初始化个元素为0
		for (int j=0;j<class_num;j++)
		{
			errornum_in_evryclass[i][j]=0;
		}
	}
	//为记录各个类别错误个数的二维数组赋值
	for (int i=0;i<classifier_num;i++)
	{
		for (int j=0;j<data_row;j++)
		{
			int temptag=data_tag[j];
			int tempId=class_id[i][j];
			if (temptag!=tempId)
			{
				int tempId_i=temptag-1;
				errornum_in_evryclass[i][tempId_i]++;
			}
		}
	}
	//建立二维数组用于得到类别精度,并赋值
	float ** class_accuracy_matrix=new float *[classifier_num];
	for (int i=0;i<classifier_num;i++)
	{
		class_accuracy_matrix[i]=new float[class_num];		
		for (int j=0;j<class_num;j++)
		{
			int temp_numinclass=num_in_evryclass[j];
			int temp_erronum=errornum_in_evryclass[i][j];
			double temp_accuracy=(temp_numinclass-temp_erronum)*1.0/temp_numinclass;
			class_accuracy_matrix[i][j]=temp_accuracy;
		}
	}
	m_class_accuracy_matrix=class_accuracy_matrix;
	m_class_num=class_num;
	m_classifier_num=classifier_num;
	m_errornum_in_evryclass=errornum_in_evryclass;
	m_num_in_evryclass=num_in_evryclass;
	//清理内存
	delete [] attValues;
	Acry_Delete_ClassUnique(classUnique_linck);
	//delete [] num_in_evryclass;
	/*for (int i=0;i<classifier_num;i++)
	{
		delete []errornum_in_evryclass[i];
	}
	delete [] errornum_in_evryclass;*/

}
void  GLC_Class_Accuracy::Acry_Set_ClassConfusionMatrix(int*data_tag/*类标签*/,int data_row/*数据行数*/,short **class_id/*分类之后的类标号*/,int& class_num/*类个数*/,int classifier_num/*分类器个数*/)
{
	//通过类标签设置参数用于统计类别的个数和各个类的元组数，链表的第一项除外
	AttValuesTab * attValues=new AttValuesTab[data_row];
	for (int i=0;i<data_row;i++)
	{
		attValues[i].AttValue=-1;
		attValues[i].AttValueIndex=i;
		attValues[i].classID=data_tag[i];
	}
	ClassUnique1 * classUnique_linck=Acry_Get_ClassUnique(attValues,data_row,class_id,classifier_num);
	class_num=classUnique_linck->cunt;//获得类别的个数
	//建立一维数组，从链表中读取各类的元组个数
	int * num_in_evryclass=new int[class_num];
	int class_num_i=0;
	ClassUnique1 * p=classUnique_linck;
	int i=0;
	while (p)
	{
		if (i==0)    //避开头节点
		{
			p=p->next;
			i++;
		}
		else
		{
			int tempnum=p->cunt;
			int tempclassid_i=p->classID-1;
			num_in_evryclass[tempclassid_i]=tempnum;			
			p=p->next;
			class_num_i++;
			i++;
		}
	};
	//************************************************************//
	/*cout<<"<-----------------Acry_Set_ClassAcryMatrix: num_in_evryclass------------------------>"<<endl;
	for (int i=0;i<class_num;i++)
	{
		cout<<"class"<<i<<":"<<num_in_evryclass[i]<<" ";
		if (i==2)
		{
			cout<<endl;
		}

	}
	cout<<endl;*/

	//************************************************************//
	
	//建立二维数组，用于记录各个类别分错的个数
	int ** errornum_in_evryclass=new int *[classifier_num];
	for (int i=0;i<classifier_num;i++)
	{
		errornum_in_evryclass[i]=new int [class_num];
		//初始化个元素为0
		for (int j=0;j<class_num;j++)
		{
			errornum_in_evryclass[i][j]=0;
		}
	}
	//为记录各个类别错误个数的二维数组赋值
	for (int i=0;i<classifier_num;i++)
	{
		for (int j=0;j<data_row;j++)
		{
			int temptag=data_tag[j];
			int tempId=class_id[i][j];
			if (temptag!=tempId)
			{
				int tempId_i=temptag-1;
				errornum_in_evryclass[i][tempId_i]++;
			}
		}
	}
	//建立二维数组用于得到类别精度,并赋值
	float ** class_accuracy_matrix=new float *[classifier_num];
	for (int i=0;i<classifier_num;i++)
	{
		class_accuracy_matrix[i]=new float[class_num];		
		for (int j=0;j<class_num;j++)
		{
			int temp_numinclass=num_in_evryclass[j];
			int temp_erronum=errornum_in_evryclass[i][j];
			double temp_accuracy=(temp_numinclass-temp_erronum)*1.0/temp_numinclass;
			class_accuracy_matrix[i][j]=temp_accuracy;
		}
	}
	m_class_accuracy_matrix=class_accuracy_matrix;
	m_class_num=class_num;
	m_classifier_num=classifier_num;
	m_errornum_in_evryclass=errornum_in_evryclass;
	m_num_in_evryclass=num_in_evryclass;
	//清理内存
	delete [] attValues;
	Acry_Delete_ClassUnique(classUnique_linck);
	//Acry_Delete_ClassUnique(classUnique_linck);
	//delete [] num_in_evryclass;
	/*for (int i=0;i<classifier_num;i++)
	{
		delete []errornum_in_evryclass[i];
	}
	delete [] errornum_in_evryclass;*/

}
ConfusionMatrix *  GLC_Class_Accuracy::Acry_Get_ClassConfusionAcfyMatrix()
{
	for (int nClassifie=0;nClassifie<m_classifier_num;nClassifie++)
	{
		int ** tempConfusionM=m_confusionMatrix[nClassifie].matrix;
		float ** tempAcfyM=new float *[m_class_num];
		for (int i=0;i<m_class_num;i++)
		{
			
			tempAcfyM[i]=new float[m_class_num];
			for (int j=0;j<m_class_num;j++)
			{
				int nValuAll=tempConfusionM[m_class_num][j];
				int nValuIn=tempConfusionM[i][j];
				tempAcfyM[i][j]=nValuIn*1.0/nValuAll;
			}
		}
		m_confusionMatrix[nClassifie].acryMatrix=tempAcfyM;
		//m_confusionMatrix[nClassifie].acryMatrix=new float *;

	}
	
	
	return m_confusionMatrix;
}
void GLC_Class_Accuracy::Acry_Delete_ClassUnique(ClassUnique1 *classLick) //清除唯一值链表
{
	ClassUnique1 *L,*p,*q;
	L=classLick;
	p=L;
	q=L;
	while (p)
	{
		q=p;//暂存被释放节点
		p=p->next;
		q->dataIndex.clear();		
		delete q;
	}
}
void  GLC_Class_Accuracy::Acry_Delete_ClassAcryMatrix()
{
	if (m_class_accuracy_matrix==NULL||m_errornum_in_evryclass==NULL||m_num_in_evryclass==NULL)
	{
		return;
	}
	for (int i=0;i<m_classifier_num;i++)
	{
		delete[] m_class_accuracy_matrix[i];
	}
	delete [] m_class_accuracy_matrix;
	for (int i=0;i<m_classifier_num;i++)
	{
		delete[] m_errornum_in_evryclass[i];
	}
	delete[] m_errornum_in_evryclass;
    delete[] m_num_in_evryclass;

}
float**  GLC_Class_Accuracy::Acry_Get_ClassAcryMatrix()
{
	if (m_class_accuracy_matrix==NULL)
	{
		return NULL;
	}
	return m_class_accuracy_matrix;
	
}
ConfusionMatrix *  GLC_Class_Accuracy::Acry_Get_ClassConfusionMatrix()
{
	if (m_confusionMatrix==NULL)
	{
		return NULL;
	}
	return m_confusionMatrix;

}
int **  GLC_Class_Accuracy::Acry_Get_ClassErrMatrix()
{
	if (m_errornum_in_evryclass==NULL)
	{
		return NULL;
	}
	return m_errornum_in_evryclass;
}
int *  GLC_Class_Accuracy::Acry_Get_ClassNumMatrix()
{
	if (m_num_in_evryclass==NULL)
	{
		return NULL;
	}
	return m_num_in_evryclass;
}
// 获得信任度矩阵
ConfusionMatrix* GLC_Class_Accuracy::Arcy_Get_ClassConfidenceMatrix(void)
{
	for (int nClassifie=0;nClassifie<m_classifier_num;nClassifie++)
	{
		int ** tempConfusionM=m_confusionMatrix[nClassifie].matrix;
		float ** tempConfidenceM=new float *[m_class_num];
		for (int i=0;i<m_class_num;i++)
		{

			tempConfidenceM[i]=new float[m_class_num];
			for (int j=0;j<m_class_num;j++)
			{
				int nValuAll=tempConfusionM[i][m_class_num];
				int nValuIn=tempConfusionM[i][j];
				if (nValuAll==0)
				{
					tempConfidenceM[i][j]=0;
					continue;
				}
				tempConfidenceM[i][j]=nValuIn*1.0/nValuAll;
			/*	float aa=nValuIn*1.0/nValuAll;
				int a=0;
				int b;*/
			}
		}
		m_confusionMatrix[nClassifie].confidenceMatrix=tempConfidenceM;
		//m_confusionMatrix[nClassifie].acryMatrix=new float *;

	}


	return m_confusionMatrix;

}

bool GLC_Class_Accuracy::Arcy_Clear(void)
{/*
 float ** m_class_accuracy_matrix;
 int m_class_num;
 int m_classifier_num;
 int **m_errornum_in_evryclass;
 int *m_num_in_evryclass;
 ConfusionMatrix * m_confusionMatrix;*/

	if(m_confusionMatrix==NULL)
	{
		return false;
	}
	for (int i=0;i<m_classifier_num;i++)
	{
		int** tempM=m_confusionMatrix[i].matrix;
		if (tempM==NULL)
		{
			return false;
		}
		for (int j=0;j<=m_class_num;j++)
		{
			delete [] tempM[j];
		}
		delete[] tempM;

		float **tempAM=m_confusionMatrix[i].acryMatrix;
		if (tempAM==NULL)
		{
			return false;
		}
		for (int j=0;j<m_class_num;j++)
		{
			delete [] tempAM[j];
		}
		delete[] tempAM;
		
		if (m_confusionMatrix[i].confidenceMatrix==NULL)
		{
			break;
		}
		for (int j=0;j<m_class_num;j++)
		{
			delete [] m_confusionMatrix[i].confidenceMatrix[j];
		}
		delete [] m_confusionMatrix[i].confidenceMatrix;

	}
	delete []m_confusionMatrix;
	if (m_class_accuracy_matrix!=NULL)
	{
		for (int i=0;i<m_classifier_num;i++)
		{
			delete [] m_class_accuracy_matrix[i];
		}
		delete [] m_class_accuracy_matrix;
	}
	
	if (m_errornum_in_evryclass!=NULL)
	{
		for (int i=0;i<m_classifier_num;i++)
		{
			delete [] m_errornum_in_evryclass[i];
		}
		delete []m_errornum_in_evryclass;
	}
	if (m_num_in_evryclass!=NULL)
	{
		delete [] m_num_in_evryclass;
	}
	
	return true;
}
