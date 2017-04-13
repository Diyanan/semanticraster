#include "StdAfx.h"
#include "RISC_Classifier.h"
#include "File.h"
RISC_Classifier::RISC_Classifier()
{
	m_trainData_row=0;
	m_trainData_line=0;
	m_trainData=NULL;
}
//RISC_Classifier::~RISC_Classifier(void)
//{
//	int xx=0;
//}
RISC_Classifier::~RISC_Classifier(void)
{

}
int RISC_Classifier::LoardSampleFile(const char * samplesetFile)
{
	try
	{
		File f(samplesetFile);//测试集
		double ** x=f.get_data_double();
		float ** y=f.get_data_float();

		double** trainData=x;
		int row=0,line=0;
		int &trainData_row=row;
		int &trainData_line=line;

		f.get_data_row_line(trainData_row,trainData_line);
	
		int xx=SetSampleData(trainData,trainData_row,trainData_line);
		return xx ;

	}
	catch (std::exception & e )
	{
		m_trainData_row=0;
		m_trainData_line=0;
		m_trainData=NULL;
		m_checkResult=0;
		throw e;		//程序异常
	}
	catch (...)
	{
		return -1;
	}

	
}
int RISC_Classifier::SetSampleData(double ** traindata,int traindata_row,int traindata_line)
{
	try
	{

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

		m_trainData_row=traindata_row;
		m_trainData_line=traindata_line;
		m_trainData=traindata;





		//Regist();//注册


		//

		//int checkResult=checkRegistInfor();
		m_checkResult=1;
		return 1;
		
	}
	
	catch (std::exception & e )
	{
		m_trainData_row=0;
		m_trainData_line=0;
		m_trainData=NULL;
		m_checkResult=0;
		return -1;
		throw e;		//程序异常
	}
	catch(...)
	{
		m_trainData_row=0;
		m_trainData_line=0;
		m_trainData=NULL;
		m_checkResult=0;
		return -1;
	}
	

}

//创建分类模型
int RISC_Classifier::Create()
{
	return 0;
}

//设置参数
void RISC_Classifier::SetParameter(const char* parameter)
{

}
//训练样本
void RISC_Classifier::ExcuteTrain()
{

}
//执行分类
int RISC_Classifier::ExcuteClassify(double data[])
{
	return 0;
}
//删除模型
int  Destroy()
{
	return 0;
}