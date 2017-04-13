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
	int connectinRate;                  //���Ӳ���
	float learningRate;             //ѧϰ��
	float desiredError;            //��ֹ������
	int iterationBetweenRepots;       //������
	int maxepoches;                  //����������
	int layes;                          //����Ĳ���
	int hidNum;                        //������Ԫ�ĸ���
	int inNum;	
	int outNum;                       //�����Ԫ�ĸ���
};
class _declspec(dllexport) GLC_ann_classifier  
{
public:
	GLC_ann_classifier();
	GLC_ann_classifier(char * traindatafilepath,char * datafilepath,char * netfilepath="GLC_ANN_NET.net");
	virtual ~GLC_ann_classifier();
	
	bool FANN_TrainNetwork_on_file(char * trainDataFilePath,char * netFilePath,	int connectinRate,float learningRate,float desiredError,
		int iterationBetweenRepots,	int maxepoches,int layes,int inNum, int hidNum,int outNum);//ʹ���ļ�ѵ��ѵ����
    bool FANN_TrainNetwork_on_data(float **input,float **output,int data_row,int connectinRate,float learningRate,float desiredError,
		int iterationBetweenRepots,	int maxepoches,int layes,int inNum, int hidNum,int outNum);//ʹ�ô�������ѵ��ѵ����
	bool ExecuteTraining(glc_ann_parameter netparameter);   //ʹ���ļ�ѵ��ѵ����
	bool ExecuteTraining_on_data(float **input,float **output,int data_row,glc_ann_parameter netparameter);//ʹ��ѵ������ѵ��ѵ�������ڴ˲���ʹ�������ļ��ı���·��������ʹ��Ĭ��·��
	
	short * ExecuteClassify_on_data(float ** data,int data_row,int& progressI);//���������
	int* ExecuteClassify();//���ļ�����
	void CopyANN(FANN::neural_net *network,bool networkclean=false);
	FANN::neural_net *GetModle();

private:
	char * m_traindatafilepath;
    char * m_datafilepath;
    char * m_netfilepath;
	int m_intput_num;                        //�����ĵ�Ԫ�����������������Եĸ���
	int m_output_num;                        //�����ĵ�Ԫ�������������
    glc_ann_parameter m_netparameter;        //������Ʋ���
	FANN::neural_net m_network;
	
};

#endif // !defined(AFX_GLC_ANN_CLASSIFIER_H__AF4A2C22_F4C2_4E1F_B1B1_79AE69E78813__INCLUDED_)
