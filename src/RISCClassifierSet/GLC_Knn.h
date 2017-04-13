#pragma once
#include "cv.h"
#include "opencv.hpp" 
#include "highgui.h" 
class GLC_Knn
{
public:
	GLC_Knn(void);
	~GLC_Knn(void);
	bool GLC_Knn_Train(double** traindata, int traindata_row, int traindata_line);
	int GLC_Knn_ExcuteClassify(double* data,int nNearest=1);
private:
	double** m_traindata;
	int  m_traindata_row;
	int  m_traindata_line;
	CvKNearest  m_kNearest;
};

