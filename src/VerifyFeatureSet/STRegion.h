#pragma once

#include <opencv2\opencv.hpp>
using namespace cv;

#include <vector>
#include <map>
#include "STPoint.h"
#include "SampleImage.h"
#include <windows.h>

namespace VerifyFeatureSet
{
	//多边形区域
	class STRegion
	{
	public:		
		STRegion();
		~STRegion(void);
		void Reset();
		void CalculateBounds(int imageBorderPointNum,STPoint* imageBorder);
		void UpdateBounds(short Bounds[4]);
		void Initialize(SampleImage* sampleImage,int regionID,int imageBorderPointNum,STPoint* imageBorder);//人工勾选出样本区时imageBorder存放的是边界区关键点而不是所有点，需要转换为所有点后存入polyPoints
		bool IsPointInRegion(int x,int y,Region* aRegion);//判断点是否在Region内。x、y分别表示列坐标、行坐标
		int                         regionID;
		bool                        valid;
		//std::vector<float>			means;			//平均灰度值
		int							area;			//面积，单位为象素
		std::vector<int>			pixels;			//记录区域包含的象素序号,该序号是针对整幅影像构建而成
		//VectorTS<int>			    pixels;			//记录区域包含的象素序号,该序号是针对整幅影像构建而成
		short						bounds[4];		//区域的边界框的范围 top,down,left,right  (该边界框与坐标系平行)		
		short						len;			//周长
		//float						dev;			//标准差，指的是各波段标准差的均值
		std::vector<STPoint>        polyPoints;     //全部边界点，用于显示，该序列存放的是边界上的所有点，而不是关键点
		int                         linesNum;       //边界线条数
		std::vector<long>           linePointsIndex; //多边界时，用于记录新边界起始点的索引	
		//如此设计polyPoints、linesNum、linePointsIndex使得本结构具有处理内部有多个空洞的影像区域的能力，但在本系统中样本区
		//不会存在空洞，所以有边界线条数等于1		

		//下面的是由OpenCV计算得到的为计算形状特征而需要的结构
		CvPoint2D32f                minBoundRect[4];//最小边界矩形，可不与坐标系平行，是能框住区域的最小矩形
		CvBox2D                     olsEcllipsBox;//最小二乘拟合椭圆的Box
		std::vector<int>			outerPixels;			//记录区域向外扩一个像素后影像包含的象素序号,用于计算协方差矩阵
		int                         outerPolyPointNum;
		STPoint*                    outerPolyPoints;     //记录区域向外扩一个像素后影像边界点，用于计算协方差矩阵，该序列存放的是边界上的关键点
		CvMat*                      covarMatrix;//协方差矩阵
		bool                        covarMatrixValid;
	private:
		void InterpolateBorder(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder,std::vector<STPoint>* points);//将人工勾选边界区关键点转换为所有点后存入polyPoints
		void UpdateRegionPixel(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder);//更新区域中所有点索引pixels
		//void UpdateOuterRegionPixel(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder);//得到区域向外扩一个像素后影像包含的象素序号后存入outerPixels
		void UpdateRegionShape();//利用OpenCV计算最小外接矩形，拟合椭圆等形状结构
		void CalculateCovarMatrix(SampleImage* m_SampleImage);//利用OpenCV

		//void Box2Rect(CvBox2D box,CvPoint2D32f* rect);
		SampleImage* m_SampleImage;		

		Mutex *lock;
	};
}

