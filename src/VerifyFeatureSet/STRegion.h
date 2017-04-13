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
	//���������
	class STRegion
	{
	public:		
		STRegion();
		~STRegion(void);
		void Reset();
		void CalculateBounds(int imageBorderPointNum,STPoint* imageBorder);
		void UpdateBounds(short Bounds[4]);
		void Initialize(SampleImage* sampleImage,int regionID,int imageBorderPointNum,STPoint* imageBorder);//�˹���ѡ��������ʱimageBorder��ŵ��Ǳ߽����ؼ�����������е㣬��Ҫת��Ϊ���е�����polyPoints
		bool IsPointInRegion(int x,int y,Region* aRegion);//�жϵ��Ƿ���Region�ڡ�x��y�ֱ��ʾ�����ꡢ������
		int                         regionID;
		bool                        valid;
		//std::vector<float>			means;			//ƽ���Ҷ�ֵ
		int							area;			//�������λΪ����
		std::vector<int>			pixels;			//��¼����������������,��������������Ӱ�񹹽�����
		//VectorTS<int>			    pixels;			//��¼����������������,��������������Ӱ�񹹽�����
		short						bounds[4];		//����ı߽��ķ�Χ top,down,left,right  (�ñ߽��������ϵƽ��)		
		short						len;			//�ܳ�
		//float						dev;			//��׼�ָ���Ǹ����α�׼��ľ�ֵ
		std::vector<STPoint>        polyPoints;     //ȫ���߽�㣬������ʾ�������д�ŵ��Ǳ߽��ϵ����е㣬�����ǹؼ���
		int                         linesNum;       //�߽�������
		std::vector<long>           linePointsIndex; //��߽�ʱ�����ڼ�¼�±߽���ʼ�������	
		//������polyPoints��linesNum��linePointsIndexʹ�ñ��ṹ���д����ڲ��ж���ն���Ӱ����������������ڱ�ϵͳ��������
		//������ڿն��������б߽�����������1		

		//���������OpenCV����õ���Ϊ������״��������Ҫ�Ľṹ
		CvPoint2D32f                minBoundRect[4];//��С�߽���Σ��ɲ�������ϵƽ�У����ܿ�ס�������С����
		CvBox2D                     olsEcllipsBox;//��С���������Բ��Box
		std::vector<int>			outerPixels;			//��¼����������һ�����غ�Ӱ��������������,���ڼ���Э�������
		int                         outerPolyPointNum;
		STPoint*                    outerPolyPoints;     //��¼����������һ�����غ�Ӱ��߽�㣬���ڼ���Э������󣬸����д�ŵ��Ǳ߽��ϵĹؼ���
		CvMat*                      covarMatrix;//Э�������
		bool                        covarMatrixValid;
	private:
		void InterpolateBorder(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder,std::vector<STPoint>* points);//���˹���ѡ�߽����ؼ���ת��Ϊ���е�����polyPoints
		void UpdateRegionPixel(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder);//�������������е�����pixels
		//void UpdateOuterRegionPixel(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder);//�õ�����������һ�����غ�Ӱ�������������ź����outerPixels
		void UpdateRegionShape();//����OpenCV������С��Ӿ��Σ������Բ����״�ṹ
		void CalculateCovarMatrix(SampleImage* m_SampleImage);//����OpenCV

		//void Box2Rect(CvBox2D box,CvPoint2D32f* rect);
		SampleImage* m_SampleImage;		

		Mutex *lock;
	};
}

