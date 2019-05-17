#include "JHCap.h"
#include "tracksys.h"

/***ȫ�ֱ���������***/
bool autoModeCanRunFlag = false;//�Զ�ģʽ�Ƿ�����б�־
bool cameraIsRunning=false;	//�������״̬��־
bool AutoTrackingTriggledFlag=false;	//�Զ����ٴ�����־
bool hasCalibrated=false;		//�Ƿ���У����־
bool hasAligned=false;		//�Ƿ��Ѷ����־

//���캯��
SeamTrackingSys::SeamTrackingSys(QWidget *parent)
	: QMainWindow(parent),scene1(0),scene2(0),video0PixItem(0),video1PixItem(0)
{
	qRegisterMetaType<SeamInfo> ("SeamInfo");
	qRegisterMetaType<WeldParameter> ("WeldParameter");

	qDebug()<<"***AutoTracking System***";
	ui_main.setupUi(this);
	cardCount=d1000_board_init();
	setDefaultUiConfiguration();
	obj_onlinetrack=new OnlineTrack(this);obj_onlinetrack->start();
	obj_imageaquisition=new ImagesAcqusition(this);

	//��������
	scene1=new QGraphicsScene(this);
	ui_main.video0GraphicsView->setScene(scene1);
	scene2=new QGraphicsScene(this);
	ui_main.video1graphicsView->setScene(scene2);

	//�ֱ�����ʾ��ǩ
	fpsLabel=new QLabel(this);
	fpsLabel->setAlignment(Qt::AlignHCenter);
	ui_main.statusBar->addWidget(fpsLabel);

	initializeVariable();
	setDefaultWeldParameters();
	
	m_timer=new QTimer(this);

	connect(this,SIGNAL(saveImages_triggered()),obj_imageaquisition,SLOT(setSavingPicturesSwitch()));
	connect(this,SIGNAL(stopSavingImages_triggered()),obj_imageaquisition,SLOT(resetSavingPicturesSwitch()));

	connect(this,SIGNAL(autoTrack_triggered(SeamTrackParameter)),obj_onlinetrack,SLOT(setAutoTrackingSwitch(SeamTrackParameter)));
	connect(this,SIGNAL(stopAutoTracking_triggered()),obj_onlinetrack,SLOT(stopAutoTracking()));
	connect(this,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),obj_onlinetrack,SLOT(updateHorizontalCalibrationRatio(double)));

	connect(this,SIGNAL(updateWeldParameters_triggered(WeldParameter)),&ui_offlinetrack,SLOT(updateWeldParameters(WeldParameter)));
	connect(this,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),&ui_offlinetrack,SLOT(updateHorizontalRatio(double)));
	connect(&ui_offlinetrack,SIGNAL(saveImage_triggered(QString)),this,SLOT(saveImageOfflineTrack(QString)));

	connect(obj_onlinetrack,SIGNAL(sendDIPResult_triggered(QImage)),this,SLOT(updateDIPResult(QImage)));
	connect(obj_onlinetrack,SIGNAL(sendDIPCalculationResult_triggered(SeamInfo)),this,SLOT(updateDIPCalculationResult(SeamInfo)));
	connect(obj_onlinetrack,SIGNAL(allowNextDIP_triggered()),this,SLOT(startDIP()));

	connect(obj_imageaquisition,SIGNAL(captured(QImage,unsigned char *)),this,SLOT(display1(QImage,unsigned char *)));
	connect(obj_imageaquisition,SIGNAL(captured2(QImage)),this,SLOT(display2(QImage)));

	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateFPSValue()));

	connect(&ui_camera,SIGNAL(saveSingleImage_triggered()),this,SLOT(saveSingleImage()));
	connect(&ui_camera,SIGNAL(saveMultiImages_triggered()),this,SLOT(saveMultiImages()));
	connect(&ui_camera,SIGNAL(stopSavingImages_triggered()),this,SLOT(stopSavingImages()));
		
	connect(&ui_simplecalibration,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),this,SLOT(updateHorizontalCalibrationRatio(double)));
	connect(&ui_weld,SIGNAL(updateWeldParameters_triggled(WeldParameter)),this,SLOT(updateWeldParameters(WeldParameter)));
	
	connect(&ui_fittingcalibration,SIGNAL(saveCalibrationData_triggered(QString)),this,SLOT(saveCalibrationData(QString)));
	connect(&ui_fittingcalibration,SIGNAL(saveValidationData_triggered(QString)),this,SLOT(saveValidationData(QString)));
	connect(&ui_fittingcalibration,SIGNAL(saveSeamLaserStripeData_triggered(QString)),this,SLOT(saveSeamLaserStripeData(QString)));

	m_timer->start(1000);


	/***����������״̬***/
	int cameraCount=0;
	CameraGetCount(&cameraCount);
	if(cameraCount>0)
	{
		isCameraConnected=true;
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(checkico));
		CameraInit(cameraIndex);
		CameraSetResolution(cameraIndex,5,&imageWidth,&imageHeight);
		cmbNotResponding=true;
		cmbNotResponding=false;
		
		qDebug()<<"Resolution= "<<imageWidth<<"*"<<imageHeight;

	}
	else if(cameraCount==0)
	{
		isCameraConnected=false;
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(wrongico));
	}
		

	d1000_out_bit(4,1);
}


