// ���������Գ���.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "ChangeDTCClassifierAPI.h"
#include "Classifier.h"
#include "File.h"
#include <iostream>

#include <fstream>
#include <string>
using namespace std;

int maxhid=0;
double maxaccy=0;
struct anntestPara
{
	int hidnum;
	double **trainData;
	int trainData_row;
	int trainData_line;
	double **testData;
	int testData_row;
	int testData_line;
};

int _tmain(int argc, _TCHAR* argv[])
{
	const char* pathTrian="D:\\�仯�����������\\��������\\sample\\train.txt";
	const char* pathTrian_new="D:\\�仯�����������\\��������\\sample\\train.txt";
	const char* pathTest="D:\\�仯�����������\\��������\\sample\\test.txt";
	const char* pathoutput="D:\\�仯�����������\\��������\\sample\\out.txt";

	/*CClasstor cl;
	cl.getchangeDTCfile(pathTest,pathTrian,pathTrian_new,2,1200);
*/
	testJC2(pathTrian_new,pathTest,pathoutput);

	//test_ANNClassify(pathTrian,pathTest,pathoutput,250);

	system("pause");
	return 0;
}

void CClasstor::getchangeDTCfile(const char* m_testfile_path,const char* m_trainfile_path,const char* m_trainupdatafile_path,int m_num,int testNUM)
{
	File f_test(m_testfile_path);
	int test_row=0,test_line=0;
	int &testData_row=test_row;
	int &testData_line=test_line;

	f_test.get_data_row_line(testData_row,testData_line);

	File f_train(m_trainfile_path);
    int row=0;
	int line=0;
	int &trainData_row=row;
	int &trainData_line=line;

	f_train.get_data_row_line(trainData_row,trainData_line);

	FILE* output;
	output = fopen(m_trainupdatafile_path,"w");
	string str_train = "";
	ifstream fin_train(m_trainfile_path);
	for (int i=0;i<trainData_row;i++)
	{
		string str;
		const char *tempstr;
		int tagI=0,cutI=0;		
		fin_train>>str;
		tempstr = str.c_str();
        
		fprintf(output,"%s\n", tempstr);
	}
	fin_train.close();

	ifstream fin_test(m_testfile_path);
	string *data;
	data = new string[testData_row];
	
	string str,str1;
	int q = m_num*testNUM;
	int w = q + testNUM;
	for (int i =0;i<testData_row;i++)
	{
		
		string str_temp ="";
		fin_test>>str;
		str1 = str;
		if (i>q&&i<w)
		{
			int t = str1.length();
			int tagI=0,cutI=0;
			str_temp = str1.substr(0,t-2);
			str_temp = str_temp +",1";
			const char*char_temp = str_temp.c_str();
			if (str_temp.length()>0)
			{
				fprintf(output, "%s\n", char_temp);
			}
		 }
		if (i==w)
		{
			int t = str1.length();
			int tagI=0,cutI=0;
			str_temp = str1.substr(0,t-2);
			str_temp = str_temp +",1";
			const char*char_temp = str_temp.c_str();
			if (str_temp.length()>0)
			{
				fprintf(output, "%s", char_temp);
			}
		}
	}
 	fin_test.close();
	fclose(output);
}

