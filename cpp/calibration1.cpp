/******************************************************************************
  FileName: calibration1.cpp
  Author: WillLi		Date:2019-5-18
  Description:
	          SimpleCalibration��ĵ�ʵ��
  Function List:
                1. SimpleCalibration	// ���ʼ��
                2. ~SimpleCalibration	// ������ʱ
                3. on_confirmPushButton_clicked // ����rho--��ˮƽ�궨����
                4. updateHorizontalCalibrationRatio_triggered
  History:
         <author>		<time>     <desc>
          WillLi99    2019-5-18     ���clibration.cppͷ��ע�ͺ͸�������ͷ��ע��  
******************************************************************************/

#include "calibration1.h"
#include "var.h"


/******************************************************************************
  Function:SimpleCalibration
  Description: 
  Calls: 
  Called By: SimpleCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
SimpleCalibration::SimpleCalibration(QWidget *parent)
{
	ui.setupUi(this);
}

/******************************************************************************
  Function:~SimpleCalibration
  Description: 
  Calls: 
  Called By: SimpleCalibration��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
SimpleCalibration::~SimpleCalibration(void)
{
}

/******************************************************************************
  Function:on_pushButtonConfirm_clicked
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SimpleCalibration::on_pushButtonConfirm_clicked()
{
	QString aswString,pswString;
	double asw,psw;
	aswString=ui.lineEditRealCalibrationBlockWidth->text();
	pswString=ui.lineEditPhotographicBlockWidth->text();

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

	dHorizontalCalibrationRatio=asw/psw;
	qDebug()<<"horizontalCalibrationRatio from simple calibration is "<<dHorizontalCalibrationRatio;
	emit updateHorizontalCalibrationRatio_triggered(dHorizontalCalibrationRatio);
	emit updateCalibrationState_triggered();;

	close();
}