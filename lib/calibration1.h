/******************************************************************************
  File name: calibration1.h
  Author: WillLi99		Date:2019-5-18
  Description:
              ������SeamTrackingSys�࣬����̳���QWidget��������У�������й���
  Others: 
  Function List:
                SimpleCalibration	// ���ʼ��
                ~SimpleCalibration	// ������ʱ
				receiveBlockWidthPixel
				setDefaultConfiguration

				getBlockWidthPixel_triggered
				on_pushButtonGetBlockWidthPixel_triggered
				on_pushButtonCalculate_clicked 
				updateHorizontalCalibrationRatio_triggered
	            updateCalibrationState_triggered
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     ���calbraiton1.hͷ��ע��
		   WillLi99    2019-7-11     �޸���calibration1.ui�ļ�������������Ӧ�Ĺ���
******************************************************************************/

#pragma once
#ifndef Calibration1_H
#define Calibration1_H

#include "qwidget.h"
#include "ui_calibration1.h"

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
	double dBlockWidthPixel;
	double dBlockWidthmm;
	
	void setDefaultConfiguration();
private slots:
	void on_pushButtonCalculate_clicked();
	void on_pushButtonGetBlockWidtPixel_clicked();
	void receiveBlockWidthPixel(double blockWidth);
signals:
	void updateHorizontalCalibrationRatio_triggered(double horizontalCalibratinRate);
	void updateCalibrationState_triggered();
	void getBlockWidthPixel_triggered();
};

#endif