void CClasstor::Classifier(const char* pathTrian,const char* pathTest,const char* pathOutput ,int method)
{
 switch(method)
 {
    case 0:
	    test_ANNClassify(pathTrian,pathTest,pathOutput,250);
	    break;
	case 1:
		test_SVMClassify(pathTrian,pathTest,pathOutput);
		break;
	case 2:
		test_BayesClassify(pathTrian,pathTest,pathOutput);
		break;
	case 3:
		test_KNNClassify(pathTrian,pathTest,pathOutput);
		break;
	case 4:
		test_TreeClassify(pathTrian,pathTest,pathOutput);
		break;
	case 5:
		test_AdTreeClassify(pathTrian,pathTest,pathOutput,5);
		break;
	case 6:
		testJC1(pathTrian,pathTest,pathOutput);
		break;
	case 7:
		testJC2(pathTrian,pathTest,pathOutput);
		break;
 }
}
void testJC1(const char* pathTrian,const char* pathTest,const char* pathOutput)
{
	
	test_TreeClassify(pathTrian,pathTest,pathOutput);
	test_KNNClassify(pathTrian,pathTest,pathOutput);
	test_BayesClassify(pathTrian,pathTest,pathOutput);
	char *oupput = new char[strlen(pathOutput)+1];
	strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�
	double ** x=f.get_data_double();
	float ** y=f.get_data_float();
	double **trainData=x;
	float **trainData_f=f.get_data_float();
	int row=0,line=0;
	int &trainData_row=row;
	int &trainData_line=line;
	f.get_data_row_line(trainData_row,trainData_line);

	File f2(pathTest);//���Լ�
	double ** x2=f2.get_data_double();
	float ** y2=f2.get_data_float();
	double **tetstData=x2;
	float **tetstData_f=f2.get_data_float();
	int row2=0,line2=0;
	int &testData_row=row2;
	int &testData_line=line2;
	f2.get_data_row_line(testData_row,testData_line);


	double ** train_data=new double*[trainData_row];
	for (int i=0;i<trainData_row;i++)
	{
		train_data[i]=new double[trainData_line];
		for (int j=0;j<trainData_line;j++)
		{
			train_data[i][j]=trainData[i][j];
		}
	}
	double **test_data=new double *[testData_row];
	for (int i=0;i<testData_row;i++)
	{
		test_data[i]=new double[testData_line];
		for (int j=0;j<testData_line;j++)
		{
			test_data[i][j]=tetstData[i][j];
		}
	}




	RISC_Classifier_JC1 *classifier=NewClassifier_JC1();
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
	}
	//int * subclassifier=new int[4];
	////classifier[0]=RISC_CTree;
	//subclassifier[0]=RISC_CTree;
	//subclassifier[1]=RISC_Bayes ;
	//subclassifier[2]=RISC_KNN ;
	//subclassifier[3]=RISC_SVM;
	//classifier->Create();
	//classifier->SetSubClassifiers(subclassifier,4);


	//classifier->SetParameter(parameter_tree,parameter_ann,parameter_svm);
	//classifier->SetTestData(test_data,row2,trainData_line);
	classifier->ExcuteTrain();

	short * classifiedIDs=new short[row2];
	int *testTag=new int[row2];
	int yy=0;

	for (int i=0;i<row2;i++)
	{
		double* data=new double[trainData_line-1];
		for (int j=0;j<trainData_line-1;j++)
		{
			data[j]=tetstData[i][j];
		}
		int xx=classifier->ExcuteClassify(data);
		fprintf(output," %d",xx);
		fprintf(output,"\n");
		classifiedIDs[i]=(short)xx;
		testTag[i]=tetstData[i][trainData_line-1];
		if (xx==tetstData[i][trainData_line-1])
		{
			yy++;
		}
	}
	fclose(output);
	//cout<<"���ɾ��ȣ�"<<yy*1.0/row2<<endl;
	classifier->Destroy();/**/


}
void testJC2(const char* pathTrian,const char* pathTest,const char* pathOutput)
{
	//cout<<"����һ��ʼ����:-----------"<<endl;
	test_TreeClassify(pathTrian,pathTest,pathOutput);
	//cout<<"����һ����������"<<endl;

	//cout<<"��������ʼ����:-----------"<<endl;
	test_KNNClassify(pathTrian,pathTest,pathOutput);
	//cout<<"����������������"<<endl;

	//cout<<"��������ʼ����:-----------"<<endl;
	test_SVMClassify(pathTrian,pathTest,pathOutput);
	//cout<<"����������������"<<endl;
	
	//cout<<"�ۺ�ͳ�Ʒ���:-------------"<<endl;
	char *oupput = new char[strlen(pathOutput)+1];
	strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�
	double ** x=f.get_data_double();
	float ** y=f.get_data_float();
	double **trainData=x;
	float **trainData_f=f.get_data_float();
	int row=0,line=0;
	int &trainData_row=row;
	int &trainData_line=line;
	f.get_data_row_line(trainData_row,trainData_line);
	cout<<"��������������  "<<trainData_row<<";    ��������������  "<<trainData_line<<endl;

	File f2(pathTest);//���Լ�
	double ** x2=f2.get_data_double();
	float ** y2=f2.get_data_float();
	double **tetstData=x2;
	float **tetstData_f=f2.get_data_float();
	int row2=0,line2=0;
	int &testData_row=row2;
	int &testData_line=line2;
	f2.get_data_row_line(testData_row,testData_line);
	cout<<"�������������  "<<testData_row<<";    �������������  "<<testData_line<<endl;

	double ** train_data=new double*[trainData_row];
	for (int i=0;i<trainData_row;i++)
	{
		train_data[i]=new double[trainData_line];
		for (int j=0;j<trainData_line;j++)
		{
			train_data[i][j]=trainData[i][j];
		}
		info_changeDec("+");
	}
	double **test_data=new double *[testData_row];
	for (int i=0;i<testData_row;i++)
	{
		test_data[i]=new double[testData_line];
		for (int j=0;j<testData_line;j++)
		{
			test_data[i][j]=tetstData[i][j];
		}
		info_changeDec("*");
	}


	RISC_Classifier_JC2 *classifier=NewClassifier_JC2();
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
	}
	//int * subclassifier=new int[4];
	////classifier[0]=RISC_CTree;
	//subclassifier[0]=RISC_CTree;
	//subclassifier[1]=RISC_Bayes ;
	//subclassifier[2]=RISC_KNN ;
	//subclassifier[3]=RISC_SVM;
	//classifier->Create();
	//classifier->SetSubClassifiers(subclassifier,4);


	//classifier->SetParameter(parameter_tree,parameter_ann,parameter_svm);
	//classifier->SetTestData(test_data,row2,trainData_line);
	classifier->ExcuteTrain();

	int yy=0;
	for (int i=0;i<row2;i++)
	{
		double* data=new double[trainData_line-1];
		for (int j=0;j<trainData_line-1;j++)
		{
			data[j]=tetstData[i][j];
		}
		int xx=classifier->ExcuteClassify(data);
		fprintf(output," %d",xx);
		fprintf(output,"\n");

		info_changeDec(".");
		if (xx==tetstData[i][trainData_line-1])
		{
			yy++;
		}

		//cout<<"*";
	}
	fclose(output);
	//cout<<"���ɾ��ȣ�"<<yy*1.0/row2<<endl;
	cout<<"����������"<<endl;
	classifier->Destroy();/**/
}

