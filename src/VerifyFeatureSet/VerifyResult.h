#pragma once
#include <string> 

namespace VerifyFeatureSet
{
class VerifyResult
{
public:
	char featureName[64];//特征名
	char layerName[64];//特征应用的图层，命名规则为“Layer Bright”、“Layer1”、“Layer2”、...
	char status[64];//执行状态，目前允许“Feature Finished”、“Feature Error”、“Input Error”、“FeatureSet Finished”，分别表示特征计算正常结束、特征计算错误，特征输入错误、算法集计算正常结束 
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