#include "StdAfx.h"
#include "STRegion.h"

//#include <opencv2\opencv.hpp>
//using namespace cv;


namespace VerifyFeatureSet
{
	STRegion::STRegion()
	{
		//GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		valid = false;
		covarMatrixValid = false;
		outerPolyPointNum = 0;
	}


	STRegion::~STRegion(void)
	{
		Reset();
	}

	void STRegion::Reset(void)
	{
		if(valid)
		{
			area = 0;
			pixels.clear();
			len = 0;
			//dev = 0;
			polyPoints.clear();
			linesNum = 1;
			linePointsIndex.clear();	
			outerPixels.clear();
			/*if(outerPolyPointNum > 0)
			{
				outerPolyPointNum = 0;
				delete outerPolyPoints;
			}*/
		}
		valid = false;
		
		if(covarMatrixValid)
		{
			cvReleaseMat(&covarMatrix);  			
		}
		covarMatrixValid = false;
	}

	/*void STRegion::Box2Rect(CvBox2D box,CvPoint2D32f* rect)
	{
		cvBoxPoints(box,rect); 
	}*/

	void STRegion::CalculateBounds(int imageBorderPointNum,STPoint* imageBorder)
	{
		bounds[0] = imageBorder[0].Y;
		bounds[1] = imageBorder[0].Y;
		bounds[2] = imageBorder[0].X;
		bounds[3] = imageBorder[0].X;
		for(int i = 1;i<imageBorderPointNum;i++)
		{
			if(bounds[0] > imageBorder[i].Y) bounds[0] = imageBorder[i].Y;
			if(bounds[1] < imageBorder[i].Y) bounds[1] = imageBorder[i].Y;
			if(bounds[2] > imageBorder[i].X) bounds[2] = imageBorder[i].X;
			if(bounds[3] < imageBorder[i].X) bounds[3] = imageBorder[i].X;
		}
	}

	void STRegion::UpdateBounds(short Bounds[4])
	{
		bounds[0] = Bounds[0];
		bounds[1] = Bounds[1];
		bounds[2] = Bounds[2];
		bounds[3] = Bounds[3];
	}

	void STRegion::Initialize(SampleImage* sampleImage,int regionID,int imageBorderPointNum,STPoint* imageBorder)
	{
		//��ʼ���ڲ�����
		Reset();
		
		m_SampleImage = sampleImage;
		this->regionID = regionID;
				
		//����polyPoints��linesNum��linePointsIndex
		UpdateRegionPixel(m_SampleImage,imageBorderPointNum,imageBorder);	
		//UpdateOuterRegionPixel(m_SampleImage,imageBorderPointNum,imageBorder);
		InterpolateBorder(m_SampleImage,imageBorderPointNum,imageBorder,&polyPoints);		
		CalculateCovarMatrix(m_SampleImage);
		UpdateRegionShape();
		linesNum = 1;
		linePointsIndex.push_back(polyPoints.size()+1);
	

		//������Ӱ����ص���
		area = pixels.size();
		len = polyPoints.size();	
		
		valid = true;
	}

	bool STRegion::IsPointInRegion(int x,int y,Region* aRegion)
	{
		return aRegion->IsVisible(x, y);
	}

	void STRegion::UpdateRegionPixel(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder)
	{		
		//����GDI��Region�ṹ�����ڼ�����Ƿ��ڶ�����ڲ�
		GraphicsPath* aGP = new GraphicsPath();
		aGP->AddPolygon(imageBorder,imageBorderPointNum);
		Region* aRegion = new Region(aGP);

		//����SampleRegion Pixel����
		pixels.clear();
		for(int row = bounds[0];row <= bounds[1];row++)
		{
			for(int col = bounds[2];col <= bounds[3];col++)
			{
				if(IsPointInRegion(col,row,aRegion))
				{
					pixels.push_back(row*(bounds[3] + 1)+col);
				}
			}
		}

		delete aGP;
		delete aRegion;
	}

