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
/* 精度评价接口                                                         */
/************************************************************************/

void SetClassfyTag(int *Tag,int n/*测试数据的个数*/,int classifierCount/*设置分类器的最多个数*/)
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
//添加分类器的分类结果
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
		//设置限制使用期限
		//SYSTEMTIME st = {0};
		//GetLocalTime(&st);
		//int year=st.wYear;
		//int month=st.wMonth;
		//int day=st.wDay;
		//int days=360*year +30*month+day;

		//if (days>360*2015+30*7+30||days<360*2015+30*5+21)
		//{
		//	//MessageBox(_T("文件损坏，代码0001，请发邮件到dp_imgclassifier@163.com"));
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
void GetClassifierCofusionMatrix_1D(int classifierIndex/*分类器编号*/,int* confusionMatrix_1d)
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
		//删除二维数组
		for (int i=0;i<maxNClassifier;i++)
		{
			delete testIDS[i];
		}
		delete[] testIDS;
		//删除对象
		delete accuracy;
		//置零
		nClassifer=0;//分类器的个数
		maxNClassifier=0;//分类器最大个数
		nClass=0;//类别的个数
		testDataRow=0;//参与测试数据的个数
		short **testIDS=NULL;//存储测试分类结果
		int *testTags=NULL;
		return 1;
	}
	catch (...)
	{
		return 0;
	}
}
double GetKappa()//计算卡巴系数
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
/// 清除
/// </summary>
/// <returns></returns>
int DestroyModel()
{
	return 1;
}


void AccuracyAccessment(string testTagsPath, string testIDSPath, int nClass, int datarow)
{
	//读入文本数据
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
		cout << "类别数据行数存在错误" << endl;
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
		cout << "类别数据行数存在错误" << endl;
		system("pause");
	}

	//生成混淆矩阵
	short **classID = new short*[1];
	classID[0] = testID;
	int nclassNum = nClass;
	accuracy = new GLC_Class_Accuracy();
	accuracy->Acry_Set_ClassConfusionMatrix(testTag, datarow, classID, nclassNum, 1);
	ConfusionMatrix* cofusionMatrix = accuracy->Acry_Get_ClassConfusionMatrix();

	//打印混淆矩阵
	cout << "分类精度评价报告" << endl;
	cout << endl;
	cout << "                对应类别真值" << endl;
	for (int j = 0; j < nclassNum + 1; j++)
	{
		if (j != nclassNum)
		{
			cout << "类别 " << j + 1 << "(分类)    ";
		}
		else
		{
			cout << "总计            ";
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

	//总体分类精度
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
	cout << "总体分类精度：    " << allaccuracy << endl;

	//KAPPA系数
	for (int i = 0; i < nclassNum; i++)
	{
		tem += cofusionMatrix[0].matrix[nclassNum][i] * cofusionMatrix[0].matrix[i][nclassNum];
	}
	kappa = (cofusionMatrix[0].matrix[nclassNum][nclassNum] * temp - tem) / (cofusionMatrix[0].matrix[nclassNum][nclassNum] * cofusionMatrix[0].matrix[nclassNum][nclassNum] - tem);
	cout << "KAPPA系数：    " << kappa << endl;

	//制图 漏分 用户 错分
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
	cout << "制图精度";
	cout.width(15); cout.setf(ios::left);
	cout << "用户精度";
	cout.width(15); cout.setf(ios::left);
	cout << "漏分精度";
	cout.width(15); cout.setf(ios::left);
	cout << "错分精度";
	cout << endl;
	for (int i = 0; i < nclassNum; i++)
	{
		cout << "类别 " << i + 1 << "    ";
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

