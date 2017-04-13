#pragma once
 struct ClassifiedResutEveryTime  //存储分类后元组信息，可以存在于叶子节点上
{ 

	int tuplesClassID;   //元组的类别	
	double pre;//预测权重
	bool isvotied;

};
 struct ResutEveryTime  //存储分类后元组信息，可以存在于叶子节点上
{ 
	int tuplesClassID;   //元组的类别	
	double pre;//预测权重
	bool isProseed;
};
class ClassifiedData
{
private:
		int i;                //元组的序号 
public:	
		int classificationTimes;   //分类次数
        ClassifiedResutEveryTime * resultEveryT;       //元组的分类信息
		int classId;   //最终类别
		int DataIndex; //元组在数据中的索引
		ClassifiedResutEveryTime Statistics[30];
		int StatisticsI;

public:
	ClassifiedData();
	~ClassifiedData(void);
	void SetData(int index, int classT);
private:
	ClassifiedData *m_classifiedData;
public:
	// 删除所有的数据和内存
	void Delete(void);
};