//�˳�����
void SeamTrackingSys::on_quitPushButton_clicked()
{
	QMessageBox msgBox;
	msgBox.setText(QStringLiteral("ȷ���˳���"));
	msgBox.exec();
	CameraFree(cameraIndex);
	d1000_board_close();
	exit(0);
}


void SeamTrackingSys::on_startAutoTrackPushButton_clicked()
{
	//check
	if(!cameraIsRunning)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���δ����"));
		msgbox.exec();
		return;
	}
	if(!isDIPStarted)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("ʶ��δ����"));
		msgbox.exec();
		return;
	}

	if(!hasAligned)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("δ���룡����ִ�ж�������"));
		msgbox.exec();
		return;
	}

	AutoTrackingTriggledFlag=true;
	qDebug()<<"AutoTrackingTriggledFlag="<<AutoTrackingTriggledFlag;
}


//��������
SeamTrackingSys::~SeamTrackingSys()
{
	d1000_board_close();
	delete scene1;
	delete scene2;
	delete obj_imageaquisition;
}

/***********************************************************************
@��������on_turnOnCameraPushButton_clicked
@��������
@����ֵ����
@���ܣ�ui���棬������������pushbutton����Ӧ�ĺ���
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_turnOnCameraPushButton_clicked()
{
	if(isCameraConnected==true)
	{
		
		testOrNot=false;
		obj_imageaquisition->start();
		obj_imageaquisition->stream();			//obj_imageaquisition��ʼ����

		//���·ֱ����б�
		int count;
		CameraGetResolutionCount(cameraIndex,&count);
		loadDefaultCameraConfiguration();
		for(int i=0; i<count; i++)
		{
			int imageWidth, imageHeight;
			CameraGetResolution(cameraIndex,i, &imageWidth, &imageHeight);
			ui_main.cameraResolutionComBox->addItem(QString("%1x%2").arg(imageWidth).arg(imageHeight));
		}

		CameraSetResolution(cameraIndex,5,0,0);//����Ĭ�Ϸֱ�����800*600
		cameraIsRunning=true;
	}
	else
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���δ���ӣ������������"));
		msgbox.exec();
	}
}

/***********************************************************************
@��������on_turnOffCameraPushButton_clicked
@��������
@����ֵ����
@���ܣ�ui���棬������������pushbutton����Ӧ�ĺ���
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_turnOffCameraPushButton_clicked()
{
	testOrNot=true;
	isDIPStarted=false;
	cameraIsRunning=false;
	obj_imageaquisition->pause();

	if(!frame.isNull())
	{
		QImage img1=frame.scaled(ui_main.video0GraphicsView->width(),ui_main.video0GraphicsView->height());
		video0PixItem=scene1->addPixmap(QPixmap::fromImage(img1));
		scene1->setSceneRect(0,0,img1.width(),img1.height());
	}
}

/***********************************************************************
@��������on_turnOnRecognitionPushButton_clicked
@��������
@����ֵ����
@���ܣ���ʶ��
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_turnOnRecognitionPushButton_clicked()
{
	if(cameraIsRunning)
	{
		testOrNot=false;
		isDIPStarted=true;
	}
	else
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���δ�������ȿ��������"));
		msgbox.exec();
	}
}

void SeamTrackingSys::on_turnOffRecognitonPushButton_clicked()
{
	isDIPStarted=false;
	isFirstDIPStarted=false;
}


/***********************************************************************
@��������on_alignPushButton_clicked
@��������
@����ֵ����
@���ܣ��Զ����ٿ�ʼʱ����ǹ��׼��������
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_alignPushButton_clicked()
{
	if(!isDIPStarted)
	{
		QMessageBox msgBox;
		msgBox.setText(QStringLiteral("ʶ��δ����"));
		msgBox.exec();
		return;
	}
	else
	{
		if(!hasCalibrated)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("δУ�������Ƚ���У��"));
			msgBox.exec();
			return;

		}
		double tolerance=0.2;
		qDebug()<<"currentSeamOffset:"<<currentSeamOffset;
		if(abs(currentSeamOffset)>tolerance)
		{
			obj_automotion.xMove(1,currentSeamOffset,0.5,1);
		}
		else
		{
			hasAligned=true;
		}
	}
}


void SeamTrackingSys::closeEvent(QCloseEvent *e)
{
	obj_imageaquisition->stop();
	obj_imageaquisition->wait();

	CameraFree(cameraIndex);
	d1000_board_close();
	exit(0);
	QMainWindow::closeEvent(e);

}

/***********************************************************************
@��������on_cameraResolutionComBox_currentIndexChanged
@��������
@����ֵ����
@���ܣ�����б��ĳ��ʱ����Ӧ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_cameraResolutionComBox_currentIndexChanged(int index)
{
	CameraGetResolution(cameraIndex,index,&imageWidth,&imageHeight);
	if(cmbNotResponding==false)
	{
		obj_imageaquisition->pause();
		CameraSetResolution(cameraIndex,index, 0, 0);
		obj_imageaquisition->stream();
	}
}

/***********************************************************************
@��������updateFPSValue
@��������
@����ֵ����
@���ܣ�����֡����ʾ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::updateFPSValue()
{
	/*if(fpsValue==0) fpsValue=m_totalFrame;
	else fpsValue=0.4*fpsValue+0.6*m_totalFrame;*/
	fpsValue=m_totalFrame;
	m_totalFrame=0;
	fpsLabel->setText(QString("FPS:%1").arg(QString::number(fpsValue, 'f', 2)));
}

