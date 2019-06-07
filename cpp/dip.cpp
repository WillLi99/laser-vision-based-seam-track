/******************************************************************************
  File name: dip.cpp
  Author: WillLi99		Date:2019-6-6
  Description: ����ͼ�����࣬�������Ǵ���ṹ��ͼ�񣬻��ROIϸ��ͼ��ͼ���һЩ�������ݡ�
			�㷨���̣�ת��Ϊ�Ҷ�ͼ-�ü�ROI-�˲�-ϸ��-��������ȡ�ͼ���-�����
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
           WillLi99    2019-6-6     ���dip.cppͷ��ע��
******************************************************************************/
#include "time.h"
#include "dip.h"

Mat DIP::lastFrame;
int DIP::roiX=0;
int DIP::roiY=0;
int DIP::nth=0;

/*****************************************************************************
Function:DIP
Description:�Դ���ͼ�ν��д���
Call:
Called By:
Input:receivedImage-�������ͼ��
Output:
Return:
Others:
*****************************************************************************/
DIP::DIP(Mat receivedImage)
{
	nth=nth+1;
	receivedImage.copyTo(image);
	processLaserStripeImage();
}


/*****************************************************************************
Function:getImageSize
Description: ���ԭʼͼ��ĳߴ�
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getImageSize()
{
	height = image.rows;
	width = image.cols;
}

/*****************************************************************************
Function:getROI
Description: �ü�ԭͼ,���ROI������Ϊ�˽���������������ͶӰ�ֲ��ü�ԭͼ��������С
		Ϊ400*100��ROIͼ��
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getROI()
{
	roiWidth=400;
	roiHeight=100;
	roi=gray(Range(roiY,roiY+roiHeight),
		Range(roiX,roiX+roiWidth));
}

/*****************************************************************************
Function:getGrayImage
Description: ��ûҶ�ͼ��
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getGrayImage()
{
	gray = Mat::zeros(Size(image.cols, image.rows), CV_8UC1);
	cvtColor(image, gray, CV_BGR2GRAY);
}

/*****************************************************************************
Function:thinLaserStripeImage
Description: ���ϸ��ͼ��
Call:
Called By:
Input:
Output:thinnedImage����Ϊ��filteredImage����ϸ�����ͼ��binImageOTSUΪ����OTSU
		�Զ���ֵ�ָ�õ��Ķ�ֵͼ��
Return:
Others:
*****************************************************************************/
void DIP::thinLaserStripeImage()
{
	filteredImage.convertTo(filteredImageDoubleType,CV_64FC1);
	
	//�Ҷ����ķ����ϸ��ͼ��
	Mat assistA=Mat::ones(Size(roiHeight,1),CV_64FC1);
	vector<double> assist;for(int i=0;i<roiHeight;i++) assist.push_back(i);
	Mat assistB=Mat(assist); 
	Mat assistBT=assistB.t();
	
	Mat num=assistBT*filteredImageDoubleType;
	Mat den=assistA*filteredImageDoubleType;
	thinnedImageInfo=num/den; 
	thinnedImageInfo.convertTo(thinnedImageInfo,CV_8UC1);	//ת��UINT8����

	thinnedImage=Mat::zeros(Size(roiWidth,roiHeight),CV_8UC1);
	uchar* ptr=thinnedImageInfo.ptr<uchar>(0);
	for (int i=0;i<roiWidth;i++)
	{
		if(ptr[i])
		{
			thinnedImage.at<uchar>(ptr[i],i)=255;
		}
	}
		
}

/*****************************************************************************
Function:findFittingData
Description: ��thinnedImage��Ѱ��������ֱ����ϵ����ݵ㡣
Call:
Called By:
Input:
Output:
Return:
Others:ע�⣺���巵�ض��������ݵ㣬�ͺ��������й�
*****************************************************************************/
PointMat DIP::findFittingData()
{
	PointMat pointsSet;
	Points points;
	switch(seam)
	{
	case BUTT_WELD_SEAM:
		for(int i=roiHeight/2-5;i<=roiHeight/2+5;i++)
		{
			uchar* ptr=thinnedImage.ptr<uchar>(i);
			for(int j=0;j<roiWidth;j++)
			{
				if(ptr[j])
				{
					points.push_back(Point2i(i,j));		//�ҵ�������ֱ����ϵ����ݵ�
				}
			}
		}

		pointsSet.push_back(points);	
		break;
	case LAP_WELD_SEAM:
		break;
	case GROOVE_WELD_SEAM:
		break;
	}

	return pointsSet;
}

