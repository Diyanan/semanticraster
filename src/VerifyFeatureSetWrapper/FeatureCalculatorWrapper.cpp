#include "StdAfx.h"
#include "FeatureCalculatorWrapper.h"
#include "WrapPoint.h"

using namespace VerifyFeatureSet;
using namespace VerifyFeatureSetWrapper;
using namespace System::Runtime::InteropServices;

FeatureCalculatorWrapper::FeatureCalculatorWrapper(ExecuteAlgorithmCallBack^ callBack)
{
	ProcessVerifyResult=callBack;
	m_FeatureCalculator = new FeatureCalculator();

	// ����ص�����
	this->nativeCallback = gcnew PFCallBack(this, &FeatureCalculatorWrapper::VerifyResultCallback);  
  	
	// ��֤ί�в��ᱻ�ڴ��ƶ����������յ�  
	this->delegateHandle = GCHandle::Alloc(this->nativeCallback);  
}

FeatureCalculatorWrapper::~FeatureCalculatorWrapper(void)
{
	if(m_FeatureCalculator != NULL)
	{
		m_FeatureCalculator->Reset();
		delete m_FeatureCalculator;
		m_FeatureCalculator = NULL;
	}
	// �ͷ�ί�о��  
	if (this->delegateHandle.IsAllocated)  
	{
		this->delegateHandle.Free();  
	}
}

int FeatureCalculatorWrapper::VerifyResultCallback(VerifyResult *result)
{
	VerifyResultWrapper wrapper;
	wrapper.featureName = gcnew System::String(result->featureName);
	wrapper.layerName = gcnew System::String(result->layerName);
	wrapper.status = gcnew System::String(result->status);
	wrapper.resultValid = result->resultValid;
	wrapper.rowNum = result->rowNum;
	wrapper.colNum = result->colNum;
	int resultCount = wrapper.rowNum * wrapper.colNum;
	wrapper.results = gcnew cli::array<double>(resultCount);
	Marshal::Copy(static_cast<System::IntPtr>(result->results), wrapper.results, 0, resultCount);
	wrapper.fId = result->fId;

	// �ص�
	ProcessVerifyResult(wrapper);

	return 0;
}

void FeatureCalculatorWrapper::Init(System::String^ imageFile,int regionID,int imageBorderPointNum,System::IntPtr imageBorder)
{
	char* file =  (char*)(void*)Marshal::StringToHGlobalAnsi(imageFile); 
	WrapPoint* pts = (WrapPoint*)(imageBorder.ToPointer());
	STPoint* stPts = (STPoint*)pts;
	// ת��Ϊ����ָ��ע��  
	System::IntPtr ptr = Marshal::GetFunctionPointerForDelegate(this->nativeCallback); 
	PFCALLBACK gCallBack =  static_cast<PFCALLBACK>(ptr.ToPointer()); 

	if(!m_FeatureCalculator->m_Initialized)
		m_FeatureCalculator->Initialize(file,regionID,imageBorderPointNum,stPts,gCallBack);
	else
	{
		if(m_FeatureCalculator->SampleValid(file,regionID) == false)
		{
			m_FeatureCalculator->Reset();
			m_FeatureCalculator->Initialize(file,regionID,imageBorderPointNum,stPts,gCallBack);
		}
	}
}

void FeatureCalculatorWrapper::Execute(int verifyBandNum,System::IntPtr verifyBandIndexs,System::String^ verifyFeatures)
{
	char* algorithmName =  (char*)(void*)Marshal::StringToHGlobalAnsi(verifyFeatures); 
	int* bandIndexs = (int*)(verifyBandIndexs.ToPointer());
	m_FeatureCalculator->execute(verifyBandNum,bandIndexs,algorithmName);
}


