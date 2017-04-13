// GLCann_classifier.h: interface for the GLCann_classifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLCANN_CLASSIFIER_H__476C2244_65FC_46FB_AACF_CD119892E101__INCLUDED_)
#define AFX_GLCANN_CLASSIFIER_H__476C2244_65FC_46FB_AACF_CD119892E101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GLC_ann_classifier.h"

class _declspec(dllexport)  GLCann_classifier  
{
public:
	bool Gann_Destroy_Model();
	GLCann_classifier();//δ����ʼ�����࣬�������ݵĸ�ʽ��
	GLCann_classifier(char* trainDataFilePath,char* classifyDataFilePtath,char* annnetFilePath="GLC_ANN_NET.net");//��ʼ���࣬ʹ���ļ�����ѵ���ͷ���
	
	~GLCann_classifier(void);
	// ת������Ϊ�����紦����ļ�
	//	void Gann_dataformat(double ** data, int data_row , int data_line,int classnum, CString dataformat_filepath);//����������и�ʽת����������
	//	void Gann_dataformat(/*CString dataoriginal_filepath, CString dataformat_filepath*/);//ͨ���ļ����и�ʽת��������
	//���ò���
	void Gann_set_parameter(int connectinRate, float learningRate, float desiredError, 
		int iterationBetweenRepots, int maxepoches, int layes, int inNum, int hidNum, int outNum);
	// ѵ��ģ��
	bool Gann_ExecuteTraining_on_file(int &progressI);
	bool Gann_ExecuteTraining_on_data(double ** traindata,int traindata_row,int traindata_line,int &progressI);
	// ִ�з���
	int* Gann_ExecuteClassify_on_file(int& progressI);
	short* Gann_ExecuteClassify_on_data(float** classifydata,int classfydata_row,int classifydata_line,int& progressI);
	short* Gann_ExecuteClassify_on_data_double(double** classifydata,int classfydata_row,int classifydata_line,int& progressI);
	//ʹ��float���ݷ���
    short  Gann_ExecuteClassify_on_data_float(float* classifydata);
    FANN::neural_net*  Gann_get_modle(); 
	
private:
	// ����
	glc_ann_parameter m_parameter;
	char* m_annnetFilePath;// ���籣��·��
	char* m_trainDataFilePath;//ѵ���ļ�·��
	char* m_classifyDataFilePtath;//�����ļ�·��
	// ������
	GLC_ann_classifier* m_Gann_classifier;
	FANN::neural_net* m_network;//���ڼ�¼����
	
};
#endif // !defined(AFX_GLCANN_CLASSIFIER_H__476C2244_65FC_46FB_AACF_CD119892E101__INCLUDED_)
