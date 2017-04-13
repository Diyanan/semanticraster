#include "StdAfx.h"
#include "VerifyResult.h"

namespace VerifyFeatureSet
{
	VerifyResult::VerifyResult(std::string m_FeatureName, std::string m_LayerName, std::string m_Status, int m_RowNum, int m_ColNum, int regionId)
	{
		fId = regionId;
		strcpy_s(featureName, m_FeatureName.c_str());
		strcpy_s(layerName, m_LayerName.c_str());
		strcpy_s(status, m_Status.c_str());

		if (m_Status.compare("Feature Finished") == 0)
		{
			resultValid = true;
			rowNum = m_RowNum;
			colNum = m_ColNum;
			results = new double[rowNum*colNum];
		}
		else
			resultValid = false;
	}
	VerifyResult::~VerifyResult()
	{
		if (results != NULL)
		{
			delete results;
			results = NULL;
		}
	}
}