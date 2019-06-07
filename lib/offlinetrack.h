/******************************************************************************
  File name: offlinetrack.h
  Author: WillLi99		Date:2019-5-20
  Description:
              ������OfflineTrack�࣬����̳���QWidget���������ߺ���ģʽ����������
  Others: 
  Function List:
               OfflineTrack
			   ~OfflineTrack
			   start200msTiming		//200ms��ʱ
			   stop200msTiming		//200ms��ʱ��ֹ
			   start100msTimming	//100ms��ʱ
			   stop100msTiming		//100ms��ʱ��ֹ
			   start150msTiming		//150ms��ʱ
			   stop150msTiming		//150ms��ʱ
			   switchOnWeld			//��������
			   switchOffWeld		//�رպ���

			   synchoriWeldParameters				//ͬ�����Ӳ���
			   syncHorizontalCalibrationRatio		//ͬ��ˮƽ�궨����
			   on_pushButtonCollectData_clicked		//�ɼ�����
			   on_pushButtonAnalyzeData_clicked		//���ݴ���
			   on_pushButtonRecoverSensorPosition_clicked	//��λ������λ��
			   on_pushButtonSaveData_clicked		//��������
			   on_pushButtonClearData_clicked		//�������
			   on_pushButtonLoadData_clicked		//��������
			   on_pushButtonStart_clicked			//����
			   on_pushButtonStop_clicked			//ֹͣ
			   timeOut200ms							//��ʱ������һЩ����

			   saveImage_triggered
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-20     ���offlinetrack.hͷ��ע��
******************************************************************************/

#ifndef _OFFLINETRACK_H
#define _OFFLINETRACK_H

#include "ui_offlinetrack.h"
#include "var.h"
#include "motion.h"
#include "dip.h"
#include "dmc1000.h"

extern bool g_isCameraWorking;

class OfflineTrack :public QWidget
{	
	Q_OBJECT
public:
	explicit OfflineTrack(QWidget *parent=0);
	~OfflineTrack(void);
private:
	Ui_offlineTrack  ui;

	Motion motionOffline;

	double dWeldDistance;
	double weldSpeed;
	bool isWeldCmdTriggered;
	int cycleCount;
	int trackTime;
	int trackCount;
	int dataNum;
	QString filePath;
	QString fileName;

	double horizontalCalibrationRatio;
	double ro;
	double lnro;

	int dataCount;
	QTimer sTimer200ms;

	bool isDataCollectionFinished;
	bool isDataAnalysisFinished;
	bool haveDataOrNot;
	bool hasTrackStarted;
	bool notAllowTrack;

	QVector<double>absOffsetList;
	QVector<double>relOffsetList;
	
	void start200msTiming();
	void stop200msTiming();
	void start100msTiming();
	void stop100msTiming();
	void start150msTiming();
	void stop150msTiming();
	void switchOnWeld();
	void switchOffWeld();

private slots:
	void syncWeldParameters(WeldParameter parameters);
	void syncHorizontalCalibrationRatio(double horizontalCalibrationRatio);
	void on_pushButtonCollectData_clicked();
	void on_pushButtonAnalyzeData_clicked();
	void on_pushButtonRecoverSensorPosition_clicked();
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonClearData_clicked();
	void on_pushButtonSaveData_clicked();
	void on_pushButtonLoadData_clicked();
	void timeOut200ms();

signals:
	void saveImage_triggered(QString fileName);
};

#endif