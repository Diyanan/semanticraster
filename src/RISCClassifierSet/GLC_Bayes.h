#pragma once
#include "cv.h"
#include "opencv.hpp" 
#include "highgui.h" 

class GLC_Bayes
{
public:
	GLC_Bayes(void);
	~GLC_Bayes(void);
	bool GLC_Bayes_Train(double ** triandata,int triandata_row,int triandata_line);
private:
	CvNormalBayesClassifier m_nbc;
	double ** m_traindata;
	int m_traindata_row;
	int m_traindata_line;
public:
	int GLC_Bayes_ExcuteClassify(double* data);
};

