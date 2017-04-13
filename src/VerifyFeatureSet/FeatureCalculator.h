#pragma once
#include "FeatureAlgorithm.h"
#include "STPoint.h"
#include "STRegion.h"
#include "SampleImage.h"
#include <vector>
#include "VerifyResult.h"
#include "FeatureAlgorithm.h"
#include "dllDefine.h"

namespace VerifyFeatureSet
{
class VERIFYFEATURESETDLL_API FeatureCalculator
{
public:
	FeatureCalculator();
	~FeatureCalculator(void);
	bool SampleValid(char* imageFile,int verifyBandNum,int* verifyBandIndexs,int regionID);//判断是否需要使用Initialize更新样本信息
	// zsl 重载验证与初始化函数，简化Calculator验证判断与计算
	bool SampleValid(char* imageFile,int regionID);
	void Initialize(char* imageFile,int regionID,int imageBorderPointNum,STPoint* imageBorder,PFCALLBACK gCallBack);
	void Reset();

	void execute(int verifyBandNum,int* verifyBandIndexs,char* verifyFeatures);
	SampleImage* m_SampleImage;//存放样本影像
	bool m_Initialized;
private:		
	STRegion* m_SampleRegion;//存放样本区域
	FeatureAlgorithm* featureAlgorithm;
	vector<string> m_VerifyFeatures;//存放待检验特征名	

	//注意：当字符串为空时，也会返回一个空字符串  
	void split(std::string& s, std::string& delim,std::vector< std::string >* ret);	
	void trim(string& str);
	PFCALLBACK m_CallBack;

};
}

