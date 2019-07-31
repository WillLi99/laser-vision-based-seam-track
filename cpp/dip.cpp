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
		   WillLi99    2019-6-10    thinLaserStripe���뼫��ֵ����ϸ����
		   WillLi99    2019-6-10    findFeaturePoint����һ���󵼷���λ����������
******************************************************************************/
#include "time.h"
#include "dip.h"

Mat DIP::lastFrame;
int DIP::roiWidth=0;
int DIP::roiHeight=0;
int DIP::roiX=0;
int DIP::roiY=0;
int DIP::roiCenterX=0;
int DIP::roiCenterY=0;
int DIP::nthFrame=0;

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
DIP::DIP(Mat mInputImage)
{
	nthFrame=nthFrame+1;
	mInputImage.copyTo(image);
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
		Ϊ400*200��ROIͼ��
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getROI()
{
	roi1Width=DIP::roiWidth;
	roi1Height=DIP::roiHeight;
	roi1X=DIP::roiX;
	roi1Y=DIP::roiY;

	roi2Width=roi1Width;
	roi2Height=40;		//20 pixels
	roi2X=DIP::roiX;
	roi2Y=DIP::roiCenterY-roi2Height/2;
	roi1=gray(Range(DIP::roiY,DIP::roiY+roi1Height),
		Range(DIP::roiX,DIP::roiX+roi1Width));
	roi2=gray(Range(roi2Y,roi2Y+roi2Height),
		Range(roi2X,roi2X+roi2Width));
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
Description: ���ϸ��ͼ��.
Call:
Called By:
Input: INTENSITY_GRAVITY�ǻҶ����ķ���INTENSITY_MAXIMUM�ǻҶȼ�ֵ��
Output:thinnedImage����Ϊ��filteredImage����ϸ�����ͼ��
Return:
Others:
*****************************************************************************/
void DIP::thin(int thinningMethod)
{
	if(thinningMethod==INTENSITY_GRAVITY)//�Ҷ����ķ����ϸ��ͼ��
	{
		filteredImage.convertTo(filteredImageDoubleType,CV_64FC1);	
		
		Mat assistA=Mat::ones(Size(roiHeight,1),CV_64FC1);
		vector<double> assist;for(int i=0;i<roiHeight;i++) assist.push_back(i);
		Mat assistB=Mat(assist); 
		Mat assistBT=assistB.t();
	
		Mat num=assistBT*filteredImageDoubleType;
		Mat den=assistA*filteredImageDoubleType;
		thinnedImageProfile=num/den; 
		thinnedImageProfile.convertTo(thinnedImageProfile,CV_8UC1);	//ת��UINT8����

		thinnedImage=Mat::zeros(Size(roiWidth,roiHeight),CV_8UC1);
		uchar* ptr=thinnedImageProfile.ptr<uchar>(0);
		for (int i=0;i<roiWidth;i++)
		{
			if(ptr[i])
			{
				thinnedImage.at<uchar>(ptr[i],i)=255;
			}
		}
	}
	else if(thinningMethod==INTENSITY_MAXIMUM)//�Ҷȼ�ֵ����ṹ������ϸ��������
	{
		vector<int> maximumPixelCoordinateY;

		INT32* ptr;
		unsigned char pixelValue;

		unsigned char temp_pixel_value;
		unsigned char temp_position;
		const unsigned char maxPixelValue=255;
		const unsigned char thresh=(unsigned char)maxPixelValue*0.4;
		thinnedImageProfile=Mat::zeros(1,roi1Width,CV_32SC1);
		thinnedImageProfile2=Mat::zeros(1,roi2Width,CV_32SC1);

		//����thinnedImage
		for(int c=0;c<roi1Width;c++)
		{
			pixelValue=filteredImage.at<uchar>(0,c);
			maximumPixelCoordinateY.clear();
			for(int r=0;r<roi1Height;r++)
			{
				temp_pixel_value=filteredImage.at<uchar>(r,c);
				if(temp_pixel_value>pixelValue)
				{
					pixelValue=temp_pixel_value;
					temp_position=r;
				}
				if(temp_pixel_value==maxPixelValue)
				{
					maximumPixelCoordinateY.push_back(r);
				}
			}
			if(maximumPixelCoordinateY.size()!=0)
			{
				thinnedImageProfile.at<INT32>(0,c)=0.5*(maximumPixelCoordinateY[0]+
					maximumPixelCoordinateY[maximumPixelCoordinateY.size()-1]);
			}
			else
			{
				thinnedImageProfile.at<INT32>(0,c)=temp_position;
			}
		}
	
		thinnedImage=Mat::zeros(Size(roi1Width,roi1Height),CV_8UC1);
		ptr=thinnedImageProfile.ptr<INT32>(0);
		for (int i=0;i<roiWidth;i++)
		{
			if(ptr[i])
			{
				thinnedImage.at<uchar>(ptr[i],i)=255;
			}
		}

		//imshow("thin version",thinnedImage);waitKey();
		imwrite("..//images//qOutputImage.tif",thinnedImage);

		//����thinnedImage2

		for(int c=0;c<roi2Width;c++)
		{
			maximumPixelCoordinateY.clear();
			for(int r=0;r<roi2Height;r++)
			{
				temp_pixel_value=filteredImage2.at<uchar>(r,c);
				if(temp_pixel_value>thresh)
				{
					maximumPixelCoordinateY.push_back(r);
				}
			}
			if(maximumPixelCoordinateY.size()!=0)
			{
				thinnedImageProfile2.at<INT32>(0,c)=0.5*(maximumPixelCoordinateY[0]+
					maximumPixelCoordinateY[maximumPixelCoordinateY.size()-1]);
			}
			else
			{
				thinnedImageProfile2.at<INT32>(0,c)=0;
			}
		}

		thinnedImage2=Mat::zeros(Size(roi2Width,roi2Height),CV_8UC1);
		ptr=thinnedImageProfile2.ptr<INT32>(0);
		for (int i=0;i<roi2Width;i++)
		{
			if(ptr[i])
			{
				thinnedImage2.at<uchar>(ptr[i],i)=255;
			}
		}
		imwrite("..//images//qOutputImage2.tif",thinnedImage2);
	}
	return;
}

/*****************************************************************************
Function:findFeaturePoints
Description: ʶ�𺸷������㡣EXTRACTION_ITERATEIONΪ�õ��������㷨ʶ�𺸷������㣻
            EXTRACTION_DIFFERENTIALΪ��һ��΢�ַ���λ����������
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::extractFeaturePoints(int extractionMethod)
{
	if(extractionMethod==EXTRACTION_ITERATION) //�õ��������㷨ʶ�𺸷�������
	{
		int width=thinnedImageProfile.cols;
		Point2i vertexPoint;
		Point2i startPoint;
		Point2i endPoint;

		//�ҵ�vertexPoint
		uchar* ptr=thinnedImageProfile.ptr<uchar>();
		uint yValue=thinnedImageProfile.ptr<uchar>()[0];
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
		ptr=thinnedImageProfile.ptr<uchar>();
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
	}// end of if(method==1)
	else if(extractionMethod==EXTRACTION_DIFFERENTIAL)
	{
		vector<INT32> diffProfile;
		INT32* ptr=thinnedImageProfile2.ptr<INT32>(0);
		for(int i=1;i<roi2Width;i++)
			diffProfile.push_back(ptr[i]-ptr[i-1]);

		int minimumValuePosition,maximumValuePosition;
		int minimumValue=0;int maximumValue=0;
		for(int i=0;i<diffProfile.size();i++)
		{
			if(minimumValue>diffProfile[i])
			{
				minimumValue=diffProfile[i];
				minimumValuePosition=i;
			}
			if(maximumValue<diffProfile[i])
			{
				maximumValue=diffProfile[i];
				maximumValuePosition=i;
			}
		}
		
		Point2i featurePointA;
		Point2i featurePointB;

		featurePointA=Point2i(minimumValuePosition,thinnedImageProfile2.ptr<int>(0)[
			minimumValuePosition]);
		featurePointB=Point2i(maximumValuePosition+1,thinnedImageProfile2.ptr<int>(0)[
			maximumValuePosition+1]);
		featurePoints.push_back(featurePointA);
		featurePoints.push_back(featurePointB);
	}//// end of if(method==2)
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
	int horizontalShift=(width-roi1Width)/2;
	Point2i leftFeaturePoint=Point2i(horizontalShift+featurePoints[0].x,featurePoints[0].y+roiY+roi1Height/2-roi2Height/2);
	Point2i rightFeaturePoint=Point2i(horizontalShift+featurePoints[1].x,featurePoints[1].y+roiY+roi1Height/2-roi2Height/2);
	drawAsterisk(image,leftFeaturePoint,Scalar(0,255,255),2);
	drawAsterisk(image,rightFeaturePoint,Scalar(0,255,255),2);

	//��Сʮ�ּܱ���������ĵ��λ��
	Point2i absWeldSeamCenterPoint=0.5*(leftFeaturePoint+rightFeaturePoint);
	drawAsterisk(image,absWeldSeamCenterPoint,Scalar(255,0,0),2);

	//����ɫ�߿����ṹ������ͼ��ROI1��λ��
	rectangle(image,Point(roiX,roiY),Point(roiX+roi1Width,
		roiY+roi1Height),Scalar(255,0,0),2,8);

	//����ɫ�߿����ṹ������ͼ��ROI2��λ��
	rectangle(image,Point(roi2X,roi2Y),Point(roi2X+roi2Width,
		roi2Y+roi2Height),Scalar(0,255,0),2,8);

	//����ɫ���߱����ǹ����λ��
	line(image,Point2i(roi1X+roi1Width/2,roiY),Point2i(roi1X+roi1Width/2,
		roi1Y+roi1Height),Scalar(255,0,0),2,8);
	
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

	mOutputImage=image;
	this->qOutputImage=DIP::mat2QImage(image);
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
	offset=roi2Width/2 - centerx;
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
	//QImage qOutputImage;
	if(in.type()==CV_8UC1)
	{
		QImage img((const unsigned char *)(in.data), in.cols, in.rows, in.cols, QImage::Format_Grayscale8); 
		return img;
	}
	else if(in.type()==CV_8UC3)
	{
		const uchar *pSrc=(const uchar *)in.data;
		QImage qOutputImage(pSrc,in.cols,in.rows,in.step,QImage::Format_RGB888);
		return qOutputImage.rgbSwapped();
	}
}


/*****************************************************************************
Function:classify
Description: �������
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::classify()
{
	weldSeamType=BUTT_WELD_SEAM;
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
	getGrayImage();
	getROI(); //imwrite("..//images//roi2.tif",roi2);
	classify();
	switch(weldSeamType)
	{
		case BUTT_WELD_SEAM:
			filter(FILTER_MED_33);
			thin(INTENSITY_MAXIMUM);	
			extractFeaturePoints(EXTRACTION_DIFFERENTIAL);	
			generateOffset();
			generateOutImage();
			generateWeldSeamInfo();
			updateLastFrame();
			break;
		case LAP_WELD_SEAM:
			break;
		case GROOVE_WELD_SEAM:
			break;
	}
}

/*****************************************************************************
Function:filter
Description: �ȶ�������֡�ṹ�⴫��ͼ����С��롱�������ٶԽṹ������ͼ������˲�
Input:FILTER_MED_33---33��ֵ�˲���FILTER_MED_55---55��ֵ�˲���
      FILTER_OPEN_15---15�����㣻FILTER_OPEN_37---37������
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::filter(int filterMethod)
{
	Mat lastFrameROI;
	if(nthFrame!=1)
	lastFrameROI=lastFrame(Range(DIP::roiY,DIP::roiY+roi1Height),Range(
		DIP::roiX,DIP::roiX+roi1Width));
	else if(nthFrame==1)
		lastFrameROI=roi1;

	roi1.copyTo(filteredImage);

	//��֡ͼ��"min"����
	for(int i=0;i<roi1Height;i++)
	{
		uchar* ptrA=lastFrameROI.ptr<uchar>();
		uchar* ptrB=roi1.ptr<uchar>();
		uchar* ptrC=filteredImage.ptr<uchar>();

		for(int j=0;j<roi1Width;j++)
		{
			if(ptrA[j]<=ptrB[j])
				ptrC[j]=ptrA[j];
			else
				ptrC[j]=ptrB[j];	
		}
	}
	
	Mat ele;
	switch(filterMethod)
	{
	case FILTER_MED_33:
		medianBlur(roi1,filteredImage,3);
		break;
	case FILTER_MED_55:
		medianBlur(roi1,filteredImage,5);
		break;
	case FILTER_OPEN_15:
		ele = getStructuringElement(MORPH_RECT, Size(5, 1));
		morphologyEx(roi1,filteredImage,MORPH_OPEN,ele);
		break;
	case FILTER_OPEN_37:
		ele = getStructuringElement(MORPH_RECT, Size(7, 3));
		morphologyEx(roi1,filteredImage,MORPH_OPEN,ele);
		break;
	}

	filteredImage2=filteredImage(Range(roi1Height/2-roi2Height/2,
		roi1Height/2+roi2Height/2),Range(0,roi2Width));

}

/*****************************************************************************
Function:generateWeldSeamInfo
Description: ����ͼ����Ľ������λ��pixel
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::generateWeldSeamInfo()
{
	seamProfileInfo.dWeldSeamOffset=offset;
	seamProfileInfo.dRemainingHeight=-1;
	seamProfileInfo.weldSeamType=weldSeamType;
	seamProfileInfo.dWeldSeamWidth=abs(featurePoints[0].x-featurePoints[1].x);
}

/*****************************************************************************
Function:locateROI
Description: ����ͶӰ������ROIͼ��ԭͼ�е�λ�ã�����ROI����λ�ú����Ͻǵ��λ��
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::locateROI(Mat& image)
{
	//ROI������X�����ԭͼ������x�غ�
	DIP::roiCenterX=image.cols/2;
	DIP::roiX=DIP::roiCenterX-DIP::roiWidth/2;

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

	DIP::roiCenterY=maxValueAddr;
	DIP::roiY=DIP::roiCenterY-DIP::roiHeight/2;
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

/*****************************************************************************
Function:setROISize
Description:����ROI��Ⱥ͸߶�
Call:
Called By:
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::setROISize(int width,int height)
{
	DIP::roiWidth=width;
	DIP::roiHeight=height;
}