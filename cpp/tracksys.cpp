/******************************************************************************
  FileName: tracksys.cpp
  Author: WillLi		Date:2019-5-18
  Description:
	          SeamTrackingSys��ʵ�֡�������Ϣ�Ľ�����ui�����ȡ�
  Function List:
                1. SeamTrackingSys				// ���ʼ��
                2. ~SeamTrackingSys				// ������ʱ���ͷ���Դ
                3. setDefaultUiConfiguration	// ����Ĭ�ϵĲ˵�����
                4. initializeVariable			//��ʼ�����˽�б���
                5. on_turnOnCameraPushButton_clicked	// �����
                6. on_turnOffCameraPushButton_clicked	// �����
                7. loadDefaultCameraConfiguration		// ���������Ĭ������
                8. updateCameraConnectionState			// ��������������
                9. on_cameraResolutionComBox_currentIndexChanged	// �ı�ֱ���
                10. updateCameraResolution				//��������ֱ���
                11. display1					// ��ʾ�����ͼ��
                12. display2					// ��ʾ�������ݼ���ͼ��
                13. saveCalibrationData		// �������У��ͼ��
                14. saveValidationData		// �������У����������֤��ͼ��
                15. saveSeamLaserStripeData	// ���漤������ͼ��
                16. on_cameraParametersSettingPushButton_clicked	// ����camera����
                17. updateFPSValue			// �������FPS
                18. saveSingleImage			// ���浥��ͼƬ
                19. saveMultiImages			// �����ر���ͼ��
                20. stopSavingImages		// ֹͣ�������ͼƬ
                21. saveImageOfflineTrack	// ��������ģʽ�µ�ͼ��
                22. startDIP				//����ͼ����
                23. updateDIPResult			// ����ʶ�����Ľ��
                24. updateDIPCalculationResult			// ����ͼ����ļ�����
                25. updateHorizontalCalibrationRatio	// ����ˮƽ�궨����
                26. updateSeamType			// ���º��������
                27. on_loadTestingDataSetsPushButton_clicked	// �򿪲������ݼ�
                28. on_unloadTestingDataSetPushButton_clicked	// �رղ������ݼ�
                29. on_quitPushButton_clicked				// �ͷ������Դ���ͷſ��ƿ�
                30. on_turnOnRecognitionPushButton_clicked	// ��ͼ��ʶ����
                31. on_turnOffRecognitonPushButton_clicked	// ��ͼ��ʶ����
                32. updateMotionControllerConnectionState	// ���¿��ƿ��������
                33. on_actiontrapezoidal_triggered			// �����˶�����ΪS�ͼ���
                34. on_actionsigmoid_triggered				// �����˶�����Ϊ���μ���
                35. on_manualControlPushButton_clicked		// �ֶ�����
                36. on_alignPushButton_clicked				// ��ͼ����Ұ�м��׼��������
                37. on_startAutoTrackPushButton_clicked		// �����Զ�����
                38. on_stopAutoTrackPushButton_clicked		// ֹͣ�Զ����ٵĹ���
                39. setDefaultWeldParameters				// ����Ĭ�ϵĺ���ͼ��
                40. updateWeldParameters					// ���º��Ӳ���
                41. on_setWeldParametersPushButton_clicked	// ���ú��Ӳ���
                42. closeEvent								// �رմ����¼�
                43. on_actionOfflineTrack_triggered			// ���ߺ���
                44. on_actionOnlineTrack_triggered			// ���ߺ���
                45. on_actionabout_triggered				// ���������ڡ���Ϣ
                46. on_actionCalibrationMethod1_triggered	// ���б궨����1-���ױ궨
                47. on_actionCalibrationMethod2_triggered	// ���б궨����2-�ݰұ궨
                48. on_actionCalibrationMethod3_triggered	// ���б궨����3-��ϱ궨
  History:
         <author>		<time>     <desc>
          WillLi99    2019-5-18     ���tracksys.cppͷ��ע�ͺ͸�������ͷ��ע��  
******************************************************************************/

#include "JHCap.h"
#include "tracksys.h"

