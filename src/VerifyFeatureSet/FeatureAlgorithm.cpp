#include "StdAfx.h"
#include "FeatureAlgorithm.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "math.h"
#include <sstream> 


namespace VerifyFeatureSet
{
	FeatureAlgorithm::FeatureAlgorithm()
	{
		//初始化计算灰度共生矩阵所需的变量
		grayLevel = 8;
		
		//dirmem[0][0] = 0;   dirmem[0][1] = 1;//0
		//dirmem[1][0] = -1;   dirmem[1][1] = 1;//45度
		//dirmem[2][0] = -1;   dirmem[2][1] = 0;//90度
		//dirmem[3][0] = -1;   dirmem[3][1] = -1;//135度
		dirmem[0][0] = 0;   dirmem[0][1] = -1;  dirmem[0][2] = 0;   dirmem[0][3] = 1;//0
		dirmem[1][0] = -1;  dirmem[1][1] = -1;  dirmem[1][2] = 1;   dirmem[1][3] = 1;//45度
		dirmem[2][0] = -1;  dirmem[2][1] = 0;   dirmem[2][2] = 1;   dirmem[2][3] = 0;//90度
		dirmem[3][0] = -1;  dirmem[3][1] = 1;   dirmem[3][2] = 1;   dirmem[3][3] = -1;//135度

		m_Initialized = false;
	}


	FeatureAlgorithm::~FeatureAlgorithm(void)
	{
		Reset();
	}

	void FeatureAlgorithm::Reset(void)
	{
		//if(bGLCMValid)
			delete gLCM;
		bGLCMValid = false;
		//if(bGLDVValid)
			delete gLDV;
		bGLDVValid = false;

		delete aGP;
		delete aRegion;

		if(bSampleStatisticsValid)
		{
			for(int i=0;i<m_SampleImage->m_CachedBands.size();i++)
			{
				delete (*(sampleStatistics+i)).histogram;
			}
			delete sampleStatistics;
		}
		bSampleStatisticsValid = false;

		m_Initialized = false;
	}

	void FeatureAlgorithm::Initialize(SampleImage* sampleImage,STRegion* sampleRegion,PFCALLBACK gCallBack)
	{
		m_SampleImage = sampleImage;
		m_SampleRegion = sampleRegion;
		m_CallBack = gCallBack;

		//重置缓存的计算特征所用的结构
		m_BrightnessValid = false;
		m_LengthWidthRatioValid = false;
		m_FitEcllipsBoxValid = false;
		m_FitRectangularBoxValid = false;
		m_RadiusLargestEnclosedEllipseValid = false;
		m_RadiusSmallestEnclosingEllipseValid = false;

		//初始化计算灰度共生矩阵所需的变量
		int gLCMSize = pow(2.0,grayLevel);
		gLCM = new float[sampleImage->m_CachedBands.size()*4*gLCMSize*gLCMSize];
		memset(gLCM,0,sampleImage->m_CachedBands.size()*4*gLCMSize*gLCMSize*sizeof(float));
		bGLCMValid = false;
		gLDV = new float[sampleImage->m_CachedBands.size()*4*gLCMSize];
		memset(gLDV,0,sampleImage->m_CachedBands.size()*4*gLCMSize*sizeof(float));
		bGLDVValid = false;

		//初始化计算统计信息所需的变量
		sampleStatistics = new SampleStatistics[sampleImage->m_CachedBands.size()];
		for(int i=0;i<m_SampleImage->m_CachedBands.size();i++)
		{			
			(*(sampleStatistics+i)).buckets = 256;
			(*(sampleStatistics+i)).histogram = new float[(*(sampleStatistics+i)).buckets];			
			//(*(sampleStatistics+i)).hasHistogram = false;
		}
		CalculateSampleStatistics();//计算统计信息

		m_Initialized = true;
	}

	template <class T> string FeatureAlgorithm::ConvertToString(T value)
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	double FeatureAlgorithm::DistPointToEcllips(CvPoint2D32f point,CvPoint2D32f focus1,CvPoint2D32f focus2,double dist)
	{
		double deltx = point.x - focus1.x;
		double delty = point.y - focus1.y;
		double dist1 = sqrt(deltx * deltx + delty * delty);
		deltx = point.x - focus2.x;
		delty = point.y - focus2.y;
		double dist2 = sqrt(deltx * deltx + delty * delty);

		return dist1 + dist2 - dist;
	}


	void FeatureAlgorithm::execute(string algorithmName, const std::vector<int> verifyBands)
	{
		if(algorithmName == "Mean")
		{
			Mean();
		}
		else if(algorithmName == "Brightness")
		{
			Brightness();
		}
		else if(algorithmName == "StdDev")
		{
			StdDev();
		}
		else if(algorithmName == "Ratio")
		{
			Ratio();
		}
		else if(algorithmName == "MaxDiff")
		{
			MaxDiff();
		}
		else if(algorithmName == "InternalConsistency")
		{
			InternalConsistency();
		}
		else if(algorithmName == "Area")
		{
			Area();
		}
		else if(algorithmName == "Length")
		{
			Length();
		}
		else if(algorithmName == "Width")
		{
			Width();
		}
		else if(algorithmName == "LengthWidthRatio")
		{
			LengthWidthRatio();
		}
		else if(algorithmName == "BorderLength")
		{
			BorderLength();
		}
		else if(algorithmName == "MainDirection")
		{
			MainDirection();
		}
		else if(algorithmName == "Asymmetry")
		{
			Asymmetry();
		}
		else if(algorithmName == "Compactness")
		{
			Compactness();
		}
		else if(algorithmName == "Density")
		{
			Density();
		}
		else if(algorithmName == "ShapeIndex")
		{
			ShapeIndex();
		}
		else if(algorithmName == "Rectangle")
		{
			Rectangle();
		}
		else if(algorithmName == "Elongatedness")
		{
			Elongatedness();
		}
		else if(algorithmName == "Borderindex")
		{
			Borderindex();
		}
		else if(algorithmName == "EllipticFit")
		{
			EllipticFit();
		}
		else if(algorithmName == "RectangularFit")
		{
			RectangularFit();
		}		
		else if(algorithmName == "Roundness")
		{
			Roundness();
		}
		else if(algorithmName == "RadiusLargestEnclosedEllipse")
		{
			RadiusLargestEnclosedEllipse();
		}
		else if(algorithmName == "RadiusSmallestEnclosingEllipse")
		{
			RadiusSmallestEnclosingEllipse();
		}
		else if(algorithmName == "GLCMASM")
		{
			GLCMASM();
		}
		else if(algorithmName == "GLCMCON")
		{
			GLCMCON();
		}
		else if(algorithmName == "GLCMCOR")
		{
			GLCMCOR();
		}
		else if(algorithmName == "GLCMENT")
		{
			GLCMENT();
		}
		else if(algorithmName == "GLCMIDM")
		{
			GLCMIDM();
		}
		else if(algorithmName == "GLDVASM")
		{
			GLDVASM();
		}
		else if(algorithmName == "GLDVMEAN")
		{
			GLDVMEAN();
		}
		else if(algorithmName == "GLDVCON")
		{
			GLDVCON();
		}
		else if(algorithmName == "GLDVENT")
		{
			GLDVENT();
		}		
		else if(algorithmName == "HistogramOriginMoment(1阶)")
		{
			HistogramOriginMoment(1);
		}
		else if(algorithmName == "HistogramCentralMoment(2阶)")
		{
			HistogramCentralMoment(2);
		}		
		else if(algorithmName == "HistogramBias")
		{
			HistogramBias();
		}
		else if(algorithmName == "HistogramKurtosis")
		{
			HistogramKurtosis();
		}
		else if(algorithmName == "HistogramEnergy")
		{
			HistogramEnergy();
		}
		else if(algorithmName == "HistogramEntropy")
		{
			HistogramEntropy();
		}
		else if(algorithmName == "NDVI")
		{
			assert(verifyBands.size() >= 2);
			NDVI(verifyBands);
		}
		else if(algorithmName == "NDWI")
		{
			assert(verifyBands.size() >= 2);
			NDWI(verifyBands);
		}
		else if(algorithmName == "SVI")
		{
			assert(verifyBands.size() >= 2);
			SVI(verifyBands);
		}
		else if(algorithmName == "RVI")
		{
			assert(verifyBands.size() >= 2);
			RVI(verifyBands);
		}
		else if(algorithmName == "GNDWI")
		{
			assert(verifyBands.size() >= 2);
			GNDWI(verifyBands);
		}
	}
	
