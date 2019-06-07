/******************************************************************************
  File name: image.h
  Author: WillLi99		Date:2019-5-18
  Description:
              ������ImageAcquisition�࣬����̳���QThread�����������ж�ȡͼ��
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
           WillLi99    2019-5-18     ���calbraiton3.hͷ��ע��
******************************************************************************/

#ifndef _IMAGE_H
#define _IMAGE_H

#include "var.h"

extern bool g_isCameraWorking;


//ͼƬ��ȡ��
class ImageAcquisition:public QThread
{
	Q_OBJECT;
public:
	explicit ImageAcquisition(QObject* parent=0);
public:
	void run();
	void setCameraIndex(int index);
	void stream();
	void pause();
	void stop();
signals:
	void captured(QImage img,unsigned char * buffer);
	void captured2(QImage image);
private:
	int intCameraIndex;
	bool isCapturingImagesAllowed;
	bool isPauseCmdTriggered;
	QDir qdirImagesDirectory;
	bool isSavingMultipleImagesCmdTriggered;
	bool isTestingModeTriggered;
	static int INT_IMAGE_COUNT;
private slots:
	void allowSavingMultipleImages();
	void declineSavingMultipleImages();
	void tirggerTestingMode();
	void detriggerTestingMode();
};

#endif