/*****************************************************************************
Function:fitLaserStripeImage
Description: �������
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::fitLaserStripeImage()
{
	PointMat dataSet;
	// ��Բ�ͬ�ĺ������ͣ����ֱ�ߵ��߶���������ͬ
	dataSet=findFittingData();
	cv::fitLine(dataSet[0], fittedLineEquation, CV_DIST_L2, 0, 0.01, 0.01);

}


/*****************************************************************************
Function:findFeaturePoints
Description: ��λ���������㣬��ͬ�ĺ����в�ͬ�������������Ͷ�λ������
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::findFeaturePoints()
{
	//��Douglas�CPeucker�㷨��ú���������
	if(seam==BUTT_WELD_SEAM)
	{
		int width=thinnedImageInfo.cols;
		Point2i vertexPoint;
		Point2i startPoint;
		Point2i endPoint;

		//�ҵ�vertexPoint
		uchar* ptr=thinnedImageInfo.ptr<uchar>();
		uint yValue=thinnedImageInfo.ptr<uchar>()[0];
		for(int i=0;i<width;i++)
		{
			if(i==0)	startPoint=Point2i(0,ptr[i]);
			if(i==width-1)	endPoint=Point2i(width-1,ptr[width-1]);
			if(yValue>ptr[i])
			{
				yValue=ptr[i];
				vertexPoint=Point2i(i,yValue);
			}
		}

		//qDebug()<<"vertex Point is "<<vertexPoint.x<<","<<vertexPoint.y<<endl;
		//qDebug()<<"start Point is "<<startPoint.x<<","<<startPoint.y<<endl;
		//qDebug()<<"endPoint is"<<endPoint.x<<","<<endPoint.y<<endl;

		double dist1=sqrt((double)(vertexPoint.x-startPoint.x)*(vertexPoint.x-startPoint.x)+
			(vertexPoint.y-startPoint.y)*(vertexPoint.y-startPoint.y));

		double dist2=sqrt((double)(vertexPoint.x-endPoint.x)*(vertexPoint.x-endPoint.x)+
			(vertexPoint.y-endPoint.y)*(vertexPoint.y-endPoint.y));

		Point2i featurePointA;
		Point2i featurePointB;

		double maxDist=0;
		double temp=0;
		double tempDist=0;
		Point2i vecTemp;
		Point2i vecA=vertexPoint-startPoint;
		ptr=thinnedImageInfo.ptr<uchar>();
		for(int i=1;i<vertexPoint.x;i++)
		{
			if(ptr[i]==0)
			{
				temp=0;
				tempDist=0;
				continue;
			}
			else{
				vecTemp=Point2i(i,ptr[i])-startPoint;
				temp=abs(det(vecTemp,vecA));
				tempDist=temp/dist1;
			}
			if(maxDist<tempDist)
			{
				maxDist=tempDist;
				featurePointA=Point2i(i,ptr[i]);
			}
		}

		Point2i vecB=vertexPoint-endPoint;
		temp=0;
		tempDist=0;
		maxDist=0;
		for(int i=vertexPoint.x+1;i<endPoint.x;i++)
		{
			if(ptr[i]==0)
			{
				temp=0;
				tempDist=0;
				continue;
			}
			else{
				vecTemp=Point2i(i,ptr[i])-endPoint;
				temp=abs(det(vecTemp,vecB));
				tempDist=temp/dist2;
			}
			if(maxDist<tempDist)
			{
				maxDist=tempDist;
				featurePointB=Point2i(i,ptr[i]);
			}
		}
	
		featurePoints.push_back(featurePointA);
		featurePoints.push_back(featurePointB);
	}
		/*qDebug()<<"featurePointA"<<featurePoints[0].x<<","<<featurePoints[0].y<<endl;
		qDebug()<<"featurePointB"<<featurePoints[1].x<<","<<featurePoints[1].y<<endl;*/
}

