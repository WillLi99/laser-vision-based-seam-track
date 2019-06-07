/******************************************************************************
  File name: weld.h
  Author: WillLi99		Date:2019-5-21
  Description:
              ������Weld�࣬����̳���QWidget���������ú��Ӳ�����               
  Others: 
  Function List:
             Weld
			 ~Weld
			 on_confirmPushButton_clicked	//��麸�Ӳ����Ϸ��ԣ�����󴫸�mainwindow
			 on_cancelPushButton_clicked	//�˳�����
			 setDefaultParameters			//����Ĭ�ϵĺ��Ӳ���
			 checkParameters				//�������lineEdit�Ĳ����Ƿ����

			 updateWeldParameters_triggled	//���ݺ��Ӳ���
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    ���weld.hͷ��ע��
******************************************************************************/
#pragma once

#include "ui_weld.h"
#include "var.h"

//���Ӳ�����
class Weld :
	public QWidget
{	
	Q_OBJECT
public:
	explicit Weld(QWidget *parent=0);
	~Weld(void);
private:
	Ui_Weld  ui;
	double dWeldSpeed;
	double dWireRate;
	double dGasFlow;
	double dWeldVoltage;
	double dWeldCurrent;
	double dWeldDistance;
	WeldParameter wpWeldParameter;

	void setDefaultParameters();
	bool checkWeldParameters();

private slots:
	void on_confirmPushButton_clicked();
	void on_cancelPushButton_clicked();
signals:
	void updateWeldParameters_triggered(WeldParameter paramters);
};
