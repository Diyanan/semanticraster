#include "StdAfx.h"
#include "GLC_Classifier_BeIntegrated.h"

#include <iostream>
using namespace  std;

ClassUnique * getClassUnique(AttValuesTab * attValues, int attNum) ;
void deleteClassUnique(ClassUnique *classLick) ;
GLC_Classifier_BeIntegrated::GLC_Classifier_BeIntegrated(void)
{
}


GLC_Classifier_BeIntegrated::~GLC_Classifier_BeIntegrated(void)
{
}
void GLC_Classifier_BeIntegrated::Integrated_Set_Parameter(TreeParameter treeparameter,svm_parameter svmparameter,glc_ann_parameter annparameter)
{
	m_treeparameter=treeparameter;
	m_svmparameter=svmparameter;
	m_annparameter=annparameter;
}

void GLC_Classifier_BeIntegrated::Integrated_Set_Data(double** data, int data_row , int data_line)
{
	m_data=data;
	m_data_row=data_row;
	m_data_line=data_line;
}
bool  GLC_Classifier_BeIntegrated::Integrated_Execut_Train(int& progressI,double **testdata,int testedata_row,int testedata_line,int intTyp)
{
	try
	{
		/************************************************************************/
		/*                          建立单个分类器此处建立决策树，knn，bayes ,svm  */
		/************************************************************************/
		//训练决策树
		int tmp=0;
		m_model_tree=new GLC_Tree();

		m_model_tree->SetParameter(m_treeparameter);
		m_model_tree->GetData(m_data,m_data_row,m_data_line);
		m_model_tree->ExecutTrain(tmp);

		//训练KNN模型
		m_modle_knn=new GLC_Knn();
		m_modle_knn->GLC_Knn_Train(m_data,m_data_row,m_data_line);
		
		//训练Bayes模型
		m_modle_bayes=new GLC_Bayes();
		m_modle_bayes->GLC_Bayes_Train(m_data,m_data_row,m_data_line);
		//训练SVM
		m_classifier=&m_classifier_svm;
		m_classifier->GetData(m_data,m_data_row,m_data_line);
		/*************为了在release下运行************************/
		svm_parameter * svmparameter=new svm_parameter;
		*svmparameter=m_svmparameter;
		m_classifier->SetParameter(svmparameter);
		/**********************************************/

		m_classifier->ExecutTrain(progressI);
		////训练ANN  wgh
		m_classifier=&m_classifier_ann;
		m_classifier->GetData(m_data,m_data_row,m_data_line);
		m_classifier->SetParameter(&m_annparameter);
		m_classifier->ExecutTrain(progressI);

		/************************************************************************/
		/*                          获取多分类器分类集成的专家                  */
		/************************************************************************/
		//分类测试
		//决策树测试
		int * classTag=new int[testedata_row];
		short * testID_Dtree=new short[testedata_row];
		short * testID_KNN=new short[testedata_row];
		short * testID_Bayes=new short[testedata_row];
		short * testID_SVM=new short[testedata_row];

		for (int i=0;i<testedata_row;i++)
		{
			int N=testedata_line-1;
			double *tdata=new double[N];
			for (int j=0;j<N;j++)
			{
				tdata[j]=testdata[i][j];
			}
			//分类测试
			testID_Dtree[i]=m_model_tree->ExecutClassify(tdata);
			testID_KNN[i]=m_modle_knn->GLC_Knn_ExcuteClassify(tdata);
			//testID_Bayes[i]=m_modle_knn->GLC_Knn_ExcuteClassify(tdata);
			testID_Bayes[i]=m_modle_bayes->GLC_Bayes_ExcuteClassify(tdata);
			
			int svmindex=0;	
			svm_node* tempdata=new svm_node[N];
			for (int j=0;j<N-1;j++)
			{

				if (tdata[j]!=0)
				{
					tempdata[svmindex].index=j+1;
					tempdata[svmindex].value=tdata[j];
					svmindex++;
				}
				tempdata[svmindex].index=-1;
				tempdata[svmindex].value=NULL;
				//cout<<"*";
			}

			testID_SVM[i]=m_classifier_svm.ExecutClassify(tempdata);
			//初始化类别标签
			classTag[i]=testdata[i][testedata_line-1];

			

		}

		//cout<<endl;
		short** classfiedIDs=new short*[4];
		classfiedIDs[0]=testID_Dtree;
		classfiedIDs[1]=testID_KNN;
		classfiedIDs[2]=testID_Bayes;
		classfiedIDs[3]=testID_SVM;

		//初始化Accuracy，以进行专家系统分类

		int classnum=6;
		int& num=classnum;
		if (testdata!=NULL)
		{


			m_class_accuracy.Acry_Set_ClassAcryMatrix(classTag,testedata_row,classfiedIDs,num,4);
			m_class_accuracy.Acry_Set_ClassConfusionMatrix(classTag,testedata_row,classfiedIDs,num,4);
			m_cofusionMatrix=m_class_accuracy.Acry_Get_ClassConfusionAcfyMatrix();

			
			m_class_num=classnum;
			m_child_classifier_accuracy=new float[4];
			for (int k=0;k<4;k++ )
			{
				float sum=0;
				for (int i=0;i<m_class_num;i++)
				{
					sum+=m_cofusionMatrix[k].matrix[i][i];
				}
				int allnum=m_cofusionMatrix[k].matrix[m_class_num][m_class_num];
				m_child_classifier_accuracy[k]=sum*1.0/allnum;
			}


		}
		else
		{
			m_class_accuracy.Acry_Set_ClassAcryMatrix(classTag,m_data_row,m_calss_acryMatix_input,num);
			m_class_accuracy.Acry_Set_ClassConfusionMatrix(classTag,m_data_row,m_calss_acryMatix_input,num);
			m_cofusionMatrix=m_class_accuracy.Acry_Get_ClassConfusionAcfyMatrix();
			m_class_num=classnum;
			m_child_classifier_accuracy=new float[4];
			for (int k=0;k<4;k++ )
			{
				float sum=0;
				for (int i=0;i<m_class_num;i++)
				{
					sum+=m_cofusionMatrix[k].matrix[i][i];
				}
				int allnum=m_cofusionMatrix[k].matrix[m_class_num][m_class_num];
				m_child_classifier_accuracy[k]=sum*1.0/allnum;
			}
			m_istest=true;

		}

		/*m_ClassAcryMatrix=m_class_accuracy.Acry_Get_ClassAcryMatrix();
		for (int i=0;i<4;i++)
		{
			cout<<i<<"-----------------------------------------"<<endl;
			for (int j=0;j<6;j++)
			{
				cout<<m_ClassAcryMatrix[i][j]<<",";
			}
			cout<<endl;
		}*/

		return true;
	}
	catch (std::exception* e)
	{
		return false;
	}
	
	

}
short GLC_Classifier_BeIntegrated::Integrated_Execut_Classify_INT1(double* data, int data_line,int* classID_selection)
{
	int classId_tree=0,classId_knn=0,classId_bayes=0,classId_svm=0;
	//分类
	classId_tree=m_model_tree->ExecutClassify(data);
	classId_knn=m_modle_knn->GLC_Knn_ExcuteClassify(data);
	classId_bayes=m_modle_bayes->GLC_Bayes_ExcuteClassify(data);


	int *classifierIndex=m_classifierIndex;
	int nClassifier=3;
	int * classIDs=new int[nClassifier];

	classIDs[0]=classId_tree;
	classIDs[1]=classId_knn;
	classIDs[2]=classId_bayes;


	//进行集成
	
	vector<double> vote;
	AttValuesTab* D=new AttValuesTab[nClassifier];
	for (int i=0;i<nClassifier;i++)
	{
		D[i].classID=classIDs[i];
		D[i].AttValue=0;   
		D[i].AttValue=0;  

		int classID=classIDs[i];
		int nClass=m_cofusionMatrix[i].nClass;
		int index=classID-1;
		double a=m_cofusionMatrix[i].matrix[index][index];
		double b=m_cofusionMatrix[i].matrix[index][nClass];
		double v=a/b;
		vote.push_back(v);//给每个分类器赋予权重
	}	                       

	ClassUnique * L,*p;
	L=getClassUnique(D,nClassifier);
	p=L->next;
	int nClass=L->cunt;
	vector<UniuqStruct> uClass;int uIndex=0;
	while(p)
	{
		UniuqStruct utemp;
		utemp.index=uIndex++;
		utemp.classID=p->classID;
		utemp.nClassSample=p->cunt;
		utemp.weight=0;
		int classID=utemp.classID;
		int cunt=p->cunt;
		double v=0;
		for (int i=0;i<nClassifier;i++)
		{
			int tempindex=0;
			if (classID==classIDs[i])
			{
				//找出该类在类别索引中的位置
				tempindex=classID-1;
				v=v+vote.at(i);
			}
		}		
		utemp.weight=v;
		uClass.push_back(utemp);
		p=p->next;
	}
	double maxV=0;
	int maxClassID=0;
	for (int i=0;i<uClass.size();i++)
	{
		if (maxV<uClass.at(i).weight)
		{
			maxV=uClass.at(i).weight;
			maxClassID=uClass.at(i).classID;
		}
	}
	if (classID_selection!=NULL)
	{
		for (int i=0;i<m_nClassifier;i++)
		{
			classID_selection[i]=classIDs[i];
		}
	}

	delete [] D;
	deleteClassUnique(L);
	delete [] classIDs;

	return maxClassID;



	

	//if (classId_bayes==classId_tree)
	//{
	//	return classId_knn;
	//}
	////获得tree的专家
	//double tree_exper=m_ClassAcryMatrix[0][classId_tree-1];
	//double knn_exper=m_ClassAcryMatrix[1][classId_knn-1];
	//double bayes_exper=m_ClassAcryMatrix[2][classId_bayes-1];
	//if (tree_exper>=bayes_exper)
	//{
	//	return tree_exper;
	//}
	//else
	//{
	//	return bayes_exper;
	//}

	////if (classId_knn==classId_tree)
	////{
	////	return classId_tree;
	////}
	//////获得tree的专家
	////double tree_exper=m_ClassAcryMatrix[0][classId_tree-1];
	////double knn_exper=m_ClassAcryMatrix[1][classId_knn-1];
	////if (tree_exper>=knn_exper)
	////{
	////	return classId_tree;
	////}
	////else
	////{
	////	return classId_knn;
	////}
}

