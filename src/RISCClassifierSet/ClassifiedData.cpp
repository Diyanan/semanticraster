#include "StdAfx.h"
#include "ClassifiedData.h"



ClassifiedData::ClassifiedData()
	: m_classifiedData(NULL)
	

{
	
	StatisticsI=0;
}


ClassifiedData::~ClassifiedData(void)
{
	delete [] resultEveryT;
}


void ClassifiedData::SetData(int index/*Ԫ�������*/,int classT/*����Ĵ���*/)
{
	i=index;
	DataIndex=i;
	classificationTimes=classT;
	resultEveryT=new ClassifiedResutEveryTime[classificationTimes];
	for (int i=0;i<classificationTimes;i++)
	{
		resultEveryT[i].tuplesClassID=-1;//��ʼ����δ����֮ǰΪ-1
		resultEveryT[i].pre=-1;
	}
}


// ɾ�����е����ݺ��ڴ�
void ClassifiedData::Delete(void)
{
	if (resultEveryT!=NULL)
	{
		delete[] resultEveryT;
	}
	if (m_classifiedData!=NULL)
	{
		delete[] m_classifiedData;
	}
}