	void FeatureAlgorithm::Mean()
	{
		//图层均值存储在m_SampleRegion中，根本不需要计算

		//输出结果
		string feature = "Mean";
		int bandNum = m_SampleImage->m_CachedBands.size();
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<bandNum;i++)
			layers = layers + ";" + ConvertToString(m_SampleImage->m_CachedBands[i]);
		
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,bandNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i=0;i<bandNum;i++)
				*(verifyResult->results + i) = (*(sampleStatistics+i)).mean;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Brightness()
	{
		//如果还没有计算Brightness，则调用BrightnessAlgorithm计算它
		if(!m_BrightnessValid)
		{
			BrightnessAlgorithm();//本方法负责将结果缓存起来
		}

		//输出结果
		string feature = "Brightness";
		string layers = "Brightness";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);	
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = m_Brightness;
		}

		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::BrightnessAlgorithm()//计算影像亮度值并负责将结果缓存起来
	{
	    //计算指标
		double Bright = 0;
		int bandNum = m_SampleImage->m_CachedBands.size();
		for(int i=0;i<bandNum;i++)
			Bright += (*(sampleStatistics+i)).mean;

		//缓存结果
		m_Brightness = Bright/bandNum;
		m_BrightnessValid = true;
	}

	void FeatureAlgorithm::StdDev()
	{
		//输出结果
		string feature = "StdDev";
		int bandNum = m_SampleImage->m_CachedBands.size();
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<bandNum;i++)
			layers = layers + ";" + ConvertToString(m_SampleImage->m_CachedBands[i]);
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,bandNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i=0;i<bandNum;i++)
				*(verifyResult->results + i) = StdDev(m_SampleImage->m_CachedBands[i]);	//由StdDev计算实际的指标值，该指标不需要缓存	
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	double FeatureAlgorithm::StdDev(int bandIndex)//计算某一波段标准差
	{	
		double totalValue = 0;
		double tempValue,curPixelValue;

		int cachedBandIndex = m_SampleImage->ImageBandIndexToCachedBandIndex(bandIndex);
		for(int i = 0;i<m_SampleRegion->pixels.size();i++)
		{
			curPixelValue = m_SampleImage->GetSamplePixel(bandIndex,m_SampleRegion->pixels[i]);
			tempValue = (curPixelValue - (*(sampleStatistics+cachedBandIndex)).mean);
			totalValue = totalValue +  tempValue * tempValue;
		}
		totalValue /= (m_SampleRegion->pixels.size() - 1);
		return sqrt(totalValue);
	}

	void FeatureAlgorithm::Ratio()
	{
		int bandNum = m_SampleImage->m_CachedBands.size();
		double totalValue = 0;
		for(int i=0;i<bandNum;i++)
			totalValue += (*(sampleStatistics+i)).mean;

		//输出结果
		string feature = "Ratio";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<bandNum;i++)
			layers = layers + ";" + ConvertToString(m_SampleImage->m_CachedBands[i]);
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,bandNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i=0;i<bandNum;i++)
				*(verifyResult->results + i) = (*(sampleStatistics+i)).mean / totalValue;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::MaxDiff()
	{
		//如果还没有计算Brightness，则调用BrightnessAlgorithm计算它
		if(!m_BrightnessValid)
		{
			BrightnessAlgorithm();
		}
		
		//计算MaxDiff特征值
		int bandNum = m_SampleImage->m_CachedBands.size();
		double maxDiff = 0;
		double tempDiff = 0;	
		for (int i=0;i<bandNum;i++)
		{
			for (int j=i+1;j<bandNum;j++)
			{
				tempDiff = abs((*(sampleStatistics+i)).mean - (*(sampleStatistics+j)).mean);
				if(tempDiff > maxDiff)
					maxDiff = tempDiff;
			}
		}
		maxDiff /= m_Brightness;//前面已经计算除了亮度值

		//输出结果
		string feature = "MaxDiff";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
				*(verifyResult->results) = maxDiff;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}
	
	void FeatureAlgorithm::InternalConsistency()
	{
		//计算特征值
		double curGray = 0;
		double averageGray = (*(sampleStatistics+0)).mean;
		if(m_SampleImage->m_CachedBands.size() >= 3)
		{
			averageGray = 0.299 * (*(sampleStatistics+0)).mean + 0.587 * (*(sampleStatistics+1)).mean + 0.114 * (*(sampleStatistics+2)).mean;
			for (int i=0;i<m_SampleRegion->pixels.size();i++)
			{
				curGray = 0.299 * m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[0],m_SampleRegion->pixels[i]) + 
					      0.587 * m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[1],m_SampleRegion->pixels[i]) +
						  0.114 * m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[2],m_SampleRegion->pixels[i]);
			}
		}
		else
		{
			curGray = m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[0],m_SampleRegion->pixels[0]);
		}
		double internalConsistency = curGray;//对计算公式的理解有歧义，以后再修改这里

		//输出结果
		string feature = "InternalConsistency";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = internalConsistency;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Area()
	{
		//样本面积存储在m_SampleRegion中，根本不需要计算

		//输出结果
		string feature = "Area";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = m_SampleRegion->area;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Length()
	{
		//输出结果
		string feature = "Length";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = LengthAlgorithm();		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Width()
	{
		//输出结果
		string feature = "Width";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = WidthAlgorithm();		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::LengthWidthRatio()
	{
		//如果还没有计算LengthWidthRatio，则调用LengthWidthRatioAlgorithm计算它
		if(!m_LengthWidthRatioValid)
		{
			LengthWidthRatioAlgorithm();//本方法负责将结果缓存起来
		}

		//输出结果
		string feature = "LengthWidthRatio";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = m_LengthWidthRatio;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::LengthWidthRatioAlgorithm()//计算长宽比并负责将结果缓存起来
	{
		//计算指标
		//基于协方差矩阵的特征值之比计算长宽比
		CvMat* eigenVector = cvCreateMat(2 , 2 ,CV_64FC1);
		CvMat* eigenValue = cvCreateMat(2 , 1 ,CV_64FC1);
		cvEigenVV(m_SampleRegion->covarMatrix,eigenVector,eigenValue);
		double lengthWidthRatio1 = cvGetReal2D(eigenValue, 0, 0 ) / cvGetReal2D(eigenValue, 1, 0 );
		if(lengthWidthRatio1 < 1)
			lengthWidthRatio1 = 1 / lengthWidthRatio1;
		cvReleaseMat(&eigenVector);
		cvReleaseMat(&eigenValue);

		//基于bounding box计算长宽比
		double kvbb = m_SampleRegion->bounds[3] - m_SampleRegion->bounds[2] + 1;
		double hvbb = m_SampleRegion->bounds[1] - m_SampleRegion->bounds[0] + 1;
		/*if(kvbb < hvbb)
		{
			double temp = hvbb;
			hvbb = kvbb;
			kvbb = temp;			
		}*/
		double a = m_SampleRegion->area / (kvbb * hvbb);
		double lengthWidthRatio2 = kvbb*kvbb+(1-a)*(1-a)*hvbb*hvbb;
		lengthWidthRatio2 = kvbb/hvbb;//lengthWidthRatio2 / m_SampleRegion->area;

		//缓存结果
   		m_LengthWidthRatio = min(lengthWidthRatio1,lengthWidthRatio2);
		m_LengthWidthRatioValid = true;
	}

	double FeatureAlgorithm::LengthAlgorithm()
	{
		//如果还没有计算LengthWidthRatio，则调用LengthWidthRatioAlgorithm计算它
		if(!m_LengthWidthRatioValid)
		{
			LengthWidthRatioAlgorithm();//本方法负责将结果缓存起来
		}

		//ecognition中的长宽比定义
		//double result = sqrt(m_SampleRegion->area * m_LengthWidthRatio);

		//对ecognition的使用发现对象长、宽更接近与对象最小外接矩形的长、宽
		double width = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[0].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[0].y),2));
		double height = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[2].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[2].y),2));
		double result = max(width,height);

		return result;
	}

	double FeatureAlgorithm::WidthAlgorithm()
	{
		//如果还没有计算LengthWidthRatio，则调用LengthWidthRatioAlgorithm计算它
		if(!m_LengthWidthRatioValid)
		{
			LengthWidthRatioAlgorithm();//本方法负责将结果缓存起来
		}

		//ecognition中的长宽比定义
		//double result = sqrt(m_SampleRegion->area / m_LengthWidthRatio);

		//对ecognition的使用发现对象长、宽更接近与对象最小外接矩形的长、宽
		double width = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[0].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[0].y),2));
		double height = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[2].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[2].y),2));
		double result = min(width,height);

		return result;
	}

	void FeatureAlgorithm::BorderLength()
	{
		//边界长度存储在m_SampleRegion中，根本不需要计算

		//输出结果
		string feature = "BorderLength";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = m_SampleRegion->len;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::MainDirection()
	{
		//基于协方差矩阵计算特征向量
		CvMat* eigenVector = cvCreateMat(2 , 2 ,CV_64FC1);
		CvMat* eigenValue = cvCreateMat(2 , 1 ,CV_64FC1);
		cvEigenVV(m_SampleRegion->covarMatrix,eigenVector,eigenValue);		

		//计算主方向
		double varxy = cvGetReal2D(m_SampleRegion->covarMatrix, 0, 1 );
		double vary = cvGetReal2D(m_SampleRegion->covarMatrix, 1, 1 );
		double lmd1 = cvGetReal2D(eigenValue, 0, 0 );
		double mainDirection = atan2(varxy,lmd1 - vary)*180/M_PI + 90;//该方向以水平向右为0，逆时针为正

		cvReleaseMat(&eigenVector);
		cvReleaseMat(&eigenValue);

		//易康定义的方向垂直向上为0，顺时针为正
		if(mainDirection <= 90)
			mainDirection = 90 - mainDirection;
		if(mainDirection > 90)
			mainDirection = 270 - mainDirection;

		//输出结果
		string feature = "MainDirection";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = mainDirection;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Asymmetry()
	{
		//计算特征值
		/*double longAxis = sqrt(pow((m_SampleRegion->fitEllipseRect[1].x - m_SampleRegion->fitEllipseRect[0].x),2) + pow((m_SampleRegion->fitEllipseRect[1].y - m_SampleRegion->fitEllipseRect[0].y),2));
		double shortAxis = sqrt(pow((m_SampleRegion->fitEllipseRect[1].x - m_SampleRegion->fitEllipseRect[2].x),2) + pow((m_SampleRegion->fitEllipseRect[1].y - m_SampleRegion->fitEllipseRect[2].y),2));
		double ratio = shortAxis / longAxis;
		if(ratio < 1) ratio = 1 / ratio;
		double featureResult = 1 - ratio;*/
		
		//基于协方差矩阵计算特征向量
		CvMat* eigenVector = cvCreateMat(2 , 2 ,CV_64FC1);
		CvMat* eigenValue = cvCreateMat(2 , 1 ,CV_64FC1);
		cvEigenVV(m_SampleRegion->covarMatrix,eigenVector,eigenValue);		

		double lmd1 = cvGetReal2D(eigenValue, 0, 0 );
		double lmd2 = cvGetReal2D(eigenValue, 1, 0 );
		double featureResult = 1 - sqrt(lmd2 / lmd1);

		cvReleaseMat(&eigenVector);
		cvReleaseMat(&eigenValue);

		//输出结果
		string feature = "Asymmetry";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Compactness()
	{
		//计算特征值
		//double featureResult = 4 * M_PI * m_SampleRegion->area / (m_SampleRegion->len * m_SampleRegion->len);
		double width = LengthAlgorithm();
		double height = WidthAlgorithm();
		double featureResult = width * height / m_SampleRegion->area;

		//输出结果
		string feature = "Compactness";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Density()
	{
		//计算特征值
		double featureResult = sqrt((double)(m_SampleRegion->area)) / (1 + sqrt(cvGetReal2D(m_SampleRegion->covarMatrix,0,0) + cvGetReal2D(m_SampleRegion->covarMatrix,1,1)));

		//输出结果
		string feature = "Density";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::ShapeIndex()
	{
		//计算特征值
		double featureResult = m_SampleRegion->len / (4 * sqrt((double)(m_SampleRegion->area))); 

		//输出结果
		string feature = "ShapeIndex";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Rectangle()
	{
		//计算特征值
		double width = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[0].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[0].y),2));
		double height = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[2].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[2].y),2));
		double featureResult = m_SampleRegion->area / (width * height); 

		//输出结果
		string feature = "Rectangle";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Elongatedness()
	{
		//计算特征值
		double width = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[0].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[0].y),2));
		double height = sqrt(pow((m_SampleRegion->minBoundRect[1].x - m_SampleRegion->minBoundRect[2].x),2) + pow((m_SampleRegion->minBoundRect[1].y - m_SampleRegion->minBoundRect[2].y),2));
		double featureResult = width / height; 
		if(featureResult < 1) featureResult = 1 / featureResult;

		//输出结果
		string feature = "Elongatedness";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Borderindex()
	{
		//计算特征值
		double width = LengthAlgorithm();
		double height = WidthAlgorithm();
		double featureResult = m_SampleRegion->len / (2 * (width + height)); 

		//输出结果
		string feature = "Borderindex";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::Roundness()
	{
		//如果还没有计算RadiusLargestEnclosedEllipse，则调用RadiusLargestEnclosedEllipseValidAlgorithm计算它
		if(!m_RadiusLargestEnclosedEllipseValid)
		{
			RadiusLargestEnclosedEllipseAlgorithm();//本方法负责将结果缓存起来
		}
		//如果还没有计算RadiusSmallestEnclosingEllipse，则调用RadiusSmallestEnclosingEllipseValidAlgorithm计算它
		if(!m_RadiusSmallestEnclosingEllipseValid)
		{
			RadiusSmallestEnclosingEllipseAlgorithm();//本方法负责将结果缓存起来
		}
		double featureResult = m_RadiusSmallestEnclosingEllipse - m_RadiusLargestEnclosedEllipse; 

		//输出结果
		string feature = "Roundness";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::EllipticFitAlgorithm()
	{
		//基于协方差矩阵计算特征值
		/*CvMat* eigenVector = cvCreateMat(2 , 2 ,CV_64FC1);
		CvMat* eigenValue = cvCreateMat(2 , 1 ,CV_64FC1);
		cvEigenVV(m_SampleRegion->covarMatrix,eigenVector,eigenValue);
		double lmd1 = cvGetReal2D(eigenValue, 0, 0 );
		double lmd2 = cvGetReal2D(eigenValue, 1, 0 );*/
		double lmd1 = LengthAlgorithm();
		double lmd2 = WidthAlgorithm();

		//计算拟合椭圆的长短半轴
		double ratio = sqrt(m_SampleRegion->area / (M_PI * lmd1 * lmd2));
		double largeRaduis = ratio * lmd1;
		double smallRaduis = ratio * lmd2;

		//将拟合椭圆放置在最小二乘拟合椭圆的位置、方向上
		m_FitEcllipsBox.angle = m_SampleRegion->olsEcllipsBox.angle;
		m_FitEcllipsBox.center = m_SampleRegion->olsEcllipsBox.center;
		m_FitEcllipsBox.size.width = smallRaduis * 2;
		m_FitEcllipsBox.size.height = largeRaduis * 2;

		m_FitEcllipsBoxValid = true;
	}

	void FeatureAlgorithm::EllipticFit()
	{
		//如果还没有计算FitEcllipsBox，则调用EllipticFitAlgorithm计算它
		if(!m_FitEcllipsBoxValid)
		{
			EllipticFitAlgorithm();//本方法负责将结果缓存起来
		}

		//计算样本对象像素落在拟合椭圆内的百分比
		double a = m_FitEcllipsBox.size.height / 2;
		double b = m_FitEcllipsBox.size.width / 2;
		double c = sqrt(a * a - b * b);
		double radian = m_FitEcllipsBox.angle * M_PI / 180;
		double deltx = c * sin(radian);
		double delty = c * cos(radian) * -1;
		CvPoint2D32f focus1,focus2,currentPoint;
		focus1.x = m_FitEcllipsBox.center.x + deltx;
		focus1.y = m_FitEcllipsBox.center.y + delty;
		focus2.x = m_FitEcllipsBox.center.x - deltx;
		focus2.y = m_FitEcllipsBox.center.y - delty;
		int pointInEcllipseNum = 0;
		for(int i=0;i<m_SampleRegion->pixels.size();i++)
		{
			currentPoint.x = m_SampleRegion->pixels[i] % (m_SampleRegion->bounds[3] + 1);
			currentPoint.y = m_SampleRegion->pixels[i] / (m_SampleRegion->bounds[3] + 1);
			if(DistPointToEcllips(currentPoint,focus1,focus2,2 * a) <= DBL_EPSILON)
				pointInEcllipseNum++;
		}
		double featureResult = 2 * ((double)pointInEcllipseNum) / m_SampleRegion->pixels.size() - 1;
		if(featureResult < 0)
			featureResult = 0;

		//输出结果
		string feature = "EllipticFit";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::RectangularFitAlgorithm()
	{
		double lengthSample = LengthAlgorithm();
		double widthSample = WidthAlgorithm();

		//计算拟合矩形的长宽
		double ratio = sqrt(m_SampleRegion->area / (lengthSample * widthSample));
		double lengthRectangular = ratio * lengthSample;
		double widthRectangular = ratio * widthSample;

		//将拟合椭圆放置在最小二乘拟合椭圆的位置、方向上
		m_FitRectangularBox.angle = m_SampleRegion->olsEcllipsBox.angle;
		m_FitRectangularBox.center = m_SampleRegion->olsEcllipsBox.center;
		m_FitRectangularBox.size.width = lengthRectangular;
		m_FitRectangularBox.size.height = lengthRectangular;

		m_FitRectangularBoxValid = true;
	}

	void FeatureAlgorithm::RectangularFit()
	{
		//如果还没有计算FitRectangularBox，则调用RectangularFitAlgorithm计算它
		if(!m_FitRectangularBoxValid)
		{
			RectangularFitAlgorithm();//本方法负责将结果缓存起来
		}

		CvPoint2D32f fitRect[4];
		STPoint fitRectPoint[4];
		cvBoxPoints(m_FitRectangularBox,fitRect);
		for(int i=0;i<4;i++)
		{
			fitRectPoint[i].X = (int)(fitRect[i].x);
			fitRectPoint[i].Y = (int)(fitRect[i].y);
		}
		//生成GDI的Region结构，用于计算点是否在多边形内部
		GraphicsPath* aGP = new GraphicsPath();
		aGP->AddPolygon(fitRectPoint,4);
		Region* aRegion = new Region(aGP);

		//计算样本对象像素落在拟合矩形内的百分比
		int pointInRectangularNum = 0;
		for(int i=0;i<m_SampleRegion->pixels.size();i++)
		{
			int col = m_SampleRegion->pixels[i] % (m_SampleRegion->bounds[3] + 1);
			int row = m_SampleRegion->pixels[i] / (m_SampleRegion->bounds[3] + 1);
			if(m_SampleRegion->IsPointInRegion(col,row,aRegion))
				pointInRectangularNum++;
		}
		double featureResult = ((double)pointInRectangularNum) / m_SampleRegion->pixels.size();
		delete aGP;
		delete aRegion;

		//输出结果
		string feature = "RectangularFit";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = featureResult;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::RadiusLargestEnclosedEllipseAlgorithm()
	{
		const int iterateNum = 100;

		//如果还没有计算FitEcllipsBox，则调用EllipticFitAlgorithm计算它
		if(!m_FitEcllipsBoxValid)
		{
			EllipticFitAlgorithm();//本方法负责将结果缓存起来
		}
		
		//得到拟合椭圆内的基本参数
		double a = m_FitEcllipsBox.size.height / 2;
		double b = m_FitEcllipsBox.size.width / 2;
		double c = sqrt(a * a - b * b);
		double radian = m_FitEcllipsBox.angle * M_PI / 180;
		double deltx = c * sin(radian);
		double delty = c * cos(radian) * -1;
		double scale = 1;
		
		CvPoint2D32f focus1,focus2,currentPoint;
		for(int iter = 0;iter < iterateNum;iter++)
		{		
			focus1.x = m_FitEcllipsBox.center.x + deltx * scale;
			focus1.y = m_FitEcllipsBox.center.y + delty * scale;
			focus2.x = m_FitEcllipsBox.center.x - deltx * scale;
			focus2.y = m_FitEcllipsBox.center.y - delty * scale;
			double minDist = DBL_MAX;		
			for(int i=0;i<m_SampleRegion->polyPoints.size();i++)
			{
				currentPoint.x = m_SampleRegion->polyPoints[i].X;
				currentPoint.y = m_SampleRegion->polyPoints[i].Y;
				double currentDist = DistPointToEcllips(currentPoint,focus1,focus2,2 * a * scale);
				if(currentDist < minDist)
					minDist = currentDist;
			}

			if(minDist < -1 * DBL_EPSILON)//当前椭圆太大，继续收缩
			{
				double curscale = abs(minDist) / (2 * a * scale);
				curscale = curscale * 0.8;//加上0.8这个比例因子有助于算法收敛
				curscale = (1 - curscale);
				scale = scale * curscale;
			}
			else if(minDist > DBL_EPSILON)//当前椭圆太小，需要放大
			{
				double curscale = abs(minDist) / (2 * a * scale);
				curscale = curscale * 0.8;//加上0.8这个比例因子有助于算法收敛
				curscale = (1 + curscale);
				scale = scale * curscale;
			}
			else//得到最大内接椭圆，退出循环
			{
				break;
			}
		}

		m_RadiusLargestEnclosedEllipse = scale;
		m_RadiusLargestEnclosedEllipseValid = true;
	}

	void FeatureAlgorithm::RadiusLargestEnclosedEllipse()
	{
		//如果还没有计算RadiusLargestEnclosedEllipse，则调用RadiusLargestEnclosedEllipseValidAlgorithm计算它
		if(!m_RadiusLargestEnclosedEllipseValid)
		{
			RadiusLargestEnclosedEllipseAlgorithm();//本方法负责将结果缓存起来
		}
		
		//输出结果
		string feature = "RadiusLargestEnclosedEllipse";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = m_RadiusLargestEnclosedEllipse;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::RadiusSmallestEnclosingEllipseAlgorithm()
	{
		const int iterateNum = 100;

		//如果还没有计算FitEcllipsBox，则调用EllipticFitAlgorithm计算它
		if(!m_FitEcllipsBoxValid)
		{
			EllipticFitAlgorithm();//本方法负责将结果缓存起来
		}

		//得到拟合椭圆内的基本参数
		double a = m_FitEcllipsBox.size.height / 2;
		double b = m_FitEcllipsBox.size.width / 2;
		double c = sqrt(a * a - b * b);
		double radian = m_FitEcllipsBox.angle * M_PI / 180;
		double deltx = c * sin(radian);
		double delty = c * cos(radian) * -1;
		double scale = 1;

		CvPoint2D32f focus1,focus2,currentPoint;
		for(int iter = 0;iter < iterateNum;iter++)
		{		
			focus1.x = m_FitEcllipsBox.center.x + deltx * scale;
			focus1.y = m_FitEcllipsBox.center.y + delty * scale;
			focus2.x = m_FitEcllipsBox.center.x - deltx * scale;
			focus2.y = m_FitEcllipsBox.center.y - delty * scale;
			double maxDist = DBL_MIN;		
			for(int i=0;i<m_SampleRegion->polyPoints.size();i++)
			{
				currentPoint.x = m_SampleRegion->polyPoints[i].X;
				currentPoint.y = m_SampleRegion->polyPoints[i].Y;
				double currentDist = DistPointToEcllips(currentPoint,focus1,focus2,2 * a * scale);
				if(currentDist > maxDist)
					maxDist = currentDist;
			}

			if(maxDist < -1 * DBL_EPSILON)//当前椭圆太大，继续收缩
			{
				double curscale = abs(maxDist) / (2 * a * scale);
				curscale = curscale * 0.8;//加上0.8这个比例因子有助于算法收敛
				curscale = (1 - curscale);
				scale = scale * curscale;
			}
			else if(maxDist > DBL_EPSILON)//当前椭圆太小，需要放大
			{
				double curscale = abs(maxDist) / (2 * a * scale);
				curscale = curscale * 0.8;//加上0.8这个比例因子有助于算法收敛
				curscale = (1 + curscale);
				scale = scale * curscale;
			}
			else//得到最小外包椭圆，退出循环
			{
				break;
			}
		}

		m_RadiusSmallestEnclosingEllipse = scale;
		m_RadiusSmallestEnclosingEllipseValid = true;
	}

	void FeatureAlgorithm::RadiusSmallestEnclosingEllipse()
	{
		//如果还没有计算RadiusSmallestEnclosingEllipse，则调用RadiusSmallestEnclosingEllipseValidAlgorithm计算它
		if(!m_RadiusSmallestEnclosingEllipseValid)
		{
			RadiusSmallestEnclosingEllipseAlgorithm();//本方法负责将结果缓存起来
		}

		//输出结果
		string feature = "RadiusSmallestEnclosingEllipse";		
		string layers = "ALL";
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			*(verifyResult->results) = m_RadiusSmallestEnclosingEllipse;		
		}
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	//void FeatureAlgorithm::CalculateGLCM()
	//{
	//	
	//	int gLCMSize = pow(2.0,grayLevel);
	//	for(int lev = 0;lev<m_SampleImage->m_CachedBands.size();lev++)
	//	{			
	//		for(int dir = 0;dir < 4;dir++)
	//		{
	//			int gLCMOffset = (lev * 4 + dir) * gLCMSize * gLCMSize;
	//			int pixelPairNum = 0;
	//							
	//			//for(int pixelIndex = 0;pixelIndex < m_SampleRegion->pixels.size();pixelIndex++)
	//			for(int pixelIndex = 0;pixelIndex < m_SampleRegion->pixels.size();pixelIndex++)//采用对象膨胀一个像素后的影像计算灰度共生矩阵，与ecogintion算法保持一致
	//			{
	//				int originYIndex = m_SampleRegion->pixels[pixelIndex] / m_SampleImage->m_Width;
	//				int originXIndex = m_SampleRegion->pixels[pixelIndex] % m_SampleImage->m_Width;
	//				int firstXIndex = originXIndex + dirmem[dir][0];
	//				int firstYIndex = originYIndex + dirmem[dir][1];
	//				int secondXIndex = originXIndex + dirmem[dir][2];
	//				int secondYIndex = originYIndex + dirmem[dir][3];
	//				
	//				BYTE originValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],originYIndex * m_SampleImage->m_Width + originXIndex));
	//				if(0 <= firstXIndex && firstXIndex < m_SampleImage->m_Width && 0 <= firstYIndex && firstYIndex < m_SampleImage->m_Height)
	//				{
	//					BYTE firstValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],firstYIndex * m_SampleImage->m_Width + firstXIndex));
	//					gLCM[gLCMOffset +  originValue * gLCMSize + firstValue]++;						
	//					pixelPairNum++;
	//				}
	//				if(0 <= secondXIndex && secondXIndex < m_SampleImage->m_Width && 0 <= secondYIndex && secondYIndex < m_SampleImage->m_Height)
	//				{
	//					BYTE secondValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],secondYIndex * m_SampleImage->m_Width + secondXIndex));
	//					gLCM[gLCMOffset +  originValue * gLCMSize + secondValue]++;
	//					pixelPairNum++;
	//				}
	//			}
	//			
	//			//保证不因为除0而溢出
	//			if(pixelPairNum == 0)
	//				continue;

	//			//位于对称位置的元素变换得到对称矩阵
	//			//for(int y = 0;y<gLCMSize;y++)
	//			//{			
	//			//	for(int x = y;x < gLCMSize;x++)
	//			//	{
	//			//		gLCM[gLCMOffset + y * gLCMSize + x] += gLCM[gLCMOffset + x * gLCMSize + y];
	//			//		gLCM[gLCMOffset + x * gLCMSize + y] = gLCM[gLCMOffset + y * gLCMSize + x];
	//			//	}
	//			//}
	//          //pixelPairNum *= 2;

	//			for(int gLCMIndex = 0;gLCMIndex < gLCMSize * gLCMSize;gLCMIndex++)//归一化
	//				gLCM[gLCMOffset + gLCMIndex] /= pixelPairNum;

	//		}
	//	}

	//	bGLCMValid = true;
	//}

	void FeatureAlgorithm::CalculateGLCM()
	{
		int gLCMSize = pow(2.0,grayLevel);
		for(int lev = 0;lev<m_SampleImage->m_CachedBands.size();lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{
				int gLCMOffset = (lev * 4 + dir) * gLCMSize * gLCMSize;
				int pixelPairNum = 0;

				for(int referenceRow = 0;referenceRow<=m_SampleRegion->bounds[1];referenceRow++)
				{			
					for(int referenceCol = 0;referenceCol <= m_SampleRegion->bounds[3];referenceCol++)
					{
						int neighborRow = referenceRow + dirmem[dir][0];
						int neighborCol = referenceCol + dirmem[dir][1];
						if(neighborRow < 0 || m_SampleRegion->bounds[1] < neighborRow || neighborCol < 0 || m_SampleRegion->bounds[3] < neighborCol)
							continue;
						if(m_SampleRegion->IsPointInRegion(referenceCol,referenceRow,aRegion) == false && m_SampleRegion->IsPointInRegion(neighborCol,neighborRow,aRegion) == false)
							continue;
						
						BYTE firstValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],referenceRow * (m_SampleRegion->bounds[3] + 1) + referenceCol));
						BYTE secondValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],neighborRow * (m_SampleRegion->bounds[3] + 1) + neighborCol));
						gLCM[gLCMOffset + firstValue * gLCMSize + secondValue] = gLCM[gLCMOffset + firstValue * gLCMSize + secondValue] + 1;
						pixelPairNum++;
					}
				}

				//保证不因为除0而溢出
				if(pixelPairNum == 0)
					continue;

				//位于对称位置的元素变换得到对称矩阵
				for(int y = 0;y<gLCMSize;y++)
				{			
					for(int x = y;x < gLCMSize;x++)
					{
						gLCM[gLCMOffset + y * gLCMSize + x] = gLCM[gLCMOffset + y * gLCMSize + x] + gLCM[gLCMOffset + x * gLCMSize + y];
						gLCM[gLCMOffset + x * gLCMSize + y] = gLCM[gLCMOffset + y * gLCMSize + x];
					}
				}
				pixelPairNum *= 2;

				for(int gLCMIndex = 0;gLCMIndex < gLCMSize * gLCMSize;gLCMIndex++)//归一化
					gLCM[gLCMOffset + gLCMIndex] /= pixelPairNum;
			}
		}

		bGLCMValid = true;
	}

	void FeatureAlgorithm::CalculateGLDV()
	{
		int gLDVSize = pow(2.0,grayLevel);
		for(int lev = 0;lev<m_SampleImage->m_CachedBands.size();lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{
				int gLDVOffset = (lev * 4 + dir) * gLDVSize;
				int pixelPairNum = 0;

				for(int originIndex = 0;originIndex<m_SampleRegion->pixels.size();originIndex++)
				{			
					int originYIndex = m_SampleRegion->pixels[originIndex] / (m_SampleRegion->bounds[3] + 1);
					int originXIndex = m_SampleRegion->pixels[originIndex] % (m_SampleRegion->bounds[3] + 1);
					int firstXIndex = originXIndex + dirmem[dir][0];
					int firstYIndex = originYIndex + dirmem[dir][1];
					int secondXIndex = originXIndex + dirmem[dir][2];
					int secondYIndex = originYIndex + dirmem[dir][3];
					
					BYTE diff;
					BYTE originValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],originYIndex * (m_SampleRegion->bounds[3] + 1) + originXIndex));
					if(0 <= firstXIndex && firstXIndex <= m_SampleRegion->bounds[3] && 0 <= firstYIndex && firstYIndex <= m_SampleRegion->bounds[1])
					{
						BYTE firstValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],firstYIndex * (m_SampleRegion->bounds[3] + 1) + firstXIndex));
						diff = abs(originValue - firstValue);
						gLDV[gLDVOffset + diff] = gLDV[gLDVOffset + diff] + 1;						
						pixelPairNum++;
					}
					if(0 <= secondXIndex && secondXIndex <= m_SampleRegion->bounds[3] && 0 <= secondYIndex && secondYIndex <= m_SampleRegion->bounds[1])
					{
						BYTE secondValue = m_SampleImage->SamplePixel2BYTE(m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],secondYIndex * (m_SampleRegion->bounds[3] + 1) + secondXIndex));
						diff = abs(originValue - secondValue);
						gLDV[gLDVOffset + diff] = gLDV[gLDVOffset + diff] + 1;	
						pixelPairNum++;
					}
				}

				//保证不因为除0而溢出
				if(pixelPairNum == 0)
					continue;

				for(int gLDVIndex = 0;gLDVIndex < gLDVSize;gLDVIndex++)//归一化
					gLDV[gLDVOffset + gLDVIndex] /= pixelPairNum;
			}
		}

		bGLDVValid = true;
	}

	void FeatureAlgorithm::CalculateSampleStatistics()
	{			
		for(int lev = 0;lev<m_SampleImage->m_CachedBands.size();lev++)
		{
			//初始化SampleStatistics结构
			(*(sampleStatistics+lev)).min = FLT_MAX;
			(*(sampleStatistics+lev)).max = FLT_MIN;
			(*(sampleStatistics+lev)).mean = 0;
			(*(sampleStatistics+lev)).number = m_SampleRegion->pixels.size();
			memset((*(sampleStatistics+lev)).histogram,0,(*(sampleStatistics+lev)).buckets*sizeof(float));

			//计算最大最小值、均值、直方图
			for(int pixelIndex = 0;pixelIndex < m_SampleRegion->pixels.size();pixelIndex++)
			{			
				float pixelValue = m_SampleImage->GetSamplePixel(m_SampleImage->m_CachedBands[lev],m_SampleRegion->pixels[pixelIndex]);
				if((*(sampleStatistics+lev)).min > pixelValue)
					(*(sampleStatistics+lev)).min = pixelValue;
				if((*(sampleStatistics+lev)).max < pixelValue)
					(*(sampleStatistics+lev)).max = pixelValue;
				(*(sampleStatistics+lev)).mean += pixelValue;

				BYTE byteValue = m_SampleImage->SamplePixel2BYTE(pixelValue);
				(*(sampleStatistics+lev)).histogram[byteValue] = (*(sampleStatistics+lev)).histogram[byteValue] + 1;
			}

			(*(sampleStatistics+lev)).mean /= m_SampleRegion->pixels.size();
			for(int bucketIndex = 0;bucketIndex < (*(sampleStatistics+lev)).buckets;bucketIndex++)
			{
				(*(sampleStatistics+lev)).histogram[bucketIndex] /= (*(sampleStatistics+lev)).number;
			}
		}
		
		bSampleStatisticsValid = true;
	}

	void FeatureAlgorithm::GLCMASM()
	{
		//如果还没有计算灰度共生矩阵，则调用CalculateGLCM计算它
		if(!bGLCMValid)
		{
			CalculateGLCM();//本方法负责将结果缓存起来
		}

		////计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLCMSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				*(featureValues+lev * 4 + dir) = 0;
				int gLCMOffset = (lev * 4 + dir) * gLCMSize * gLCMSize;
				for(int y = 0;y<gLCMSize;y++)
				{			
					for(int x = 0;x < gLCMSize;x++)
					{
						*(featureValues+lev * 4 + dir) += gLCM[gLCMOffset + y * gLCMSize + x] * gLCM[gLCMOffset + y * gLCMSize + x];
					}
				}
			}
		}

		//输出结果		
		string feature = "GLCMASM";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLCMCON()
	{
		//如果还没有计算灰度共生矩阵，则调用CalculateGLCM计算它
		if(!bGLCMValid)
		{
			CalculateGLCM();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLCMSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				*(featureValues+lev * 4 + dir) = 0;
				int gLCMOffset = (lev * 4 + dir) * gLCMSize * gLCMSize;
				for(int y = 0;y<gLCMSize;y++)
				{			
					for(int x = 0;x < gLCMSize;x++)
					{
						*(featureValues+lev * 4 + dir) += (x - y) * (x - y) * gLCM[gLCMOffset + y * gLCMSize + x];
					}
				}
			}
		}

		//输出结果		
		string feature = "GLCMCON";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLCMCOR()
	{
		//如果还没有计算灰度共生矩阵，则调用CalculateGLCM计算它
		if(!bGLCMValid)
		{
			CalculateGLCM();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLCMSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				int gLCMOffset = (lev * 4 + dir) * gLCMSize * gLCMSize;
				
				int i,j;
				float correlation = 0;
				//计算ux
				float ux=0,uy=0,sigmax=0,sigmay=0,localtotal=0;
				for(i=0;i<gLCMSize;i++)
				{
					localtotal = 0;
					for(j=0;j<gLCMSize;j++)
					{
						localtotal += gLCM[gLCMOffset + j*gLCMSize+i];
					}
					ux += (float)i*localtotal;
				}
				//计算uy
				for(j=0;j<gLCMSize;j++)
				{
					localtotal = 0;
					for(i=0;i<gLCMSize;i++)
					{
						localtotal += gLCM[gLCMOffset + j*gLCMSize+i];
					}
					uy += (float)j*localtotal;
				}
				//计算sigmax
				for(i=0;i<gLCMSize;i++)
				{
					localtotal = 0;
					for(j=0;j<gLCMSize;j++)
					{
						localtotal += gLCM[gLCMOffset + j*gLCMSize+i];
					}
					sigmax += (float)(i-ux)*(float)(i-ux)*localtotal;
				}
				//计算sigmay
				for(j=0;j<gLCMSize;j++)
				{
					localtotal = 0;
					for(i=0;i<gLCMSize;i++)
					{
						localtotal += gLCM[gLCMOffset + j*gLCMSize+i];
					}
					sigmay += (float)(j-uy)*(float)(j-uy)*localtotal;
				}
				//计算相关
				for(i=0;i<gLCMSize;i++)
				{
					for(j=0;j<gLCMSize;j++)
					{
						correlation += i*j*gLCM[gLCMOffset + j*gLCMSize+i];
					}
				}
				correlation -= ux*uy;
				correlation /= sigmax;
				correlation /= sigmay;
				*(featureValues+lev * 4 + dir) = correlation;
			}
		}

		//输出结果		
		string feature = "GLCMCOR";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLCMENT()
	{
		//如果还没有计算灰度共生矩阵，则调用CalculateGLCM计算它
		if(!bGLCMValid)
		{
			CalculateGLCM();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLCMSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				int gLCMOffset = (lev * 4 + dir) * gLCMSize * gLCMSize;
				
				int i,j;
				float entropy = 0;
				for(i=0;i<gLCMSize;i++)
				{
					for(j=0;j<gLCMSize;j++)
					{
						float cellValue = gLCM[gLCMOffset + j*gLCMSize+i];
						if(cellValue>1e-12)
							entropy -= log(cellValue) * cellValue;
					}
					
				}
				//entropy = -1 * entropy / log(2.0);
				*(featureValues+lev * 4 + dir) = (double)entropy;
			}
		}

		//输出结果		
		string feature = "GLCMENT";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLCMIDM()
	{
		//如果还没有计算灰度共生矩阵，则调用CalculateGLCM计算它
		if(!bGLCMValid)
		{
			CalculateGLCM();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLCMSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				int gLCMOffset = (lev * 4 + dir) * gLCMSize * gLCMSize;

				int i,j;
				float idm = 0;
				for(i=0;i<gLCMSize;i++)
				{
					for(j=0;j<gLCMSize;j++)
					{
						idm += gLCM[gLCMOffset + j*gLCMSize+i] / (1+(i-j)*(i-j));
					}
				}

				*(featureValues+lev * 4 + dir) = (double)idm;
			}
		}

		//输出结果		
		string feature = "GLCMIDM";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLDVASM()
	{
		//如果还没有计算灰度差分矢量，则调用CalculateGLDV计算它
		if(!bGLDVValid)
		{
			CalculateGLDV();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLDVSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				int gLDVOffset = (lev * 4 + dir) * gLDVSize;

				float gldvasm = 0;
				for(int i=0;i<gLDVSize;i++)
				{
					float cellValue = gLDV[gLDVOffset +i];
					gldvasm = gldvasm + cellValue * cellValue;
				}
				*(featureValues+lev * 4 + dir) = (double)gldvasm;
			}
		}

		//输出结果		
		string feature = "GLDVASM";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLDVMEAN()
	{
		//如果还没有计算灰度差分矢量，则调用CalculateGLDV计算它
		if(!bGLDVValid)
		{
			CalculateGLDV();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLDVSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				int gLDVOffset = (lev * 4 + dir) * gLDVSize;

				float mean = 0;
				for(int i=0;i<gLDVSize;i++)
				{
					float cellValue = gLDV[gLDVOffset + i];
					mean = mean + i * cellValue;
				}
				//entropy = -1 * entropy / log(2.0);
				*(featureValues+lev * 4 + dir) = (double)mean;
			}
		}

		//输出结果		
		string feature = "GLDVMEAN";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLDVCON()
	{
		//如果还没有计算灰度差分矢量，则调用CalculateGLDV计算它
		if(!bGLDVValid)
		{
			CalculateGLDV();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLDVSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				int gLDVOffset = (lev * 4 + dir) * gLDVSize;

				float cov = 0;
				for(int i=0;i<gLDVSize;i++)
				{
					float cellValue = gLDV[gLDVOffset + i];
					cov = cov + i * i * cellValue;
				}
				//entropy = -1 * entropy / log(2.0);
				*(featureValues+lev * 4 + dir) = (double)cov;
			}
		}

		//输出结果		
		string feature = "GLDVCON";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GLDVENT()
	{
		//如果还没有计算灰度差分矢量，则调用CalculateGLDV计算它
		if(!bGLDVValid)
		{
			CalculateGLDV();//本方法负责将结果缓存起来
		}

		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		int gLDVSize = pow(2.0,grayLevel);
		double* featureValues = new double[levelNum*4];
		for(int lev = 0;lev<levelNum;lev++)
		{			
			for(int dir = 0;dir < 4;dir++)
			{				
				int gLDVOffset = (lev * 4 + dir) * gLDVSize;

				float entropy = 0;
				for(int i=0;i<gLDVSize;i++)
				{
					float cellValue = gLDV[gLDVOffset + i];
					if(cellValue>1e-12)
						entropy -= log(cellValue) * cellValue;
				}
				//entropy = -1 * entropy / log(2.0);
				*(featureValues+lev * 4 + dir) = (double)entropy;
			}
		}

		//输出结果		
		string feature = "GLDVENT";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",levelNum,4,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum*4;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	float FeatureAlgorithm::HistogramMean(int buckets,float* histogram)
	{
		float total = 0;
		for(int i=0;i<buckets;i++)
			total += i * (*(histogram+i));

		return total / buckets;
	}

	void FeatureAlgorithm::HistogramOriginMoment(int p)
	{
		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		double* featureValues = new double[levelNum];
		for(int lev = 0;lev<levelNum;lev++)
		{		
			SampleStatistics* curSampleStatistics = sampleStatistics + lev;
			float total = 0;
			for(int i=0;i<curSampleStatistics->buckets;i++)
			{
				total += pow((float)i,(float)p) * (*(curSampleStatistics->histogram + i));
			}
			*(featureValues + lev) = total;
		}

		//输出结果		
		string feature = "HistogramOriginMoment(" + ConvertToString(p) + "阶)";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,levelNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::HistogramCentralMoment(int p)
	{
		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		double* featureValues = new double[levelNum];
		for(int lev = 0;lev<levelNum;lev++)
		{		
			SampleStatistics* curSampleStatistics = sampleStatistics + lev;
			float mean = HistogramMean(curSampleStatistics->buckets,curSampleStatistics->histogram);
			float total = 0;
			for(int i=0;i<curSampleStatistics->buckets;i++)
			{
				total += pow((float)(i-mean),(float)p) * (*(curSampleStatistics->histogram + i));
			}
			*(featureValues + lev) = total;
		}

		//输出结果		
		string feature = "HistogramCentralMoment(" + ConvertToString(p) + "阶)";			
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,levelNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::HistogramBias()
	{
		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		double* featureValues = new double[levelNum];
		for(int lev = 0;lev<levelNum;lev++)
		{		
			SampleStatistics* curSampleStatistics = sampleStatistics + lev;
			float mean = HistogramMean(curSampleStatistics->buckets,curSampleStatistics->histogram);
			float numerator = 0;
			float denominator = 0;
			for(int i=0;i<curSampleStatistics->buckets;i++)
			{
				numerator += pow((float)(i-mean),3) * (*(curSampleStatistics->histogram + i));
				denominator += pow((float)(i-mean),2) * (*(curSampleStatistics->histogram + i));
			}
			*(featureValues + lev) = numerator / pow(sqrt(denominator),3);
		}

		//输出结果		
		string feature = "HistogramBias";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,levelNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::HistogramKurtosis()
	{
		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		double* featureValues = new double[levelNum];
		for(int lev = 0;lev<levelNum;lev++)
		{		
			SampleStatistics* curSampleStatistics = sampleStatistics + lev;
			float mean = HistogramMean(curSampleStatistics->buckets,curSampleStatistics->histogram);
			float numerator = 0;
			float denominator = 0;
			for(int i=0;i<curSampleStatistics->buckets;i++)
			{
				numerator += pow((float)(i-mean),4) * (*(curSampleStatistics->histogram + i));
				denominator += pow((float)(i-mean),2) * (*(curSampleStatistics->histogram + i));
			}
			*(featureValues + lev) = numerator / (denominator * denominator);
		}

		//输出结果		
		string feature = "HistogramKurtosis";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,levelNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::HistogramEnergy()
	{
		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		double* featureValues = new double[levelNum];
		for(int lev = 0;lev<levelNum;lev++)
		{		
			SampleStatistics* curSampleStatistics = sampleStatistics + lev;
			float total = 0;
			for(int i=0;i<curSampleStatistics->buckets;i++)
			{
				total += (*(curSampleStatistics->histogram + i)) * (*(curSampleStatistics->histogram + i));
			}
			*(featureValues + lev) = total;
		}

		//输出结果		
		string feature = "HistogramEnergy";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,levelNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::HistogramEntropy()
	{
		//计算特征值
		int levelNum = m_SampleImage->m_CachedBands.size();
		double* featureValues = new double[levelNum];
		for(int lev = 0;lev<levelNum;lev++)
		{		
			SampleStatistics* curSampleStatistics = sampleStatistics + lev;
			float total = 0;
			for(int i=0;i<curSampleStatistics->buckets;i++)
			{
				float bucketValue = *(curSampleStatistics->histogram + i);
				if(bucketValue > 1e-12)
					total += log(bucketValue) * bucketValue;
			}
			*(featureValues + lev) = -1 * total / log(2.0);
		}

		//输出结果		
		string feature = "HistogramEntropy";		
		string layers = ConvertToString(m_SampleImage->m_CachedBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(m_SampleImage->m_CachedBands[i]));
		}
		VerifyResult* verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,levelNum,m_SampleRegion->regionID);
		if(verifyResult->resultValid)
		{
			for(int i = 0;i<levelNum;i++)
				*(verifyResult->results+i) = *(featureValues+i);		
		}

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::NDVI(std::vector<int> verifyBands)
	{
		VerifyResult* verifyResult;
		int levelNum = verifyBands.size();
		string feature = "NDVI";		
		string layers = ConvertToString(verifyBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(verifyBands[i]));
		}
		//计算特征值		
		double* featureValues = new double[1];
		*(featureValues) = 0;
		if(levelNum == 2)
		{
			for(int i=0;i<m_SampleRegion->pixels.size();i++)
			{
				float firstBandValue = m_SampleImage->GetSamplePixel(verifyBands[0],m_SampleRegion->pixels[i]);
				float secondBandValue = m_SampleImage->GetSamplePixel(verifyBands[1],m_SampleRegion->pixels[i]);
				float numerator = firstBandValue - secondBandValue;
				float denominator = firstBandValue + secondBandValue;
				*(featureValues) += numerator / denominator;
			}
			*(featureValues) /= m_SampleRegion->pixels.size();

			//输出结果					
			verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
			if(verifyResult->resultValid)
			{
				*(verifyResult->results) = *(featureValues);		
			}
		}
		else
		{
			verifyResult = new VerifyResult(feature,layers,"Input Error",1,1,m_SampleRegion->regionID);
			verifyResult->resultValid = false;
		}		

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::NDWI(std::vector<int> verifyBands)
	{
		VerifyResult* verifyResult;
		int levelNum = verifyBands.size();
		string feature = "NDWI";		
		string layers = ConvertToString(verifyBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(verifyBands[i]));
		}
		//计算特征值		
		double* featureValues = new double[1];
		*(featureValues) = 0;
		if(levelNum == 2)
		{
			for(int i=0;i<m_SampleRegion->pixels.size();i++)
			{
				float firstBandValue = m_SampleImage->GetSamplePixel(verifyBands[0],m_SampleRegion->pixels[i]);
				float secondBandValue = m_SampleImage->GetSamplePixel(verifyBands[1],m_SampleRegion->pixels[i]);
				float numerator = firstBandValue - secondBandValue;
				float denominator = firstBandValue + secondBandValue;
				*(featureValues) += numerator / denominator;
			}
			*(featureValues) /= m_SampleRegion->pixels.size();

			//输出结果					
			verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
			if(verifyResult->resultValid)
			{
				*(verifyResult->results) = *(featureValues);		
			}
		}
		else
		{
			verifyResult = new VerifyResult(feature,layers,"Input Error",1,1,m_SampleRegion->regionID);
			verifyResult->resultValid = false;
		}		

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::SVI(std::vector<int> verifyBands)
	{
		VerifyResult* verifyResult;
		int levelNum = verifyBands.size();
		string feature = "SVI";		
		string layers = ConvertToString(verifyBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(verifyBands[i]));
		}
		//计算特征值		
		double* featureValues = new double[1];
		*(featureValues) = 0;
		if(levelNum == 2)
		{
			for(int i=0;i<m_SampleRegion->pixels.size();i++)
			{
				float firstBandValue = m_SampleImage->GetSamplePixel(verifyBands[0],m_SampleRegion->pixels[i]);
				float secondBandValue = m_SampleImage->GetSamplePixel(verifyBands[1],m_SampleRegion->pixels[i]);
				float numerator = firstBandValue - secondBandValue;
				float denominator = firstBandValue + secondBandValue;
				*(featureValues) += numerator * firstBandValue / denominator;
			}
			*(featureValues) /= m_SampleRegion->pixels.size();

			//输出结果					
			verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
			if(verifyResult->resultValid)
			{
				*(verifyResult->results) = *(featureValues);		
			}
		}
		else
		{
			verifyResult = new VerifyResult(feature,layers,"Input Error",1,1,m_SampleRegion->regionID);
			verifyResult->resultValid = false;
		}		

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::RVI(std::vector<int> verifyBands)
	{
		VerifyResult* verifyResult;
		int levelNum = verifyBands.size();
		string feature = "RVI";		
		string layers = ConvertToString(verifyBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(verifyBands[i]));
		}
		//计算特征值		
		double* featureValues = new double[1];
		*(featureValues) = 0;
		if(levelNum == 2)
		{
			for(int i=0;i<m_SampleRegion->pixels.size();i++)
			{
				float firstBandValue = m_SampleImage->GetSamplePixel(verifyBands[0],m_SampleRegion->pixels[i]);
				float secondBandValue = m_SampleImage->GetSamplePixel(verifyBands[1],m_SampleRegion->pixels[i]);
				*(featureValues) += ((float)firstBandValue) / secondBandValue;
			}
			*(featureValues) /= m_SampleRegion->pixels.size();

			//输出结果					
			verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
			if(verifyResult->resultValid)
			{
				*(verifyResult->results) = *(featureValues);		
			}
		}
		else
		{
			verifyResult = new VerifyResult(feature,layers,"Input Error",1,1,m_SampleRegion->regionID);
			verifyResult->resultValid = false;
		}		

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}

	void FeatureAlgorithm::GNDWI(std::vector<int> verifyBands)
	{
		VerifyResult* verifyResult;
		int levelNum = verifyBands.size();
		string feature = "GNDWI";		
		string layers = ConvertToString(verifyBands[0]);
		for(int i=1;i<levelNum;i++)
		{
			layers.append(";" + ConvertToString(verifyBands[i]));
		}
		//计算特征值		
		double* featureValues = new double[1];
		*(featureValues) = 0;
		if(levelNum == 2)
		{
			//采用快速方法计算NDWI的均值、标准差
			// zsl 设置处置为0
			double meanNDWI = 0;
			double varianceNDWI = 0;
			for(int i=0;i<m_SampleRegion->pixels.size();i++)
			{				
				float firstBandValue = m_SampleImage->GetSamplePixel(verifyBands[0],m_SampleRegion->pixels[i]);
				float secondBandValue = m_SampleImage->GetSamplePixel(verifyBands[1],m_SampleRegion->pixels[i]);
				float numerator = firstBandValue - secondBandValue;
				float denominator = firstBandValue + secondBandValue;
				*(featureValues) = numerator / denominator;
				if(i==0)
				{
					meanNDWI = *(featureValues);
					varianceNDWI = 0;
				}
				else
				{
					double lastMeanNDWI = meanNDWI;
					meanNDWI = (meanNDWI * i + *(featureValues)) / (i + 1.0);
					varianceNDWI = (varianceNDWI * (i - 1) + ((*(featureValues) - lastMeanNDWI) * (*(featureValues) - meanNDWI))) / i;
				}
			}

			//计算GNDWI的均值
			double stdNDWI = sqrt(varianceNDWI);
			for(int i=0;i<m_SampleRegion->pixels.size();i++)
			{
				float firstBandValue = m_SampleImage->GetSamplePixel(verifyBands[0],m_SampleRegion->pixels[i]);
				float secondBandValue = m_SampleImage->GetSamplePixel(verifyBands[1],m_SampleRegion->pixels[i]);
				float numerator = firstBandValue - secondBandValue;
				float denominator = firstBandValue + secondBandValue;
				double ndwi = numerator / denominator;
				double gndwi = (ndwi - meanNDWI) / stdNDWI;
				*(featureValues) += gndwi;
			}
			*(featureValues) /= m_SampleRegion->pixels.size();

			//输出结果					
			verifyResult = new VerifyResult(feature,layers,"Feature Finished",1,1,m_SampleRegion->regionID);
			if(verifyResult->resultValid)
			{
				*(verifyResult->results) = *(featureValues);		
			}
		}
		else
		{
			verifyResult = new VerifyResult(feature,layers,"Input Error",1,1,m_SampleRegion->regionID);
			verifyResult->resultValid = false;
		}		

		delete featureValues;
		m_CallBack(verifyResult);
		delete verifyResult;
	}
}