/*****************************************************************************
Function:generateOutImage
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::generateOutImage()
{

	//��Сʮ�ּܱ���������λ��
	int horizontalShift=(width-roiWidth)/2;
	Point2i leftFeaturePoint=Point2i(horizontalShift+featurePoints[0].x,featurePoints[0].y+roiY);
	Point2i rightFeaturePoint=Point2i(horizontalShift+featurePoints[1].x,featurePoints[1].y+roiY);
	drawAsterisk(image,leftFeaturePoint,Scalar(0,255,255),2);
	drawAsterisk(image,rightFeaturePoint,Scalar(0,255,255),2);

	//��Сʮ�ּܱ���������ĵ��λ��
	Point2i absWeldSeamCenterPoint=0.5*(leftFeaturePoint+rightFeaturePoint);
	drawAsterisk(image,absWeldSeamCenterPoint,Scalar(255,0,0),2);

	//����ɫ�߿����ṹ������ͼ��ROI��λ��
	rectangle(image,Point(roiX,roiY),Point(roiX+roiWidth,
		roiY+roiHeight),Scalar(255,0,0),2,8);

	//����ɫʵ�߱����ǹ����λ��
	line(image,Point2i(roiX+roiWidth/2,roiY),Point2i(roiX+roiWidth/2,roiY+roiHeight),Scalar(255,0,0),2,8);
	
	//��ʾ��ROI�������ͺ���ƫ����Ϣ
	String ROITitle="ROI";
	putText(image,ROITitle,Point(roiX,roiY-5), FONT_HERSHEY_SIMPLEX,1.5,
		Scalar(0,0,255),2,8);

	String offsetInfo="Offset:"+std::to_string((long double)offset)+"pixels";
	putText(image,offsetInfo,Point(20,height-40), FONT_HERSHEY_SIMPLEX,1.5,Scalar(0,0,255),2,8);

	//��ʾ������
	double weldSeamWidth=abs(featurePoints[0].x-featurePoints[1].x);
	string weldSeamWidthInfo="Seam Width:"+std::to_string((long double)weldSeamWidth)+"pixels";
	putText(image,weldSeamWidthInfo,Point(20,height-80), FONT_HERSHEY_SIMPLEX,1.5,Scalar(0,0,255),2,8);

	outputImage=image;
	this->out=DIP::mat2QImage(image);
}


/*****************************************************************************
Function:generateOffset
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::generateOffset()
{
	double centerx;
	centerx=(featurePoints[0].x+featurePoints[1].x)/2;
	offset=roiWidth/2 - centerx;
}


/*****************************************************************************
Function:qImage2Mat
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
Mat DIP:: qImage2Mat(QImage in)
{
	Mat matImage;
	//qDebug()<<in.format();
	//matImage=Mat(in.height(),in.width(),CV_8UC3,(void*)in.constBits(), in.bytesPerLine());
	switch(in.format())
	{
	case QImage::Format_RGB888:
		matImage=Mat(in.height(),in.width(),CV_8UC3,(void*)in.constBits(),in.bytesPerLine());
		cvtColor(matImage,matImage,CV_BGR2RGB);
		break;
	case QImage::Format_Grayscale8:
		matImage = cv::Mat(in.height(), in.width(), CV_8UC1, (void*)in.constBits(), in.bytesPerLine());
		break;
	}

	return matImage;
}

/*****************************************************************************
Function:mat2QImage
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
QImage DIP::mat2QImage(Mat& in)
{
	//mat to QImage
	//QImage out;
	if(in.type()==CV_8UC1)
	{
		QImage img((const unsigned char *)(in.data), in.cols, in.rows, in.cols, QImage::Format_Grayscale8); 
		return img;
	}
	else if(in.type()==CV_8UC3)
	{
		const uchar *pSrc=(const uchar *)in.data;
		QImage out(pSrc,in.cols,in.rows,in.step,QImage::Format_RGB888);
		return out.rgbSwapped();
	}
}


/*****************************************************************************
Function:getWeldSeamType
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getWeldSeamType()
{
	seam=BUTT_WELD_SEAM;
}

/*****************************************************************************
Function:processLaserStripeImage
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::processLaserStripeImage()
{
	getImageSize();
	getGrayImage();	//ͼ�����㷨���ǻ��ڻҶ�ͼ
	getROI(); 
	getWeldSeamType();	//����
	switch(seam)
	{
		case BUTT_WELD_SEAM:
			filterLaserStripeImage(0);
			thinLaserStripeImage();	
			findFeaturePoints();	
			generateOffset();
			generateOutImage();
			generateDIPResult();
			updateLastFrame();
			break;
		case LAP_WELD_SEAM:
			break;
		case GROOVE_WELD_SEAM:
			break;
	}
}

/*****************************************************************************
Function:filterLaserStripeImage
Description: �ȶ�������֡�ṹ�⴫��ͼ����С��롱�������ٶԽṹ������ͼ������˲�
Input:method=0---33��ֵ�˲���=1---55��ֵ�˲���=2---15�����㣻=3---37������
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::filterLaserStripeImage(int method)
{
	//�Ƚ���������ͼ�����߶�Ӧ���ص���Сֵ��Ϊͼ����
	Mat lastFrameROI;
	if(nth!=1)
	lastFrameROI=lastFrame(Range(roiY,roiY+roiHeight),Range(
		roiX,roiX+roiWidth));
	else if(nth==1)
		lastFrameROI=roi;

	roi.copyTo(filteredImage);
	for(int i=0;i<roiHeight;i++)
	{
		uchar* ptrA=lastFrameROI.ptr<uchar>();
		uchar* ptrB=roi.ptr<uchar>();
		uchar* ptrC=filteredImage.ptr<uchar>();

		for(int j=0;j<roiWidth;j++)
		{
			if(ptrA[j]<=ptrB[j])
				ptrC[j]=ptrA[j];
			else
				ptrC[j]=ptrB[j];	
		}
	}
	

	Mat ele;
	switch(method)
	{
	case 0:
		medianBlur(roi,filteredImage,3);
		break;
	case 1:
		medianBlur(roi,filteredImage,5);
		break;
	case 2:
		ele = getStructuringElement(MORPH_RECT, Size(5, 1));
		morphologyEx(roi,filteredImage,MORPH_OPEN,ele);
		break;
	case 3:
		ele = getStructuringElement(MORPH_RECT, Size(7, 3));
		morphologyEx(roi,filteredImage,MORPH_OPEN,ele);
		break;
	}

}

/*****************************************************************************
Function:generateDIPResult
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::generateDIPResult()
{
	seaminfo.dWeldSeamOffset=offset;
	seaminfo.dRemainingHeight=-1;
	seaminfo.weldSeamType=seam;
	seaminfo.dWeldSeamWidth=abs(featurePoints[0].x-featurePoints[1].x);
}

/*****************************************************************************
Function:getROIPosition
Description: 
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getROIPosition(Mat& image,int* ROIX,int* ROIY)
{
	*ROIX=image.cols/2-200;

	Mat gray=Mat::zeros(Size(image.cols, image.rows), CV_8UC1);
	cvtColor(image, gray, CV_BGR2GRAY);

	//ͶӰ��ȷ��ROIY��λ��
	int maxValueAddr=0;
	Mat assist=Mat::ones(Size(1,gray.cols),CV_64FC1);

	/***�ԽӺ����I�ͽӿں�V�ͽӿڿ������·���***/
	Mat imageClone;gray.copyTo(imageClone);
	imageClone.convertTo(imageClone,CV_64FC1);
	Mat project2Y=imageClone*assist;

	//����project2Y,�ҳ������ֵ�ĵ�ַ
	int temp=0,i=0;
	for(;i<gray.rows;i++)
	{
		double* pt=project2Y.ptr<double>(i);
		if(temp<*pt)
		{
			maxValueAddr=i;
			temp=*pt;
		}
	}

	*ROIY=maxValueAddr-50;
}

