#include "weld.h"


Weld::Weld(QWidget *parent)
{
	ui.setupUi(this);
	setDefaultParameters();
}


Weld::~Weld(void)
{
}

/***********************************************************************
@��������on_confirmPushButton_clicked
@��������
@����ֵ����
@���ܣ���Ӧ���Ӳ������ý���ġ�ȷ�ϡ���ť���������Ӳ���������UI
@�޸���Ϣ��
***********************************************************************/
void Weld::on_confirmPushButton_clicked()
{
	int checkResult=checkParameters();
	
	if(checkResult==1)	//���ͨ����emit���Ӳ���
	{
		emit updateWeldParameters_triggled(parameters);
		close();
	}
	else if(checkResult==-1)
	{
		return;
	}
}

void Weld::on_cancelPushButton_clicked()
{
	close();
}

/***********************************************************************
@��������setDefaultParameters
@��������
@����ֵ����
@���ܣ�����Ĭ�ϵĺ��Ӳ��������������Ӧ��LineEdit
@�޸���Ϣ��
***********************************************************************/
void Weld::setDefaultParameters()
{
	weldSpeed=2.0;
	wireFeedRate=2.0;
	sheildingGasFlow=0.5;
	weldVoltage=25.0;
	weldCurrent=100.0;

	ui.weldSpeedLineEdit->setText(QString::number(weldSpeed));
	ui.wireFeedRateLineEdit->setText(QString::number(wireFeedRate));
	ui.sheildingGasFlowLineEdit->setText(QString::number(sheildingGasFlow));
	ui.weldVoltageLineEdit->setText(QString::number(weldVoltage));
	ui.weldCurrentLineEdit->setText(QString::number(weldCurrent));
}

/***********************************************************************
@��������checkParameters
@��������
@����ֵ������1��ʾ���ͨ��������-1��ʾ��鲻ͨ��
@���ܣ��������Ƿ�ϸ����ϸ�ʹ�����Ӧ����ֵ�����ϸ��򵯳���������
@�޸���Ϣ��
***********************************************************************/
int Weld::checkParameters()
{
	QString str1,str2,str3,str4,str5,str6;
	
	str1=ui.weldSpeedLineEdit->text();
	str2=ui.wireFeedRateLineEdit->text();
	str3=ui.sheildingGasFlowLineEdit->text();
	str4=ui.weldVoltageLineEdit->text();
	str5=ui.weldCurrentLineEdit->text();
	str6=ui.trackingDistanceLineEdit->text();
	
	weldSpeed=str1.toDouble();
	wireFeedRate=str2.toDouble();
	sheildingGasFlow=str3.toDouble();
	weldVoltage=str4.toDouble();
	weldCurrent=str5.toDouble();
	trackingDistance=str6.toDouble();

	//���
	if(weldSpeed==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("�����ٶȸ�ʽ����ȷ���������"));
		msgbox.exec();
		return -1;
	}

	if(wireFeedRate==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("��˿�ٶȸ�ʽ����ȷ���������"));
		msgbox.exec();
		return -1;
	}

	if(sheildingGasFlow==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("������������ʽ����ȷ���������"));
		msgbox.exec();
		return -1;
	}

	if(weldVoltage==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӵ�ѹ��ʽ����ȷ���������"));
		msgbox.exec();
		return -1;
	}

	if(weldCurrent==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӵ�����ʽ����ȷ���������"));
		msgbox.exec();
		return -1;
	}

	if(trackingDistance==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӳ��ȸ�ʽ����ȷ���������"));
		msgbox.exec();
		return -1;
	}
	//�����Ų飬û�����⣬���ɱ�����صĲ���
	parameters.weldSpeed=weldSpeed;
	parameters.wireFeedRate=wireFeedRate;
	parameters.sheildingGasFlow=sheildingGasFlow;
	parameters.weldVoltage=weldVoltage;
	parameters.weldCurrent=weldCurrent;
	parameters.trackingDistance=trackingDistance;
	
	return 1;
}