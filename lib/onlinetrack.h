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
				sumUpList			//�б����
				enableAutoTrack		//�����Զ����ٹ���
				disableAutoTrack	//�����Զ����ٹ���
				updateHorizontalCalibrationRatio	//����ˮƽ�궨����
				dip					//����ͼ����
				
				sendDIPResult_triggered				//����DIP����������һ������ͼ
				sendDIPCalculationResult_triggered	//����DIP������
				enableNextDIP_triggered				//��һ֡��ͼ����

				sendDIPResult_triggered
				sendDIPCalculationResult_triggered
				enableNextDIP_triggered
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    ���onlinetrack.hͷ��ע��
******************************************************************************/
#pragma once
#ifndef _ONLINETRACK_H_
#define _ONLINETRACK_H_

#include "var.h"
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
	double sumUpList(vector<double>lst,int start,int end);

private:
	bool isDIPAllowed;
	QImage _image;
	bool isAutoTrackTriggered;
	Motion _rectificationObject;
	Motion motionRectification;
	double dHorizontalCalibrationRatio;

	int intBufferNum;		//��Ҫ�����ƫ����
	double dSensorFrameRate;
	double dWeldSpeed;
	double dTorchSensorDistance;

	vector<double> absOffsetList;	//����ƫ������
	vector<double> rOffsetList;		//��ƫƫ������
	double dAbsOffset;	//����ƫ��
	double dLastAbsOffset;	//��һ������ƫ��
	double dNthROffset;	//��n����ƫƫ��
	double dCROffset;	//��ǰ��ƫ���
	double dTinyOffset;	//ƫ��̫С�������´ξ�ƫ

	int intROffsetCount;	//��ƫ��������
	int intOffsetCount;	//����ƫ��������
	
	QTimer sTimer;
	bool timeoutFlag;

	bool isTestModeTriggered;	
signals:
	void sendDIPResult_triggered(QImage);
	void sendDIPCalculationResult_triggered(SeamInfo);
	void enableNextDIP_triggered();

private slots:
	void enableAutoTrack(SeamTrackParameter spParameter);
	void disableAutoTrack();
	void syncHorizontalCalibrationRatio(double hcr);
	void timeOut200ms();
	void enableTestingMode();
	void disableTestingMode();
};

#endif