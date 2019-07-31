/******************************************************************************
  File name: camera.cpp
  Author: WillLi99		Date:2019-5-18
  Description: ʵ��Camera�࣬Camera��Ĺ���Ϊʵ�ֵ����������
  Others: 
  Function List:
				1. Camera
  				2. ~Camera
  				3. on_pushButtonWhiteBalance_clicked				//һ����ƽ��
  				4. on_pushButtonLoadCameraConfiguration_clicked		//�����������
  				5. on_pushButtonSaveCameraConfiguration_clicked		//�����������
  				6. on_pushButtonSaveSingleImage_clicked				//���浥��ͼƬ
  				7. on_pushButtonSaveMultipleImages_clicked			//�������ͼ��
  				8. on_pushButtonStopSavingMultipleImages_clicked	//ֹͣ����ͼ��
  				9. on_horizontalSliderGamma_valueChanged			//�ı�gammaֵ
  				10. on_horizontalSliderContrast_valueChanged		//�ı�Աȶ�
  				11. on_horizontalSliderGain_valueChanged			//�ı��������
  				12. on_horizontalSliderExposure_valueChanged		//�ı��ع�ֵ
  				13  on_checkBoxGain_toggled			//�Զ�����
  				14. on_checkBoxExposure_toggled		//�Զ��ع�
  				15. initializeParamters				//��ʼ������
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     ���camra.hͷ��ע��
******************************************************************************/

#include "JHCap.h"
#include "camera.h"

/******************************************************************************
  Function:Camera
  Description:
  Calls: 
  Called By: tracksys.cpp
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Camera::Camera(QWidget *parent):
QWidget(parent)
{
	ui.setupUi(this);
	intCameraIndex=0;
}

/******************************************************************************
  Function:~Camera
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Camera::~Camera()
{

}

/******************************************************************************
  Function:on_pushButtonWhiteBalance_clicked
  Description: һ����ƽ��
  Calls: JHCap.h/CameraOnePushWB
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_pushButtonWhiteBalance_clicked()
{
	CameraOnePushWB(intCameraIndex);
}

/******************************************************************************
  Function:on_pushButtonLoadCameraConfiguration_clicked
  Description: ����λ��../cameraĿ¼�µ�xml��������ļ�
  Calls: QFileDialog��
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_pushButtonLoadCameraConfiguration_clicked()
{
	QString qstrFileName=QFileDialog::getOpenFileName(this,"Open XML","..\\camera",
		"XML Files (*.xml)");
	if(qstrFileName.isEmpty()==false)
	{
		QFile file(qstrFileName);
		if(!file.open(QFile::ReadOnly|QFile::Text))
		{
			std::cerr<<"Error::Cannot read file"<<qPrintable(qstrFileName)
				<<":"<<qPrintable(file.errorString())<<std::endl;
		}

		QXmlStreamReader xmlReader;
		xmlReader.setDevice(&file);
		xmlReader.readNext();

		while(!xmlReader.atEnd() && !xmlReader.hasError())
		{
			QXmlStreamReader::TokenType token=xmlReader.readNext();
			if(token==QXmlStreamReader::StartDocument) continue;
			if(token==QXmlStreamReader::StartElement)
			{
				if(xmlReader.name()==QStringLiteral("�������"))
				{
					continue;
				}
				else if(xmlReader.name()=="Gamma")
				{
					ui.horizontalSliderGamma->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Contrast")
				{
					ui.horizontalSliderContrast->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Gain")
				{
					ui.horizontalSliderGain->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Exposure")
				{
					ui.horizontalSliderExposure->setValue(xmlReader.readElementText().toInt());
				}
			}
		}

		if(xmlReader.hasError())
		{
			QMessageBox::critical(this,
				"xmlFile.xml Parse Error",xmlReader.errorString(),QMessageBox::Ok);
			return;
		}
	}
}

/******************************************************************************
  Function:on_pushButtonSaveCameraConfiguration_clicked
  Description: ���浱ǰ���������
  Calls: QFileDialog��
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_pushButtonSaveCameraConfiguration_clicked()
{
	QString qstrFileName=QFileDialog::getSaveFileName(this,"Save XML",".","XML Files (*.xml)");
	
	if(qstrFileName.isEmpty()==false)
	{
		QFile file(qstrFileName);
		file.open(QIODevice::WriteOnly);

		QXmlStreamWriter xmlWriter(&file);
		xmlWriter.setAutoFormatting(true);
		xmlWriter.writeStartDocument();

		xmlWriter.writeStartElement(QStringLiteral("�������"));
		xmlWriter.writeTextElement("Gamma",QString::number(ui.horizontalSliderGamma->value()));
		xmlWriter.writeTextElement("Contrast",QString::number(ui.horizontalSliderGamma->value()));

		int intGain;
		if(API_OK==CameraGetGain(intCameraIndex,&intGain));
		else intGain=0;
		xmlWriter.writeTextElement("Gain",ui.checkBoxGain->isChecked()?QString::number(intGain):QString
			::number(ui.horizontalSliderGain->value()));

		int intExposure;
		if(API_OK==CameraGetExposure(intCameraIndex,&intExposure));
		else intExposure=0;

		xmlWriter.writeTextElement("Exposure",ui.checkBoxExposure->isChecked()?QString::number(intExposure):QString
			::number(ui.horizontalSliderContrast->value()));

		xmlWriter.writeEndElement();
		xmlWriter.writeEndDocument();
		file.close();

		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("������ʾ"));
		msgbox.setText(qstrFileName+QStringLiteral("�ѱ���"));
		msgbox.exec();
	}
}


/******************************************************************************
  Function:on_pushButtonSaveCameraConfiguration_clicked
  Description: ���浥��ͼƬ������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: tracksys.cpp����
******************************************************************************/
void Camera::on_pushButtonSaveSingleImage_clicked()
{
	emit saveSingleImage_triggered();
}