void test_TreeClassify(const char* pathTrian,const char* pathTest,const char* pathOutput)
{
	char *oupput = new char[strlen(pathOutput)+1];
	strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�

	cout<<"��ȡ����-1------------";
	double ** x=f.get_data_double();
	cout<<"����-1��ȡ����"<<endl;

	double **trainData=x;
	int trainData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int trainData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f.get_data_row_line(trainData_row,trainData_line);//���������

	cout<<"��������������  "<<trainData_row<<"��     ��������������  "<<trainData_line<<endl;


	File f2(pathTest);//���Լ�

	cout<<"��ȡ����-2-----------"<<endl;
	cout<<"\n";
	double ** x2=f2.get_data_double();
	cout<<"����-2��ȡ����"<<endl;

	double **testData=x2;
	int testData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int testData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f2.get_data_row_line(testData_row,testData_line);//���������
	cout<<"�������������  "<<testData_row<<"��     �������������  "<<testData_line<<endl;

	RISC_Classifier_Tree * classifier=NewClassifier_Tree();
	//classifier->LoardSampleFile(pathTrian);//����ֱ�Ӷ�ȡ.data�ļ�
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
		//return 0;
	}
	classifier->SetParameter(7,3);//���ò���������1���������߶ȣ�����2��������Ҷ�ڵ������
	classifier->ExcuteTrain();//��ʼѵ��

	//��ʼ����
	cout<<"��ʼ����-----------------";
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<testData_row;i++)
	{
		double * data=new double[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			data[j]=testData[i][j];
		}
		int result=classifier->ExcuteClassify(data);
		//cout<<result<<",";
		if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		{
			correct++;
		}
		fprintf(output," %d",result);
		fprintf(output,"\n");

		//info_changeDec(".");
		delete [] data;
	}
	fclose(output);
