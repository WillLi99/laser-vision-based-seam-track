/*****************************************************************************
Copyright: 
Author: Li Yangjin
Date: 2018-8-21
Description:Motion���������ж�ƽ̨�����ֶ����ƣ�ƽ̨������ϵ����ͼ��ʾ
                      ^z+
                      |
                      |-----> y+
                     /
                    /
                   <x+

*****************************************************************************/

#include "motion.h"
#include "manual.h"
#include "dmc1000.h"

Manual::Manual(QWidget *parent)
{
	ui.setupUi(this);
	manualMotionObject=new Motion;

	checkPass=0;
	initializeParameters();		//��ʼ������manuanlControl ui�������
	fillDefaultParamters();		//���
	manualMotionObject->start();
	d1000_out_bit(4,1);		//��ʼ������Ϊ�ߵ�ƽ,�Ͽ��̵���
}

Manual::~Manual()
{
	
}

/*****************************************************************************
Function:void Manual::on_startPushButton_clicked()
Description:��Ӧ�ֶ����ƽ���ԡ���ʼ����ť�ĵ���¼�
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_startPushButton_clicked()
{
	updateParameters();			//�������ݣ��������LineEdit������


	//�ֵ���ģʽ���Ƕ���ģʽ����ͬ����
	if(controlMode==0)
	{
		if(!checkPass)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("�������ó���δ�ܿ�ʼ�˶�"));
			msgBox.exec();
			return;
		}

		//�����˶�
		if (ui.cntnsModeRadioButton->isChecked())
		{
			if (rotationDirectionA == 1)	maxSpeedA = abs(maxSpeedA);
			else if (rotationDirectionA == 2)	maxSpeedA = -abs(maxSpeedA);

			switch (axisA)
			{
			case 0:manualMotionObject->xRun(maxSpeedA,accTimeA,accelerationModeA);break;
			case 2:manualMotionObject->zRun(maxSpeedA,accTimeA,accelerationModeA);break;
			case 3:manualMotionObject->yRun(maxSpeedA,accTimeA,accelerationModeA);break;
			}
		}

		//�����˶�
		if (ui.fixedModeRadioButton->isChecked())
		{
			//����
			if (rotationDirectionA == 1)	moveDistanceA = abs(moveDistanceA);//����
			else if (rotationDirectionA == 2)	moveDistanceA = -abs(moveDistanceA);//����

			switch (axisA)
			{
			case 0:manualMotionObject->xMove(maxSpeedA,moveDistanceA,accTimeA,accelerationModeA);break;
			case 2:manualMotionObject->zMove(maxSpeedA,moveDistanceA,accTimeA,accelerationModeA);break;
			case 3:manualMotionObject->yMove(maxSpeedA,moveDistanceA,accTimeA,accelerationModeA);break;
			}
		}
	}
	else if(controlMode==1)
	{
		if(!checkPass)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("�������ó���δ�ܿ�ʼ�˶�"));
			msgBox.exec();
			return;
		}

		
		manualMotionObject->xMove(xMaxSpeedB,xMoveDistanceB,xAccTimeB,xAccModeB);
		manualMotionObject->yMove(yMaxSpeedB,yMoveDistanceB,yAccTimeB,yAccModeB);
		manualMotionObject->zMove(zMaxSpeedB,zMoveDistanceB,zAccTimeB,zAccModeB);

	}
	
}


/*****************************************************************************
Function:voidManual::on_stopPushButton_clicked()
Description:��Ӧ�ֶ����ƽ���ԡ�ֹͣ����ť�ĵ���¼�
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_stopPushButton_clicked()
{
	manualMotionObject->allStop();
}

/*****************************************************************************
Function:Manual::on_returnOriginPushButton_clicked()
Description:��Ӧ�ֶ����ƽ���ԡ�����ԭ�㡱��ť�ĵ���¼�
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_returnOriginPushButton_clicked()
{
	qDebug()<<"on_returnOriginPushButton_clicked"<<endl;
}


/*****************************************************************************
Function:void Manual::_fillDefaultParamters()
Description:��ʼ��Ĭ�ϲ���
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::initializeParameters()
{

	//*************************����ģʽ�µ�Ĭ�ϲ����趨**************************//
	axisA=3;					//Ĭ�������3,��Ӧѡ��X��
	rotationDirectionA=1;		//Ĭ������
	motionModeA=2;			//Ĭ�϶����˶�
	accelerationModeA=1;		//Ĭ�����μ���
	moveDistanceA=10.0;		//Ĭ���ƶ�����10mm
	maxSpeedA=5.0;		//Ĭ������ٶ�5mm/s	
	accTimeA=0.2;				//Ĭ�ϼ���ʱ��0.5s

	//*********************************************************************//

	//*************************����ģʽ�µ�Ĭ�ϲ����趨**************************//
	xMoveDistanceB=0;
	xMaxSpeedB=5;
	xAccTimeB=0.1;
	xAccModeB=0;

	yMoveDistanceB=0;
	yMaxSpeedB=5;
	yAccTimeB=0.1;
	yAccModeB=0;

	zMoveDistanceB=0;
	zMaxSpeedB=5;
	zAccTimeB=0.1;
	zAccModeB=0;
	//************************************************************************//

	//*******************************����Ĭ�ϲ����趨***************************//
	weldVoltage=25;
	weldCurrent=100;
	feedRate=2;
	gasFlow=0.5;
	weldSwitchMode=0;		//=0ΪĬ���ֶ��򿪺���
	
	//************************************************************************//

	//*******************************����ģʽ˫��ģʽ�趨***************************//
	controlMode=0;
	//************************************************************************//

	//����_weldingSwitchOnFlagΪfalse
	weldSwitchedOnFlag=false;
}



