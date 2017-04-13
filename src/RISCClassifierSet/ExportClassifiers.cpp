#include "StdAfx.h"
#include "ExportClassifiers.h"
#include <fstream>

RISC_Classifier_ANN * NewClassifier_ANN()
{
	ann=new RISC_Classifier_ANN();
	
	
	return ann;
}
RISC_Classifier_Bayes * NewClassifier_Bayes()
{
	bayes=new RISC_Classifier_Bayes();
	
	return bayes;
}


RISC_Classifier_Tree *NewClassifier_Tree()
{
	tree=new RISC_Classifier_Tree();
	
	return tree;
}

RISC_Classifier_KNN  *NewClassifier_KNN()
{
	knn=new RISC_Classifier_KNN();
	
	return knn;
}
RISC_Classifier_SVM  *NewClassifier_SVM()
{
	svm=new RISC_Classifier_SVM();
	return svm;
}
RISC_Classifier_JC1  *NewClassifier_JC1()
{
	jc1=new RISC_Classifier_JC1();
	return jc1;
}
RISC_Classifier_JC2  *NewClassifier_JC2()
{
	jc2=new RISC_Classifier_JC2();
	return jc2;
}
 RISC_Classifier_AdTree  *NewClassifier_AdTree()
 {
	 adTree=new RISC_Classifier_AdTree();
	 return adTree;
 }
 

int DestroyModel(RISC_Classifier * classifier)
{
	
	delete classifier;
	return 0;
}

/************************************************************************/
/* �������۽ӿ�                                                         */
/************************************************************************/

void SetClassfyTag(int *Tag,int n/*�������ݵĸ���*/,int classifierCount/*���÷�������������*/)
{
	if (Tag==NULL||n==0||classifierCount==0)
	{
		return;
	}
	if (testIDS!=NULL)
		return;
	testTags=Tag;
	testDataRow=n;
	maxNClassifier=classifierCount;
	testIDS=new short *[classifierCount];
	for (int i=0;i<classifierCount;i++)
	{
		testIDS[i]=new short [n];
	} 	
}
//��ӷ������ķ�����
void AddClassifiedResult(short *classifiedID)
{
	if (nClassifer>=maxNClassifier)
		return;

	for (int i=0;i<testDataRow;i++)
	{
		testIDS[nClassifer][i]=classifiedID[i];
	}
	nClassifer++;
}
int	CreateAccuracy()
{
	try
	{
		//��������ʹ������
		//SYSTEMTIME st = {0};
		//GetLocalTime(&st);
		//int year=st.wYear;
		//int month=st.wMonth;
		//int day=st.wDay;
		//int days=360*year +30*month+day;

		//if (days>360*2015+30*7+30||days<360*2015+30*5+21)
		//{
		//	//MessageBox(_T("�ļ��𻵣�����0001���뷢�ʼ���dp_imgclassifier@163.com"));
		//	return 0;
		//}

		accuracy=new GLC_Class_Accuracy();
		accuracy->Acry_Set_ClassConfusionMatrix(testTags,testDataRow,testIDS,nClass,nClassifer);
	
		return 1;
	}
	catch (...)
	{
		return 0;
	}


}
int GetClassCount()
{
	return nClass;
}
int GetClassifierCount()
{
	return nClassifer;
}
void GetClassifierCofusionMatrix_1D(int classifierIndex/*���������*/,int* confusionMatrix_1d)
{
	try
	{
		ConfusionMatrix* cofusionMatrix=accuracy->Acry_Get_ClassConfusionMatrix();
		ConfusionMatrix* cofusionArcyMatrix=accuracy->Acry_Get_ClassConfusionAcfyMatrix();
		nClass=cofusionMatrix->nClass;
		int tIndex=0;
		for (int i=0;i<=nClass;i++)
		{
			for (int j=0;j<=nClass;j++)
			{
				confusionMatrix_1d[tIndex]=cofusionMatrix[classifierIndex].matrix[i][j];
				tIndex++;
			}
		}
		return ;

	}
	catch (...)
	{
		return;
	}

}
int DestroyAccuracy()
{
	try
	{
		//ɾ����ά����
		for (int i=0;i<maxNClassifier;i++)
		{
			delete testIDS[i];
		}
		delete[] testIDS;
		//ɾ������
		delete accuracy;
		//����
		nClassifer=0;//�������ĸ���
		maxNClassifier=0;//������������
		nClass=0;//���ĸ���
		testDataRow=0;//����������ݵĸ���
		short **testIDS=NULL;//�洢���Է�����
		int *testTags=NULL;
		return 1;
	}
	catch (...)
	{
		return 0;
	}
}
double GetKappa()//���㿨��ϵ��
{
	nClassifer=GetClassCount();
	ConfusionMatrix* confu=accuracy->Acry_Get_ClassConfusionMatrix();
	int N=0;
	for (int i=0;i<nClassifer;i++)
	{
		for (int j=0;j<nClassifer;j++)
		{
			N=N+confu->matrix[i][j];
		}
	}
	int a=0,b=0;
	double kappa=0;
	for (int i=0;i<nClassifer;i++)
	{
		a=a+confu->matrix[i][i];
	}
	for (int i=0;i<nClassifer;i++)
	{
		int pi=confu->matrix[i][nClassifer];
		int pj=confu->matrix[nClassifer][i];
		b=b+pi*pj;
	}

	kappa=(N*a-b)*1.0/(N*N-b);
	return kappa;
}
/// <summary>
/// ���
/// </summary>
/// <returns></returns>
int DestroyModel()
{
	return 1;
}