//	cout<<endl<<"test_TreeClassify�������"<<endl;
//	cout<<"���ྫ��Ϊ��"<<(correct*1.0/testData_row)*100<<"%"<<endl;

	//cout<<"����������ִ����ϣ�"<<endl;
	classifier->Destroy();//���ٷ���ģ��

}

void test_BayesClassify(const char* pathTrian,const char* pathTest,const char* pathOutput)
{
	char *oupput = new char[strlen(pathOutput)+1];
	strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�

	cout<<"��ȡ����-1------------------";
	double ** x=f.get_data_double();
	cout<<"����-1��ȡ����"<<endl;

	double **trainData=x;
	int trainData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int trainData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f.get_data_row_line(trainData_row,trainData_line);//���������
	cout<<"��������������  "<<trainData_row<<"��������������  "<<trainData_line<<endl;

	File f2(pathTest);//���Լ�

	cout<<"��ȡ����-2";
	double ** x2=f2.get_data_double();
	cout<<"����-2��ȡ����"<<endl;

	double **testData=x2;
	int testData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int testData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f2.get_data_row_line(testData_row,testData_line);//���������
	cout<<"�������������  "<<testData_row<<"�������������  "<<testData_line<<endl;

	RISC_Classifier_Bayes * classifier=NewClassifier_Bayes();
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
	}
	//�������ò�����ֱ��ѵ��
	classifier->ExcuteTrain();//��ʼѵ��

	//��ʼ����
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<testData_row;i++)
	{
		double * data=new double[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			data[j]=testData[i][j];
		}
		int result=classifier->ExcuteClassify(data);
		//cout<<result<<",";
		if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		{
			correct++;
		}
		fprintf(output," %d",result);
		fprintf(output,"\n");
		delete [] data;
	}
	fclose(output);
	cout<<endl<<"�������"<<endl;
	cout<<"test_BayesClassify���ྫ��Ϊ��"<<(correct*1.0/testData_row)*100<<"%"<<endl;
	classifier->Destroy();//���ٷ���ģ��

}
void test_KNNClassify(const char* pathTrian,const char* pathTest,const char* pathOutput)
{
	char *oupput = new char[strlen(pathOutput)+1];
	strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�

	cout<<"��ȡ����-1--------------"<<endl;
	double ** x=f.get_data_double();
	cout<<"����-1��ȡ����"<<endl;

	double **trainData=x;
	int trainData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int trainData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f.get_data_row_line(trainData_row,trainData_line);//���������
	cout<<"��������������  "<<trainData_row<<";     ��������������  "<<trainData_line<<endl;

	File f2(pathTest);//���Լ�
	cout<<"��ȡ����-2-------------";
	double ** x2=f2.get_data_double();
	cout<<"����-2��ȡ����"<<endl;
	double **testData=x2;
	int testData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int testData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f2.get_data_row_line(testData_row,testData_line);//���������
	cout<<"�������������  "<<testData_row<<";       �������������  "<<testData_line<<endl;

	RISC_Classifier_KNN * classifier=NewClassifier_KNN();
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
	}
	//�������ò�����ֱ��ѵ��
	classifier->ExcuteTrain();//��ʼѵ��

	//��ʼ����
	cout<<"��ʼ����---------------";
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<testData_row;i++)
	{
		double * data=new double[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			data[j]=testData[i][j];
		}
		int result=classifier->ExcuteClassify(data);
		//cout<<result<<",";
		if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		{
			correct++;
		}
		fprintf(output," %d",result);
		fprintf(output,"\n");

		//info_changeDec(".");
		delete [] data;
	}
	fclose(output);