/***********************************************************************
@��������on_cameraParameterssetPushButton_clicked
@��������
@����ֵ����
@���ܣ�����������ý���
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_cameraParametersSettingPushButton_clicked()
{
	ui_camera.show();	//����cameraControl����
}

/***********************************************************************
@��������saveImage
@��������
@����ֵ����
@���ܣ�����ͼƬ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::saveSingleImage()
{

	obj_imageaquisition->pause();

	/***���浥��ͼƬ***/
	QString fileName;
	fileName=QFileDialog::getSaveFileName(this,QStringLiteral("����"),"..","Images(*.png *.bmp *.jpg)");

	//qDebug()<<fileName;

	if(!fileName.isNull())
	{
		if(!frame.isNull())
		{

			bool retV=false;
			retV=frame.save(fileName,0,-1);
			if(retV)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("�ѱ���")+fileName);
				msgbox.exec();
				obj_imageaquisition->stream();
			}
		}
		else
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("��ǰ��ͼƬ��"));
			msgbox.exec();
			return;
		}

	}
	else
	{
		return;
	}
}

/***********************************************************************
@��������saveImages
@��������
@����ֵ����
@���ܣ��������ͼƬ������Ŀ¼"../Images/"
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::saveMultiImages()
{
	//emit saveImages_triggered();	//��signal����obj_imageaquisition����
	isSavingMultiImagesCommandTriggered=true;
}

/***********************************************************************
@��������stopSavingImages
@��������
@����ֵ����
@���ܣ�ֹͣ�������ͼƬ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::stopSavingImages()
{
	//emit stopSavingImages_triggered();//��signal����obj_imageaquisition����
	isSavingMultiImagesCommandTriggered=false;
	multiImagesNo=0;
}


/***********************************************************************
@��������updateDIPResult
@��������
@����ֵ����
@���ܣ�����ʶ������ʾ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::updateDIPResult(QImage img)
{
	//��ʾ����Ƶ2����

	if(video1PixItem)
	{
		scene2->removeItem(video1PixItem);
		delete video1PixItem;
		video1PixItem=0;
	}

	img=img.scaled(ui_main.video1graphicsView->width(),ui_main.video1graphicsView->height());
	video1PixItem=scene2->addPixmap(QPixmap::fromImage(img));
	scene1->setSceneRect(0,0,img.width(),img.height());

}

/***********************************************************************
@��������updatecurrentOffsetValue
@��������
@����ֵ����
@���ܣ�����ƫ��ֵ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::updateDIPCalculationResult(SeamInfo seaminfo)
{
	receivedFrameNumber++;
	if(receivedFrameNumber!=0)
	{
		lastSeamOffset=currentSeamOffset;
	}
	currentSeamOffset=seaminfo.weldSeamOffset*horizontalCalibrationRatio;
	double sw=int(seaminfo.weldSeamWidth*horizontalCalibrationRatio*1000)/1000.0;
	QString seamWidth=QString::number(sw);
	
	QString weldTorchHeight=QString::number(seaminfo.torchHeight);
	
	double co=int(seaminfo.weldSeamOffset*horizontalCalibrationRatio*1000)/1000.0;
	QString weldingcurrentOffset=QString::number(co);
	ui_main.seamWidthDisplayLabel->setText(seamWidth);
	ui_main.seamRemainingHeightDisplayLabel->setText(weldTorchHeight);
	ui_main.seamOffsetDisplayLabel->setText(weldingcurrentOffset);
	
}


/***********************************************************************
@��������on_stopPushButton_clicked
@��������
@����ֵ����
@���ܣ�ֹͣ����
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_stopAutoTrackPushButton_clicked()
{
	AutoTrackingTriggledFlag=false;
	qDebug()<<"AutoTrackingTriggledFlag="<<AutoTrackingTriggledFlag;
	emit stopAutoTracking_triggered();

	d1000_decel_stop(0);
	d1000_decel_stop(1);
	d1000_decel_stop(2);
	d1000_decel_stop(3);
}

/***********************************************************************
@��������_updateSeamType
@��������
@����ֵ����
@���ܣ����º�������
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::updateSeamType()
{
	switch(weldSeamType)
	{
	case BUTT_WELD:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("�ԽӺ���"));
		break;
	case LAP_WElD:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("��Ӻ���"));
		break;
	case GROOVE_WELD:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("V�ͺ���"));
		break;
	}
}


/***********************************************************************
@��������setDefaultUiConfiguration
@��������
@����ֵ����
@���ܣ����ò˵�ѡ���Ĭ��ֵ�����ý����Ĭ��ֵ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::setDefaultUiConfiguration()
{
	//���ò˵���ѡ���Ĭ��ѡ��
	ui_main.actionOnlineTrack->setCheckable(true);
	ui_main.actionOfflineTrack->setCheckable(true);
	ui_main.actionTrapezoidal->setCheckable(true);
	ui_main.actionSigmoid->setCheckable(true);

	ui_main.actionCalibrationMethod1->setCheckable(true);
	ui_main.actionCalibrationMethod2->setCheckable(true);
	
	ui_main.actionOnlineTrack->setChecked(true);
	ui_main.actionOfflineTrack->setChecked(false);
	
	ui_main.actionTrapezoidal->setChecked(true);
	ui_main.actionSigmoid->setChecked(false);
	
	ui_main.actionNormal->setChecked(true);
	ui_main.actionPID->setChecked(false);

	ui_main.actionCalibrationMethod1->setCheckable(false);
	ui_main.actionCalibrationMethod2->setCheckable(false);

	//���ú��ӿ�������Ĭ�ϲ���
	
	isWeldTorchTurnedOn=false;

	ui_main.switchOffRadioButton->setChecked(true);
	ui_main.switchOnRadioButton->setChecked(false);

	updateSeamType();
	updateMotionControllerConnectionState();
	updateCameraConnectionState();

}

/***********************************************************************
@��������updateMotionControllerConnectionState
@��������
@����ֵ����
@���ܣ����¿��ƿ��������
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::updateMotionControllerConnectionState()
{
	if(0==cardCount) isControlCardConnected=false;
	else if (1==cardCount) isControlCardConnected=true;

	if(isControlCardConnected)
	{
		ui_main.cardStateDisplayLabel->setPixmap(QPixmap::fromImage(wrongico));
	}
	else
	{
		ui_main.cardStateDisplayLabel->setPixmap(QPixmap::fromImage(checkico));
	}
}

/***********************************************************************
@��������updateCameraConnectionState
@��������
@����ֵ����
@���ܣ���������������
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::updateCameraConnectionState()
{
	if(isCameraConnected)
	{
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(checkico));
	}
	else
	{
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(wrongico));
	}

}



/***********************************************************************
@��������on_actiontrapezoidal_triggered
@��������
@����ֵ����
@���ܣ���Ӧactiontrapezoidal
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_actionTrapezoidal_triggered()
{
	ui_main.actionTrapezoidal->setChecked(true);
	ui_main.actionSigmoid->setChecked(false);
}

/***********************************************************************
@��������on_actionsigmoid_triggered
@��������
@����ֵ����
@���ܣ���Ӧactiontrapezoidal
@�޸���Ϣ��
***********************************************************************/
void  SeamTrackingSys::on_actionSigmoid_triggered()
{
	ui_main.actionTrapezoidal->setChecked(false);
	ui_main.actionSigmoid->setChecked(true);
}

