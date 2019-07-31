/******************************************************************************
  File name: manual.h
  Author: WillLi99		Date:2019-5-19
  Description:
              ������Manual��,����̳���QWidget,�������˶�ƽ̨���ֶ�����.�ڱ������ֵ�
			  ��׺��1�ı�ʾ�����˶�ģʽ�ı���;�ڱ������ֵĺ�׺��2�ı�ʾ�����˶�ģʽ�ı���;
  Others: 
  Function List:
                Manual
				~Manual
				initializeConfiguration		//��ʼ���˶�����
				fillDefaultConfiguration	//���Ĭ�ϵ��˶�����
				updateConfiguration			//�����˶�����
				switchOnWeld				//������
				switchOffWeld				//�غ���
				on_pushButtonSwitchOnWeld_clicked
				on_pushButtonSwitchOffWeld_clicked
				on_radioButtonContinousMotion_toggled	//�л��������˶�ģʽ
				on_radioButtonFixedMotion_toggled		//�л����̶��˶�ģʽ
				on_pushButtonStart_clicked				//����
				on_pushButtonStop_clicked				//�ƶ�
				on_pushButtonRecoverWeldTorch_clicked	//��ǹ����ԭ��λ��
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-19     ���manual.hͷ��ע��
		   WillLi99	   2019-6-6      �޸��˲��ֱ���������
******************************************************************************/

#ifndef _MANUAL_H
#define _MANUAL_H

#include "common.h"
#include "ui_manual.h"
#include "motion.h"

//�ֶ�������
class Manual:public QWidget
{
	Q_OBJECT

public:
	explicit Manual(QWidget* parent=0);
	~Manual();

public:
	Motion* motionManual;

private:
	Ui_Manual ui;

	/*����ģʽ�µı���*/
	int intAxleMode1;				// ���.0-x;2-z;3-y
	int intRotationDirectionMode1;	// ���ƶ�����.1-����;2-����
	int intMotionMethodMode1;		// �˶�ģʽ.1-�����˶�;2-�����˶�
	int intAccelerationMethodMode1;	// ����ģʽ.1-���μ���;2-S�ͼ���

	double dMoveDistanceMode1;	
	double dMaxSpeedMode1;	
	double dAccelerationTimeMode1;	

	/*����ģʽ�µı���*/
	double dXMoveDistanceMode2;	
	double dXMaxSpeedMode2;	
	double dXAccelerationTimeMode2;	
	int intXAccelerationMethodMode2;	// ����ģʽ.1-���μ���;2-S�ͼ���

	double dYMoveDistanceMode2;	
	double dYMaxSpeedMode2;
	double dYAccelerationTimeMode2;
	int intYAccelerationMethodMode2;	// ����ģʽ.1-���μ���;2-S�ͼ���

	double dZMoveDistanceMode2;
	double dZMaxSpeedMode2;	
	double dZAccelerationTimeMode2;
	int intZAccelerationMethodMode2;	// ����ģʽ.1-���μ���;2-S�ͼ���


	/*���Ӳ���*/
	double dWeldVoltage;
	double dWeldCurrent;
	double dWireRate;
	double dGasFlow;
	int intWeldStartCondition;		// ���ӿ�������.1-�ֶ�����;2-�˶�ʱ�Զ�����

	/*����ģʽ*/
	int intControlMode;				// �˶�ģʽ.1-����ģʽ;2-����ģʽ
	
	bool isCheckQualified;
	bool isWeldSwitchedOn;
	
	void initializeConfiguration();
	void fillDefaultConfiguration();
	void checkConfiguration();
	void switchOnWeld();
	void switchOffWeld();

private slots:
	void on_pushButtonSwitchOnWeld_clicked();
	void on_pushButtonSwitchOffWeld_clicked();
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonRecoverWeldTorch_clicked();
	void on_radioButtonContinousMotion_toggled();
	void on_radioButtonFixedMotion_toggled();
};

#endif