//	cout<<endl<<"test_KNNClassify�������"<<endl;
//	cout<<"���ྫ��Ϊ��"<<(correct*1.0/testData_row)*100<<"%"<<endl;
	//cout<<"KNNͳ�Ʒ���������"<<endl;
	classifier->Destroy();//���ٷ���ģ��

}
void test_SVMClassify(const char* pathTrian,const char* pathTest,const char* pathOutput)
{
	char *oupput = new char[strlen(pathOutput)+1];
	strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�

	cout<<"��ȡ����-1---------------";
	double ** x=f.get_data_double();
	cout<<"����-1��ȡ����"<<endl;

	double **trainData=x;
	int trainData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int trainData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f.get_data_row_line(trainData_row,trainData_line);//���������
	cout<<"��������������  "<<trainData_row<<"��������������  "<<trainData_line<<endl;
	//info_changeDec(".");
	File f2(pathTest);//���Լ�
	cout<<"��ȡ����-2---------------";
	double ** x2=f2.get_data_double();
	cout<<"����-2��ȡ����"<<endl;

	double **testData=x2;
	int testData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int testData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f2.get_data_row_line(testData_row,testData_line);//���������
	cout<<"�������������  "<<testData_row<<"��     �������������  "<<testData_line<<endl;
	   
	RISC_Classifier_SVM   * classifier=NewClassifier_SVM();
	//classifier->LoardSampleFile(pathTrian);//����ֱ�Ӷ�ȡ.data�ļ�
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
		//return 0;
	}

	/************************************************************************/
	/* svm�Ĳ���                                                            */
	/************************************************************************/
	svm_parameter parameter_svm;	
	parameter_svm.svm_type =0;///*NU_SVC*/C_SVC; 
	parameter_svm.kernel_type = 1;//LINEAR/*RBF*/; 
	parameter_svm.degree = 3; 

	parameter_svm.gamma = 0.00001;//0.0001;//0.00001;//1;// 0.15;//0.25;//0.00001;//0.00001/*1.0/m_sampleDataLine*/; 
	parameter_svm.coef0 = 0; 
	parameter_svm.nu =0.5;// 0.5;  
	parameter_svm.cache_size = 100; 
	parameter_svm.C = 32;//32;//2;//25;//100; 
	parameter_svm.eps = 1e-3; 
	parameter_svm.p = 0.1;
	parameter_svm.shrinking = 1; 
	parameter_svm.probability = 0; 
	parameter_svm.nr_weight = 0; 

	classifier->SetParameter(parameter_svm);//���ò���������1���������߶ȣ�����2��������Ҷ�ڵ������
	cout<<"���ò���----------------"<<endl;
	classifier->ExcuteTrain();//��ʼѵ��
	cout<<"��ʼ����----------------"<<endl;
	//��ʼ����
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<testData_row;i++)
	{
		double * data=new double[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			data[j]=testData[i][j];
		}
		int result=classifier->ExcuteClassify(data);
		//cout<<result<<",";
		if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		{
			correct++;
		}
		fprintf(output," %d",result);
		fprintf(output,"\n");

		//info_changeDec(".");
		delete [] data;
	}
	//cout<<"��ʼ��������"<<endl;
	fclose(output);
	//cout<<endl<<"test_SVMClassify�������"<<endl;
	//cout<<"test_SVMClassify���ྫ��Ϊ��"<<(correct*1.0/testData_row)*100<<"%"<<endl;
	//cout<<"SVMͳ�Ʒ���������"<<endl;
	classifier->Destroy();//���ٷ���ģ��

}
void test_AdTreeClassify(const char* pathTrian,const char* pathTest,const char* pathOutput,int adK)
{
	char *oupput = new char[strlen(pathOutput)+1];
	strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�
	double ** x=f.get_data_double();
	double **trainData=x;
	int trainData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int trainData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f.get_data_row_line(trainData_row,trainData_line);//���������

	File f2(pathTest);//���Լ�
	double ** x2=f2.get_data_double();
	double **testData=x2;
	int testData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int testData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f2.get_data_row_line(testData_row,testData_line);//���������

	RISC_Classifier_AdTree * classifier=NewClassifier_AdTree();
	//classifier->LoardSampleFile(pathTrian);//����ֱ�Ӷ�ȡ.data�ļ�
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
		//return 0;
	}
	classifier->SetParameter(7,1,adK);//���ò���������1���������߶ȣ�����2��������Ҷ�ڵ������
	classifier->ExcuteTrain();//��ʼѵ��

	//��ʼ����
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<testData_row;i++)
	{
		double * data=new double[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			data[j]=testData[i][j];
		}
		int result=classifier->ExcuteClassify(data);
		//cout<<result<<",";
		if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		{
			correct++;
		}
		fprintf(output," %d",result);
		fprintf(output,"\n");
		delete [] data;
	}
	fclose(output);
	cout<<endl<<"�������"<<endl;
	cout<<"test_AdTreeClassify���ྫ��Ϊ��"<<(correct*1.0/testData_row)*100<<"%"<<endl;
	classifier->Destroy();//���ٷ���ģ��

}

