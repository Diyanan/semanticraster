#pragma once

namespace VerifyFeatureSetWrapper 
{
public value struct VerifyResultWrapper
{
public:
	System::String^ featureName;
	System::String^ layerName;
	System::String^ status;
	bool resultValid;
	int rowNum;
	int colNum;
	cli::array<double>^ results;
	int fId;

public:
	double GetResult(int row, int col)
	{
		if(!resultValid || row < 1 || row > rowNum || col < 1 || col > colNum)
		{
			return 0;
		}

		int offset = (row - 1) * colNum + col - 1;
		return results[offset];
	}
};
}