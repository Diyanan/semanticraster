#pragma once

#include <vector>
#include "VerifyResultWrapper.h"
#include "FeatureCalculator.h"

using namespace System::Runtime::InteropServices;
using namespace VerifyFeatureSet;

namespace VerifyFeatureSetWrapper 
{	
	public delegate void ExecuteAlgorithmCallBack(VerifyResultWrapper verifyResult);
	public delegate int PFCallBack(VerifyResult* verifyResult);

	public ref class FeatureCalculatorWrapper
	{
	private:
		FeatureCalculator* m_FeatureCalculator;
	private:
		// »Øµ÷
		GCHandle delegateHandle; 
		PFCallBack^ nativeCallback; 
		ExecuteAlgorithmCallBack^ ProcessVerifyResult;
		int VerifyResultCallback(VerifyResult *result);
	public:
		FeatureCalculatorWrapper(ExecuteAlgorithmCallBack^ callBack);
		~FeatureCalculatorWrapper(void);
	public:
		void Execute(int verifyBandNum,System::IntPtr verifyBandIndexs,System::String^ verifyFeatures);
	public:
		void Init(System::String^ imageFile,int regionID,int imageBorderPointNum,System::IntPtr imageBorder);
	};
	
}

