#pragma once
#include <string> 

namespace VerifyFeatureSet
{
class VerifyResult
{
public:
	char featureName[64];//������
	char layerName[64];//����Ӧ�õ�ͼ�㣬��������Ϊ��Layer Bright������Layer1������Layer2����...
	char status[64];//ִ��״̬��Ŀǰ����Feature Finished������Feature Error������Input Error������FeatureSet Finished�����ֱ��ʾ�����������������������������������������㷨�������������� 
	BOOL resultValid;
	int rowNum;
	int colNum;
	double* results;
	int fId;
public:
	VerifyResult(std::string m_FeatureName, std::string m_LayerName, std::string m_Status, int m_RowNum, int m_ColNum, int regionId);
	~VerifyResult();
};

typedef int (WINAPI *PFCALLBACK)(VerifyResult* verifyResult);
}