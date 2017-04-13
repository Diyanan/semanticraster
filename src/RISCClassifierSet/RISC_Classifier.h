

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
	// virtual ~RISC_Classifier(void)=0{}; //ע�⣬��ö�������������������������಻���������������������������Ϊ�������������������������壬��Ϊ�����������ø�����������
	 // ~RISC_Classifier(void); //ע�⣬��ö�������������������������಻���������������������������Ϊ�������������������������壬��Ϊ�����������ø�����������
	 ////��������ģ��
	 int Create();
	 //���ļ��л�ȡ����
	 int LoardSampleFile(const char * samplesetFile);
	 int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	 //���ò���
	 void SetParameter(const char* parameter);
	 //ѵ������
	 void ExcuteTrain();
	 //ִ�з���
	 int ExcuteClassify(double data[]);
	 //ɾ��ģ��
	 int  Destroy();



	
	////��������ģ��
	//virtual int Create()=0;
	////���ļ��л�ȡ����
	// int LoardSampleFile(const char * samplesetFile);
	// int SetSampleData(double ** traindata,int traindata_row,int traindata_line);
	////���ò���
	//virtual void SetParameter(const char* parameter)=0;
	////ѵ������
	//virtual void ExcuteTrain()=0;
	////ִ�з���
	//virtual int ExcuteClassify(double data[])=0;
	////ɾ��ģ��
	//virtual int  Destroy()=0;

	
};