#include "calibration1.h"
#include "var.h"


/***********************************************************************
@��������SimpleCalibration
@������
@����ֵ��
@���ܣ�SimpleCalibration��ĳ�ʼ������
@�޸���Ϣ��
***********************************************************************/
SimpleCalibration::SimpleCalibration(QWidget *parent)
{
	ui.setupUi(this);
}

/***********************************************************************
@��������~SimpleCalibration
@������
@����ֵ��
@���ܣ�SimpleCalibration�����������
@�޸���Ϣ��
***********************************************************************/
SimpleCalibration::~SimpleCalibration(void)
{
}

/***********************************************************************
@��������on_confirmPushButton_clicked
@������
@����ֵ��
@���ܣ���Ӧ�ԡ�confirmPushButton"�ĵ���¼�
@�޸���Ϣ��
***********************************************************************/
void SimpleCalibration::on_confirmPushButton_clicked()
{
	QString aswString,pswString;
	double asw,psw,rho;
	aswString=ui.actualSeamWidthLineEdit->text();
	pswString=ui.pictorialSeamWidthLineEdit->text();

	//���kString��bString�Ƿ�Ϸ�
	asw=aswString.toDouble();
	psw=pswString.toDouble();

	if(0==asw)
	{
		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("��ʾ"));
		msgbox.setText(QStringLiteral("����Ƿ����������"));
		msgbox.exec();
		return;
	}
	
	if(0==psw)
	{
		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("��ʾ"));
		msgbox.setText(QStringLiteral("����Ƿ����������"));
		msgbox.exec();
		return;
	}

	rho=asw/psw;
	qDebug()<<"rho from simple calibration is "<<rho;
	emit updateHorizontalCalibrationRatio_triggered(rho);
	hasCalibrated=true;

	close();
}
