#pragma once
 struct ClassifiedResutEveryTime  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
{ 

	int tuplesClassID;   //Ԫ������	
	double pre;//Ԥ��Ȩ��
	bool isvotied;

};
 struct ResutEveryTime  //�洢�����Ԫ����Ϣ�����Դ�����Ҷ�ӽڵ���
{ 
	int tuplesClassID;   //Ԫ������	
	double pre;//Ԥ��Ȩ��
	bool isProseed;
};
class ClassifiedData
{
private:
		int i;                //Ԫ������ 
public:	
		int classificationTimes;   //�������
        ClassifiedResutEveryTime * resultEveryT;       //Ԫ��ķ�����Ϣ
		int classId;   //�������
		int DataIndex; //Ԫ���������е�����
		ClassifiedResutEveryTime Statistics[30];
		int StatisticsI;

public:
	ClassifiedData();
	~ClassifiedData(void);
	void SetData(int index, int classT);
private:
	ClassifiedData *m_classifiedData;
public:
	// ɾ�����е����ݺ��ڴ�
	void Delete(void);
};

