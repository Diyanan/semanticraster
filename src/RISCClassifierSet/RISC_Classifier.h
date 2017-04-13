

#pragma once

#pragma once
#ifdef GLC_EXPORTS
#define GLC_EXP _declspec(dllexport)
#else
#define GLC_EXP _declspec(dllimport)
#endif


#include "cv.h"
#include "opencv.hpp" 
#include "highgui.h" 


class GLC_EXP RISC_Classifier
{
protected:
	double **m_trainData;
	int m_trainData_row;
	int m_trainData_line;
	int m_checkResult;
public:
	 RISC_Classifier(void);
	 virtual ~RISC_Classifier(void);
	 // ~RISC_Classifier(void);
	// virtual ~RISC_Classifier(void)=0{}; //注意，最好定义此虚析构函数，够避免子类不能正常调用析构函数；如果定义为纯虚析构函数，则必须带定义体，因为子类隐含调用该析构函数。
	 // ~RISC_Classifier(void); //注意，最好定义此虚析构函数，够避免子类不能正常调用析构函数；如果定义为纯虚析构函数，则必须带定义体，因为子类隐含调用该析构函数。
	 ////创建分类模型
	 int Create();
	 //从文件中获取样本
	 int LoardSampleFile(const char * samplesetFile);
	 int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	 //设置参数
	 void SetParameter(const char* parameter);
	 //训练样本
	 void ExcuteTrain();
	 //执行分类
	 int ExcuteClassify(double data[]);
	 //删除模型
	 int  Destroy();



	
	////创建分类模型
	//virtual int Create()=0;
	////从文件中获取样本
	// int LoardSampleFile(const char * samplesetFile);
	// int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	////设置参数
	//virtual void SetParameter(const char* parameter)=0;
	////训练样本
	//virtual void ExcuteTrain()=0;
	////执行分类
	//virtual int ExcuteClassify(double data[])=0;
	////删除模型
	//virtual int  Destroy()=0;

	
};