/******************************************************************************
  FileName: calibration1.cpp
  Author: WillLi		Date:2019-5-18
  Description:
	          SimpleCalibration��ĵ�ʵ��
  Function List:
				SimpleCalibration	// ���ʼ��
  				~SimpleCalibration	// ������ʱ
  				receiveBlockWidthPixel
  				setDefaultConfiguration

  				getBlockWidthPixel_triggered
  				on_pushButtonGetBlockWidthPixel_triggered
  				on_pushButtonCalculate_clicked 
  				updateHorizontalCalibrationRatio_triggered
  				updateCalibrationState_triggered
  History:
         <author>		<time>     <desc>
          WillLi99    2019-5-18     ���clibration.cppͷ��ע�ͺ͸�������ͷ��ע��  
******************************************************************************/

#include "calibration1.h"
#include "common.h"


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
	setDefaultConfiguration();
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
void SimpleCalibration::on_pushButtonCalculate_clicked()
{
	dHorizontalCalibrationRatio=dBlockWidthmm/dBlockWidthPixel;

	ui.lineEditBlockWidthPixel->setText(QString::number(dBlockWidthPixel));
	ui.lineEditHorizontalCalibrationRatio->setText(QString::number(dHorizontalCalibrationRatio));

	emit updateHorizontalCalibrationRatio_triggered(dHorizontalCalibrationRatio);
	emit updateCalibrationState_triggered();
}

/******************************************************************************
  Function:on_pushButtonGetBlockWidtPixel_clicked
  Description: ����ָ���tracksys.cpp������ͨ��qtͨ�ţ����ر궨�������ֵ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SimpleCalibration::on_pushButtonGetBlockWidtPixel_clicked()
{
	QString sBlockWidth=ui.lineEditBlockWidth->text();
	bool bConversionState;
	double dBlockWidth=sBlockWidth.toDouble(&bConversionState);

	if(bConversionState)
	{
		dBlockWidthmm=dBlockWidth;
	}
	else
	{
		dBlockWidthmm=10.0;
		ui.lineEditBlockWidth->setText(QString::number(dBlockWidthmm));

		QMessageBox msg;
		msg.setText(QStringLiteral("�����������������ΪĬ��"));
		msg.exec();

		return;
	}
	emit getBlockWidthPixel_triggered();
}


/******************************************************************************
  Function:receiveBlockWidth
  Description: ����tracksys.cpp��������blockWidth
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SimpleCalibration::receiveBlockWidthPixel(double blockWidth)
{
	dBlockWidthPixel=blockWidth;
}

/******************************************************************************
  Function:setDefaultConfiguration
  Description: ����Ĭ�ϵı궨�飨block)���Ϊ10mm
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SimpleCalibration::setDefaultConfiguration()
{
	dBlockWidthmm=10.0;
	dHorizontalCalibrationRatio=0.0;
	dBlockWidthPixel=0.0;

	ui.lineEditBlockWidth->setText(QString::number(dBlockWidthmm));
	ui.lineEditHorizontalCalibrationRatio->setText(QString::number(dHorizontalCalibrationRatio));
	ui.lineEditBlockWidthPixel->setText(QString::number(dBlockWidthPixel));
}