/******************************************************************************
 Name:g_isCameraWorking
 Scope:true/false
 Used by:
 Others:
******************************************************************************/
bool g_isCameraWorking=false;


/******************************************************************************
  Function:SeamTrackingSys
  Description:
             1. ע���Զ�������ݽṹ
			 2. ����һЩQt���
			 3. ����Qt��Ϣ��
			 4. ����һЩ����Ϳ��ƿ�������
  Calls: 
		d1000_board_init setDefaultUiConfiguration initializeVariable
		setDefaultWeldParameters CameraInit CameraSetResolution
  Called By: SeamTrackingSys��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
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
	obj_imageaquisition=new ImageAcquisition(this);

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
	connect(this,SIGNAL(triggerTestingMode_triggered()),obj_imageaquisition,SLOT(tirggerTestingMode()));
	connect(this,SIGNAL(detriggerTestingMode_triggered()),obj_imageaquisition,SLOT(detriggerTestingMode_triggered()));

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
	connect(&ui_simplecalibration,SIGNAL(updateCalibrationState_triggered()),this,SLOT(updateCalibrationState()));
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


/******************************************************************************
  Function:~SeamTrackingSys
  Description: ж�ؿ��ƿ���deleteһЩ����
  Calls: 
		d1000_board_close
  Called By: SeamTrackingSys��
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
SeamTrackingSys::~SeamTrackingSys()
{
	d1000_board_close();
	delete scene1;
	delete scene2;
	delete obj_imageaquisition;
}


/******************************************************************************
  Function:setDefaultUiConfiguration
  Description: ���ò˵������������ϵ�һЩ����
  Calls: 
		1. updateSeamType
		2. updateMotionControllerConnectionState
		3. updateCameraConnectionState
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
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

/******************************************************************************
  Function:initializeVariable
  Description: ��ʼ��SeamTrackingSys��ľֲ�����
  Calls: 
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::initializeVariable()
{
	cameraIndex=0;
	fpsValue=0;
	m_totalFrame=0;
	receivedFrameNumber=0;
	multiImagesNo=0;
	multiImagesFilePath="..\\Images\\Multi";

	isDIPStarted=false;
	isAlignmentFinished=false;
	isWeldParametersUpdated=false;
	isFirstDIPStarted=false;
	isSavingMultiImagesCommandTriggered=false;
	isCalibrationFinished=false;
	isTestModeTriggered=false;

	weldSeamType=BUTT_WELD_SEAM;

	checkico=QImage("check_16px.ico");
	wrongico=QImage("wrong_16px.ico");
	unknownico=QImage("unknown_16px.ico");
}


/******************************************************************************
  Function:on_turnOnCameraPushButton_clicked
  Description: ��������߳�
  Calls: 
        1. CameraGetResolutionCount
		2. loadDefaultCameraConfiguration
		3. CameraGetResolution
		4. CameraSetResolution
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: ����������on_****_clicked����on_****_triggered�ȶ���һ�ֶ�����Ӧ����
******************************************************************************/
void SeamTrackingSys::on_turnOnCameraPushButton_clicked()
{
	if(isCameraConnected==true)
	{

		isTestModeTriggered=false;
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
		g_isCameraWorking=true;
	}
	else
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���δ���ӣ������������"));
		msgbox.exec();
	}
}

