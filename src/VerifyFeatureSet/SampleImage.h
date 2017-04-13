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

	//Ӱ��������
	char* m_FileName;
	int m_Width;
	int m_Height;
	int m_BandNum;
	GDALDataType imageDataType;

	//���鴦������
	//int m_VerifyBandNum;	
    short m_Bounds[4];// m_imageBorders����ı߽��ķ�Χ top,down,left,right;
	
	int m_CachedBandNum;
	std::vector<int> m_CachedBands;
	BYTE** m_BandBuffers;//�������ݣ���Ҫֻ������m_erifyBandIndexs���ε�����
	bool IsBandCached(int bandIndex);//�жϲ����Ƿ��Ѿ�����

	//�������㷨ʹ�õ���Ҫ�����������˽��������ͼ��ɻ�ȡ����ֵ,��Ϊ�洢�����ݿ���Ϊfloat���������ͳһΪ������
	float GetSamplePixel(int bandIndex,int regionIndex);//regionIndex��STRegion�ṹ������������Ӱ���е�������
	BYTE SamplePixel2BYTE(float pixelValue);//��Ӱ��ֵתΪBYTE����
	int ImageBandIndexToCachedBandIndex(int imageBandIndex);//�����Ӱ��Ĳ�������bandIndex����Ϊ����Ѿ�������Ӱ�񲨶�
	//int SampleRegionIndexToCachedRegionIndex(int SampleRegionIndex);//��SampleRegion�������Ӱ�����������SampleRegionIndex����Ϊ����Ѿ�������Ӱ����������

	bool m_Initialized;

};
}

