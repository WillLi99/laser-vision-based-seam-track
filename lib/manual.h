#ifndef _MANUAL_H
#define _MANUAL_H

#include "var.h"
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
	Motion* manualMotionObject;	//�˶����ƶ���

private:
	Ui_Manual ui;			//����

	//���²��������У���׺ΪA��ʾ��Ӧ����ģʽ�µı�������׺ΪB��ʾ��Ӧ����ģʽ�µı���

	//***************����ģʽ�µĲ���******************************//
	int axisA;					//��ѡ��
	int rotationDirectionA;		//ת��ѡ��1����2-����
	int motionModeA;			//�˶�ģʽ. 1-����ģʽ��2-����ģʽ
	int accelerationModeA;		//����ģʽ����Ϊ0-���μ��ٺ�1-S�ͼ���

	double moveDistanceA;		//�ƶ�����	��λmm
	double maxSpeedA;			//����ٶ�	��λ��mm/s
	double accTimeA;			//����ʱ��
	//*************************************************************//

	//***************����ģʽ�µĲ���********************************//
	double xMoveDistanceB;	//x�ƶ�����	��λmm
	double xMaxSpeedB;		//x����ٶ�	��λ��mm/s
	double xAccTimeB;		//x����ʱ��	��λ s
	int xAccModeB;			//x����ģʽ	0-���μ���;1-S�ͼ���

	double yMoveDistanceB;	//y�ƶ�����	��λmm
	double yMaxSpeedB;		//y����ٶ�	��λ��mm/s
	double yAccTimeB;		//y����ʱ��	��λ s
	int yAccModeB;			//y����ģʽ	0-���μ���;1-S�ͼ���

	double zMoveDistanceB;	//z�ƶ�����	��λmm
	double zMaxSpeedB;		//z����ٶ�	��λ��mm/s
	double zAccTimeB;		//z����ʱ��	��λ s
	int zAccModeB;			//z����ģʽ	0-���μ���;1-S�ͼ���

	//*************************************************************//

	//**********************���Ӳ���********************************//
	double weldVoltage;			//���ӵ�ѹ	��λV
	double weldCurrent;			//���ӵ���	��λA
	double feedRate;			//��˿�ٶ�	��λm/min
	double gasFlow;				//����������	��λL/min
	int weldSwitchMode;			//����������ģʽ 0-�ֶ�������1-���˶�����

	//****************************����ģʽ****************************//
	int controlMode;			//����ģʽ��=0����ģʽ��=1˫��ģʽ
	//*****************************************************************//
	bool checkPass;				//��⣬�����1��ʾͨ����0��ʾ��ͨ��
	bool weldSwitchedOnFlag;	//����������־


	void initializeParameters();
	void fillDefaultParamters();
	void updateParameters();
	void switchOnWeld();
	void switchOffWeld();

private slots:
	void on_switchOnWeldPushButton_clicked();
	void on_switchOffWeldPushButton_clicked();
	void on_startPushButton_clicked();
	void on_stopPushButton_clicked();
	void on_returnOriginPushButton_clicked();
	void on_cntnsModeRadioButton_toggled();
	void on_fixedModeRadioButton_toggled();



};

#endif