double test_ANNClassify(const char* pathTrian,const char* pathTest,const char* pathOutput,int hidnum)
{

	char *oupput = new char[strlen(pathOutput)+1];
	  strcpy(oupput, pathOutput);
	char* out_file_name =oupput;
	FILE* output;
	output = fopen(out_file_name,"w");
	File f(pathTrian);//���Լ�
	double ** x=f.get_data_double();
	double **trainData=x;
	int trainData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int trainData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f.get_data_row_line(trainData_row,trainData_line);//���������

	File f2(pathTest);//���Լ�
	double ** x2=f2.get_data_double();
	double **testData=x2;
	int testData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int testData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f2.get_data_row_line(testData_row,testData_line);//���������

	RISC_Classifier_ANN * classifier=NewClassifier_ANN();
	//classifier->LoardSampleFile(pathTrian);//����ֱ�Ӷ�ȡ.data�ļ�
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
		//return 0;
	}
	classifier->SetParameter(trainData_line-1,6,hidnum,3,1,0.0000001,0.00001,50,5000);//�ο�����˵�������������������

	classifier->ExcuteTrain();//��ʼѵ��

	//��ʼ����
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<testData_row;i++)
	{
		float * data=new float[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			float xx=(float)testData[i][j];//�ڴ˱�������һ�������ı���xx,ֱ��data[j]=(float)testData[i][j]���Ǵ����д�����޷��õ�������
			data[j]=xx;
		
		}

		int result=classifier->ExcuteClassify(data);
		if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		{
			correct++;
		}
		fprintf(output," %d",result);
		fprintf(output,"\n");
		delete [] data;
	}
	fclose(output);
	cout<<endl<<"�������"<<endl;
	double accuracy=correct*1.0/testData_row;
	cout<<"���ྫ��Ϊ��"<<accuracy*100<<"%"<<endl;
	classifier->Destroy();//���ٷ���ģ��
	return accuracy;
}
double test_ANNClassify(double **trainData,int trainData_row,int trainData_line,double **testData,int testData_row,int testData_line,int hidnum)
{


	RISC_Classifier_ANN * classifier=NewClassifier_ANN();
	//classifier->LoardSampleFile(pathTrian);//����ֱ�Ӷ�ȡ.data�ļ�
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
		//return 0;
	}
	classifier->SetParameter(trainData_line-1,6,hidnum);//�ο�����˵�������������������
	classifier->ExcuteTrain();//��ʼѵ��

	//��ʼ����
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<testData_row;i++)
	{
		double * data=new double[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			data[j]=testData[i][j];
		}
		int result=classifier->ExcuteClassify(data);
		//	cout<<result<<",";
		if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		{
			correct++;
		}
		delete [] data;
	}
	cout<<endl<<"�������"<<endl;
	double accuracy=correct*1.0/testData_row;
	cout<<"���ྫ��Ϊ��"<<accuracy*100<<"%"<<endl;
	classifier->Destroy();//���ٷ���ģ��
	return accuracy;
}

