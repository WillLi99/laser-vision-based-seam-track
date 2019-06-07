/******************************************************************************
  File name: image.cpp
  Author: WillLi99		Date:2019-5-18
  Description:
              ImagesAquisition���װ�˹���ͼƬ�Ļ�ȡ��������ݡ�ͼƬ��ȡ��Ӳ����JH300m
              �����ͼƬ��ȡ������ǵ���stream������
  Others: 
  Function List:
                1. ImageAcquisition
                2. ~ImageAcquisiton
                3. run	// ��ȡ���ͼ��
                4. setCameraIndex	// �趨�����
                5. pause	//��ͣ��ȡͼƬ
                6. stop		//ֹͣ��ȡͼƬ
                7. allowSavingMultipleImages	// ���Զ�����ͼƬ
                8. declineSavingMultipleImages //	���Զ�����ͼƬ       
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     ���calbraiton3.cppͷ��ע�ͺ͸�������ͷ��ע��
******************************************************************************/
#include "JHCap.h"
#include "dip.h"
#include "image.h"

bool isTestingModeTriggered=true;
int ImageAcquisition::INT_IMAGE_COUNT=1;


/******************************************************************************
  Function:ImageAcquisition
  Description:��ʼ������ֵ������../ImagesĿ¼
  Calls: 
  Called By: ImageAcquisition�����
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
ImageAcquisition::ImageAcquisition(QObject *parent)
{
	isCapturingImagesAllowed=false;
	isPauseCmdTriggered=false;
	isTestingModeTriggered=false;
	intCameraIndex=0;
	isSavingMultipleImagesCmdTriggered=false;
	QString qstrFileDirectory="..\\Images";

	if(!qdirImagesDirectory.exists(qstrFileDirectory))
	{
		qdirImagesDirectory.mkdir(qstrFileDirectory);
	}
}

/******************************************************************************
  Function:run
  Description: ���ڴ�����Դץȡ���ݣ�����Դ��������../Images/testing images��ͼƬ��
               �����ȡ��ͼƬ��
  Calls: JHCap.h/CameraGetImageSize JHCap.h/CameraGetImageBufferSize
         JHCap.h/CameraGetISPImageBufferSize JHCap.h/CameraQueryImage
  Called By: ImageAcquisition�����
  Input:          
  Output: 
  Return:       
  Others: ImagesAcquisition�̳���QThread�����߳̿���ʱ���˺������ڵ����ݱ�һֱִ�С���
  ��ȡ��һ֡ͼ��ʱ��emit��һ���źţ���SeamTrackingSysģ�鴦��
******************************************************************************/
void ImageAcquisition::run()
{
	while(true)
	{
		if(isTestingModeTriggered)	//isTestingModeTriggerd=trueΪ����ģʽ
		{
			int intNthImageNo;
			int intTestingImageNum=39;
			cv::string strTestingFilePath="..//Images//testing images//";
			for(intNthImageNo=1;intNthImageNo<=intTestingImageNum;intNthImageNo++)
			{
				cv::string strNewfilePath=strTestingFilePath+std::to_string((long long)intNthImageNo)+".bmp";
				cv::Mat matRawImage=cv::imread(strNewfilePath);
				
				QImage image=DIP::mat2QImage(matRawImage);

				emit captured2(image);

				if(!isTestingModeTriggered)	break;

				Sleep(500);;
			}
			if(intNthImageNo==intTestingImageNum)	intNthImageNo=1;
		}
		else if(!isTestingModeTriggered && g_isCameraWorking) // isTestingModeTriggered=true g_isCameraWorking=ture, Ϊ����ģʽ
		{
			QTime runtime;
			runtime.start();
			int intWidth, intHeight, intLen, intSize;

			if(isCapturingImagesAllowed)
			{

				CameraGetImageSize(intCameraIndex,&intWidth, &intHeight);
				CameraGetImageBufferSize(intCameraIndex,&intLen, CAMERA_IMAGE_RAW8);
				CameraGetISPImageBufferSize(intCameraIndex,&intSize,intWidth,intHeight,CAMERA_IMAGE_RGB24);
				unsigned char *buffer = new unsigned char[intLen];
				unsigned char *ISPbuf = new unsigned char[intSize];
				if(CameraQueryImage(intCameraIndex,buffer, &intLen, CAMERA_IMAGE_RAW8)==API_OK)
				{
					if(isPauseCmdTriggered) break;
					if(CameraISP(intCameraIndex,buffer,ISPbuf,intWidth,intHeight,CAMERA_IMAGE_RGB24)==API_OK)
					{
						QImage img(ISPbuf, intWidth, intHeight, QImage::Format_RGB888);
						if(isSavingMultipleImagesCmdTriggered)
						{
							QString filePath="..\\Images\\"+QString::number(INT_IMAGE_COUNT++,10) + ".bmp";
							img.save(filePath);
						}
						emit captured(img, ISPbuf);
					}
				}
				else
				{
					delete [] ISPbuf;
				}
				delete [] buffer;

			} // end of "else if(!isTestingModeTriggered && g_isCameraWorking)"
			else usleep(1000);
			if(isPauseCmdTriggered) break;
		}
	}
}

/******************************************************************************
  Function: stream
  Description: ����ץȡ���ͼ������
  Calls: 
  Called By: ImageAcquisition�����
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::stream()
{
	isCapturingImagesAllowed=true;
	isPauseCmdTriggered=false;
}

/******************************************************************************
  Function: setCameraIndex
  Description: �������index��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::setCameraIndex(int index)
{
	this->intCameraIndex=index;
}

/******************************************************************************
  Function: setCameraIndex
  Description: ��ͣ��ȡͼ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::pause()
{
	isCapturingImagesAllowed=false;
}

/******************************************************************************
  Function: setCameraIndex
  Description: ֹͣ��ȡͼ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::stop()
{
	isCapturingImagesAllowed=false;
	isPauseCmdTriggered=true;
}

/******************************************************************************
  Function: allowSavingMultipleImages
  Description: ���������ͼ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::allowSavingMultipleImages()
{
	if(!isTestingModeTriggered)
		isSavingMultipleImagesCmdTriggered=true;
}

/******************************************************************************
  Function: declineSavingMultipleImages
  Description: ֹͣ�������ͼ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::declineSavingMultipleImages()
{
	isSavingMultipleImagesCmdTriggered=false;
}

/******************************************************************************
  Function: tirggerTestingMode
  Description: ��������ģʽ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::tirggerTestingMode()
{
	isTestingModeTriggered=true;
}

/******************************************************************************
  Function: detriggerTestingMode
  Description: �رղ���ģʽ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::detriggerTestingMode()
{
	isTestingModeTriggered=false;
}