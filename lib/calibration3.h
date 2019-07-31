/******************************************************************************
  File name: calibration3.h
  Author: WillLi99		Date:2019-5-18
  Description:
              ������FittingCalibration�࣬����̳���QWidget��������ϱ궨�еĹ��ڲɼ�
			  ԭʼͼ��ͼ��龫�ȵ�ͼ���ܡ����⣬Ҳ���Բɼ�����ļ�������ͼ�񣬱�������
			  ���к�������3d�ؽ���
  Others: 
  Function List:
                1. on_pushButtonCollectCalibrationData_clicked	// �ɼ��궨ͼ��
                2. on_pushButtonCollectValidationData_clicked	// �ɼ���֤ͼƬ
                3. on_pushButtonCollectSeamLaserStripeData_clicked // �ɼ����켤��
				����ͼ
                4. on_pushButtonRecoverSensorPosition_clicked	// ʹ�������ָ�����ʼ�ɼ���
				λ��
                5. timeOut200ms();	// ��ʱ200ms
                6. on_terminatePushButton_clicked();	// ����ֹͣ
                7. start200msTiming();	// ������ʱ200ms
                8. stop200msTiming();	// ֹͣ��ʱ200ms
                9. saveCalibrationData_triggered	// �ź�
                10. saveValidationData_triggered	// �ź�
                11. saveSeamLaserStripeData_triggered	// �ź�        
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     ���calbraiton3.hͷ��ע��
******************************************************************************/

#pragma once
#ifndef Calibration3_H
#define Calibration3_H
#include "common.h"
#include "ui_calibration3.h"
#include "motion.h"

class FittingCalibration:public QWidget
{
	Q_OBJECT
public:
	explicit FittingCalibration(QWidget *parent=0);
	~FittingCalibration();

private:
	Ui_Calibration3 ui;
	Motion motionCalibration3;
	String strCalibrationDataPath;
	String strValidationDataPath;
	String strLaserStripeDataPath;
	int intFileNo;
	int intIterationNum;
	int intIterationCount;
	unsigned char ucharCurrentMode;

	QTimer qTimer200ms;
	bool isEmergencyStopTriggered;

private slots:
	void on_pushButtonCollectCalibrationData_clicked();
	void on_pushButtonCollectValidationData_clicked();
	void on_pushButtonCollectSeamLaserStripeData_clicked();
	void on_pushButtonRecoverSensorPosition_clicked();
	void on_pushButtonTerminate_clicked();
	void timeOut200ms();

private:
	void start200msTiming();
	void stop200msTiming();

signals:
	void saveCalibrationData_triggered(QString fileName);
	void saveValidationData_triggered(QString fileName);
	void saveSeamLaserStripeData_triggered(QString fileName);
};

#endif