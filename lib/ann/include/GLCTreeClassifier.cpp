#include "StdAfx.h"
#include "Tree.h"
#include "GLCTreeClassifier.h"
#include "ModelIntegrator.h"
#include "DataClassification.h"

GLCTreeClassifier::GLCTreeClassifier(void)
	:  m_treeHeight(0)
	, adaBoostingK(0)
	, m_leafsSize(0)
	, m_adaBoostingK(0)
	, m_sampleDataRatio(0)
	, m_trainData(NULL)
	, m_trainData_row(0)
	, m_trainData_line(0)
	, m_treeSelct(NULL)
	, m_TreeSelection(NULL)
{
}


GLCTreeClassifier::~GLCTreeClassifier(void)
{
}


// ����ѵ����
void GLCTreeClassifier::CreateTrainingSet(double** trainData,int trainData_row,int trainData_line, /*CProgressCtrl* progressBar,*/int &progressBarI)//�����·����������
{
	m_trainData=trainData;
	m_trainData_row=trainData_row;
	m_trainData_line=trainData_line;
	//m_progressBar=progressBar;
	//����ѵ����
	ModelIntegrator Model;	
	Model.GetControlsParameter(m_treeHeight,m_adaBoostingK,m_leafsSize,m_sampleDataRatio);//��ò���
	//���ý�����
	int leftRage=(int)trainData_row*m_adaBoostingK;
	//m_progressBar->SetRange(1,leftRage);
	//Model.SetProgressBar(m_progressBar);
	

	//��ʼ�����Լ�
	int * attList;
	int attlist_n=trainData_line-1;
	int *y;
	y=new int[attlist_n];
	for (int i=0;i<attlist_n;i++)
	{
		y[i]=i;
	}
	attList=y;
	//ִ��ѵ��
	Model.AdaBoostingTrainingRoles(m_trainData,m_trainData_row,m_trainData_line,attList,attlist_n,progressBarI);
	int trees=0;
	int &treesR=trees;
	TreeSelection * treeSeltc;
	treeSeltc=Model.GetTreeSelection(treesR);
	//�洢ѵ����õ�������
	m_treeSelct=treeSeltc;
	

}
//void GLCTreeClassifier::CreateTrainingSet(double** trainData,int trainData_row,int trainData_line,/* CProgressCtrl* progressBar,*/int &progressBarI,CString & Eror) //��������
//{
//	m_trainData=trainData;
//	m_trainData_row=trainData_row;
//	m_trainData_line=trainData_line;
//	//m_progressBar=progressBar;
//	//����ѵ����
//	ModelIntegrator Model;	
//	Model.GetControlsParameter(m_treeHeight,m_adaBoostingK,m_leafsSize,m_sampleDataRatio);//��ò���
//	//���ý�����
//	int leftRage=(int)trainData_row*m_adaBoostingK;
//	//m_progressBar->SetRange(1,leftRage);
//	//Model.SetProgressBar(m_progressBar);
//	
//	//��ʼ�����Լ�
//	int * attList;
//	int attlist_n=trainData_line-1;
//	int *y;
//	y=new int[attlist_n];
//	for (int i=0;i<attlist_n;i++)
//	{
//		y[i]=i;
//	}
//	attList=y;
//	//ִ��ѵ��
//	Model.AdaBoostingTrainingRoles1(m_trainData,m_trainData_row,m_trainData_line,attList,attlist_n,progressBarI,Eror);
//	int trees=0;
//	int &treesR=trees;
//	TreeSelection * treeSeltc;
//	treeSeltc=Model.GetTreeSelection(treesR);
//	//�洢ѵ����õ�������
//	m_treeSelct=treeSeltc;
//	//���������
//	for (int i=progressBarI;i<=leftRage;i++)
//	{
//		// m_progressBar->SetPos(i);Sleep(0.1); 
//	}
//}
void GLCTreeClassifier::CreateTrainingSet(double** trainData,int trainData_row,int trainData_line) //����·��������
{
	m_trainData=trainData;
	m_trainData_row=trainData_row;
	m_trainData_line=trainData_line;
}
//��ȡ���Ʋ���
void GLCTreeClassifier::GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize,double sampleDataRatio)
{
	m_treeHeight=treeHeight;
	m_adaBoostingK=adaBoostingK;
	m_leafsSize=leafsSize;
	m_sampleDataRatio=sampleDataRatio;
}
void GLCTreeClassifier::GetControlsParameter(int treeHeight, int adaBoostingK,int leafsSize)
{
	m_treeHeight=treeHeight;
	m_adaBoostingK=adaBoostingK;
	m_leafsSize=leafsSize;

}


