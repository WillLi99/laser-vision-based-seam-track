/*****************************************************************************
Copyright: Guangdong Provincial Welding Technology Research Center
Author: Li Yangjin
Date: 2018-8-27
Description:ImagesAquisition���װ�˹���ͼƬ�Ļ�ȡ��������ݡ�ͼƬ��ȡ��Ӳ����JH300m
�����ͼƬ��ȡ������ǵ���stream������
*****************************************************************************/
#include "JHCap.h"
#include "dip.h"
#include "image.h"

bool testOrNot=true;

int ImagesAcqusition::imageCount=1;

ImagesAcqusition::ImagesAcqusition(QObject *parent)
{
	workState=false;
	terminateState=false;
	cameraIndex=0;
	savingImagesSwitch=false;
	QString dir="..\\Images";

	if(!directory.exists(dir))
	{
		directory.mkdir(dir);
	}
}

/*****************************************************************************
Function:void ImagesAcqusition::run()
Description:�߳����е�ִ�еĺ���
Call:
Input:
Output:
Return:
Others:ImagesAcquisition�̳���QThread�����߳̿���ʱ���˺������ڵ����ݱ�һֱִ�С���
��ȡ��һ֡ͼ��ʱ��emit��һ���źţ���SeamTrackingSysģ�鴦��
*****************************************************************************/
void ImagesAcqusition::run()
{
	while(true)
	{
		if(testOrNot)	//���ڲ���ģʽ
		{
			int i;
			int testingImageNum=39;
			cv::string testingFilePath="..//Images//testing images//";
			for(i=1;i<=testingImageNum;i++)
			{

				//qDebug()<<"Now the "<<i<<"th image is on process..."<<endl;

				cv::string newfilePath=testingFilePath+std::to_string((long long)i)+".bmp";
				cv::Mat img=cv::imread(newfilePath);
				
				QImage image=DIP::mat2QImage(img);

				emit captured2(image);

				if(!testOrNot)	break;

				Sleep(500);;
			}
			
			if(i==testingImageNum)	i=1;

		}
		else if(!testOrNot && cameraIsRunning)
		{
			QTime runtime;
			runtime.start();
			int width, height, len, size;

			if(workState)
			{

				CameraGetImageSize(cameraIndex,&width, &height);
				CameraGetImageBufferSize(cameraIndex,&len, CAMERA_IMAGE_RAW8);
				CameraGetISPImageBufferSize(cameraIndex,&size,width,height,CAMERA_IMAGE_RGB24);
				unsigned char *buffer = new unsigned char[len];
				unsigned char *ISPbuf = new unsigned char[size];
				if(CameraQueryImage(cameraIndex,buffer, &len, CAMERA_IMAGE_RAW8)==API_OK)
				{
					if(terminateState) break;
					if(CameraISP(cameraIndex,buffer,ISPbuf,width,height,CAMERA_IMAGE_RGB24)==API_OK)
					{
						QImage img(ISPbuf, width, height, QImage::Format_RGB888);
						if(savingImagesSwitch)
						{
							QString filePath="..\\Images\\"+QString::number(imageCount++,10) + ".bmp";
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

			} else usleep(1000);
			if(terminateState) break;
		}
	}
}

/*****************************************************************************
Function:void ImagesAcqusition::stream()
Description:�൱�ڿ��ع���
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::stream()
{
	workState=true;
	terminateState=false;
}

/*****************************************************************************
Function:ImagesAcqusition::setCameraIndex(int index)
Description:������Index��
Call:
Input:index-�����ţ�����n������������ӵ����ԣ��������Ϊ0,1,...n-1
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::setCameraIndex(int index)
{
	this->cameraIndex=index;
}

/*****************************************************************************
Function:void ImagesAcqusition::pause()
Description:��ͣ��ȡͼƬ
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::pause()
{
	workState=false;
}

/*****************************************************************************
Function:void ImagesAcqusition::stop()
Description:ֹͣ��ȡͼƬ
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::stop()
{
	workState=false;
	terminateState=true;
}

/*****************************************************************************
Function:void ImagesAcqusition::setSavingPicturesSwitch()
Description:�Զ�����ͼƬ��
Call:
Input:
Output:
Return:
Others:����·����"..\\Images"Ŀ¼����Ŵ�1��ʼ
*****************************************************************************/
void ImagesAcqusition::setSavingPicturesSwitch()
{
	if(!testOrNot)
		savingImagesSwitch=true;
}

/*****************************************************************************
Function:void ImagesAcqusition::setSavingPicturesSwitch()
Description:�Զ�����ͼƬ��
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::resetSavingPicturesSwitch()
{
	savingImagesSwitch=false;
}