#pragma once

#include <opencv2\opencv.hpp>
using namespace cv;

#include "VerifyResult.h"//��ͷ�ļ��������opencv���ͷ�ļ��ĺ��棬�������ʱ����������
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

		//std::vector<int> m_VerifyBands;//��Ŵ����鲨�����

		GraphicsPath* aGP;
		Region* aRegion;
	private:
		SampleImage* m_SampleImage;//�������Ӱ��
		STRegion* m_SampleRegion;//�����������		
	
		//�����ǻҶȹ������󼰼���Ҷȹ���������Ҫ�ı���
		int grayLevel;      //�Ҷȼ����������Ȳ���ܴ��Ӱ������ѹ����һ���ĻҶȼ������Ӷ����Ƽ�������һ����Χ
		int dirmem[4][4];	//�Ҷȹ�������ķ���0�ȡ�45�ȡ�90�ȡ�135��
		float* gLCM;        //�洢����ͼ���4������ĻҶȹ�������,����ͼ�㡢�����˳�����У����з���Ϊ0�ȡ�45�ȡ�90�ȡ�135��˳��
		float* gLDV;        //�洢����ͼ���4������ĻҶȲ��ʸ��,����ͼ�㡢�����˳�����У����з���Ϊ0�ȡ�45�ȡ�90�ȡ�135��˳��
		bool bGLCMValid;//��ʶgLCM�Ƿ���Ч
		bool bGLDVValid;//��ʶgLCM�Ƿ���Ч

		//��������������ͳ����Ϣ��������������ͳ����Ϣ��Ҫ�ı���
		SampleStatistics* sampleStatistics;        //�洢��������ͳ����Ϣ��һ��ͼ��һ��Statistics
		bool bSampleStatisticsValid;//��ʶampleStatistics�Ƿ���Ч


		PFCALLBACK m_CallBack;
		template <class T> string ConvertToString(T);
		double DistPointToEcllips(CvPoint2D32f point,CvPoint2D32f focus1,CvPoint2D32f focus2,double dist);//�㵽��Բ�ľ��룬������ֵ��0����ֵ�ֱ��ʾ����Բ�⡢��Բ�ϡ���Բ��

		//�����ǻ�����Ѿ�������Ĳ�������ֵ������Ϊ���������������м���
		double m_Brightness;
		bool   m_BrightnessValid;//��ʶm_Brightness�Ƿ���Ч
		double m_LengthWidthRatio;
		bool   m_LengthWidthRatioValid;//��ʶm_LengthWidthRatio�Ƿ���Ч
		CvBox2D m_FitEcllipsBox;//ecognition����������Բ��Box
		bool   m_FitEcllipsBoxValid;//��ʶm_FitEcllipsBox�Ƿ���Ч
		CvBox2D m_FitRectangularBox;//ecognition����������Բ��Box
		bool   m_FitRectangularBoxValid;//��ʶm_FitEcllipsBox�Ƿ���Ч
		double m_RadiusLargestEnclosedEllipse;//����ڽ���Բ�뾶
		bool   m_RadiusLargestEnclosedEllipseValid;//��ʶm_RadiusLargestEnclosedEllipse�Ƿ���Ч
		double m_RadiusSmallestEnclosingEllipse;//��С�����Բ�뾶
		bool   m_RadiusSmallestEnclosingEllipseValid;//��ʶm_RadiusSmallestEnclosingEllipse�Ƿ���Ч

		//����Ϊ�����������ȡ����㷨������˲�������ɵ��м��������þ�����㷨ģ�顢����������
		//��������
		void Mean();//��ֵ
		void Brightness();//����
		void StdDev();//��׼��	
		void Ratio();//������
		void MaxDiff();//���������
		void InternalConsistency();//�ڲ�һ���� 
		//��״����
		void Area();//���
		void Length();//����
		void Width();//���
		void LengthWidthRatio();//�����
		void BorderLength();//�߽糤��
		void MainDirection();//��Ҫ����
		void Asymmetry();//���Գ���
		void Compactness();//���¶�
		void Density();//�ܶ�
		void ShapeIndex();//��״ָ��
		void Rectangle();//���ζ�
		void Elongatedness();//ϸ����
		void Borderindex();//�߽�ָ��
		void Roundness();//Բ����
		void EllipticFit();//��Բ���
		void RectangularFit ();//�������
		void RadiusLargestEnclosedEllipse();//����ڽ���Բ�뾶
		void RadiusSmallestEnclosingEllipse();//��С�����Բ�뾶

		//��������
		void GLCMASM();//����(�Ƕ��׾�)
		void GLCMCON();//�Աȶȣ����Ծأ�
		void GLCMCOR();//�����
		void GLCMENT();//��
		void GLCMIDM();//����(���ȶ�)
		void GLDVASM();//����(�Ƕ��׾�)
		void GLDVCON();//�Աȶȣ����Ծأ�
		void GLDVMEAN();//�����
		void GLDVENT();//��
		void HistogramOriginMoment(int p);//ֱ��ͼ��ԭ��� �׾�
		void HistogramCentralMoment(int p);//ֱ��ͼ�Ծ�ֵ �� �����ľ�
		void HistogramBias();//ֱ��ͼ��ƫ��
		void HistogramKurtosis ();//���
		void HistogramEnergy();//ֱ��ͼ������
		void HistogramEntropy();//ֱ��ͼ����

		//ָ������
		void NDVI(std::vector<int> verifyBands);//��һ��ֲ��ָ��
		void NDWI(std::vector<int> verifyBands);//��һ�����ˮ��ָ��
		void SVI(std::vector<int> verifyBands);//��Ӱֲ��ָ��
		void RVI(std::vector<int> verifyBands);//��ֲֵ��ָ��
		void GNDWI(std::vector<int> verifyBands);//��˹��һ��ˮ��ָ��


		//�����Ǿ����������ȡ�㷨�������㷨����洢����������������������ȡ�㷨���м����룩���ɹ�������ȡ����㷨�����������������ȡ�㷨����
		void BrightnessAlgorithm();
		double StdDev(int bandIndex);//����ָ��ͼ��ı�׼���void StdDev()�ڲ�����
		void LengthWidthRatioAlgorithm();
		double LengthAlgorithm();//���󳤶��㷨�����ڸ��㷨�ܼ򵥣���˲��洢�������ʹ��ʱֱ�ӵ���
		double WidthAlgorithm();//�������㷨�����ڸ��㷨�ܼ򵥣���˲��洢�������ʹ��ʱֱ�ӵ���
		void EllipticFitAlgorithm();//��Բ����㷨
		void RectangularFitAlgorithm();//��Բ����㷨
		void RadiusLargestEnclosedEllipseAlgorithm();//����ڽ���Բ�뾶�㷨
		void RadiusSmallestEnclosingEllipseAlgorithm();//��С�����Բ�뾶�㷨
		void CalculateGLCM();//����Ҷȹ�������
		void CalculateGLDV();//����ҶȲ��ʸ��
		void CalculateSampleStatistics();//����SampleStatistics
		float HistogramMean(int buckets,float* histogram);//����ֱ��ͼ�ľ�ֵ
	};
}