/******************************************************************************
  Function:on_turnOffCameraPushButton_clicked
  Description: �ر����
  Calls: 
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_turnOffCameraPushButton_clicked()
{
	isTestModeTriggered=true;
	isDIPStarted=false;
	g_isCameraWorking=false;
	obj_imageaquisition->pause();

	if(!frame.isNull())
	{
		QImage img1=frame.scaled(ui_main.video0GraphicsView->width(),ui_main.video0GraphicsView->height());
		video0PixItem=scene1->addPixmap(QPixmap::fromImage(img1));
		scene1->setSceneRect(0,0,img1.width(),img1.height());
	}
}

/******************************************************************************
  Function:loadDefaultCameraConfiguration
  Description: ����Ĭ�ϵ��������
  Calls:
        1. CameraSetGamma
        2. CameraSetContrast
		3. CameraSetGain
		4. CameraSetExposure
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
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

/******************************************************************************
  Function:updateCameraConnectionState
  Description: �������������״̬
  Calls:
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
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


/******************************************************************************
  Function:on_cameraResolutionComBox_currentIndexChanged
  Description: �ı�����ķֱ���
  Calls:
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
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

/******************************************************************************
  Function: updateCameraResolution
  Description: ��������ֱ���Ϊ480*640��
  Calls:
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateCameraResolution()
{
	QString cameraResolutionString="480*640";
	ui_main.cameraResolutionDisplayLabel->setText(cameraResolutionString);
}


/******************************************************************************
  Function: display1
  Description: ��ʾ����ɼ���ͼƬ
  Calls:
  Called By: 
  Input: img-ͼ�� buffer-ͼ����ڴ�λ��
  Output: 
  Return:       
  Others: 
******************************************************************************/
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
	if(isDIPStarted && !isTestModeTriggered && !isFirstDIPStarted)
	{
		obj_onlinetrack->dip(frame);
		isFirstDIPStarted=true;//�Ժ���startDIP������ͼ����
	}

	m_totalFrame++;
	delete [] buffer;
}

/******************************************************************************
  Function: display2
  Description: ��ʾ����ͼ��
  Calls:
  Called By: 
  Input: image-����ͼ��
  Output: 
  Return:       
  Others: 
******************************************************************************/
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

/******************************************************************************
  Function: saveCalibrationData
  Description: ��������У����ͼƬ
  Calls:
  Called By:  
  Input: fileName-ͼ���Ŀ¼���ļ���
  Output: 
  Return:       
  Others: ��Ӧui_fittingcalibration��saveCalibrationData_triggered
******************************************************************************/
void SeamTrackingSys::saveCalibrationData(QString fileName)
{
	frame.save(fileName,0,-1);
}

/******************************************************************************
  Function: saveValidationData
  Description: ����������֤У�����ȵ�ͼƬ
  Calls:
  Called By:  
  Input: fileName-ͼ���Ŀ¼���ļ���
  Output: 
  Return:       
  Others: ��Ӧui_fittingcalibration��saveValidationData_triggered
******************************************************************************/
void SeamTrackingSys::saveValidationData(QString fileName)
{
	frame.save(fileName,0,-1);
}

/******************************************************************************
  Function: saveSeamLaserStripeData
  Description: ���溸�������Ľṹ������ͼ��
  Calls:
  Called By:  
  Input: fileName-ͼ���Ŀ¼���ļ���
  Output: 
  Return:       
  Others: ��Ӧui_fittingcalibration��saveSeamLaserStripeData_triggered���������
		�ɼ���ͼ������ڽ��к������ά�ؽ�
******************************************************************************/
void SeamTrackingSys::saveSeamLaserStripeData(QString fileName)
{
	frame.save(fileName,0,-1);
}


/******************************************************************************
  Function: on_cameraParametersSettingPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::on_cameraParametersSettingPushButton_clicked()
{
	ui_camera.show();	//����cameraControl����
}

/******************************************************************************
  Function: updateFPSValue
  Description: ����֡����ʾ
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::updateFPSValue()
{
	/*if(fpsValue==0) fpsValue=m_totalFrame;
	else fpsValue=0.4*fpsValue+0.6*m_totalFrame;*/
	fpsValue=m_totalFrame;
	m_totalFrame=0;
	fpsLabel->setText(QString("FPS:%1").arg(QString::number(fpsValue, 'f', 2)));
}

