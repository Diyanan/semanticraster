// 这是主 DLL 文件。

#include "stdafx.h"

#include "ChangeDetectWrapper.h"
#include "ChangeDTCClassifierAPI.h"

using namespace ChangeDetectWrapper;
using namespace System::Runtime::InteropServices;

void CDTWrapper::Classifier(System::String^ trainFile, System::String^ testFile, System::String^ outFile, int method)
{
	char* pathTrian =  (char*)(void*)Marshal::StringToHGlobalAnsi(trainFile); 
	char* pathTest =  (char*)(void*)Marshal::StringToHGlobalAnsi(testFile); 
	char* pathOutput =  (char*)(void*)Marshal::StringToHGlobalAnsi(outFile); 
	switch(method)
	{
	case 0:
	    test_ANNClassify(pathTrian,pathTest,pathOutput,250);
	    break;
	case 1:
		test_SVMClassify(pathTrian,pathTest,pathOutput);
		break;
	case 2:
		test_BayesClassify(pathTrian,pathTest,pathOutput);
		break;
	case 3:
		test_KNNClassify(pathTrian,pathTest,pathOutput);
		break;
	case 4:
		test_TreeClassify(pathTrian,pathTest,pathOutput);
		break;
	case 5:
		test_AdTreeClassify(pathTrian,pathTest,pathOutput,5);
		break;
	case 6:
		testJC1(pathTrian,pathTest,pathOutput);
		break;
	case 7:
		testJC2(pathTrian,pathTest,pathOutput);
		break;
	}
}

