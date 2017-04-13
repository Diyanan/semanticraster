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
	bool SampleValid(char* imageFile,int verifyBandNum,int* verifyBandIndexs,int regionID);//�ж��Ƿ���Ҫʹ��Initialize����������Ϣ
	// zsl ������֤���ʼ����������Calculator��֤�ж������
	bool SampleValid(char* imageFile,int regionID);
	void Initialize(char* imageFile,int regionID,int imageBorderPointNum,STPoint* imageBorder,PFCALLBACK gCallBack);
	void Reset();

	void execute(int verifyBandNum,int* verifyBandIndexs,char* verifyFeatures);
	SampleImage* m_SampleImage;//�������Ӱ��
	bool m_Initialized;
private:		
	STRegion* m_SampleRegion;//�����������
	FeatureAlgorithm* featureAlgorithm;
	vector<string> m_VerifyFeatures;//��Ŵ�����������	

	//ע�⣺���ַ���Ϊ��ʱ��Ҳ�᷵��һ�����ַ���  
	void split(std::string& s, std::string& delim,std::vector< std::string >* ret);	
	void trim(string& str);
	PFCALLBACK m_CallBack;

};
}

