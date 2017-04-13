#pragma once

#include <opencv2\opencv.hpp>
using namespace cv;

#include "VerifyResult.h"//该头文件必须放在opencv相关头文件的后面，否则编译时出大量错误
#include "STPoint.h"
#include "STRegion.h"
#include "SampleImage.h"
#include "SampleStatistics.h"

namespace VerifyFeatureSet
{
	class FeatureAlgorithm
	{
	public:
		FeatureAlgorithm();
		~FeatureAlgorithm(void);
		void Initialize(SampleImage* sampleImage,STRegion* sampleRegion,PFCALLBACK gCallBack);
		void Reset();

		void execute(string algorithmName, const std::vector<int> verifyBands);
		bool m_Initialized;

		//std::vector<int> m_VerifyBands;//存放待检验波段序号

		GraphicsPath* aGP;
		Region* aRegion;
	private:
		SampleImage* m_SampleImage;//存放样本影像
		STRegion* m_SampleRegion;//存放样本区域		
	
		//下面是灰度共生矩阵及计算灰度共生矩阵需要的变量
		int grayLevel;      //灰度级，即将精度差异很大的影像数据压缩到一定的灰度级数，从而控制计算量到一定范围
		int dirmem[4][4];	//灰度共生矩阵的方向0度、45度、90度、135度
		float* gLCM;        //存储所有图层的4个方向的灰度共生矩阵,按照图层、方向的顺序排列，其中方向为0度、45度、90度、135度顺序
		float* gLDV;        //存储所有图层的4个方向的灰度差分矢量,按照图层、方向的顺序排列，其中方向为0度、45度、90度、135度顺序
		bool bGLCMValid;//标识gLCM是否有效
		bool bGLDVValid;//标识gLCM是否有效

		//下面是样本对象统计信息及计算样本对象统计信息需要的变量
		SampleStatistics* sampleStatistics;        //存储样本对象统计信息，一个图层一个Statistics
		bool bSampleStatisticsValid;//标识ampleStatistics是否有效


		PFCALLBACK m_CallBack;
		template <class T> string ConvertToString(T);
		double DistPointToEcllips(CvPoint2D32f point,CvPoint2D32f focus1,CvPoint2D32f focus2,double dist);//点到椭圆的距离，返回正值、0、负值分别表示在椭圆外、椭圆上、椭圆内

		//下面是缓存的已经计算出的部分特征值，可作为计算其它特征的中间结果
		double m_Brightness;
		bool   m_BrightnessValid;//标识m_Brightness是否有效
		double m_LengthWidthRatio;
		bool   m_LengthWidthRatioValid;//标识m_LengthWidthRatio是否有效
		CvBox2D m_FitEcllipsBox;//ecognition定义的拟合椭圆的Box
		bool   m_FitEcllipsBoxValid;//标识m_FitEcllipsBox是否有效
		CvBox2D m_FitRectangularBox;//ecognition定义的拟合椭圆的Box
		bool   m_FitRectangularBoxValid;//标识m_FitEcllipsBox是否有效
		double m_RadiusLargestEnclosedEllipse;//最大内接椭圆半径
		bool   m_RadiusLargestEnclosedEllipseValid;//标识m_RadiusLargestEnclosedEllipse是否有效
		double m_RadiusSmallestEnclosingEllipse;//最小外包椭圆半径
		bool   m_RadiusSmallestEnclosingEllipseValid;//标识m_RadiusSmallestEnclosingEllipse是否有效

		//以下为具体的特征提取入口算法，完成了查找已完成的中间结果、调用具体的算法模块、结果输出功能
		//光谱特征
		void Mean();//均值
		void Brightness();//亮度
		void StdDev();//标准差	
		void Ratio();//贡献率
		void MaxDiff();//最大异质性
		void InternalConsistency();//内部一致性 
		//形状特征
		void Area();//面积
		void Length();//长度
		void Width();//宽度
		void LengthWidthRatio();//长宽比
		void BorderLength();//边界长度
		void MainDirection();//主要方向
		void Asymmetry();//不对称性
		void Compactness();//紧致度
		void Density();//密度
		void ShapeIndex();//形状指数
		void Rectangle();//矩形度
		void Elongatedness();//细长度
		void Borderindex();//边界指数
		void Roundness();//圆滑度
		void EllipticFit();//椭圆拟合
		void RectangularFit ();//矩形拟合
		void RadiusLargestEnclosedEllipse();//最大内接椭圆半径
		void RadiusSmallestEnclosingEllipse();//最小外包椭圆半径

		//纹理特征
		void GLCMASM();//能量(角二阶矩)
		void GLCMCON();//对比度（惯性矩）
		void GLCMCOR();//相关性
		void GLCMENT();//熵
		void GLCMIDM();//逆差距(均匀度)
		void GLDVASM();//能量(角二阶矩)
		void GLDVCON();//对比度（惯性矩）
		void GLDVMEAN();//相关性
		void GLDVENT();//熵
		void HistogramOriginMoment(int p);//直方图对原点的 阶矩
		void HistogramCentralMoment(int p);//直方图对均值 的 阶中心矩
		void HistogramBias();//直方图的偏度
		void HistogramKurtosis ();//峰度
		void HistogramEnergy();//直方图的能量
		void HistogramEntropy();//直方图的熵

		//指数特征
		void NDVI(std::vector<int> verifyBands);//归一化植被指数
		void NDWI(std::vector<int> verifyBands);//归一化差分水体指数
		void SVI(std::vector<int> verifyBands);//阴影植被指数
		void RVI(std::vector<int> verifyBands);//比值植被指数
		void GNDWI(std::vector<int> verifyBands);//高斯归一化水体指数


		//下面是具体的特征提取算法，该类算法负责存储计算结果（可做其它特征提取算法的中间输入），可供特征提取入口算法及其它具体的特征提取算法调用
		void BrightnessAlgorithm();
		double StdDev(int bandIndex);//计算指定图层的标准差，由void StdDev()内部调用
		void LengthWidthRatioAlgorithm();
		double LengthAlgorithm();//对象长度算法，由于该算法很简单，因此不存储结果而是使用时直接调用
		double WidthAlgorithm();//对象宽度算法，由于该算法很简单，因此不存储结果而是使用时直接调用
		void EllipticFitAlgorithm();//椭圆拟合算法
		void RectangularFitAlgorithm();//椭圆拟合算法
		void RadiusLargestEnclosedEllipseAlgorithm();//最大内接椭圆半径算法
		void RadiusSmallestEnclosingEllipseAlgorithm();//最小外包椭圆半径算法
		void CalculateGLCM();//计算灰度共生矩阵
		void CalculateGLDV();//计算灰度差分矢量
		void CalculateSampleStatistics();//计算SampleStatistics
		float HistogramMean(int buckets,float* histogram);//计算直方图的均值
	};
}

