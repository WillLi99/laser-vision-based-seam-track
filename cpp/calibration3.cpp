#include "calibration3.h"

FittingCalibration::FittingCalibration(QWidget *parent)
{
	ui.setupUi(this);
	calibrationDataPath="..//images//calibdata//";
	validationDataPath="..//images//validata//";
	laserStripeDataPath="..//images//seamdata//";
	fileNo=0;
	iterationNum=0;
	iterationCount=0;
	current_mode=0x00;
	isAbruptStopTriggered=false;
	
	connect(&sTimer200ms, SIGNAL(timeout()), this, SLOT(timeOut200ms()));
}


void FittingCalibration::on_collectCalibrationDataPushButton_clicked()
{
	current_mode=0x01;
	double d=20;
	double zVelocity=1;		//�趨z����ƶ�����Ϊ1mmps
	double frameRate=5;
	iterationNum=(2*d/zVelocity)*frameRate;
	iterationCount=0;
	fileNo=0;
	sTimer200ms.start(200);

}
void FittingCalibration::on_collectValidationDataPushButton_clicked()
{
	
	current_mode=0x02;
	double d=20;
	double zVelocity=1;		//�趨z����ƶ�����Ϊ1mmps
	double frameRate=5;
	iterationNum=(2*d/zVelocity)*frameRate;
	fileNo=0;
	sTimer200ms.start(200);

}
void FittingCalibration::on_collectSeamLaserStripeDataPushButton_clicked()
{
	current_mode=0x04;
	double d=100;
	double yVelocity=0.5;	//�趨y����ƶ�����Ϊ0.5mmps
	double frameRate=5;
	iterationNum=(d/yVelocity)*frameRate;
	iterationCount=0;
	fileNo=0;
	sTimer200ms.start(200);

}
void FittingCalibration::on_returnSensorPushButton_clicked()
{
	current_mode=0x02;	//ȷ���Ѿ��ɼ��˱궨���ݺ���֤����
	calibrationMethod3MotionObject.yMove(10,-100,0.5,1);
}


void FittingCalibration::timeOut200ms()
{
	fileNo++;
	iterationCount++;
	stop200msTiming();
	//����궨����
	if((current_mode & 0x01)&& (!isAbruptStopTriggered))
	{
		QString fileName="..//images//calibdata//"+QString::number(fileNo)+".bmp";
		emit saveCalibrationData_triggered(fileName);

		if(iterationCount<iterationNum)
		{
			start200msTiming();
		}
		else
		{
			iterationCount=0;
			fileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�궨���ݲɼ���ϣ�"));
			msgbox.exec();
			return;
		}
	}

	//�����������
	if((current_mode & 0x02) && (!isAbruptStopTriggered))
	{
		QString fileName="..//images//validata//"+QString::number(fileNo)+".bmp";
		emit saveValidationData_triggered(fileName);

		if(iterationCount<iterationNum)
		{
			start200msTiming();
		}
		else
		{
			iterationCount=0;
			fileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�������ݲɼ���ϣ�"));
			msgbox.exec();
			return;
		}
	}

	if((current_mode & 0x04) && !(isAbruptStopTriggered))
	{
		QString fileName="..//images//seamdata//"+QString::number(fileNo)+".bmp";
		emit saveSeamLaserStripeData_triggered(fileName);

		if(iterationCount<iterationNum)
		{
			start200msTiming();
		}
		else
		{
			iterationCount=0;
			fileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�������ݲɼ���ϣ�"));
			msgbox.exec();
			return;
		}
	}
	
}

void FittingCalibration::start200msTiming()
{
	sTimer200ms.start(200);
	if(current_mode & 0x01)
		calibrationMethod3MotionObject.zMove(1.2,0.2,0.01,1);
	if(current_mode & 0x02)
		calibrationMethod3MotionObject.zMove(1.2,-0.2,0.01,1);
	if(current_mode & 0x04)
		calibrationMethod3MotionObject.yMove(0.8,0.1,0.01,1);
}

void FittingCalibration::stop200msTiming()
{
	sTimer200ms.stop();
}

void FittingCalibration::on_terminatePushButton_clicked()
{
	isAbruptStopTriggered=true;
	calibrationMethod3MotionObject.allStop();
	iterationNum=0;
	iterationCount=0;
	fileNo=0;
}

FittingCalibration::~FittingCalibration()
{

}