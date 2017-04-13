#include "StdAfx.h"
#include "GLC_Bayes.h"

#include <iostream>
using namespace  std;
GLC_Bayes::GLC_Bayes(void)
	: m_traindata(NULL)
	, m_traindata_row(0)
	, m_traindata_line(0)
{
}


GLC_Bayes::~GLC_Bayes(void)
{
}

bool GLC_Bayes::GLC_Bayes_Train(double ** triandata,int triandata_row,int triandata_line)
{
	m_traindata=triandata;
	m_traindata_row=triandata_row;
	m_traindata_line=triandata_line;
	cv::Mat trainData(triandata_row, triandata_line-1, CV_32FC1);//构建训练样本的特征向量
	cv::Mat trainResponse(triandata_row,1, CV_32FC1);//构建训练样本的类别标签 
	for (int i=0;i<triandata_row;i++)
	{
		for (int j=0;j<triandata_line;j++)
		{
			if (j==triandata_line-1)
			{
				trainResponse.at<float>(i,0)=triandata[i][j];
			}
			else
				trainData.at<float>(i,j)=triandata[i][j];			
		}

		//cout<<"*";
	}
	//cout<<endl;
	bool trainFlag=m_nbc.train(trainData,trainResponse);
	return trainFlag;
}


int GLC_Bayes::GLC_Bayes_ExcuteClassify(double* data)
{
	int data_line=m_traindata_line-1,classId=0;
	cv::Mat matdata(1,data_line,CV_32FC1);
	for (int j=0;j<data_line;j++)
	{
		matdata.at<float>(0,j)=data[j];	

		//cout<<"*";
	}
	//cout<<endl;

	classId=m_nbc.predict(matdata);
	return classId;
}