/***********************************************************************
@��������on_actionabout_triggered
@��������
@����ֵ����
@���ܣ���Ӧactionabout
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_actionAbout_triggered()
{
	QMessageBox msgbox;
	msgbox.setWindowTitle("About");
	msgbox.setText(QStringLiteral("�������ϵͳ Version 2.0.0 �㶫ʡ���ӹ��̼����о�������"));
	msgbox.exec();
}


void SeamTrackingSys::on_actionCalibrationMethod1_triggered()
{
	ui_simplecalibration.show();
	ui_main.actionCalibrationMethod1->setChecked(true);
	ui_main.actionCalibrationMethod2->setChecked(false);

}

void SeamTrackingSys::on_actionCalibrationMethod2_triggered()
{

}

void SeamTrackingSys::on_actionCalibrationMethod3_triggered()
{
	ui_fittingcalibration.show();
}


void SeamTrackingSys::on_manualControlPushButton_clicked()
{

	ui_manual.show();
	//ui_manual._manualMotionObject->start();   //�����Ļ����߳̿�����ԭ��δ֪
}

void SeamTrackingSys::updateHorizontalCalibrationRatio(double value)
{
	horizontalCalibrationRatio=value;
	//����horizontalCalibrationRatio��obj_onlinetrack
	emit updateHorizontalCalibrationRatio_triggered(horizontalCalibrationRatio);
}


/***********************************************************************
@��������on_setWeldParametersPushButton_clicked
@��������
@����ֵ����
@���ܣ���Ӧ�����Ӳ������á�groupBox����ġ����á�����
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_setWeldParametersPushButton_clicked()
{
	ui_weld.show();
}

/***********************************************************************
@��������updateWeldParameters
@��������
@����ֵ����
@���ܣ��յ���sendingOutWeldParameter���źź󣬸�����UI��ߵĺ��Ӳ�����Ϣ
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::updateWeldParameters(WeldParameter parameters)
{
	weldparameter=parameters;
	isWeldParametersUpdated=true;
	emit updateWeldParameters_triggered(weldparameter);
	//������ʾ�����ٶ�
}

/***********************************************************************
@��������display1
@������img-;buffer;
@����ֵ����
@���ܣ���ʾ�����ͼ��
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::display1(QImage img, unsigned char* buffer)
{
	if(video0PixItem)
	{
		scene1->removeItem(video0PixItem);
		delete video0PixItem;
		video0PixItem=0;
	}

	frame=img.copy(0,0,img.width(),img.height());	//����ͼ��,���

	if(isSavingMultiImagesCommandTriggered)
	{
		multiImagesNo++;
		multiImagesName=multiImagesFilePath+QString::number(multiImagesNo)+".bmp";
		frame.save(multiImagesName,0,-1);
	}

	/***�̶���С��ʾ***/
	if(!img.isNull())
	{
		QImage img1=img.scaled(ui_main.video0GraphicsView->width(),ui_main.video0GraphicsView->height());
		video0PixItem=scene1->addPixmap(QPixmap::fromImage(img1));
		scene1->setSceneRect(0,0,img1.width(),img1.height());
	}
	if(isDIPStarted && !testOrNot && !isFirstDIPStarted)
	{
			obj_onlinetrack->dip(frame);
			isFirstDIPStarted=true;//�Ժ���startDIP������ͼ����
	}

	m_totalFrame++;
	delete [] buffer;
}


