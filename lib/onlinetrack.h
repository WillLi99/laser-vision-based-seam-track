/******************************************************************************
  File name: onlinetrack.h
  Author: WillLi99		Date:2019-5-21
  Description:
              ���ļ�������onlinetrack��. onlinetrack�ฺ������ͼ��������߸��١�            
  Others: 
  Function List:
                OnlineTrack			//��������
				~OnlineTrack		//
				start100msTiming	//����100ms��ʱ
				start200msTiming	//����200ms��ʱ
				timeOut200ms		//��ʱ�ﵽ200ms��Ĵ���
				stopTiming			//�رն�ʱ
				enableAutoTrack		//�����Զ����ٹ���
				disableAutoTrack	//�����Զ����ٹ���
				updateHorizontalCalibrationRatio	//����ˮƽ�궨����
				dip					//����ͼ����
				receiveReferenctOffset	//���ղο�ƫ��
				checkRecitificationAvailable		//����Ƿ���Ҫ���о�ƫ
				recitifyWeldTorch					//������ǹλ��
				moveWeldTorch						//�����ƶ���ǹ�����Ǿ�ƫ�ķ���
				recordSeamCenterCoordinate			//��¼��������λ�õľ�������
				recordWeldTorchCoordinate			//��¼��ǹλ��
				resetWeldTrackConfiguration			//����

				sendDIPResult_triggered				//����DIP����������һ������ͼ
				sendDIPCalculationResult_triggered	//����DIP������
				enableNextDIP_triggered				//��һ֡��ͼ����
				sendDIPResult_triggered
				sendDIPCalculationResult_triggered
				enableNextDIP_triggered
				tirggerTestingMode
				detirggerTestingMode

  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    ���onlinetrack.hͷ��ע��
		   WillLi99    2019-6-21    ���checkRecitificationAvailable��recordSeam
		                            CenterCoordinate�Ⱥ���. ɾ��sumuplist
		   WillLi99    2019-7-18    ���resetWeldTrackConfiguration
		   WillLi99    2019-7-22     ���referenceTrack������globalCoordinateTrack
		                            ���������ǽ���ص�ʵ�ַ�װ����
******************************************************************************/
#pragma once
#ifndef _ONLINETRACK_H_
#define _ONLINETRACK_H_

#include "common.h"
#include "dip.h"
#include "motion.h"

/******************************************************************************
  Name:OnlineTrack
  Description: ���߸��ٺ�����ͼ�������
  Used By: tracksys.h tracksys.cpp
  Others: 
******************************************************************************/
class OnlineTrack :
	public QThread
{
	Q_OBJECT
public:
	explicit OnlineTrack(QObject* parent=0);
	~OnlineTrack(void);
public:
	void run();
	void dip(QImage image);

private:
	void start100msTiming();
	void start200msTiming();
	void stopTiming();

	bool checkRecitificationAvailable(double deviation);
	void recitifyWeldTorch(double dSpeed,double dDistance,double dAccelerationTime,double dAccelerationMethod);
	void moveWeldTorch(double dSpeed,double dDistance,double dAccelerationTime,double dAccelerationMethod);
	void recordSeamCenterCoordinate(double coordinateX,double coordinateY);
	void recordWeldTorchCoordinate(double coordinateX,double coordinateY);
	void resetWeldTrackConfiguration();
	void referenceTrack(double dDeviation);
	void globalCoordinateTrack();

private:
	bool isDIPAllowed;
	QImage _image;
	int nthFrame;
	bool isAutoTrackTriggered;
	Motion _rectificationObject;
	Motion motionRectification;
	double dHorizontalCalibrationRatio;

	int intBufferNum;
	int intTotalTrackPeriodNum;
	double dSensorFrameRate;
	double dWeldSpeed;
	double dTrackDistance;
	double dTorchSensorDistance;

	double dOffset;	
	
	int intOffsetCount;
	int intNthPeriod;

	int intTrackMethod;
	double dReferenceOffset;
	double dLastPeriodRemain;
	
	CoordinateSet seamCenterCoordinateSet;
	CoordinateSet weldTorchCoordinateSet;
	double dLastWeldTorchCoordinateX;
	double dWeldTorchCoordinateX;
	double dWeldTorchCoordinateY;
	double dWeldSeamCoordinateX;
	double dWeldSeamCoordinateY;

	QTimer sTimer;
	bool timeoutFlag;

	bool isTestModeTriggered;	
signals:
	void sendDIPResult_triggered(QImage);
	void sendDIPCalculationResult_triggered(SeamInfo);
	void sendDIPResult2_triggered(QImage);
	void enableNextDIP_triggered();

private slots:
	void enableAutoTrack(SeamTrackParameter spParameter);
	void disableAutoTrack();
	void syncHorizontalCalibrationRatio(double dHorizontalCalibrationRatio);
	void timeOut200ms();
	void enableTestingMode();
	void disableTestingMode();
	void syncTrackMethod(int intTrackMethod);
	void receiveReferenceOffset(double dReferenceOffset);
	void tirggerTestingMode();
	void detriggerTestingMode();
};
#endif