// ִ�з��෵�ط�����
short * GLCTreeClassifier::ExecuteClassify(double** data, int row, int line)
{


	int datablockSize=5000;//�������ݿ�
	if (row<100)
	{
		short *classifiedId;
		TreeSelection *treeSelct=m_treeSelct;
		int treeN=m_adaBoostingK;
		DataClassification classy(data,row,line);
		classy.doClassify(treeSelct,treeN); 
		classifiedId=classy.GetClassifiedID();
		return classifiedId;
	}

	//����������100�����ݷֿ���з��࣬
	int remainder=row%datablockSize;
	int integer=row-remainder;
	short * classifiedId;
	classifiedId=new short[row];
	int classifiedID_i=0;
	double** cursor;
	int dataRowI=0;
	//ʹ�÷���1

	if (datablockSize==1)
	{
		double **p;
		short *classifiedId_temp;
		TreeSelection *treeSelct=m_treeSelct;
		int treeN=m_adaBoostingK;
		for (int i=0;i<row;i++)
		{
			p=data+i;

			DataClassification classy(p,datablockSize,line);
			classy.doClassify(treeSelct,treeN); 
			classifiedId_temp=classy.GetClassifiedID();
			classifiedId[i]=classifiedId_temp[0];

		}
	}
	else
	{
		double **p;
		short *classifiedId_temp;
		TreeSelection *treeSelct=m_treeSelct;
		int treeN=m_adaBoostingK;
		int classifiedId_i=0;
		for (int i=0;i<integer;i+=datablockSize)
		{
			p=data+i;

			DataClassification classy(p,datablockSize,line);
			classy.doClassify(treeSelct,treeN); 
			classifiedId_temp=classy.GetClassifiedID();
			for (int j=0;j<datablockSize;j++)
			{
				classifiedId[classifiedId_i]=classifiedId_temp[j];
				classifiedId_i++;
			}

		}
		//�������µ�remainder
		for (int i=classifiedId_i;i<row;i++)
		{
			p=data+i;

			DataClassification classy(p,datablockSize,line);
			classy.doClassify(treeSelct,treeN); 
			classifiedId_temp=classy.GetClassifiedID();
			for (int j=0;j<datablockSize;j++)
			{
				classifiedId[classifiedId_i]=classifiedId_temp[j];
				classifiedId_i++;
			}

		}
		if (classifiedID_i!=row)
		{
			MessageBox(NULL,_T("���ݷ�����©��"),_T("ע�⣡"),MB_OK);
		}

	}
	TRY 
	{
		for (int i=0;i<row;i++)
		{
			delete []data[i];
		}
		delete []data;
	}
	CATCH (CMemoryException, e)
	{
		MessageBox(NULL,_T("����delete []dataλ��GLC_Classifier::ExecuteClassify"),_T("����"),MB_OK);
		return NULL;	
		
	}
	END_CATCH
	


	return classifiedId;
}
//short* GLCTreeClassifier::ExecuteClassify(double** data, int row, int line,CProgressCtrl* progressBar,int &progressBarI,short * classifiedId)
//{
//
//
//	int datablockSize=200;//�������ݿ�
//	if (row<100)
//	{
//		short *classifiedId;
//		TreeSelection *treeSelct=m_treeSelct;
//		int treeN=m_adaBoostingK;
//		DataClassification classy(data,row,line);
//		classy.doClassify(treeSelct,treeN); 
//		classifiedId=classy.GetClassifiedID();
//		return classifiedId;
//	}
//
//	//����������100�����ݷֿ���з��࣬
//	int remainder=row%datablockSize;
//	int integer=row-remainder;
//	//���ý�����
//	int leftRage=(int)row/200+1;
//	//progressBar->SetRange(1,leftRage);
//	if (classifiedId==NULL)
//	{
//		classifiedId=new short[row];
//	}
//	//short * classifiedId;
//	
//	int classifiedID_i=0;
//	double** cursor;
//	int dataRowI=0;
//	//ʹ�÷���1
//
//	if (datablockSize==1)
//	{
//		MessageBox(NULL,_T("���ݿ鲻��Ϊ1��"),_T("ע�⣡"),MB_OK);
//	}
//	else
//	{
//		double **p;
//		short *classifiedId_temp;
//		TreeSelection *treeSelct=m_treeSelct;
//		int treeN=m_adaBoostingK;
//		int classifiedId_i=0;
//		for (int i=0;i<integer;i+=datablockSize)
//		{
//			p=data+i;
//			TRY 
//			{				
//				DataClassification classy(p,datablockSize,line);//MessageBox(NULL,temp,_T("����"),MB_OK);
//				classy.doClassify(treeSelct,treeN); 
//
//
//				classifiedId_temp=classy.GetClassifiedID();
//				if (classifiedId_temp==NULL)
//				{
//					return NULL;
//				}
//				for (int j=0;j<datablockSize;j++)
//				{
//					classifiedId[classifiedId_i]=classifiedId_temp[j];
//					classifiedId_i++;
//					progressBarI=progressBarI+1;
//
//				}
//				delete [] classifiedId_temp;
//				//classy.Delete();
//				//���ý�����
//				//progressBarI=progressBarI+1;
//
//
//
//				//progressBar->SetPos(progressBarI);Sleep(0.1);
//
//			}
//			CATCH (CMemoryException, e)
//			{
//				double pp[100][8];
//				for (int ii=0;ii<100;ii++)
//				{
//					for (int j=0;j<8;j++)
//					{
//						pp[ii][j]=p[ii][j];
//					}
//				}
//				CString temp;
//				temp.Format(_T("%d"),i);
//				CString st=_T("���ݳ�������Χ")+temp;
//				MessageBox(NULL,st,_T("����"),MB_OK);
//				return NULL;
//			}
//			END_CATCH
//				
//
//		}
//		//�������µ�remainder
//		p=data+classifiedId_i;
//		DataClassification classy(p,remainder,line);
//		classy.doClassify(treeSelct,treeN); 
//		classifiedId_temp=classy.GetClassifiedID();
//		for (int j=0;j<remainder;j++)
//		{
//
//			classifiedId[classifiedId_i]=classifiedId_temp[j];
//			classifiedId_i++;
//			progressBarI=progressBarI+1;
//		}
//		//���ý�����
//		//progressBarI=progressBarI+1;
//		//progressBar->SetPos(progressBarI);Sleep(0.1);
//	}
//	
//
//	return classifiedId;
//}

// �������
TreeSelection* GLCTreeClassifier::GetTreeSelection(void)
{
	return m_treeSelct;
}


// ʹ�õ���ֵԤ��ķ��෽��
short* GLCTreeClassifier::ExcuteClassifySingle(double** data, int data_row, int data_line,int & progresI)
{
	short *classifiedId;
	classifiedId=new short[data_row];
	TreeSelection *treeSelct=m_treeSelct;
	int treeN=m_adaBoostingK;
	DataClassification classy;
	for (int i=0;i<data_row;i++)
	{
		classifiedId[i]=classy.doClassifySingle(data[i],treeSelct,treeN);
		progresI++;
	}
	return classifiedId;
}