/******************************************************************************
  Function:on_pushButtonSaveMultipleImages_clicked
  Description: ����������ͼ������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: tracksys.cpp����
******************************************************************************/
void Camera::on_pushButtonSaveMultipleImages_clicked()
{
	emit stopSavingMultipleImages_triggered();
}

/******************************************************************************
  Function:on_pushButtonStopSavingMultipleImages_clicked
  Description: ��ֹ����������ͼ������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: tracksys.cpp����
******************************************************************************/
void Camera::on_pushButtonStopSavingMultipleImages_clicked()
{
	emit stopSavingMultipleImages_triggered();
}

/******************************************************************************
  Function:initializeCameraConfiguration
  Description: ��ʼ���������
  Calls: JHCap.h/CameraGetGamma JHCap.h/CameraGetContrast JHCap.h/CameraGetAGC 
         JHCap.h/CameraGetAEC JHCap.h/CameraGetGain
  Called By: Camera()
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::initialize()
{
	//����horizontalSlider�ķ�Χ
	ui.horizontalSliderGamma->setMaximum(20);
	ui.horizontalSliderGamma->setMinimum(3);

	ui.horizontalSliderContrast->setMaximum(20);
	ui.horizontalSliderContrast->setMinimum(3);

	ui.horizontalSliderGain->setMaximum(32);
	ui.horizontalSliderGain->setMinimum(1);

	ui.horizontalSliderExposure->setMaximum(4000);
	ui.horizontalSliderExposure->setMinimum(1);

	double dGamma;
	CameraGetGamma(intCameraIndex,&dGamma);
	ui.labelGammaValue->setText(QString("%1").arg(dGamma));
	ui.horizontalSliderGamma->setValue(dGamma*10);

	double dContrast;
	CameraGetContrast(intCameraIndex,&dContrast);
	ui.labelContrastValue->setText(QString("%1").arg(dContrast));
	ui.horizontalSliderGamma->setValue(dContrast*100);

	int intGain;
	CameraGetGain(intCameraIndex,&intGain);
	ui.labelGainValue->setText(QString("%1").arg(intGain));
	ui.horizontalSliderGain->setValue(intGain);

	int intExposure;
	CameraGetExposure(intCameraIndex,&intExposure);
	ui.labelExposureValue->setText(QString("%1").arg(intExposure));
	ui.horizontalSliderExposure->setValue(intExposure);
}

/******************************************************************************
  Function:on_horizontalSliderGamma_valueChanged
  Description: �������gammaֵ���������gammaֵ��Χ��0.3-2.
  Calls: JHCap.h/CameraSetGamma
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderGamma_valueChanged(int intGamma)
{
	double dGamma=intGamma/10.0;
	CameraSetGamma(intCameraIndex,dGamma);

	ui.labelGammaValue->setText(QString("%1").arg(intGamma));
}

/******************************************************************************
  Function:on_horizontalSliderContrast_valueChanged
  Description: �������contrastֵ,�������contrastֵ��Χ��0.3-2.
  Calls: JHCap.h/CameraSetContrast
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderContrast_valueChanged(int intContrastValue)
{
	double dContrast=intContrastValue/10.0;
	CameraSetContrast(intCameraIndex,dContrast);

	ui.labelContrastValue->setText(QString("%1").arg(intContrastValue));
}

/******************************************************************************
  Function:on_horizontalSliderGain_valueChanged
  Description: �������gainֵ.���淶ΧΪ1-255������ֲ��Ͻ�����С��32
  Calls: JHCap.h/CameraSetGain
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderGain_valueChanged(int intGainValue)
{
	CameraSetGain(intCameraIndex,intGainValue);
	ui.labelGainValue->setText(QString("%1").arg(intGainValue));
}

/******************************************************************************
  Function:on_exposureHS_valueChanged
  Description: ��������ع�ֵ�����淶ΧΪ1-4000
  Calls: JHCap.h/CameraSetExposure
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderExposure_valueChanged(int intExposureValue)
{
	CameraSetExposure(intCameraIndex,intExposureValue);
	ui.labelExposureValue->setText(QString("%1").arg(intExposureValue));
}

/******************************************************************************
  Function:on_checkBoxGain_toggled
  Description: ��������ع�ֵ
  Calls: initializeCameraConfiguration
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_checkBoxGain_toggled(bool checked)
{
	CameraSetAGC(intCameraIndex,checked);

	CameraGetAGC(intCameraIndex,&bAGC);
	if(bAGC)//����ѿ����Զ�����
	{
		ui.horizontalSliderGain->setEnabled(false);
		ui.checkBoxGain->setChecked(true);
	}
	else //����ر��Զ�����
	{
		int gain;
		CameraGetGain(intCameraIndex,&gain);
		ui.labelGammaValue->setText(QString("%1").arg(gain));
		ui.horizontalSliderGain->setEnabled(true);
		ui.horizontalSliderGain->setValue(gain);
	}
	
}

/******************************************************************************
  Function:on_checkBoxExposure_toggled
  Description: ��������ع�ֵ
  Calls: initializeCameraConfiguration
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_checkBoxExposure_toggled(bool checked)
{
	CameraSetAEC(intCameraIndex,checked);
	
	CameraGetAEC(intCameraIndex,&bAEC);
	if(bAEC)//����ѿ����Զ��ع�
	{
		ui.horizontalSliderExposure->setEnabled(false);
		ui.checkBoxExposure->setChecked(true);
	}
	else //����ر��Զ��ع�
	{
		int exposure;
		CameraGetExposure(intCameraIndex,&exposure);
		ui.labelExposureValue->setText(QString("%1").arg(exposure));
		ui.horizontalSliderExposure->setEnabled(true);
		ui.horizontalSliderExposure->setValue(exposure);
	}
}