/***********************************************************************
@��������display2
@��������
@����ֵ����
@���ܣ��������ͼƬ����������ʾ����������ߵĴ���
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::display2(QImage image)
{
	if(video0PixItem)
	{
		scene1->removeItem(video0PixItem);
		delete video0PixItem;
		video0PixItem=0;
	}

	frame=image.copy(0,0,image.width(),image.height());
	QImage img1=image.scaled(ui_main.video0GraphicsView->width(),ui_main.video0GraphicsView->height());
	video0PixItem=scene1->addPixmap(QPixmap::fromImage(img1));
	scene1->setSceneRect(0,0,img1.width(),img1.height());

	if(isDIPStarted)
	{
		obj_onlinetrack->dip(frame);
	}
	m_totalFrame++;
}

/***********************************************************************
@��������on_loadTestingDataSetPushButton_clicked
@��������
@����ֵ����
@���ܣ������Ӧ��ť��������ع���
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_loadTestingDataSetsPushButton_clicked()
{
	testOrNot=true;

	obj_imageaquisition->start();
	obj_imageaquisition->stream();			//obj_imageaquisition��ʼ����
}

/***********************************************************************
@��������on_unloadTestingDataSetPushButton_clicked
@��������
@����ֵ����
@���ܣ������Ӧ��ť���ر���ع���
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::on_unloadTestingDataSetsPushButton_clicked()
{
	testOrNot=false;

	obj_imageaquisition->pause();
	
}

/***********************************************************************
@��������loadDefaultCameraConfiguration
@��������
@����ֵ����
@���ܣ�����Ĭ�ϵ�������ò���
@�޸���Ϣ��
***********************************************************************/
void SeamTrackingSys::loadDefaultCameraConfiguration()
{
	QString fileName="..\\camera\\default.xml";
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
				double gamma=xmlReader.readElementText().toInt()/100.0;
				CameraSetGamma(cameraIndex,gamma);
			}
			else if(xmlReader.name()=="Contrast")
			{
				double contrast=xmlReader.readElementText().toInt()/100.0;
				CameraSetContrast(cameraIndex,contrast);
			}
			else if(xmlReader.name()=="Gain")
			{
				CameraSetGain(cameraIndex,xmlReader.readElementText().toInt());
			}
			else if(xmlReader.name()=="Exposure")
			{
				CameraSetExposure(cameraIndex,xmlReader.readElementText().toInt());
			}
		}
	}

	if(xmlReader.hasError())
	{
		QMessageBox::critical(this,
			"xmlFile.xml Parse Error",xmlReader.errorString(),
			QMessageBox::Ok);
		return;
	}

}

