#include "StdAfx.h"
#include "FeatureCalculator.h"
//using namespace System::Reflection;

namespace VerifyFeatureSet
{
FeatureCalculator::FeatureCalculator()
{
	m_Initialized = false;

	m_SampleRegion = NULL;
	m_SampleImage = NULL;
	featureAlgorithm = NULL;
}


FeatureCalculator::~FeatureCalculator(void)
{
}

bool FeatureCalculator::SampleValid(char* imageFile,int regionID)
{
	bool sampleValid = true;

	if(strcmp(m_SampleImage->m_FileName,imageFile) != 0)
		sampleValid = false;
	else if(regionID != m_SampleRegion->regionID)
		sampleValid = false;

	return sampleValid;
}

bool FeatureCalculator::SampleValid(char* imageFile,int verifyBandNum,int* verifyBandIndexs,int regionID)
{
	bool sampleValid = true;

	if(strcmp(m_SampleImage->m_FileName,imageFile) != 0)
		sampleValid = false;
	else if(regionID != m_SampleRegion->regionID)
		sampleValid = false;
	else
	{
		for(int index = 0;index < verifyBandNum;index++)
		{
			int bandIndex = verifyBandIndexs[index];
			if (m_SampleImage->IsBandCached(bandIndex) == false) //没找到
			{
				sampleValid = false;
				break;
			}
		}
	}

	return sampleValid;
}

void FeatureCalculator::Initialize(char* imageFile,int regionID,int imageBorderPointNum,STPoint* imageBorder,/*char* verifyFeatures,*/PFCALLBACK gCallBack)
{
	m_CallBack = gCallBack;

	/*string featureString(verifyFeatures);
	string semicolon = ";";
	m_VerifyFeatures.clear();
	split(featureString,semicolon,&m_VerifyFeatures);*/

	// Add by zsl at 2015-10-19
	// 将成员的构造部分从构造函数迁移至初始化函数

	m_SampleRegion = new STRegion();
	m_SampleImage = new SampleImage();
	featureAlgorithm = new FeatureAlgorithm();
	
	m_SampleRegion->CalculateBounds(imageBorderPointNum,imageBorder);
	m_SampleImage->Initialize(imageFile,m_SampleRegion->bounds);
	if(m_SampleImage->m_Initialized)
	{
		m_SampleRegion->UpdateBounds(m_SampleImage->m_Bounds);
		//将面向全图的imageBorder改为面向局部区域的imageBorder，从而与m_SampleImage缓存的影像区域一致
		STPoint* imageRegionBorder = new STPoint[imageBorderPointNum];
		for(int i=0;i<imageBorderPointNum;i++)
		{
			imageRegionBorder[i].X = imageBorder[i].X - m_SampleRegion->bounds[2];
			imageRegionBorder[i].Y = imageBorder[i].Y - m_SampleRegion->bounds[0];
		}
		m_SampleRegion->bounds[1] -= m_SampleRegion->bounds[0];
		m_SampleRegion->bounds[3] -= m_SampleRegion->bounds[2];
		m_SampleRegion->bounds[0] = 0;
		m_SampleRegion->bounds[2] = 0;

		m_SampleRegion->Initialize(m_SampleImage,regionID,imageBorderPointNum,imageRegionBorder);
		if(m_SampleRegion->valid)
		{
			featureAlgorithm->Initialize(m_SampleImage,m_SampleRegion,m_CallBack);
			if(featureAlgorithm->m_Initialized)
			{
				m_Initialized = true;
			}

			featureAlgorithm->aGP = new GraphicsPath();
			featureAlgorithm->aGP->AddPolygon(imageRegionBorder,imageBorderPointNum);
			featureAlgorithm->aRegion = new Region(featureAlgorithm->aGP);
		}

		delete []imageRegionBorder;
	}
}

void FeatureCalculator::Reset()
{
	if(m_Initialized)
	{
		m_VerifyFeatures.clear();
		delete featureAlgorithm;
		featureAlgorithm = NULL;
		delete m_SampleRegion;
		m_SampleRegion = NULL;
		delete m_SampleImage;
		m_SampleImage = NULL;
	}

	m_Initialized = false;
}

//注意：当字符串为空时，也会返回一个空字符串  
void FeatureCalculator::split(std::string& s, std::string& delim,std::vector< std::string >* ret)  
{  
	if(s.length() == 0)
		return;

	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=std::string::npos)  
	{  
		string substr = s.substr(last,index-last);
		trim(substr);
		ret->push_back(substr);  
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	if (index-last>0)  
	{  
		string substr = s.substr(last,index-last);
		trim(substr);
		ret->push_back(substr);  
	}  
}

void FeatureCalculator::trim(string& str) 
{ 
	string::size_type pos = str.find_last_not_of(' '); 
	if(pos != string::npos) 
	{ 
		str.erase(pos + 1); 
		pos = str.find_first_not_of(' '); 
		if(pos != string::npos) str.erase(0, pos); 
	} 
	else 
		str.erase(str.begin(), str.end()); 
	//return str; 
} 

void FeatureCalculator::execute(int verifyBandNum,int* verifyBandIndexs,char* verifyFeatures)
{
	// featureAlgorithm->m_VerifyBands.clear();
	// for(int i = 0;i<verifyBandNum;i++)
	//	featureAlgorithm->m_VerifyBands.push_back(verifyBandIndexs[i]);	
	
	// 使用函数局部变量替换成员变量
	std::vector<int> verifyBands;//存放待检验波段序号
	for(int i = 0;i<verifyBandNum;i++)
		verifyBands.push_back(verifyBandIndexs[i]);	

	string featureString(verifyFeatures);
	string semicolon = ";";

	// change by zsl，2015-8-24，for parallel execute
	// m_VerifyFeatures.clear();
	// split(featureString,semicolon,&m_VerifyFeatures);

	// for(int i=0;i<m_VerifyFeatures.size();i++)
	// {
	//	 featureAlgorithm->execute(m_VerifyFeatures[i]);
	// }

	vector<string> verifyFeatureList;
	split(featureString,semicolon,&verifyFeatureList);

	for(int i=0;i<verifyFeatureList.size();i++)
	{
		featureAlgorithm->execute(verifyFeatureList[i], verifyBands);
	}
	
}


}
