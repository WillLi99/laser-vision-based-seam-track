/******************************************************************************
  File name: calibration1.h
  Author: WillLi99		Date:2019-5-18
  Description:
              ������SeamTrackingSys�࣬����̳���QWidget��������У�������й���
  Others: 
  Function List:
                1. SimpleCalibration	// ���ʼ��
                2. ~SimpleCalibration	// ������ʱ
	            3. on_confirmPushButton_clicked // ����rho--��ˮƽ�궨����
	            4. updateHorizontalCalibrationRatio_triggered 
	            
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     ���calbraiton1.hͷ��ע��
******************************************************************************/

#pragma once
#ifndef Calibration1_H
#define Calibration1_H

#include "qwidget.h"
#include "ui_calibration1.h"

extern bool hasCalibrated;

class SimpleCalibration :
	public QWidget
{	
	Q_OBJECT
public:
	explicit SimpleCalibration(QWidget *parent=0);
	~SimpleCalibration(void);
private:
	Ui_Calibration1 ui;
	double dHorizontalCalibrationRatio;
private slots:
	void on_pushButtonConfirm_clicked();
signals:
	void updateHorizontalCalibrationRatio_triggered(double hcr);
	void updateCalibrationState_triggered();
};

#endif