/*****************************************************************************
Function:void Manual::_fillDefaultParamters()
Description:������lineEdit�������Ĭ�ϲ�����
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::fillDefaultParamters()
{
	QString str1,str2,str3,str4;
	//******************************��䵥��ģʽ��GroupBox�Ĳ���**************//
	
	str1 = QString::number((double)moveDistanceA);
	str2 = QString::number((double)maxSpeedA);
	str3 = QString::number((double)accTimeA);

	ui.distanceSettinglineEdit->setText(str1);
	ui.speedSettingLineEdit->setText(str2);
	ui.acceleratTimeTextLabel->setText(str3);

	ui.yRadioButton->setChecked(true);
	ui.CWRadioButton->setChecked(true);
	ui.fixedModeRadioButton->setChecked(true);
	ui.trapeModeRadionButton->setChecked(true);

	//************************************************************************//

	//******************************������ģʽ��GroupBox�Ĳ���**************//

	str1 = QString::number((double)xMoveDistanceB);
	str2 = QString::number((double)xMaxSpeedB);
	str3 = QString::number((double)xAccTimeB);
	ui.xDistanceSettingLineEdit->setText(str1);
	ui.xSpeedSettingLineEdit->setText(str2);
	ui.xAcceleratTimeLineEdit->setText(str3);
	ui.xTrapeModeRadionButton->setChecked(true);

	str1 = QString::number((double)yMoveDistanceB);
	str2 = QString::number((double)yMaxSpeedB);
	str3 = QString::number((double)yAccTimeB);
	ui.yDistanceSettingLineEdit->setText(str1);
	ui.ySpeedSettingLineEdit->setText(str2);
	ui.yAcceleratTimeLineEdit->setText(str3);
	ui.yTrapeModeRadionButton->setChecked(true);

	str1 = QString::number((double)zMoveDistanceB);
	str2 = QString::number((double)zMaxSpeedB);
	str3 = QString::number((double)zAccTimeB);
	ui.zDistanceSettingLineEdit->setText(str1);
	ui.zSpeedSettingLineEdit->setText(str2);
	ui.zAcceleratTimeLineEdit->setText(str3);
	ui.zTrapeModeRadionButton->setChecked(true);

	//************************************************************************//

	//******************************��亸�Ӳ�������groupbox������*****************//
	str1=QString::number((double)weldVoltage);
	str2=QString::number((double)weldCurrent);
	str3=QString::number((double)feedRate);
	str4=QString::number((double)gasFlow);
	ui.weldingVoltageSettingLineEdit->setText(str1);
	ui.weldingCurrentSettingLineEdit->setText(str2);
	ui.feedRateSettingLineEdit->setText(str3);
	ui.gasFlowSettingLineEdit->setText(str4);
	ui.manuallySwitchOnWeldingProcessRadioButton->setChecked(true);
	ui.automaticallySwitchOnWeldingProcessRadioButton->setChecked(false);

	//************************************************************************//

	ui.controlMode1RadioButton->setChecked(true);

}

