#include "StdAfx.h"
#include "GLC_Classifier.h"


GLC_Classifier::GLC_Classifier(void)
	: m_data_row(0)
	, m_testclassID(NULL)

	, m_classifiedresultID(NULL)
{
}
GLC_Classifier::GLC_Classifier(double ** data,int data_row,int data_line)
{
	/*m_data=data;
	m_data_row=data_row;
	m_data_line=data_line;*/
}

GLC_Classifier::~GLC_Classifier(void)
{
	int xx=0;
}
