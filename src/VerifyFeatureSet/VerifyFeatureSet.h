#pragma once
#include "dllDefine.h"
#include "STPoint.h"
#include "FeatureCalculator.h"
#include "VerifyResult.h"

namespace VerifyFeatureSet
{
// 改造为全局单间
static FeatureCalculator* m_FeatureCalculator;


//typedef int (WINAPI *PFCALLBACK)(VerifyResult* verifyResult);
VERIFYFEATURESETDLL_API void ExecuteVerifyAlgorithm(char* imageFile,int verifyBandNum,int* verifyBandIndexs,int regionID,int imageBorderPointNum,STPoint* imageBorder,char* verifyFeatures,PFCALLBACK gCallBack);

}