/******************************************************************************
  Function: saveImage
  Description: ����ͼƬ
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
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

/******************************************************************************
  Function: saveMultiImages
  Description: �������ͼƬ������Ŀ¼"../Images/"
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::saveMultiImages()
{
	//emit saveImages_triggered();	//��signal����obj_imageaquisition����
	isSavingMultiImagesCommandTriggered=true;
}


/******************************************************************************
  Function: stopSavingImages
  Description: ֹͣ�������ͼƬ
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::stopSavingImages()
{
	//emit stopSavingImages_triggered();//��signal����obj_imageaquisition����
	isSavingMultiImagesCommandTriggered=false;
	multiImagesNo=0;
}

/******************************************************************************
  Function: saveImageOfflineTrack
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::saveImageOfflineTrack(QString fileName)
{
	frame.save(fileName,0,-1);
}

/******************************************************************************
  Function: startDIP
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::startDIP()
{
	if(isDIPStarted && isFirstDIPStarted)
		obj_onlinetrack->dip(frame);
}

/******************************************************************************
  Function: updateDIPResult
  Description: ����ʶ������ʾ
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
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

/******************************************************************************
  Function: updateDIPCalculationResult
  Description: ����ƫ��ֵ
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateDIPCalculationResult(SeamInfo seaminfo)
{
	receivedFrameNumber++;
	if(receivedFrameNumber!=0)
	{
		lastSeamOffset=currentSeamOffset;
	}
	currentSeamOffset=seaminfo.dWeldSeamOffset*horizontalCalibrationRatio;
	double sw=int(seaminfo.dWeldSeamWidth*horizontalCalibrationRatio*1000)/1000.0;
	QString seamWidth=QString::number(sw);

	QString weldTorchHeight=QString::number(seaminfo.dRemainingHeight);

	double co=int(seaminfo.dWeldSeamOffset*horizontalCalibrationRatio*1000)/1000.0;
	QString weldingcurrentOffset=QString::number(co);
	ui_main.seamWidthDisplayLabel->setText(seamWidth);
	ui_main.seamRemainingHeightDisplayLabel->setText(weldTorchHeight);
	ui_main.seamOffsetDisplayLabel->setText(weldingcurrentOffset);

}

/******************************************************************************
  Function: updateHorizontalCalibrationRatio
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateHorizontalCalibrationRatio(double value)
{
	horizontalCalibrationRatio=value;
	//����horizontalCalibrationRatio��obj_onlinetrack
	emit updateHorizontalCalibrationRatio_triggered(horizontalCalibrationRatio);
}

/******************************************************************************
  Function: updateSeamType
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateSeamType()
{
	switch(weldSeamType)
	{
	case BUTT_WELD_SEAM:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("�ԽӺ���"));
		break;
	case LAP_WELD_SEAM:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("��Ӻ���"));
		break;
	case GROOVE_WELD_SEAM:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("V�ͺ���"));
		break;
	}
}

/******************************************************************************
  Function: on_loadTestingDataSetsPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_loadTestingDataSetsPushButton_clicked()
{
	emit triggerTestingMode_triggered();

	obj_imageaquisition->start();
	obj_imageaquisition->stream();			//obj_imageaquisition��ʼ����
}

/******************************************************************************
  Function: on_unloadTestingDataSetsPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_unloadTestingDataSetsPushButton_clicked()
{
	emit detriggerTestingMode_triggered();

	obj_imageaquisition->pause();

}

/******************************************************************************
  Function: on_quitPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_quitPushButton_clicked()
{
	QMessageBox msgBox;
	msgBox.setText(QStringLiteral("ȷ���˳���"));
	msgBox.exec();
	CameraFree(cameraIndex);
	d1000_board_close();
	exit(0);
}

/******************************************************************************
  Function: on_turnOnRecognitionPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_turnOnRecognitionPushButton_clicked()
{
	if(g_isCameraWorking)
	{
		isTestModeTriggered=false;
		isDIPStarted=true;
	}
	else
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("���δ�������ȿ��������"));
		msgbox.exec();
	}
}

/******************************************************************************
  Function: on_turnOffRecognitonPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_turnOffRecognitonPushButton_clicked()
{
	isDIPStarted=false;
	isFirstDIPStarted=false;
}

/******************************************************************************
  Function: updateMotionControllerConnectionState
  Description: ���¿��ƿ��������
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
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

/******************************************************************************
  Function: on_actionSigmoid_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void  SeamTrackingSys::on_actionSigmoid_triggered()
{
	ui_main.actionTrapezoidal->setChecked(false);
	ui_main.actionSigmoid->setChecked(true);
}

/******************************************************************************
  Function: on_actionTrapezoidal_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionTrapezoidal_triggered()
{
	ui_main.actionTrapezoidal->setChecked(true);
	ui_main.actionSigmoid->setChecked(false);
}

/******************************************************************************
  Function: on_manualControlPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_manualControlPushButton_clicked()
{
	ui_manual.show();
}

/******************************************************************************
  Function: on_alignPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
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
		if(!isCalibrationFinished)
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
			isAlignmentFinished=true;
		}
	}
}

/******************************************************************************
  Function: on_startAutoTrackPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_startAutoTrackPushButton_clicked()
{
	//check
	if(!g_isCameraWorking)
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

	if(!isAlignmentFinished)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("δ���룡����ִ�ж�������"));
		msgbox.exec();
		return;
	}

	isAutoTrackTriggered=true;
	qDebug()<<"isAutoTrackTriggered="<<isAutoTrackTriggered;
}


/******************************************************************************
  Function: on_stopAutoTrackPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_stopAutoTrackPushButton_clicked()
{
	isAutoTrackTriggered=false;
	qDebug()<<"isAutoTrackTriggered="<<isAutoTrackTriggered;
	emit stopAutoTracking_triggered();

	d1000_decel_stop(0);
	d1000_decel_stop(1);
	d1000_decel_stop(2);
	d1000_decel_stop(3);
}

/******************************************************************************
  Function: setDefaultWeldParameters
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::setDefaultWeldParameters()
{
	weldparameter.dWeldSpeed=2.0;
	weldparameter.dGasFlow=0.5;
	weldparameter.dWeldVoltage=25.0;
	weldparameter.dWeldCurrent=100.0;
	weldparameter.dWireRate=2.0;
	weldparameter.dWeldDistance=100.0;
	weldparameter.dGasPressure=5;
	emit updateWeldParameters_triggered(weldparameter);
}

/******************************************************************************
  Function: updateWeldParameters
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateWeldParameters(WeldParameter parameters)
{
	weldparameter=parameters;
	isWeldParametersUpdated=true;
	emit updateWeldParameters_triggered(weldparameter);
	//������ʾ�����ٶ�
}


/******************************************************************************
  Function: on_setWeldParametersPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_setWeldParametersPushButton_clicked()
{
	ui_weld.show();
}

/******************************************************************************
  Function: closeEvent
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::closeEvent(QCloseEvent *e)
{
	obj_imageaquisition->stop();
	obj_imageaquisition->wait();

	CameraFree(cameraIndex);
	d1000_board_close();
	exit(0);
	QMainWindow::closeEvent(e);

}

/******************************************************************************
  Function: on_actionOnlineTrack_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void  SeamTrackingSys::on_actionOnlineTrack_triggered()
{

}

/******************************************************************************
  Function: on_actionOfflineTrack_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionOfflineTrack_triggered()
{
	ui_main.actionOnlineTrack->setChecked(false);
	ui_offlinetrack.show();
}

/******************************************************************************
  Function: on_actionAbout_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionAbout_triggered()
{
	QMessageBox msgbox;
	msgbox.setWindowTitle("About");
	msgbox.setText(QStringLiteral("�������ϵͳ Version 2.0.0 "));
	msgbox.exec();
}

/******************************************************************************
  Function: on_actionCalibrationMethod1_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibrationMethod1_triggered()
{
	ui_simplecalibration.show();
	ui_main.actionCalibrationMethod1->setChecked(true);
	ui_main.actionCalibrationMethod2->setChecked(false);

}

/******************************************************************************
  Function: on_actionCalibrationMethod2_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibrationMethod2_triggered()
{

}

/******************************************************************************
  Function: on_actionCalibrationMethod3_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibrationMethod3_triggered()
{
	ui_fittingcalibration.show();
}

void SeamTrackingSys::updateCalibrationState()
{
	isCalibrationFinished=true;
}