/******************************************************************************
  File name: dip.h
  Author: WillLi99		Date:2019-6-6
  Description: ����ͼ�����࣬�������Ǵ���ṹ��ͼ�񣬻��ROIϸ��ͼ��ͼ���һЩ�������ݡ�
  Others: 
  Function List:
				DIP				//DIP�๹�캯��
				getImageSize	//���ԭʼͼ��ߴ�
				getGrayImage	//���ԭʼͼ�ĻҶ�ͼ
				getROI			//��ȡROI
  				getWeldSeamType				//�������ͷ���
  				processLaserStripeImage		//����������ͼ��
  				filterLaserStripeImage		//��������ͼ���˲�
  				thinLaserStripeImage		//��������ͼ��ϸ��
  				fitLaserStripeImage			//��������ͼ�����
  				findFittingData				//����������ϵ�����
  				findFeaturePoints			//��λ����������
  				updateLastFrame				//����lastFrame
				generateOutImage			//�������ͼ��
  				generateOffset				//��������ƫ��
  				generateDIPResult			//����ͼ�������
  				qImage2Mat					
  				mat2QImage
				getROIPosition				//����ROIλ��
				det							//������������ɾ��������ʽ
				drawDashLine				//������
				drawAsterisk				//����

  History: 
          <author>		<time>       <desc>
           WillLi99    2019-6-6     ���dip.hͷ��ע��
******************************************************************************/

#pragma once

#ifndef _DIP_H_
#define _DIP_H_
#include <vector>
#include <QImage>
#include "dip.h"
#include "common.h"

//ͼ������
class DIP
{
private:
	enum SeamType weldSeamType;
	
	Mat image;
	Mat gray;
	int width;
	int height;

	Mat roi1;
	int roi1X;
	int roi1Y;
	int roi1Width;
	int roi1Height;
	
	Mat roi2;
	int roi2X;
	int roi2Y;
	int roi2Width;
	int roi2Height;

	Mat filteredImage;
	Mat filteredImageDoubleType;

	Mat filteredImage2;
	
	Mat thinnedImage;
	Mat thinnedImageProfile;
	Mat thinnedImage2;
	Mat thinnedImageProfile2;

	vector<Point2d> featurePoints;
	Point2i weldSeamCenterPoint;

private:
	void getImageSize();
	void getGrayImage();
	void getROI();
	void classify();
	
	void processLaserStripeImage();	
	void filter(int filterMethod=FILTER_MED_33);
	void thin(int thinningMethod=INTENSITY_GRAVITY);
	void extractFeaturePoints(int extractionMethod=EXTRACTION_ITERATION);
	void updateLastFrame();

	void generateOutImage();
	void generateOffset();
	void generateWeldSeamInfo();
public:
	QImage qOutputImage;	
	Mat mOutputImage;
	int offset;
	SeamInfo seamProfileInfo;

	static void setROISize(int width,int height);
	static void locateROI(Mat& image);

	DIP(Mat mInputImage);

	static Mat qImage2Mat(QImage in);
	static QImage mat2QImage(Mat& in);
	
	static int roiWidth;
	static int roiHeight;
	static int roiX;		//ROI��gray�еĺ���λ��
	static int roiY;		//ROI��gray�е�����λ��
	static int roiCenterX;
	static int roiCenterY;
	static Mat lastFrame;			//��һ֡ͼ��
	static int nthFrame;

	double det(Point2i a,Point2i b);
	void drawDashLine(Mat& image,Point pt1, Point pt2,Scalar& color,int thickness);
	void drawAsterisk(Mat& image,Point pt,Scalar& color,int thickness);
};

#endif