void test_Accrucy(const char* pathTrian,const char* pathTest)
{
	/************************************************************************/
	/* ����                                                                  */
	/************************************************************************/
	File f(pathTrian);//���Լ�
	double ** x=f.get_data_double();
	double **trainData=x;
	int trainData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int trainData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f.get_data_row_line(trainData_row,trainData_line);//���������

	File f2(pathTest);//���Լ�
	double ** x2=f2.get_data_double();
	double **testData=x2;
	int testData_row=0;//ѵ�����ĸ�����.data�������ݵ�����
	int testData_line=0;//ѵ����������+����ǩ��.data�������ݵ�����
	f2.get_data_row_line(testData_row,testData_line);//���������

	RISC_Classifier_Tree * classifier=NewClassifier_Tree();
	//classifier->LoardSampleFile(pathTrian);//����ֱ�Ӷ�ȡ.data�ļ�
	classifier->SetSampleData(trainData,trainData_row,trainData_line);//����ѵ������
	//�ȴ�������ѵ�����ٷ���
	if (classifier->Create()!=1)
	{
		cout<<"����������ʧ��"<<endl;
		//return 0;
	}
	classifier->SetParameter(7,3);//���ò���������1���������߶ȣ�����2��������Ҷ�ڵ������
	classifier->ExcuteTrain();//��ʼѵ��

	//��ʼ����
	int * testTag=new int[testData_row];//һά�������ڼ�¼����ǩ
	short * classifiedID=new short[testData_row];//һά�������ڼ�¼����֮��Ľ��
	int nTest=testData_row;//������������ĸ���
	int nClass=0;//���������е�������	
	int nClassifier=1;//�������ĸ���
	
	for (int i=0;i<testData_row;i++)
	{
		testTag[i]=(int)testData[i][testData_line-1];
		double * data=new double[testData_line-1];//֮����-1������Ϊ�������ݺ�����˱�ǩ������������ݲ���Ҫ��ǩ
		for (int j=0;j<testData_line-1;j++)
		{
			data[j]=testData[i][j];
		}
		int result=classifier->ExcuteClassify(data);
		classifiedID[i]=result;

		//if (result==testData[i][testData_line-1])//�жϷ������Ƿ�������ǩ���
		//{
		//	correct++;
		//}
		delete [] data;
	}
	classifier->Destroy();//���ٷ���ģ��
	//��������
	SetClassfyTag(testTag,nTest,nClassifier);
	AddClassifiedResult(classifiedID);

	
	if (CreateAccuracy()==1)
	{
		cout<<"�������ۿ�ʼ...."<<endl;
	}
	else
		cout<<"��������ʧ�ܣ�"<<endl;
	//��ȡ��������
	nClass=GetClassCount();
	int n=nClass+1;
	int *matrix1d=new int[n*n];
	int **matrix2d=new int*[n];
	for (int i=0;i<n;i++)
	{
		matrix2d[i]=new int[n];
	}

	GetClassifierCofusionMatrix_1D(0,matrix1d);
	//��������������ά������
	
	int index=0;
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			matrix2d[i][j]=matrix1d[index++];//��ȡ��������
		}
	}
	//������ྫ�ȣ��Խ����ϵ�Ҫ�ظ���֮��Ϊ��ȷ����ĸ���
	int sum=0;
	int correct=0;//ͳ�Ʒ�����ȷ�ĸ���
	for (int i=0;i<nClass;i++)
	{
	     correct+=matrix2d[i][i];
	}
	sum=matrix2d[n-1][n-1];//��ά�������һ������Ϊ�����ܺ�

	cout<<"���ྫ��Ϊ��"<<(correct*1.0/sum)*100<<"%"<<endl;
	cout<<"Kappaϵ��Ϊ"<<GetKappa()<<endl;
}
