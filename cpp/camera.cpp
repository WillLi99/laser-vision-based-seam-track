/*****************************************************************************
Copyright: Guangdong Provincial Welding Technology Research Center
Author: Li Yangjin
Date: 2018-8-27
Description:Cameraģ���װ�˹���������������ã�����ͼƬ�ȹ��ܡ�
*****************************************************************************/

#include "JHCap.h"
#include "camera.h"

Camera::Camera(QWidget *parent):
QWidget(parent)
{
	ui.setupUi(this);
	cameraIndex=0;
}

Camera::~Camera()
{

}

/***********************************************************************
@��������onWBClicked
@��������
@����ֵ����
@���ܣ�һ����ƽ��
@�޸���Ϣ��
***********************************************************************/
void Camera::on_wbPushButton_clicked()
{
	CameraOnePushWB(cameraIndex);
}

/***********************************************************************
@��������on_loadCameraParasPushButton_clicked
@��������
@����ֵ����
@���ܣ������������
@�޸���Ϣ��
***********************************************************************/
void Camera::on_loadCameraParasPushButton_clicked()
{
	QString fileName=QFileDialog::getOpenFileName(this,"Open XML","..\\camera",
		"XML Files (*.xml)");
	if(fileName.isEmpty()==false)
	{
		QFile file(fileName);
		if(!file.open(QFile::ReadOnly|QFile::Text))
		{
			std::cerr<<"Error::Cannot read file"<<qPrintable(fileName)
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
					ui.gammaHS->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Contrast")
				{
					ui.contrastHS->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Gain")
				{
					ui.hsGain->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Exposure")
				{
					ui.exposureHS->setValue(xmlReader.readElementText().toInt());
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

/***********************************************************************
@��������on_saveCameraParasPushButton_clicked
@��������
@����ֵ����
@���ܣ������������
@�޸���Ϣ��
***********************************************************************/
void Camera::on_saveCameraParasPushButton_clicked()
{
	QString fileName=QFileDialog::getSaveFileName(this,"Save XML",".","XML Files (*.xml)");
	
	if(fileName.isEmpty()==false)
	{
		QFile file(fileName);
		file.open(QIODevice::WriteOnly);

		QXmlStreamWriter xmlWriter(&file);
		xmlWriter.setAutoFormatting(true);
		xmlWriter.writeStartDocument();

		xmlWriter.writeStartElement(QStringLiteral("�������"));
		xmlWriter.writeTextElement("Gamma",QString::number(ui.gammaHS->value()));
		xmlWriter.writeTextElement("Contrast",QString::number(ui.gammaHS->value()));

		int gain;
		if(API_OK==CameraGetGain(cameraIndex,&gain));
		else gain=0;
		xmlWriter.writeTextElement("Gain",ui.gainCheckBox->isChecked()?QString::number(gain):QString
			::number(ui.hsGain->value()));

		int exposure;
		if(API_OK==CameraGetExposure(cameraIndex,&exposure));
		else exposure=0;

		xmlWriter.writeTextElement("Exposure",ui.exprosureCheckBox->isChecked()?QString::number(exposure):QString
			::number(ui.exposureHS->value()));

		xmlWriter.writeEndElement();
		xmlWriter.writeEndDocument();
		file.close();

		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("������ʾ"));
		msgbox.setText(fileName+QStringLiteral("�ѱ���"));
		msgbox.exec();
	}
}


/***********************************************************************
@��������on_saveSingleImagePushButton_clicked
@��������
@����ֵ����
@���ܣ����浥��ͼƬ
@�޸���Ϣ��
***********************************************************************/
void Camera::on_saveSingleImagePushButton_clicked()
{
	emit saveSingleImage_triggered();
}

/***********************************************************************
@��������on_saveMultiImagesPushButton_clicked
@��������
@����ֵ����
@���ܣ��������ͼƬ
@�޸���Ϣ��
***********************************************************************/
void Camera::on_saveMultiImagesPushButton_clicked()
{
	emit saveMultiImages_triggered();
}

/***********************************************************************
@��������on_stopSavingPicturesPushButton_clicked
@��������
@����ֵ����
@���ܣ�ֹͣ�������ͼƬ
@�޸���Ϣ��
***********************************************************************/
void Camera::on_stopSavingImagesPushButton_clicked()
{
	emit stopSavingImages_triggered();
}

/***********************************************************************
@��������_initializeParamters
@��������
@����ֵ����
@���ܣ����ø���Ĭ�ϲ���
@�޸���Ϣ��
***********************************************************************/
void Camera::initializeParamters()
{
	/***���ø���Sliders�ķ�Χ***/
	ui.contrastHS->setMaximum(200);
	ui.contrastHS->setMinimum(30);
	ui.exposureHS->setMaximum(2000);
	ui.exposureHS->setMinimum(1);
	ui.hsGain->setMaximum(64);
	ui.hsGain->setMinimum(8);
	ui.gammaHS->setMaximum(200);
	ui.gammaHS->setMinimum(30);

	double gamma;
	CameraGetGamma(cameraIndex,&gamma);//��ȡ���gammaֵ
	ui.gammaLabel->setText(QString("%1").arg(gamma));
	ui.gammaHS->setValue(gamma*100);

	double contrast;
	CameraGetContrast(cameraIndex,&contrast);
	ui.contrastLabel->setText(QString("%1").arg(contrast));
	ui.contrastHS->setValue(contrast*100);

	bool aec,agc;
	CameraGetAGC(cameraIndex,&agc);//��ȡ�Զ�����״̬
	CameraGetAEC(cameraIndex,&aec);//��ȡ�Զ��ع�״̬

	if(agc)
	{
		ui.hsGain->setEnabled(false);
		ui.gainCheckBox->setChecked(true);
	}
	else
	{
		int gain;
		CameraGetGain(cameraIndex,&gain);
		ui.gainLabel->setText(QString("%1").arg(gain));
		ui.hsGain->setValue(gain);
		ui.hsGain->setEnabled(true);
	}

	if(aec)
	{
		ui.exposureHS->setEnabled(false);
		ui.exprosureCheckBox->setChecked(true);
	}
	else
	{
		int exposure;
		CameraGetExposure(cameraIndex,&exposure);
		ui.exposureLabel->setText(QString("%1").arg(exposure));
		ui.exposureHS->setValue(exposure);
		ui.exposureHS->setEnabled(true);
		ui.exprosureCheckBox->setChecked(false);
	}
}

void Camera::on_gammaHS_valueChanged(int value)
{
	double gamma=value/100.0;
	CameraSetGamma(cameraIndex,gamma);

	ui.space1->setText(QString("%1").arg(value));
}

void Camera::on_contrastHS_valueChanged(int value)
{
	double contrast=value/100.0;
	CameraSetContrast(cameraIndex,contrast);

	ui.space2->setText(QString("%1").arg(value));
}

void Camera::on_hsGain_valueChanged(int value)
{
	CameraSetGain(cameraIndex,value);
	ui.space3->setText(QString("%1").arg(value));
}

void Camera::on_exposureHS_valueChanged(int value)
{
	CameraSetExposure(cameraIndex,value);
	ui.space4->setText(QString("%1").arg(value));
}

void Camera::on_gainCheckBox_toggled(bool checked)
{
	CameraSetAGC(cameraIndex,checked);
	initializeParamters();
}

void Camera::on_exprosureCheckBox_toggled(bool checked)
{
	CameraSetAEC(cameraIndex,checked);
	initializeParamters();
}