#pragma once
#include "STPoint.h"
#include "gdal_priv.h"
#include <vector>
using namespace std;

namespace VerifyFeatureSet
{
class SampleImage
{
public:
	SampleImage();
	~SampleImage(void);
	void Reset();
	void Initialize(char* imageFile,short bounds[4]);
	void Initialize(char* imageFile,int verifyBandNum,int* verifyBandIndexs,short bounds[4]);

	//影像本身属性
	char* m_FileName;
	int m_Width;
	int m_Height;
	int m_BandNum;
	GDALDataType imageDataType;

	//检验处理属性
	//int m_VerifyBandNum;	
    short m_Bounds[4];// m_imageBorders区域的边界框的范围 top,down,left,right;
	
	int m_CachedBandNum;
	std::vector<int> m_CachedBands;
	BYTE** m_BandBuffers;//波段数据，主要只读出了m_erifyBandIndexs波段的数据
	bool IsBandCached(int bandIndex);//判断波段是否已经缓存

	//供检验算法使用的主要函数，无需了解数据类型即可获取数据值,因为存储的数据可能为float，所以输出统一为浮点型
	float GetSamplePixel(int bandIndex,int regionIndex);//regionIndex是STRegion结构中样本像素在影像中的索引号
	BYTE SamplePixel2BYTE(float pixelValue);//将影像值转为BYTE类型
	int ImageBandIndexToCachedBandIndex(int imageBandIndex);//将针对影像的波段索引bandIndex换算为针对已经读出的影像波段
	//int SampleRegionIndexToCachedRegionIndex(int SampleRegionIndex);//将SampleRegion针对整幅影像的像素索引SampleRegionIndex换算为针对已经读出的影像区域索引

	bool m_Initialized;

};
}

