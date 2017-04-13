#include "StdAfx.h"
#include "SampleImage.h"
#include <algorithm>

namespace VerifyFeatureSet
{
SampleImage::SampleImage()
{
	GDALAllRegister(); 

	m_Initialized = false;
}


SampleImage::~SampleImage(void)
{
	Reset();
}

void SampleImage::Reset(void)
{
	if(m_Initialized)
	{
		for(int k=0; k<m_CachedBandNum; k++)
		{	
			delete m_BandBuffers[k];
		}
		delete m_BandBuffers;

		m_CachedBands.clear();
		delete m_FileName;
	}

	m_Initialized = false;
}

void SampleImage::Initialize(char* imageFile,short bounds[4])
{
	/*m_Initialized = false;*/
	Reset();

	//Ӱ��������
	m_FileName = new char[strlen(imageFile)+1];
	strcpy(m_FileName,imageFile);

	//GDALAllRegister();  
	GDALDataset *poDataset = (GDALDataset*)GDALOpen(m_FileName,GA_ReadOnly);  
	if( poDataset == NULL )  
	{  
		return;  
	} 	
	m_Width = poDataset->GetRasterXSize();
	m_Height = poDataset->GetRasterYSize();
	m_BandNum = poDataset->GetRasterCount();



	//���鴦������
	bounds[0] = max(bounds[0],0);
	bounds[1] = min(bounds[1],m_Height - 1);
	bounds[2] = max(bounds[2],0);
	bounds[3] = min(bounds[3],m_Width - 1);
	for(int i = 0;i<4;i++)
	{
		m_Bounds[i] = bounds[i];
	}

	// ��ʼ�����沨����Ϣ
	
	m_CachedBands.clear();

	int verifyBandNum = poDataset->GetRasterCount();
	for(int i = 0;i<verifyBandNum;i++)
		m_CachedBands.push_back(i+1);	
	m_CachedBandNum = m_CachedBands.size();
	//if(m_CachedBandNum >=3 )
	//	m_CachedBandNum++;//����һ���������ڴ�����Ȳ��Σ��ò��δ���������
	m_BandBuffers = new BYTE*[m_CachedBandNum];
	if (m_BandBuffers == NULL)
	{
		return;
	}
	
	//��ȡ����
	GDALRasterBand* poBand;
	//int readWidth = m_Width;
	//int readHeight = m_Height;
	int readWidth = m_Bounds[3]-m_Bounds[2]+1;
	int readHeight = m_Bounds[1]-m_Bounds[0]+1;

	for(int k=0; k<m_CachedBandNum; k++)
	{	
		int readBandIndex = m_CachedBands[k];		
		poBand = poDataset->GetRasterBand(readBandIndex); 
		imageDataType = poBand->GetRasterDataType();
		int dataSize = 0;//�ж�����λ��
		if(imageDataType==1)     //8λ����
		{	
			dataSize = 1;
		}
		else if(imageDataType==2||imageDataType==3)     //16λ����
		{	
			dataSize = 2;
		}
		else if(imageDataType==4||imageDataType==5||imageDataType==6)//32λ����
		{	
			dataSize = 4;
		}
		else //UnKnow���ͣ�64λ���ͣ�typecount����
		{
			return;
		}
		m_BandBuffers[k] = new BYTE[readWidth*readHeight*dataSize];
		if (m_BandBuffers[k]==NULL)
		{
			return;
		}
		poBand->RasterIO( GF_Read, m_Bounds[2], m_Bounds[0],readWidth ,readHeight ,m_BandBuffers[k], readWidth, readHeight, imageDataType,0,0);
		//poBand->RasterIO( GF_Read, 0, 0,readWidth ,readHeight ,m_BandBuffers[k], readWidth, readHeight, imageDataType,0,0);
	}	
	
	GDALClose(poDataset);
	/*CPLFree(poBand);
	CPLFree(poDataset);*/

	m_Initialized = true;
}

void SampleImage::Initialize(char* imageFile,int verifyBandNum,int* verifyBandIndexs,short bounds[4])
{
	/*m_Initialized = false;*/
	Reset();

	//Ӱ��������
	m_FileName = new char[strlen(imageFile)+1];
	strcpy(m_FileName,imageFile);

	//GDALAllRegister();  
	GDALDataset *poDataset = (GDALDataset*)GDALOpen(m_FileName,GA_ReadOnly);  
	if( poDataset == NULL )  
	{  
		return;  
	} 	
	m_Width = poDataset->GetRasterXSize();
	m_Height = poDataset->GetRasterYSize();
	m_BandNum = poDataset->GetRasterCount();



	//���鴦������
	bounds[0] = max(bounds[0],0);
	bounds[1] = min(bounds[1],m_Height - 1);
	bounds[2] = max(bounds[2],0);
	bounds[3] = min(bounds[3],m_Width - 1);
	for(int i = 0;i<4;i++)
	{
		m_Bounds[i] = bounds[i];
	}
	
	m_CachedBands.clear();
	for(int i = 0;i<verifyBandNum;i++)
		m_CachedBands.push_back(verifyBandIndexs[i]);	
	m_CachedBandNum = m_CachedBands.size();
	//if(m_CachedBandNum >=3 )
	//	m_CachedBandNum++;//����һ���������ڴ�����Ȳ��Σ��ò��δ���������
	m_BandBuffers = new BYTE*[m_CachedBandNum];
	if (m_BandBuffers == NULL)
	{
		return;
	}
	
	//��ȡ����
	GDALRasterBand* poBand;
	//int readWidth = m_Width;
	//int readHeight = m_Height;
	int readWidth = m_Bounds[3]-m_Bounds[2]+1;
	int readHeight = m_Bounds[1]-m_Bounds[0]+1;

	for(int k=0; k<m_CachedBandNum; k++)
	{	
		int readBandIndex = m_CachedBands[k];		
		poBand = poDataset->GetRasterBand(readBandIndex); 
		imageDataType = poBand->GetRasterDataType();
		int dataSize = 0;//�ж�����λ��
		if(imageDataType==1)     //8λ����
		{	
			dataSize = 1;
		}
		else if(imageDataType==2||imageDataType==3)     //16λ����
		{	
			dataSize = 2;
		}
		else if(imageDataType==4||imageDataType==5||imageDataType==6)//32λ����
		{	
			dataSize = 4;
		}
		else //UnKnow���ͣ�64λ���ͣ�typecount����
		{
			return;
		}
		m_BandBuffers[k] = new BYTE[readWidth*readHeight*dataSize];
		if (m_BandBuffers[k]==NULL)
		{
			return;
		}
		poBand->RasterIO( GF_Read, m_Bounds[2], m_Bounds[0],readWidth ,readHeight ,m_BandBuffers[k], readWidth, readHeight, imageDataType,0,0);
		//poBand->RasterIO( GF_Read, 0, 0,readWidth ,readHeight ,m_BandBuffers[k], readWidth, readHeight, imageDataType,0,0);
	}	
	
	GDALClose(poDataset);
	/*CPLFree(poBand);
	CPLFree(poDataset);*/

	m_Initialized = true;
}

bool SampleImage::IsBandCached(int bandIndex)
{
	//vector<int>::iterator result = find(m_CachedBands.begin(), m_CachedBands.end(), bandIndex ); //����
	//if ( result == m_CachedBands.end( ) ) //û�ҵ�
	//	return false;
	//else
	//	return true;

	bool result = false;
	for(int i=0;i<m_CachedBands.size();i++)
	{
		if(m_CachedBands[i] == bandIndex)
		{
			result = true;
			break;
		}
	}
	return result;
}

int SampleImage::ImageBandIndexToCachedBandIndex(int imageBandIndex)
{
	for(int i=0;i<m_CachedBands.size();i++)
	{
		if(m_CachedBands[i] == imageBandIndex)
		{
			return i;
		}
	}

	return -1;
}

//int SampleImage::SampleRegionIndexToCachedRegionIndex(int SampleRegionIndex)
//{	
//	int boundWidth = m_Bounds[3] - m_Bounds[2] + 1;
//	if(m_Bounds[0] != 0 || m_Bounds[2] != 0 || m_Width != boundWidth)//��regionindexת��Ϊ���bounds��Χ��index
//	{		
//		int row = SampleRegionIndex / m_Width;
//		int col = SampleRegionIndex - row*m_Width;
//		row -= m_Bounds[0];
//		col -= m_Bounds[2];
//		return row * boundWidth + col;
//	}
//
//	return SampleRegionIndex;
//}

float SampleImage::GetSamplePixel(int bandIndex,int regionIndex)
{
	//�����regionIndex���������Ӱ���index��������Ϊ�˼����������洢����bounds��Χ�ڵ�Ӱ��
	//regionIndex = SampleRegionIndexToCachedRegionIndex(regionIndex);
	
	//��������bandIndexҲ��Ҫ��������в��λ���Ϊ��Զ�����Ӱ�񲨶�
	bandIndex = ImageBandIndexToCachedBandIndex(bandIndex);

	int dataSize = 0;
	switch(imageDataType)
	{
	case GDT_Byte:
		dataSize = 1;
		return (float)(*(m_BandBuffers[bandIndex]+regionIndex*dataSize));
		break;
	case GDT_UInt16:
		dataSize = 2;
		return (float)(*((unsigned short*)(m_BandBuffers[bandIndex]+regionIndex*dataSize)));
		break;
	case GDT_Int16:
		dataSize = 2;
		return (float)(*((short*)(m_BandBuffers[bandIndex]+regionIndex*dataSize)));
		break;
	case GDT_UInt32:
		dataSize = 4;
		return (float)(*((unsigned int*)(m_BandBuffers[bandIndex]+regionIndex*dataSize)));
		break;
	case GDT_Int32:
		dataSize = 4;
		return (float)(*((int*)(m_BandBuffers[bandIndex]+regionIndex*dataSize)));
		break;
	case GDT_Float32:
		dataSize = 4;
		return *((float*)(m_BandBuffers[bandIndex]+regionIndex*dataSize));
		break;
	default:
		return -1;
		break;
	}
}

BYTE SampleImage::SamplePixel2BYTE(float pixelValue)
{	
	float returnValue = pixelValue;
	
	/*if(imageDataType == GDALDataType::GDT_CInt16)
		returnValue = */

	return returnValue;
}
}