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


void ClassifiedData::SetData(int index/*元组的索引*/,int classT/*分类的次数*/)
{
	i=index;
	DataIndex=i;
	classificationTimes=classT;
	resultEveryT=new ClassifiedResutEveryTime[classificationTimes];
	for (int i=0;i<classificationTimes;i++)
	{
		resultEveryT[i].tuplesClassID=-1;//初始化，未分类之前为-1
		resultEveryT[i].pre=-1;
	}
}


// 删除所有的数据和内存
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