	void STRegion::InterpolateBorder(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder,std::vector<STPoint>* points)
	{	
		CvPoint point1,point2;
		CvLineIterator iterator;
		STPoint borderPoint;
	
		//IplImage* image = cvLoadImage(m_SampleImage->m_FileName,CV_LOAD_IMAGE_GRAYSCALE );
		IplImage* image = cvCreateImage(cvSize(bounds[3] + 1,bounds[1] + 1),IPL_DEPTH_8U,1);

		points->clear();
		for(int i=0;i<imageBorderPointNum-1;i++)
		{
			point1.x = imageBorder[i].X;
			point1.y = imageBorder[i].Y;
			point2.x = imageBorder[i+1].X;
			point2.y = imageBorder[i+1].Y;
		
			int count = cvInitLineIterator(image, point1,point2, &iterator,4);
			for( int i = 0; i < count - 1; i++ )//���������һ���㣬��������һ���߶εĵ�һ�����ظ�
			{     
				int offset = iterator.ptr - (uchar*)(image->imageData);
				borderPoint.Y = offset/image->widthStep;             
				borderPoint.X = (offset - borderPoint.Y*image->widthStep)/(3*sizeof(uchar));
				points->push_back(borderPoint);

				CV_NEXT_LINE_POINT(iterator); 
			}
		}

		//points.push_back(polyPoints[0]);//ʹ�����߽��һ���������һ�����غϣ��γɷ�ձ�Ե
		cvReleaseImage(&image);
	}

	/*void STRegion::UpdateOuterRegionPixel(SampleImage* m_SampleImage,int imageBorderPointNum,STPoint* imageBorder)
	{	
		//����GDI��Region�ṹ�����ڼ�����Ƿ��ڶ�����ڲ�
		GraphicsPath* aGP = new GraphicsPath();
		aGP->AddPolygon(imageBorder,imageBorderPointNum);
		Region* aRegion = new Region(aGP);

	    //����ԭʼͼ��Ķ�ֵͼ��
		IplImage* image = cvCreateImage(cvSize(m_SampleImage->m_Width,m_SampleImage->m_Height),IPL_DEPTH_8U,1);  
		//����ͼ���ʼֵΪ0
		cvZero(image);
		CvScalar s;
		for(int row = bounds[0];row <= bounds[1];row++)
		{
			for(int col = bounds[2];col <= bounds[3];col++)
			{
				if(IsPointInRegion(col,row,aRegion))
				{
					s.val[0] = 255;
					cvSet2D(image,row,col,s);
				}								
			}
		}

		delete aGP;
		delete aRegion;
		//s.val[0] = 3;
		//cvSet2D(image,101,102,s);
		//cvSaveImage("d:\\x1.png",image); 
		//IplImage* image3 = cvCreateImage(cvSize(m_SampleImage->m_Width,m_SampleImage->m_Height),IPL_DEPTH_8U,1);

		//���ͺ�õ�Ŀ��ͼ��Ķ�ֵͼ��
		cvDilate( image,image, NULL,1); //���� 

		//�����ͺ��Ŀ��ͼ���ֵͼ��õ����ص�
		outerPixels.clear();
		for(int row = bounds[0] - 1;row <= bounds[1] + 1;row++)
		{
			for(int col = bounds[2] - 1;col <= bounds[3] + 1;col++)
			{
				s = cvGet2D(image,row,col);
				if(s.val[0] > 0)
				{
					outerPixels.push_back(row*m_SampleImage->m_Width+col);
				}
			}
		}

				
		cvThreshold(image,image,1,255,CV_THRESH_BINARY );

		CvMemStorage *storage = cvCreateMemStorage(); //����һ���ڴ����򣬸������Ƕ���Զ�̬������   
		CvSeq *contour = NULL;  //����һ�����У���Щ���п��Դ����������ڴ�������  
		int cnt = cvFindContours(image,storage,&contour,sizeof(CvContour),CV_RETR_LIST);// // //Ĭ�ϣ�mode=CV_RETR_LIST��������ż����           		
		outerPolyPointNum = contour->total;	
		outerPolyPoints = new STPoint[outerPolyPointNum];
		CvSeqReader reader;       //-- ������һ����������  
        cvStartReadSeq(contour,&reader);       //��ʼ��ȡ 
		CvPoint pt = cvPoint(0,0);   
        for(int i = 0 ;i < outerPolyPointNum; i++)
		{   
             CV_READ_SEQ_ELEM(pt,reader);     //--������һ�������е�һ��Ԫ�ص�
			 outerPolyPoints[i].X = pt.x; 
			 outerPolyPoints[i].Y = pt.y; 
        }  				
		cvReleaseMemStorage(&contour->storage); 

		//InterpolateBorder(m_SampleImage,borderNum,borderPoints,&outerPolyPoints);

		cvReleaseImage(&image);
	}*/