/*****************************************************************************
Function:void Manual::_updateParameters()
Description:�����˶�����
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::updateParameters()
{
	QString str1,str2,str3,str4;
	double temp1,temp2,temp3,temp4;
	bool conversionState1=true;
	bool conversionState2=true;
	bool conversionState3=true;


	if(ui.controlMode1RadioButton->isChecked())//���µ���ģʽ�²���
	{
		//******************************���µ���ģʽ��GroupBox�Ĳ���**************//
	
	//���
	if (ui.xRadioButton->isChecked())			axisA = 0;
	else if (ui.yRadioButton->isChecked())		axisA = 3;
	else if (ui.zRadioButton->isChecked())		axisA = 2;
	
	//����
	if (ui.CWRadioButton->isChecked())			rotationDirectionA = 1;	//����
	else if (ui.CCWRatdioButton->isChecked())	rotationDirectionA = 2;	//����
	
	//�˶�ģʽ
	if (ui.cntnsModeRadioButton->isChecked())		motionModeA = 1;		//����ģʽ
	else if (ui.fixedModeRadioButton->isChecked())	motionModeA = 2;		//�����˶�
	
	//����ģʽ
	if (ui.trapeModeRadionButton->isChecked())		accelerationModeA = 0;	//���μ���
	else if (ui.sigmoidRadioButton->isChecked())	accelerationModeA = 1;	//S����

	//���롢�ٶȡ�����ʱ���
	if(ui.distanceSettinglineEdit->isEnabled())	//����ģʽ�£�distanceSettingLineEdit������
	{	
		str1 = ui.distanceSettinglineEdit->text();
		temp1 = str1.toDouble();
	}
	else
	{
		str1="";
		temp1=-1;
	}
	str2 = ui.speedSettingLineEdit->text();
	str3 = ui.acceleratTimeTextLabel->text();
	
	temp2 = str2.toDouble();
	temp3 = str3.toDouble();
	
	if (temp1 == 0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("�ƶ�������������������"));
		msgbox.exec();
		checkPass = 0;		//��ʽ��鲻ͨ��
		return;
	}
	
	if (temp2 == 0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("����ٶ���������������"));
		msgbox.exec();
		checkPass = 0;		//��ʽ��鲻ͨ��
		return;
	}
	
	if (temp3 == 0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("����ʱ����������������"));
		msgbox.exec();
		checkPass = 0;
		return;
	}
	
	moveDistanceA = temp1;
	maxSpeedA = temp2;
	accTimeA = temp3;
	
	}
	else if(ui.controlMode2RadioButton->isChecked())//���¶���ģʽ�²���
	{
		
		//********************************X��*********************************//
		if(ui.xTrapeModeRadionButton->isChecked()) xAccModeB=0;
		else if(ui.xSigmoidRadioButton->isChecked()) xAccModeB=1;

		str1=ui.xDistanceSettingLineEdit->text();
		str2=ui.xSpeedSettingLineEdit->text();
		str3=ui.xAcceleratTimeLineEdit->text();
		
		temp1=str1.toDouble(&conversionState1);
		temp2 = str2.toDouble(&conversionState2);
		temp3 = str3.toDouble(&conversionState3);

		if(temp1==0.0 && conversionState1==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�ƶ�������������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		if(temp2==0.0 && conversionState2==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("����ٶ���������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		if(temp3==0.0 && conversionState3==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("����ʱ����������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		xMoveDistanceB=temp1;
		xMaxSpeedB=temp2;
		xAccTimeB=temp3;

		//***************************************************************************//

		//**********************************Y��**************************************//
		if(ui.yTrapeModeRadionButton->isChecked()) yAccModeB=0;
		else if(ui.ySigmoidRadioButton->isChecked()) yAccModeB=1;

		str1=ui.yDistanceSettingLineEdit->text();
		str2=ui.ySpeedSettingLineEdit->text();
		str3=ui.yAcceleratTimeLineEdit->text();

		temp1=str1.toDouble(&conversionState1);
		temp2 = str2.toDouble(&conversionState2);
		temp3 = str3.toDouble(&conversionState3);

		if(temp1==0.0 && conversionState1==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�ƶ�������������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		if(temp2==0.0 && conversionState2 == false) 
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("����ٶ���������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		if(temp3==0.0 && conversionState3==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("����ʱ����������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		yMoveDistanceB=temp1;
		yMaxSpeedB=temp2;
		yAccTimeB=temp3;

		//***************************************************************************//


		//**********************************Z��**************************************//
		if(ui.zTrapeModeRadionButton->isChecked()) zAccModeB=0;
		else if(ui.zSigmoidRadioButton->isChecked()) zAccModeB=1;

		str1=ui.zDistanceSettingLineEdit->text();
		str2=ui.zSpeedSettingLineEdit->text();
		str3=ui.zAcceleratTimeLineEdit->text();

		temp1 = str1.toDouble(&conversionState1);
		temp2 = str2.toDouble(&conversionState2);
		temp3 = str3.toDouble(&conversionState3);

		if(temp1==0.0 && conversionState1==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("�ƶ�������������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		if(temp2==0.0 && conversionState2==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("����ٶ���������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		if(temp3==0.0 &&conversionState3==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("����ʱ����������������"));
			msgbox.exec();
			checkPass = 0;		//��ʽ��鲻ͨ��
			return;
		}

		zMoveDistanceB=temp1;
		zMaxSpeedB=temp2;
		zAccTimeB=temp3;

		//***************************************************************************//


	}

	//****************************���º��Ӳ���GroupBox�Ĳ���*************************//
	str1=ui.weldingVoltageSettingLineEdit->text();
	str2=ui.weldingCurrentSettingLineEdit->text();
	str3=ui.feedRateSettingLineEdit->text();
	str4=ui.gasFlowSettingLineEdit->text();

	temp1=str1.toDouble();
	temp2=str2.toDouble();
	temp3=str3.toDouble();
	temp4=str4.toDouble();

	if(ui.manuallySwitchOnWeldingProcessRadioButton->isChecked()) weldSwitchMode=0;
	else if(ui.automaticallySwitchOnWeldingProcessRadioButton->isChecked()) weldSwitchMode=1;

	//���
	if(temp1==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӵ�ѹ��������������"));
		msgbox.exec();
		checkPass = 0;		//��ʽ��鲻ͨ��
		return;
	}

	if(temp2==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӵ�����������������"));
		msgbox.exec();
		checkPass = 0;		//��ʽ��鲻ͨ��
		return;
	}

	if(temp3==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("��˿�ٶ���������������"));
		msgbox.exec();
		checkPass = 0;		//��ʽ��鲻ͨ��
		return;
	}

	if(temp4==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("������������������������"));
		msgbox.exec();
		checkPass = 0;		//��ʽ��鲻ͨ��
		return;
	}

	weldVoltage=temp1;
	weldCurrent=temp2;
	feedRate=temp3;
	gasFlow=temp4;

	//********************************************************************************//

	//**********************************���¿���ģʽ***********************************//
	if(ui.controlMode1RadioButton->isChecked())	controlMode=0;
	else if(ui.controlMode2RadioButton->isChecked()) controlMode=1;
	//**********************************************************************************//

	checkPass = 1;		//���ͨ��

}

void Manual::on_cntnsModeRadioButton_toggled()
{
	ui.distanceSettinglineEdit->setEnabled(false);
}

void Manual::on_fixedModeRadioButton_toggled()
{
	ui.distanceSettinglineEdit->setEnabled(true);
}



/*****************************************************************************
Function:_switchOnWelding
Description:�������ӡ���Ϊͨ�������4�����˼̵�����������ͨ�����Ƽ̵���4�����ƺ��ӵ�����
��ƽ�ɸ߱�ͣ���ʼ���ӡ�
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::switchOnWeld()
{
	d1000_out_bit(4,0);
}


/*****************************************************************************
Function:_switchOffWelding
Description:��ƽ�ɵ͵��ߣ��رպ��ӡ�
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::switchOffWeld()
{
	d1000_out_bit(4,1);
}


/*****************************************************************************
Function:on_switchOnWeldingProcessPushButton_clicked
Description:��Ӧ����ʼ���ӡ�pushButton�ĵ���¼���
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_switchOnWeldPushButton_clicked()
{
	switchOnWeld();
}


/*****************************************************************************
Function:on_switchOffWeldingProcessPushButton_clicked
Description:��Ӧ���رպ��ӡ�pushButton�ĵ���¼���
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_switchOffWeldPushButton_clicked()
{
	switchOffWeld();
}