/*****************************************************************************
Function:updateLastFrame
Description: ���ڻ�ȡ��һ֡�Ľṹ������ͼ��
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::updateLastFrame()
{
	lastFrame=gray;
}

/*****************************************************************************
Function:det
Description:����������������ʽ
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
double DIP::det(Point2i a,Point2i b)
{
	double res;
	res=a.x*b.y-a.y*b.x;
	return res;
}

/*****************************************************************************
Function:drawDashLine
Description:������
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::drawDashLine(Mat& image,Point pt1, Point pt2,Scalar& color,int thickness)
{
	int singleDashLineDist=15;
	double absDist=sqrt((double)(pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
	int singleDistLineAmount;
	singleDistLineAmount=(int)(absDist/singleDashLineDist);
	int xDiv=abs(pt2.x-pt1.x)/singleDistLineAmount;
	int yDiv=abs(pt2.y-pt1.y)/singleDistLineAmount;
	
	for(int i=0;i<singleDistLineAmount;i++)
	{
		line(image,Point(pt1.x+i*xDiv,pt1.y+i*yDiv),
			Point(pt1.x+(i+1)*xDiv*9/15,pt1.y+(i+1)*yDiv*9/15),color,thickness);
		line(image,Point(pt1.x+(i+1)*xDiv*11/15,pt1.y+(i+1)*yDiv*11/15),
			Point(pt1.x+(i+1)*xDiv*13/15,pt1.y+(i+1)*yDiv*13/15),color,thickness);
	}
}

/*****************************************************************************
Function:drawAsterisk
Description:����
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::drawAsterisk(Mat& image,Point pt,Scalar& color,int thickness)
{
	line(image,Point2i(pt.x-3,pt.y),Point2i(pt.x+3,pt.y),color,thickness);
	line(image,Point2i(pt.x,pt.y-5),Point2i(pt.x,pt.y+5),color,thickness);
}