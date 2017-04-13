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
	GLCann_classifier();//未经初始化的类，用于数据的格式化
	GLCann_classifier(char* trainDataFilePath,char* classifyDataFilePtath,char* annnetFilePath="GLC_ANN_NET.net");//初始化类，使用文件进行训练和分类
	
	~GLCann_classifier(void);
	// 转换数据为神经网络处理的文件
	//	void Gann_dataformat(double ** data, int data_row , int data_line,int classnum, CString dataformat_filepath);//传入数组进行格式转换，并保存
	//	void Gann_dataformat(/*CString dataoriginal_filepath, CString dataformat_filepath*/);//通过文件进行格式转换并保存
	//设置参数
	void Gann_set_parameter(int connectinRate, float learningRate, float desiredError, 
		int iterationBetweenRepots, int maxepoches, int layes, int inNum, int hidNum, int outNum);
	// 训练模型
	bool Gann_ExecuteTraining_on_file(int &progressI);
	bool Gann_ExecuteTraining_on_data(double ** traindata,int traindata_row,int traindata_line,int &progressI);
	// 执行分类
	int* Gann_ExecuteClassify_on_file(int& progressI);
	short* Gann_ExecuteClassify_on_data(float** classifydata,int classfydata_row,int classifydata_line,int& progressI);
	short* Gann_ExecuteClassify_on_data_double(double** classifydata,int classfydata_row,int classifydata_line,int& progressI);
	//使用float数据分类
    short  Gann_ExecuteClassify_on_data_float(float* classifydata);
    FANN::neural_net*  Gann_get_modle(); 
	
private:
	// 参数
	glc_ann_parameter m_parameter;
	char* m_annnetFilePath;// 网络保存路径
	char* m_trainDataFilePath;//训练文件路径
	char* m_classifyDataFilePtath;//分类文件路径
	// 分类器
	GLC_ann_classifier* m_Gann_classifier;
	FANN::neural_net* m_network;//用于记录网络
	
};
#endif // !defined(AFX_GLCANN_CLASSIFIER_H__476C2244_65FC_46FB_AACF_CD119892E101__INCLUDED_)
