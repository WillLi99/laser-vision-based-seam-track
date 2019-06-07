/******************************************************************************
  File name: calibration3.cpp
  Author: WillLi99		Date:2019-5-18
  Description:
              FittingCalibration���ʵ�֣�����̳���QWidget��������ϱ궨�еĹ��ڲɼ�
			  ԭʼͼ��ͼ��龫�ȵ�ͼ���ܡ����⣬Ҳ���Բɼ�����ļ�������ͼ�񣬱�������
			  ���к�������3d�ؽ���
  Others: 
  Function List:
                1. on_collectCalibrationDataPushButton_clicked	// �ɼ��궨ͼ��
                2. on_collectValidationDataPushButton_clicked	// �ɼ���֤ͼƬ
                3. on_collectSeamLaserStripeDataPushButton_clicked // �ɼ����켤��
				����ͼ
                4. on_returnSensorPushButton_clicked	// ʹ�������ָ�����ʼ�ɼ���
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
           WillLi99    2019-5-18     ���calbraiton3.cppͷ��ע�ͺ͸�������ͷ��ע��
******************************************************************************/
#include "calibration3.h"

/******************************************************************************
  Function:FittingCalibration
  Description:
              1. ��ʼ��
			  2. �����ź�-��
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
FittingCalibration::FittingCalibration(QWidget *parent)
{
	ui.setupUi(this);
	strCalibrationDataPath="..//images//calibdata//";
	strValidationDataPath="..//images//validata//";
	strLaserStripeDataPath="..//images//seamdata//";
	intFileNo=0;
	intIterationNum=0;
	intIterationNum=0;
	ucharCurrentMode=0x00;
	isEmergencyStopTriggered=false;
	
	connect(&qTimer200ms, SIGNAL(timeout()), this, SLOT(timeOut200ms()));
}

/******************************************************************************
  Function:FittingCalibration
  Description:
			��������Z�������Ժ㶨�ٶ�1m/s�ƶ�40mm,��������5frame/s�����ʲɼ�ͼ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonCollectCalibrationData_clicked()
{
	ucharCurrentMode=0x01;
	double dHalfDistance=20;
	double dZVelocity=1;	
	double dFrameRate=5;

	intIterationNum=(2*dHalfDistance/dZVelocity)*dFrameRate;
	intIterationCount=0;
	intFileNo=0;

	qTimer200ms.start(200);
}

/******************************************************************************
  Function:on_collectValidationDataPushButton_clicked
  Description:
			��������Z�Ḻ���Ժ㶨�ٶ�1m/s�ƶ�40mm,��������5frame/s�����ʲɼ�ͼ��
			Ҫȷ���ɼ��궨��������ɼ����ȼ�������
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonCollectValidationData_clicked()
{
	ucharCurrentMode=0x02;
	double dHalfDistance=20;
	double dZVelocity=1;	
	double dFrameRate=5;

	intIterationNum=(2*dHalfDistance/dZVelocity)*dFrameRate;
	intIterationCount=0;
	intFileNo=0;
	qTimer200ms.start(200);
}

/******************************************************************************
  Function:on_pushButtonCollectSeamLaserStripeData_clicked
  Description:
			��������Y�������Ժ㶨�ٶ�0.5mmps�ƶ�100mm,��������5frame/s�����ʲɼ�ͼ��
			Ҫȷ���ɼ��궨���ݺͲɼ����ȼ�����������ɼ����켤������ͼ��
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonCollectSeamLaserStripeData_clicked()
{
	ucharCurrentMode=0x04;
	double dHalfDistance;
	double yVelocity=0.5;	//�趨y����ƶ�����Ϊ0.5mmps
	double frameRate=5;

	intIterationNum=(2*dHalfDistance/yVelocity)*frameRate;
	intIterationNum=0;
	intFileNo=0;
	qTimer200ms.start(200);
}

/******************************************************************************
  Function:on_pushButtonRecoverSensorPosition_clicked
  Description:
			�ָ���������λ�ã���������ָ����ɼ����켤������ͼ��֮ǰ
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonRecoverSensorPosition_clicked()
{
	ucharCurrentMode=0x02;	//ȷ���Ѿ��ɼ��˱궨���ݺ���֤����
	motionCalibration3.yMove(10,-100,0.5,1);
}

/******************************************************************************
  Function:timeOut200ms
  Description:
			200ms��ʱ����ʱ������һЩ����
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::timeOut200ms()
{
	intFileNo++;
	intIterationNum++;

	stop200msTiming();

	//**�����������б궨������*//
	if((ucharCurrentMode & 0x01)&& (!isEmergencyStopTriggered))
	{
		QString fileName="..//images//calibdata//"+QString::number(intFileNo)+".bmp";
		emit saveCalibrationData_triggered(fileName);

		if(intIterationNum<intIterationNum)
		{
			start200msTiming();
		}
		else
		{
			intIterationNum=0;
			intFileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�궨���ݲɼ���ϣ�"));
			msgbox.exec();
			return;
		}
	}

	//**�����������о��ȼ��������*//
	if((ucharCurrentMode & 0x02) && (!isEmergencyStopTriggered))
	{
		QString fileName="..//images//validata//"+QString::number(intFileNo)+".bmp";
		emit saveValidationData_triggered(fileName);

		if(intIterationNum<intIterationNum)
		{
			start200msTiming();
		}
		else
		{
			intIterationNum=0;
			intFileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�������ݲɼ���ϣ�"));
			msgbox.exec();
			return;
		}
	}

	//**���溸�켤������ͼ��*//
	if((ucharCurrentMode & 0x04) && !(isEmergencyStopTriggered))
	{
		QString fileName="..//images//seamdata//"+QString::number(intFileNo)+".bmp";
		emit saveSeamLaserStripeData_triggered(fileName);

		if(intIterationNum<intIterationNum)
		{
			start200msTiming();
		}
		else
		{
			intIterationNum=0;
			intFileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�������ݲɼ���ϣ�"));
			msgbox.exec();
			return;
		}
	}
	
}

/******************************************************************************
  Function:start200msTiming
  Description:
			����200ms��ʱ
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::start200msTiming()
{
	qTimer200ms.start(200);
	if(ucharCurrentMode & 0x01)
		motionCalibration3.zMove(1.2,0.2,0.01,1);
	if(ucharCurrentMode & 0x02)
		motionCalibration3.zMove(1.2,-0.2,0.01,1);
	if(ucharCurrentMode & 0x04)
		motionCalibration3.yMove(0.8,0.1,0.01,1);
}

/******************************************************************************
  Function:stop200msTiming
  Description:
			ֹͣ200ms��ʱ
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::stop200msTiming()
{
	qTimer200ms.stop();
}

/******************************************************************************
  Function:on_pushButtonTerminate_clicked
  Description:
			����ֹͣ
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonTerminate_clicked()
{
	isEmergencyStopTriggered=true;
	motionCalibration3.allStop();
	intIterationNum=0;
	intIterationNum=0;
	intFileNo=0;
}

/******************************************************************************
  Function:FittingCalibration
  Description: ��������
  Calls: 
  Called By: FittingCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
FittingCalibration::~FittingCalibration()
{

}