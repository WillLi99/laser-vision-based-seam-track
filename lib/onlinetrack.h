#pragma once
#ifndef _ONLINETRACK_H_
#define _ONLINETRACK_H_

#include "var.h"
#include "dip.h"
#include "motion.h"

extern bool testOrNot;
extern bool AutoTrackingTriggledFlag;


//ͼ������Զ�������
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
	bool dipSwitch;		//����ͼ�񿪹�
	QImage _image;
	bool _autoTrackingSwitch;
	Motion _rectificationObject;
	double _rho;

	int bufferNum;		//��Ҫ�����ƫ����
	double sensorFrameRate;
	double weldVelocity;
	double torchSensorDistance;

	vector<double> absOffsetList;	//����ƫ������
	vector<double> rOffsetList;		//��ƫƫ������
	double absOffset;	//����ƫ��
	double lAbsOffset;	//��һ������ƫ��
	double nROffset;	//��n����ƫƫ��
	double cROffset;	//��ǰ��ƫ���
	double tinyOffset;	//ƫ��̫С�������´ξ�ƫ

	int nROffsetCount;	//��ƫ��������
	int offsetCount;	//����ƫ��������
	
	QTimer sTimer;
	bool timeoutFlag;
	
signals:
	void sendDIPResult_triggered(QImage);
	void sendDIPCalculationResult_triggered(SeamInfo);
	void allowNextDIP_triggered();

private slots:
	void setAutoTrackingSwitch(SeamTrackParameter seamTrackingParas);
	void updateHorizontalCalibrationRatio(double rho);
	void stopAutoTracking();
	void timeOut200ms();
};

#endif