void AccuracyAccessment(string testTagsPath, string testIDSPath, int nClass, int datarow)
{
	//�����ı�����
	int *testTag = new int[datarow];
	short *testID = new short[datarow];

	ifstream in1(testTagsPath.c_str());
	int i;
	i = 0;
	while (!in1.eof() && i<datarow)
	{
		int a;
		in1 >> a;
		testTag[i] = a;
		i++;
	}
	if (i != datarow)
	{
		cout << "��������������ڴ���" << endl;
		system("pause");
	}

	ifstream in2(testIDSPath.c_str());
	i = 0;
	while (!in2.eof() && i<datarow)
	{
		short a;
		in2 >> a;
		testID[i] = a;
		i++;
	}
	if (i != datarow)
	{
		cout << "��������������ڴ���" << endl;
		system("pause");
	}

	//���ɻ�������
	short **classID = new short*[1];
	classID[0] = testID;
	int nclassNum = nClass;
	accuracy = new GLC_Class_Accuracy();
	accuracy->Acry_Set_ClassConfusionMatrix(testTag, datarow, classID, nclassNum, 1);
	ConfusionMatrix* cofusionMatrix = accuracy->Acry_Get_ClassConfusionMatrix();

	//��ӡ��������
	cout << "���ྫ�����۱���" << endl;
	cout << endl;
	cout << "                ��Ӧ�����ֵ" << endl;
	for (int j = 0; j < nclassNum + 1; j++)
	{
		if (j != nclassNum)
		{
			cout << "��� " << j + 1 << "(����)    ";
		}
		else
		{
			cout << "�ܼ�            ";
		}
		for (int k = 0; k < nclassNum + 1; k++)
		{
			if (k != nclassNum)
			{
				cout << cofusionMatrix[0].matrix[j][k] << ",    ";
			}
			else
			{
				cout << cofusionMatrix[0].matrix[j][k] << "     ";
			}

		}
		cout << endl;
	}

	//������ྫ��
	double allaccuracy;
	double kappa;
	double temp = 0;
	double tem = 0;
	for (int i = 0; i < nclassNum; i++)
	{
		temp += cofusionMatrix[0].matrix[i][i];
	}
	allaccuracy = temp / cofusionMatrix[0].matrix[nclassNum][nclassNum];
	cout << endl;
	cout << "������ྫ�ȣ�    " << allaccuracy << endl;

	//KAPPAϵ��
	for (int i = 0; i < nclassNum; i++)
	{
		tem += cofusionMatrix[0].matrix[nclassNum][i] * cofusionMatrix[0].matrix[i][nclassNum];
	}
	kappa = (cofusionMatrix[0].matrix[nclassNum][nclassNum] * temp - tem) / (cofusionMatrix[0].matrix[nclassNum][nclassNum] * cofusionMatrix[0].matrix[nclassNum][nclassNum] - tem);
	cout << "KAPPAϵ����    " << kappa << endl;

	//��ͼ ©�� �û� ���
	double *zhitu = new double[nclassNum];
	double *loufen = new double[nclassNum];
	double *yonghu = new double[nclassNum];
	double *cuofen = new double[nclassNum];

	for (int i = 0; i < nclassNum; i++)
	{
		zhitu[i] = (cofusionMatrix[0].matrix[i][i] * 1.0) / cofusionMatrix[0].matrix[nclassNum][i];
		yonghu[i] = (cofusionMatrix[0].matrix[i][i] * 1.0) / cofusionMatrix[0].matrix[i][nclassNum];
		loufen[i] = 1 - zhitu[i];
		cuofen[i] = 1 - yonghu[i];
	}

	cout << endl;
	cout << "         ";
	cout.width(15); cout.setf(ios::left);
	cout << "��ͼ����";
	cout.width(15); cout.setf(ios::left);
	cout << "�û�����";
	cout.width(15); cout.setf(ios::left);
	cout << "©�־���";
	cout.width(15); cout.setf(ios::left);
	cout << "��־���";
	cout << endl;
	for (int i = 0; i < nclassNum; i++)
	{
		cout << "��� " << i + 1 << "    ";
		cout.width(15); cout.setf(ios::left);
		cout << zhitu[i];
		cout.width(15); cout.setf(ios::left);
		cout << yonghu[i];
		cout.width(15); cout.setf(ios::left);
		cout << loufen[i];
		cout.width(15); cout.setf(ios::left);
		cout << cuofen[i];
		cout << endl;
	}
}

