// GLC_ann_classifier.h: interface for the GLC_ann_classifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLC_ANN_CLASSIFIER_H__AF4A2C22_F4C2_4E1F_B1B1_79AE69E78813__INCLUDED_)
#define AFX_GLC_ANN_CLASSIFIER_H__AF4A2C22_F4C2_4E1F_B1B1_79AE69E78813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "floatfann.h"
#include "fann_cpp.h"
#include "structs.h"

struct glc_ann_parameter
{	
	int connectinRate;                  //链接参数
	float learningRate;             //学习率
	float desiredError;            //终止错误率
	int iterationBetweenRepots;       //报告间隔
	int maxepoches;                  //最大迭代次数
	int layes;                          //网络的层数
	int hidNum;                        //隐藏神经元的个数
	int inNum;	
	int outNum;                       //输出单元的个数
};
class _declspec(dllexport) GLC_ann_classifier  
{
public:
	GLC_ann_classifier();
	GLC_ann_classifier(char * traindatafilepath,char * datafilepath,char * netfilepath="GLC_ANN_NET.net");
	virtual ~GLC_ann_classifier();
	
	bool FANN_TrainNetwork_on_file(char * trainDataFilePath,char * netFilePath,	int connectinRate,float learningRate,float desiredError,
		int iterationBetweenRepots,	int maxepoches,int layes,int inNum, int hidNum,int outNum);//使用文件训练训练集
    bool FANN_TrainNetwork_on_data(float **input,float **output,int data_row,int connectinRate,float learningRate,float desiredError,
		int iterationBetweenRepots,	int maxepoches,int layes,int inNum, int hidNum,int outNum);//使用传入数组训练训练集
	bool ExecuteTraining(glc_ann_parameter netparameter);   //使用文件训练训练集
	bool ExecuteTraining_on_data(float **input,float **output,int data_row,glc_ann_parameter netparameter);//使用训练数组训练训练集，在此不再使用网络文件的保存路径，而是使用默认路径
	
	short * ExecuteClassify_on_data(float ** data,int data_row,int& progressI);//用数组分类
	int* ExecuteClassify();//用文件分类
	void CopyANN(FANN::neural_net *network,bool networkclean=false);
	FANN::neural_net *GetModle();

private:
	char * m_traindatafilepath;
    char * m_datafilepath;
    char * m_netfilepath;
	int m_intput_num;                        //输入层的单元个数即参与分类的属性的个数
	int m_output_num;                        //输出层的单元个数，即类别数
    glc_ann_parameter m_netparameter;        //网络控制参数
	FANN::neural_net m_network;
	
};

#endif // !defined(AFX_GLC_ANN_CLASSIFIER_H__AF4A2C22_F4C2_4E1F_B1B1_79AE69E78813__INCLUDED_)