void SeamTrackingSys::startDIP()
{
	if(isDIPStarted && isFirstDIPStarted)
		obj_onlinetrack->dip(frame);
}


void SeamTrackingSys::on_actionOfflineTrack_triggered()
{
	ui_main.actionOnlineTrack->setChecked(false);
	ui_offlinetrack.show();
}

void  SeamTrackingSys::on_actionOnlineTrack_triggered()
{

}


void SeamTrackingSys::setDefaultWeldParameters()
{
	weldparameter.weldSpeed=2.0;
	weldparameter.sheildingGasFlow=0.5;
	weldparameter.weldVoltage=25.0;
	weldparameter.weldCurrent=100.0;
	weldparameter.wireFeedRate=2.0;
	weldparameter.trackingDistance=100.0;
	weldparameter.gasPressure=5;
	emit updateWeldParameters_triggered(weldparameter);
}

void SeamTrackingSys::saveImageOfflineTrack(QString fileName)
{
	frame.save(fileName,0,-1);
}

void SeamTrackingSys::updateCameraResolution()
{
	QString cameraResolutionString="480*640";
	ui_main.cameraResolutionDisplayLabel->setText(cameraResolutionString);
}

void SeamTrackingSys::saveCalibrationData(QString fileName)
{
	frame.save(fileName,0,-1);
}

void SeamTrackingSys::saveValidationData(QString fileName)
{
	frame.save(fileName,0,-1);
}

void SeamTrackingSys::saveSeamLaserStripeData(QString fileName)
{
	frame.save(fileName,0,-1);
}

void SeamTrackingSys::initializeVariable()
{
	cameraIndex=0;
	fpsValue=0;
	m_totalFrame=0;
	receivedFrameNumber=0;
	multiImagesNo=0;
	multiImagesFilePath="..\\Images\\Multi";

	isDIPStarted=false;
	hasAligned=false;
	isWeldParametersUpdated=false;
	isFirstDIPStarted=false;
	isSavingMultiImagesCommandTriggered=false;

	weldSeamType=BUTT_WELD;
	
	checkico=QImage("check_16px.ico");
	wrongico=QImage("wrong_16px.ico");
	unknownico=QImage("unknown_16px.ico");
}