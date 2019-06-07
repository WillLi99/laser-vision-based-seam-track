/******************************************************************************
  File name: weld.h
  Author: WillLi99		Date:2019-5-21
  Description:
              ������Weld�࣬����̳���QWidget���������ú��Ӳ��������ļ�Ϊ��Weld��ʵ�֡�               
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
#include "weld.h"

/******************************************************************************
  Function:Weld
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Weld::Weld(QWidget *parent)
{
	ui.setupUi(this);
	setDefaultParameters();
}

/******************************************************************************
  Function:~Weld
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Weld::~Weld(void)
{
}

/******************************************************************************
  Function:setDefaultParameters
  Description:����Ĭ�ϵĺ��Ӳ�����
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Weld::setDefaultParameters()
{
	dWeldSpeed=2.0;
	dWireRate=2.0;
	dGasFlow=0.5;
	dWeldVoltage=25.0;
	dWeldCurrent=100.0;

	ui.lineEditWeldSpeed->setText(QString::number(dWeldSpeed));
	ui.lineEditWireRate->setText(QString::number(dWireRate));
	ui.lineEditGasFlow->setText(QString::number(dGasFlow));
	ui.lineEditWeldVoltage->setText(QString::number(dWeldVoltage));
	ui.lineEditWeldCurrent->setText(QString::number(dWeldCurrent));
}

/******************************************************************************
  Function:checkWeldParameters
  Description:��麸�Ӳ��������ԡ�����true��ʾ���ͨ��������false��ʾ��鲻ͨ��
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
bool Weld::checkWeldParameters()
{
	QString str1,str2,str3,str4,str5,str6;

	str1=ui.lineEditWeldSpeed->text();
	str2=ui.lineEditWireRate->text();
	str3=ui.lineEditWeldCurrent->text();
	str4=ui.lineEditWeldVoltage->text();
	str5=ui.lineEditWeldCurrent->text();
	str6=ui.lineEditWeldDistance->text();

	dWeldSpeed=str1.toDouble();
	dWireRate=str2.toDouble();
	dGasFlow=str3.toDouble();
	dWeldVoltage=str4.toDouble();
	dWeldCurrent=str5.toDouble();
	dWeldDistance=str6.toDouble();

	//���
	if(dWeldSpeed==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("�����ٶȸ�ʽ����ȷ���������"));
		msgbox.exec();
		return false;
	}

	if(dWireRate==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("��˿�ٶȸ�ʽ����ȷ���������"));
		msgbox.exec();
		return false;
	}

	if(dGasFlow==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("������������ʽ����ȷ���������"));
		msgbox.exec();
		return false;
	}

	if(dWeldVoltage==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӵ�ѹ��ʽ����ȷ���������"));
		msgbox.exec();
		return false;
	}

	if(dWeldCurrent==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӵ�����ʽ����ȷ���������"));
		msgbox.exec();
		return false;
	}

	if(dWeldDistance==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���ӳ��ȸ�ʽ����ȷ���������"));
		msgbox.exec();
		return false;
	}
	
	//�����Ų飬û�����⣬���ɱ�����صĲ���
	wpWeldParameter.dWeldSpeed=dWeldSpeed;
	wpWeldParameter.dWireRate=dWireRate;
	wpWeldParameter.dGasFlow=dGasFlow;
	wpWeldParameter.dWeldVoltage=dWeldVoltage;
	wpWeldParameter.dWeldCurrent=dWeldCurrent;
	wpWeldParameter.dWeldDistance=dWeldDistance;

	return true;
}

/******************************************************************************
  Function:on_confirmPushButton_clicked
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Weld::on_confirmPushButton_clicked()
{
	bool checkResult=checkWeldParameters();
	
	if(checkResult==true)	//���ͨ����emit���Ӳ���
	{
		emit updateWeldParameters_triggered(wpWeldParameter);
		close();
	}
	else if(checkResult==false)
	{
		return;
	}
}

/******************************************************************************
  Function:on_cancelPushButton_clicked
  Description:�˳����ý���
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Weld::on_cancelPushButton_clicked()
{
	close();
}