short GLC_Classifier_BeIntegrated::Integrated_Execut_Classify_INT2(double* data, int data_line,int* classID_selection)
{
	int classId_tree=0,classId_knn=0,classId_svm=0;
	//分类
	classId_tree=m_model_tree->ExecutClassify(data);
	classId_knn=m_modle_knn->GLC_Knn_ExcuteClassify(data);


	int svmindex=0;	
	int N=data_line;
	svm_node* tempdata=new svm_node[N];
	for (int j=0;j<N-1;j++)
	{

		if (data[j]!=0)
		{
			tempdata[svmindex].index=j+1;
			tempdata[svmindex].value=data[j];
			svmindex++;

			//cout<<"*";
		}
		tempdata[svmindex].index=-1;
		tempdata[svmindex].value=NULL;
	}

	classId_svm=m_classifier_svm.ExecutClassify(tempdata);
	delete [] tempdata;


	int *classifierIndex=m_classifierIndex;
	int nClassifier=3;
	int * classIDs=new int[nClassifier];

	classIDs[0]=classId_tree;
	classIDs[1]=classId_knn;
	classIDs[2]=classId_svm;


	//进行集成

	vector<double> vote;
	AttValuesTab* D=new AttValuesTab[nClassifier];
	for (int i=0;i<nClassifier;i++)
	{
		
		D[i].classID=classIDs[i];
		D[i].AttValue=0;   
		D[i].AttValue=0;  

		int classID=classIDs[i];
		int nClass=m_cofusionMatrix[i].nClass;
		int index=classID-1;
		if (i==2)
		{
			i++;//跳过bayes,转到svm
		}
		double a=m_cofusionMatrix[i].matrix[index][index];
		double b=m_cofusionMatrix[i].matrix[index][nClass];
		double v=a/b;
		vote.push_back(v);//给每个分类器赋予权重

		//cout<<"*";
	}	                       

	ClassUnique * L,*p;
	L=getClassUnique(D,nClassifier);
	p=L->next;
	int nClass=L->cunt;
	vector<UniuqStruct> uClass;int uIndex=0;
	while(p)
	{
		UniuqStruct utemp;
		utemp.index=uIndex++;
		utemp.classID=p->classID;
		utemp.nClassSample=p->cunt;
		utemp.weight=0;
		int classID=utemp.classID;
		int cunt=p->cunt;
		double v=0;
		for (int i=0;i<nClassifier;i++)
		{
			int tempindex=0;
			if (classID==classIDs[i])
			{
				//找出该类在类别索引中的位置
				tempindex=classID-1;
				v=v+vote.at(i);
			}

			//cout<<"*";
		}		
		utemp.weight=v;
		uClass.push_back(utemp);
		p=p->next;
	}
	double maxV=0;
	int maxClassID=0;
	for (int i=0;i<uClass.size();i++)
	{
		if (maxV<uClass.at(i).weight)
		{
			maxV=uClass.at(i).weight;
			maxClassID=uClass.at(i).classID;
		}
		//cout<<"*";
	}
	if (classID_selection!=NULL)
	{
		for (int i=0;i<m_nClassifier;i++)
		{
			classID_selection[i]=classIDs[i];
		}
		//cout<<"*";
	}

	delete [] D;
	deleteClassUnique(L);
	delete [] classIDs;

	return maxClassID;

}
ClassUnique * getClassUnique(AttValuesTab * attValues, int attNum) 
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

void deleteClassUnique(ClassUnique *classLick) 
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