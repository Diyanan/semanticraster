#include "StdAfx.h"
#include "GLC_Knn.h"
#include <iostream>
using namespace  std;

GLC_Knn::GLC_Knn(void)
	: m_traindata(NULL)
	, m_traindata_row(0)
	, m_traindata_line(0)
{
}


GLC_Knn::~GLC_Knn(void)
	
{
}


bool GLC_Knn::GLC_Knn_Train(double** traindata, int traindata_row, int traindata_line)
{
	m_traindata=traindata;
	m_traindata_row=traindata_row;
	m_traindata_line=traindata_line;
	cv::Mat trainData(traindata_row, traindata_line-1, CV_32FC1);//构建训练样本的特征向量
	cv::Mat trainResponse(traindata_row,1, CV_32FC1);//构建训练样本的类别标签 
	for (int i=0;i<traindata_row;i++)
	{
		for (int j=0;j<traindata_line;j++)
		{
			if (j==traindata_line-1)
			{
				trainResponse.at<float>(i,0)=traindata[i][j];
			}
			else
				trainData.at<float>(i,j)=traindata[i][j];			
		}

		//cout<<".";
	}
	//cout<<endl;
	bool trainFlag=m_kNearest.train(trainData,trainResponse);
	return trainFlag;
}


int GLC_Knn::GLC_Knn_ExcuteClassify(double* data,int nNearest)
{
	int data_line=m_traindata_line-1,classId=0;
	cv::Mat matdata(1,data_line,CV_32FC1);
	for (int j=0;j<data_line;j++)
	{
		matdata.at<float>(0,j)=data[j];		

		//cout<<"*";
	}
	//cout<<endl;
	classId=m_kNearest.find_nearest(matdata,nNearest);
	return classId;
}
