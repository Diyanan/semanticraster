#include "stdafx.h"
#include "VerifyFeatureSet.h"

namespace VerifyFeatureSet
{
void ExecuteVerifyAlgorithm(char* imageFile,int verifyBandNum,int* verifyBandIndexs,int regionID,int imageBorderPointNum,STPoint* imageBorder,char* verifyFeatures,PFCALLBACK gCallBack)
{
	if(m_FeatureCalculator == NULL)
		m_FeatureCalculator = new FeatureCalculator();
	if(!m_FeatureCalculator->m_Initialized)
		m_FeatureCalculator->Initialize(imageFile,regionID,imageBorderPointNum,imageBorder,gCallBack);
	else
	{
		if(m_FeatureCalculator->SampleValid(imageFile,verifyBandNum,verifyBandIndexs,regionID) == false)
		{
			m_FeatureCalculator->Reset();
			m_FeatureCalculator->Initialize(imageFile,regionID,imageBorderPointNum,imageBorder,gCallBack);
		}
	}
	if(m_FeatureCalculator->m_Initialized)
		m_FeatureCalculator->execute(verifyBandNum,verifyBandIndexs,verifyFeatures);
}
}