	void STRegion::UpdateRegionShape()
	{
//		IplImage* img = cvCreateImage (cvSize(500, 500), 8, 3);  
//		cvNamedWindow ("hull", 1);  
//
//#if ! ARRAY  
//		CvMemStorage* storage = cvCreateMemStorage (0);  
//#endif  
//		int i;  
//		int contour = rand() % 100 + 1;  
//		int hullcontour;  
//		CvPoint pt0;  
//#if ! ARRAY  
//		CvSeq* ptseq = cvCreateSeq (CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage);  
//		CvSeq* hull;  
//		for (i = 0; i < contour; i++)  
//		{  
//			pt0.x = rand() % (img->width / 2) + img->width / 4;  
//			pt0.y = rand() % (img->height / 2) + img->height / 4;  
//			cvSeqPush (ptseq, &pt0);  
//		}  
//		hull = cvConvexHull2 (ptseq, 0, CV_CLOCKWISE, 0);  
//		/*hull = cvConvexHull2 (ptseq, 0, CV_CLOCKWISE, 1);*/  
//		hullcontour = hull->total;  
//
//		CvBox2D box = cvMinAreaRect2(ptseq,NULL); 
//		int a=3;
//
//#else  
//		CvPoint* points = (CvPoint*)malloc(contour * sizeof(points[0]));  
//		int* hull = (int*)malloc(contour * sizeof(hull[0]));  
//		CvMat point_mat = cvMat (1, contour, CV_32SC2, points);  
//		CvMat hull_mat = cvMat (1, contour, CV_32SC1, hull);  
//		for (i = 0; i < contour; i++)  
//		{  
//			pt0.x = rand() % (img->width / 2) + img->width / 4;  
//			pt0.y = rand() % (img->height / 2) + img->height / 4;  
//			points[i] = pt0;  
//		}  
//		cvConvexHull2 (&point_mat, &hull_mat, CV_CLOCKWISE, 0);  
//		hullcontour = hull_mat.cols;  
//#endif  
//		cvZero (img);  
//		for (i = 0; i < contour; i++)  
//		{  
//#if ! ARRAY  
//			pt0 = * CV_GET_SEQ_ELEM(CvPoint, ptseq, i);  
//			/*pt0 = ** CV_GET_SEQ_ELEM(CvPoint*, ptseq, i);*/  
//#else  
//			pt0 = points[i];  
//#endif  
//			cvCircle (img, pt0, 2, CV_RGB(255, 0, 0), CV_FILLED);  
//		}  
//#if ! ARRAY  
//		pt0 = ** CV_GET_SEQ_ELEM(CvPoint*, hull, hullcontour - 1);  
//		/*pt0 = * CV_GET_SEQ_ELEM(CvPoint, hull, hullcontour - 1);*/  
//#else  
//		pt0 = points[hull[hullcontour - 1]];  
//#endif  
//
//		for (i = 0; i < hullcontour; i++)  
//		{  
//#if ! ARRAY  
//			CvPoint pt = **CV_GET_SEQ_ELEM(CvPoint*, hull, i);  
//			/*CvPoint pt = *CV_GET_SEQ_ELEM(CvPoint, hull, i);*/  
//#else  
//			CvPoint pt = points[hull[i]];  
//#endif  
//			cvLine (img, pt0, pt, CV_RGB(255, 0, 0));  
//			pt0 = pt;  
//		}  
//
//		cvShowImage ("hull", img);  




		//����polyPoints����squ�ṹ
		CvPoint tempPoint;
		CvMemStorage *storage = cvCreateMemStorage(0); 
		CvSeq* seq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint),storage);
		for(int i=0;i<polyPoints.size();i++)
		{
			tempPoint.x = polyPoints[i].X;
			tempPoint.y = polyPoints[i].Y;
			cvSeqPush(seq,&tempPoint);
		}

		//����opencv��ȡ��С��Χ���β�����minBoundRect
		CvBox2D box = cvMinAreaRect2(seq,NULL); 		  
		cvBoxPoints(box,minBoundRect);  
		//����opencv��ȡ�����Բ������ԲRect����fitEllipseRect
		olsEcllipsBox = cvFitEllipse2(seq); 	

		//����������˳�
		cvReleaseMemStorage(&seq->storage); 		

		return;


			/*IplImage *src = cvLoadImage("D:\\2.png",0);  
			CvMemStorage *storage = cvCreateMemStorage(); //����һ���ڴ����򣬸������Ƕ���Զ�̬������   
			CvSeq *seq = NULL;  //����һ�����У���Щ���п��Դ����������ڴ�������  
			cvThreshold(src,src,100,255,CV_THRESH_BINARY_INV);
			int cnt = cvFindContours(src,storage,&seq,sizeof(CvContour),CV_RETR_LIST);// // //Ĭ�ϣ�mode=CV_RETR_LIST��������ż����  
			//cvDrawContours(src, seq, CV_RGB(255,0,0), CV_RGB(255, 0, 0),  
				2, 2, 8, cvPoint(0,0));
			for(;seq->h_next != NULL;seq = seq->h_next)
			{   
			int c=seq->total;//��ǰ�����������ٸ�Ԫ�أ������Ԫ��Ϊ��  
			if(c < 40)
				continue;

			double length = cvArcLength(seq); //�õ�ָ�����Ǹ��������ܳ�  
			//�ú�����3�����������У���㣨Ĭ�ϼ����������ߣ����Ƿ�������  
			double area = cvContourArea(seq);  //�õ�ָ�����Ǹ����������  
			CvRect rect = cvBoundingRect(seq,1);  //�������У�����������Χ���Σ�  
			CvBox2D box = cvMinAreaRect2(seq,NULL); //��С��Χ����   

			//cout<<"Length = "<<length<<endl;    
			//cout<<"Area = "<<area<<endl;   

			CvPoint pt1,pt2;  
			pt1.x=rect.x;  
			pt1.y=rect.y;  
			pt2.x=rect.x+rect.width;  
			pt2.y=rect.y+rect.height;  


			IplImage *dst = cvLoadImage("D:\\2.png"); 

			cvDrawContours(dst,seq,CV_RGB(255,0,0),CV_RGB(255,0,0),0);    
			cvRectangle(dst,pt1,pt2,CV_RGB(0,255,0));   
			cvNamedWindow("dst",1);  
			cvShowImage("dst",dst);    
			//cvWaitKey();    

		CvPoint2D32f center;    
		float radius;    
		int a=  cvMinEnclosingCircle(seq,&center,&radius); //�������л�����С������Բ  


		//cout<<"center.x = "<<center.x<<endl;    
		//cout<<"center.y = "<<center.y<<endl;   
		//cout<<"radius = "<<radius<<endl;   
			//Բ�����ǰ������е㣬�ɹ�����1�����ҵõ�Բ�ĺͰ뾶  


			cvCircle(dst,cvPointFrom32f(center),cvRound(radius),CV_RGB(100,100,100));    
			cvShowImage("dst",dst);    
			//cvWaitKey();    

			CvBox2D ellipse = cvFitEllipse2(seq);//��С���˷�����Բ���    
			cvEllipseBox(dst,ellipse,CV_RGB(255,255,0)); //��ͼ�ϻ���Բ   
		    
			//cvWaitKey();    

			//���������С����     
			CvPoint2D32f pt[4];    
			cvBoxPoints(box,pt);    
			for(int i = 0;i<4;++i){    
				cvLine(dst,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(0,0,255));  
			
			cvShowImage("dst",dst);  
			}    
	  
			cvWaitKey(4000);    

			cvReleaseImage(&src);    
			cvReleaseImage(&dst);    
			//cvReleaseMemStorage(&storage);   
	  
			}*/
	}

	void STRegion::CalculateCovarMatrix(SampleImage* m_SampleImage)
	{
		int pixelNum = pixels.size();  
		CvMat *sampleMat = cvCreateMat(pixelNum , 2 ,CV_32FC1);
		int x,y;
		for(int i=0;i<pixelNum;i++)
		{
			y = pixels[i] / (bounds[3] + 1);
			x = pixels[i] % (bounds[3] + 1);
			cvSetReal2D(sampleMat, i, 0, y);
			cvSetReal2D(sampleMat, i, 1, x);
		}
  
		CvMat *Vector[1]; 
		Vector[0]=sampleMat;   
		covarMatrix = cvCreateMat( 2 , 2 ,CV_32FC1);   
		CvMat *avg = cvCreateMat( 1 , 2 ,CV_32FC1);   

		cvCalcCovarMatrix(( const CvArr **)Vector, pixelNum ,covarMatrix,avg,CV_COVAR_SCALE+CV_COVAR_NORMAL+CV_COVAR_ROWS);  

		cvReleaseMat(&sampleMat);  
		cvReleaseMat(&avg); 

		covarMatrixValid = true;
	}
}