/******************************************************************************
  FileName: tracksys.cpp
  Author: WillLi		Date:2019-5-18
  Description:
	          SeamTrackingSys��ʵ�֡�������Ϣ�Ľ�����ui�����ȡ�
  Function List:
                SeamTrackingSys				// ���ʼ��
                ~SeamTrackingSys				// ������ʱ���ͷ���Դ
                setDefaultUiConfiguration	// ����Ĭ�ϵĲ˵�����
                initializeVariable			//��ʼ�����˽�б���
                on_pushButtonTurnOnCamera_clicked	// �����
                on_pushButtonTurnOffCamera_clicked	// �����
                loadDefaultCameraConfiguration		// ���������Ĭ������
                updateCameraConnectionState			// ��������������
                on_cameraResolutionComBox_currentIndexChanged	// �ı�ֱ���
                updateCameraResolution				//��������ֱ���
                display1					// ��ʾ�����ͼ��
                display2					// ��ʾ�������ݼ���ͼ��
                saveCalibrationData		// �������У��ͼ��
                saveValidationData		// �������У����������֤��ͼ��
                saveSeamLaserStripeData	// ���漤������ͼ��
                on_pushButtonCameraParametersSetting_clicked	// ����camera����
                updateFPSValue			// �������FPS
                saveSingleImage			// ���浥��ͼƬ
                saveMultiImages			// �����ر���ͼ��
                stopSavingImages		// ֹͣ�������ͼƬ
                saveImageOfflineTrack	// ��������ģʽ�µ�ͼ��
                startDIP				//����ͼ����
                updateDIPResult			// ����ʶ�����Ľ��
                updateDIPCalculationResult			// ����ͼ����ļ�����
                updateHorizontalCalibrationRatio	// ����ˮƽ�궨����
                updateSeamType			// ���º��������
                on_pushButtonLoadTestData_clicked	// �򿪲������ݼ�
                on_unloadTestingDataSetPushButton_clicked	// �رղ������ݼ�
                on_pushButtonQuit_clicked				// �ͷ������Դ���ͷſ��ƿ�
                on_pushButtonTurnOnRecognition_clicked	// ��ͼ��ʶ����
                on_pushButtonTurnOffRecognition_clicked	// ��ͼ��ʶ����
                updateMotionControllerConnectionState	// ���¿��ƿ��������
                on_actiontrapezoidal_triggered			// �����˶�����ΪS�ͼ���
                on_actionsigmoid_triggered				// �����˶�����Ϊ���μ���
                on_pushButtonManualControl_clicked		// �ֶ�����
                on_pushButtonAlign_clicked				// ��ͼ����Ұ�м��׼��������
                on_pushButtonStartAutoTrack_clicked		// �����Զ�����
                on_pushButtonStopAutoTrack_clicked		// ֹͣ�Զ����ٵĹ���
                setDefaultWeldParameters				// ����Ĭ�ϵĺ���ͼ��
                updateWeldParameters					// ���º��Ӳ���
                on_pushButtonSetWeldParameters_clicked	// ���ú��Ӳ���
                closeEvent								// �رմ����¼�
                on_actionabout_triggered				// ���������ڡ���Ϣ
                on_actionCalibrationMethod1_triggered	// ���б궨����1-���ױ궨
                on_actionCalibrationMethod2_triggered	// ���б궨����2-�ݰұ궨
                on_actionCalibrationMethod3_triggered	// ���б궨����3-��ϱ궨
  History:
         <author>		<time>     <desc>
          WillLi99    2019-5-18     ���tracksys.cppͷ��ע�ͺ͸�������ͷ��ע��
		  Will99	  2019-7-8      ɾ����actionOnlineTrack��actionOfflineTrack�������
		                            ������trackMethodMenu����
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
	qRegisterMetaType<SeamTrackParameter> ("SeamTrackParameter");

	qDebug()<<"***AutoTracking System***";
	ui_main.setupUi(this);
	intCardCount=d1000_board_init();
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
	setDefaultSeamTrackParameters();

	m_timer=new QTimer(this);

	connect(this,SIGNAL(saveImages_triggered()),obj_imageaquisition,SLOT(allowSavingMultipleImages()));
	connect(this,SIGNAL(stopSavingImages_triggered()),obj_imageaquisition,SLOT(declineSavingMultipleImages()));
	connect(this,SIGNAL(triggerTestingMode_triggered()),obj_imageaquisition,SLOT(tirggerTestingMode()));
	connect(this,SIGNAL(detriggerTestingMode_triggered()),obj_imageaquisition,SLOT(detriggerTestingMode()));

	connect(this,SIGNAL(autoTrack_triggered(SeamTrackParameter)),obj_onlinetrack,SLOT(enableAutoTrack(SeamTrackParameter)));
	connect(this,SIGNAL(stopAutoTracking_triggered()),obj_onlinetrack,SLOT(disableAutoTrack()));
	connect(this,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),obj_onlinetrack,SLOT(syncHorizontalCalibrationRatio(double)));
	connect(this,SIGNAL(sendReferenceOffset_triggered(double)),obj_onlinetrack,SLOT(receiveReferenceOffset(double)));
	connect(this,SIGNAL(updateTrackMethod_triggered(int)),obj_onlinetrack,SLOT(syncTrackMethod(int)));
	connect(this,SIGNAL(triggerTestingMode_triggered()),obj_onlinetrack,SLOT(tirggerTestingMode()));
	connect(this,SIGNAL(detriggerTestingMode_triggered()),obj_onlinetrack,SLOT(detriggerTestingMode()));


	connect(this,SIGNAL(updateWeldParameters_triggered(WeldParameter)),&ui_offlinetrack,SLOT(syncWeldParameters(WeldParameter)));
	connect(this,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),&ui_offlinetrack,SLOT(syncHorizontalCalibrationRatio(double)));
	connect(&ui_offlinetrack,SIGNAL(saveImage_triggered(QString)),this,SLOT(saveImageOfflineTrack(QString)));

	connect(obj_onlinetrack,SIGNAL(sendDIPResult_triggered(QImage)),this,SLOT(updateDIPResult(QImage)));
	connect(obj_onlinetrack,SIGNAL(sendDIPResult2_triggered(QImage)),this,SLOT(updateDIPResult(QImage)));
	connect(obj_onlinetrack,SIGNAL(sendDIPCalculationResult_triggered(SeamInfo)),this,SLOT(updateDIPCalculationResult(SeamInfo)));
	connect(obj_onlinetrack,SIGNAL(enableNextDIP_triggered()),this,SLOT(startDIP()));

	connect(obj_imageaquisition,SIGNAL(captured(QImage,unsigned char *)),this,SLOT(display1(QImage,unsigned char *)));
	connect(obj_imageaquisition,SIGNAL(captured2(QImage)),this,SLOT(display2(QImage)));

	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateFPSValue()));

	connect(&ui_camera,SIGNAL(saveSingleImage_triggered()),this,SLOT(saveSingleImage()));
	connect(&ui_camera,SIGNAL(saveMultipleImages_triggered()),this,SLOT(saveMultiImages()));
	connect(&ui_camera,SIGNAL(stopSavingMultipleImages_triggered()),this,SLOT(stopSavingImages()));
		
	connect(&ui_simplecalibration,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),this,SLOT(syncHorizontalCalibrationRatio(double)));
	connect(&ui_simplecalibration,SIGNAL(updateCalibrationState_triggered()),this,SLOT(syncCalibrationState()));
	connect(&ui_simplecalibration,SIGNAL(getBlockWidthPixel_triggered()),this,SLOT(sendBlockWidthPixel()));
	connect(this,SIGNAL(sendBlockWidthPixel_triggered(double)),&ui_simplecalibration,SLOT(receiveBlockWidthPixel(double)));
	connect(&ui_weld,SIGNAL(updateWeldParameters_triggered(WeldParameter)),this,SLOT(syncWeldParameters(WeldParameter)));
	connect(&ui_weld,SIGNAL(updateSeamTrackParameters_triggerd(SeamTrackParameter)),this,SLOT(syncSeamTrackParameters(SeamTrackParameter)));
	
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
		ui_main.label_cameraState->setPixmap(QPixmap::fromImage(checkico));
		CameraInit(intCameraIndex);
		CameraSetResolution(intCameraIndex,5,&intImageWidth,&intImageHeight);
		cmbNotResponding=true;
		cmbNotResponding=false;
		
		qDebug()<<"Resolution= "<<intImageWidth<<"*"<<intImageHeight;

	}
	else if(cameraCount==0)
	{
		isCameraConnected=false;
		ui_main.label_cameraState->setPixmap(QPixmap::fromImage(wrongico));
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

	ui_main.actionTrackMethod1->setCheckable(true);
	ui_main.actionTrackMethod2->setCheckable(true);
	ui_main.actionTrackMethod3->setCheckable(true);
	ui_main.actionTrackMethod4->setCheckable(true);
	ui_main.actionTrackMethod1->setChecked(true);

	ui_main.actionTrapezoidal->setCheckable(true);
	ui_main.actionSigmoid->setCheckable(true);

	ui_main.actionCalibration1->setCheckable(true);
	ui_main.actionCalibration2->setCheckable(true);

	ui_main.actionTrapezoidal->setChecked(true);
	ui_main.actionSigmoid->setChecked(false);

	ui_main.actionNormal->setChecked(true);
	ui_main.actionPID->setChecked(false);

	ui_main.actionCalibration1->setCheckable(false);
	ui_main.actionCalibration2->setCheckable(false);

	//���ú��ӿ�������Ĭ�ϲ���

	isWeldTorchTurnedOn=false;

	ui_main.radioButtonSwitchOff->setChecked(true);
	ui_main.RadioButtonSwitchOn->setChecked(false);

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
	intCameraIndex=0;
	dFpsValue=0;
	intTotalFrame=0;
	longReceivedFrameNumber=0;
	intMultipleImagesNo=0;
	dReferenceOffset=0.0;
	qsMultipleImagesFilePath="..\\Images\\Multi";

	isDIPStarted=false;
	isAlignmentFinished=false;
	isWeldParametersUpdated=false;
	isFirstDIPStarted=false;
	isSavingMultiImagesCommandTriggered=false;
	isCalibrationFinished=false;
	isTestModeTriggered=false;

	stWeldSeamType=BUTT_WELD_SEAM;

	checkico=QImage("check_16px.ico");
	wrongico=QImage("wrong_16px.ico");
	unknownico=QImage("unknown_16px.ico");

	intTrackMethod=1;
	emit updateTrackMethod_triggered(intTrackMethod);
}


/******************************************************************************
  Function:on_pushButtonTurnOnCamera_clicked
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
void SeamTrackingSys::on_pushButtonTurnOnCamera_clicked()
{
	if(isCameraConnected==true)
	{

		isTestModeTriggered=false;
		obj_imageaquisition->start();
		obj_imageaquisition->stream();			//obj_imageaquisition��ʼ����

		//���·ֱ����б�
		int count;
		CameraGetResolutionCount(intCameraIndex,&count);
		loadDefaultCameraConfiguration();
		for(int i=0; i<count; i++)
		{
			int intImageWidth, intImageHeight;
			CameraGetResolution(intCameraIndex,i, &intImageWidth, &intImageHeight);
			ui_main.comboxCameraResolution->addItem(QString("%1x%2").arg(intImageWidth).arg(intImageHeight));
		}

		CameraSetResolution(intCameraIndex,5,0,0);//����Ĭ�Ϸֱ�����800*600
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
  Function:on_pushButtonTurnOffCamera_clicked
  Description: �ر����
  Calls: 
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonTurnOffCamera_clicked()
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
				CameraSetGamma(intCameraIndex,gamma);
			}
			else if(xmlReader.name()=="Contrast")
			{
				double contrast=xmlReader.readElementText().toInt()/100.0;
				CameraSetContrast(intCameraIndex,contrast);
			}
			else if(xmlReader.name()=="Gain")
			{
				CameraSetGain(intCameraIndex,xmlReader.readElementText().toInt());
			}
			else if(xmlReader.name()=="Exposure")
			{
				CameraSetExposure(intCameraIndex,xmlReader.readElementText().toInt());
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
		ui_main.label_cameraState->setPixmap(QPixmap::fromImage(checkico));
	}
	else
	{
		ui_main.label_cameraState->setPixmap(QPixmap::fromImage(wrongico));
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
void SeamTrackingSys::on_comboxCameraResolution_currentIndexChanged(int index)
{
	CameraGetResolution(intCameraIndex,index,&intImageWidth,&intImageHeight);
	if(cmbNotResponding==false)
	{
		obj_imageaquisition->pause();
		CameraSetResolution(intCameraIndex,index, 0, 0);
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
	ui_main.label_cameraResolution->setText(cameraResolutionString);
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
		intMultipleImagesNo++;
		qsMultipleImagesName=qsMultipleImagesFilePath+QString::number(intMultipleImagesNo)+".bmp";
		frame.save(qsMultipleImagesName,0,-1);
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

	intTotalFrame++;
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

	if(isDIPStarted && isTestModeTriggered && !isFirstDIPStarted)
	{
		obj_onlinetrack->dip(frame);
		isFirstDIPStarted=true;
	}
	intTotalFrame++;
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
  Function: on_pushButtonCameraParametersSetting_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::on_pushButtonCameraParametersSetting_clicked()
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
	/*if(dFpsValue==0) dFpsValue=intTotalFrame;
	else dFpsValue=0.4*dFpsValue+0.6*intTotalFrame;*/
	dFpsValue=intTotalFrame;
	intTotalFrame=0;
	fpsLabel->setText(QString("FPS:%1").arg(QString::number(dFpsValue, 'f', 2)));
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
	intMultipleImagesNo=0;
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
	longReceivedFrameNumber++;
	siSeamInfo=seaminfo;

	dCurrentOffset=seaminfo.dWeldSeamOffset*dHorizontalCalibrationRatio;
	double sw=int(seaminfo.dWeldSeamWidth*dHorizontalCalibrationRatio*1000)/1000.0;
	QString seamWidth=QString::number(sw);

	QString weldTorchHeight=QString::number(seaminfo.dRemainingHeight);

	double co=int(seaminfo.dWeldSeamOffset*dHorizontalCalibrationRatio*1000)/1000.0;
	QString weldingcurrentOffset=QString::number(co);
	ui_main.label_seamWidth->setText(seamWidth);
	ui_main.label_seamRemainingHeight->setText(weldTorchHeight);
	ui_main.label_seamOffset->setText(weldingcurrentOffset);
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
void SeamTrackingSys::syncHorizontalCalibrationRatio(double horizontalCalibrationRate)
{
	dHorizontalCalibrationRatio=horizontalCalibrationRate;
	//����horizontalCalibrationRatio��obj_onlinetrack
	emit updateHorizontalCalibrationRatio_triggered(dHorizontalCalibrationRatio);
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
	switch(stWeldSeamType)
	{
	case BUTT_WELD_SEAM:ui_main.label_seamType->setText(QStringLiteral("�ԽӺ���"));
		break;
	case LAP_WELD_SEAM:ui_main.label_seamType->setText(QStringLiteral("��Ӻ���"));
		break;
	case GROOVE_WELD_SEAM:ui_main.label_seamType->setText(QStringLiteral("V�ͺ���"));
		break;
	}
}

/******************************************************************************
  Function: on_pushButtonLoadTestData_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonLoadTestData_clicked()
{
	emit triggerTestingMode_triggered();
	isTestModeTriggered=true;

	obj_imageaquisition->start();
	obj_imageaquisition->stream();			//obj_imageaquisition��ʼ����
}

/******************************************************************************
  Function: on_pushButtonUnloadTestDataSets_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonUnloadTestDataSets_clicked()
{
	emit detriggerTestingMode_triggered();
	obj_imageaquisition->pause();

}

/******************************************************************************
  Function: on_pushButtonQuit_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonQuit_clicked()
{
	QMessageBox msgBox;
	msgBox.setText(QStringLiteral("ȷ���˳���"));
	msgBox.exec();
	CameraFree(intCameraIndex);
	d1000_board_close();
	exit(0);
}

/******************************************************************************
  Function: on_pushButtonTurnOnRecognition_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonTurnOnRecognition_clicked()
{
	if(g_isCameraWorking)
	{
		isTestModeTriggered=false;
		isDIPStarted=true;
	}
	else
	{
		if(!isTestModeTriggered)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("���δ�������ȿ��������"));
			msgbox.exec();
		}
		else //����ģʽ�µ�ʶ��
		{
			isDIPStarted=true;
		}
	}
}

/******************************************************************************
  Function: on_pushButtonTurnOffRecognition_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonTurnOffRecognition_clicked()
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
	if(0==intCardCount) isControlCardConnected=false;
	else if (1==intCardCount) isControlCardConnected=true;

	if(isControlCardConnected)
	{
		ui_main.label_cardState->setPixmap(QPixmap::fromImage(wrongico));
	}
	else
	{
		ui_main.label_cardState->setPixmap(QPixmap::fromImage(checkico));
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
  Function: on_pushButtonManualControl_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonManualControl_clicked()
{
	ui_manual.show();
}

/******************************************************************************
  Function: on_pushButtonAlign_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonAlign_clicked()
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
		//qDebug()<<"currentSeamOffset:"<<currentSeamOffset;
		if(abs(dCurrentOffset)>tolerance)
		{
			obj_automotion.xMove(1,dCurrentOffset,0.5,1);
		}
		else
		{
			isAlignmentFinished=true;
		}
	}
}

/******************************************************************************
  Function: on_pushButtonStartAutoTrack_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonStartAutoTrack_clicked()
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

	if(!isCalibrationFinished)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("У��δ��ɣ�"));
		msgbox.exec();
		return;
	}
	if(isCalibrationFinished)
	{
		isAutoTrackTriggered=true;		
		setReferenceOffset();
		emit sendReferenceOffset_triggered(dReferenceOffset);
		emit autoTrack_triggered(spSeamTrackParameter);
	}
}


/******************************************************************************
  Function: on_pushButtonStopAutoTrack_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonStopAutoTrack_clicked()
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
	wpWeldParameter.dWeldSpeed=2.0;
	wpWeldParameter.dGasFlow=0.5;
	wpWeldParameter.dWeldVoltage=25.0;
	wpWeldParameter.dWeldCurrent=100.0;
	wpWeldParameter.dWireRate=2.0;
	wpWeldParameter.dWeldDistance=100.0;
	wpWeldParameter.dGasPressure=5;
	emit updateWeldParameters_triggered(wpWeldParameter);
}

/******************************************************************************
  Function: syncWeldParameters
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::syncWeldParameters(WeldParameter parameters)
{
	wpWeldParameter=parameters;
	isWeldParametersUpdated=true;
	emit updateWeldParameters_triggered(wpWeldParameter);
	//������ʾ�����ٶ�
}


/******************************************************************************
  Function: on_pushButtonSetWeldParameters_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_pushButtonSetWeldParameters_clicked()
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

	CameraFree(intCameraIndex);
	d1000_board_close();
	exit(0);
	QMainWindow::closeEvent(e);

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
  Function: on_actionCalibration1_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibration1_triggered()
{
	ui_simplecalibration.show();
	ui_main.actionCalibration1->setChecked(true);
	ui_main.actionCalibration2->setChecked(false);

}

/******************************************************************************
  Function: on_actionCalibration2_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibration2_triggered()
{

}

/******************************************************************************
  Function: on_actionCalibration3_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibration3_triggered()
{
	ui_fittingcalibration.show();
}

/******************************************************************************
  Function: updateCalibrationState
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::syncCalibrationState()
{
	isCalibrationFinished=true;
}

/******************************************************************************
  Function: setReferenceOffset
  Description: �������߸���ʱ��������Ұ���ߺͺ������ߵľ��룬Ҳ���ǲο�ƫ��
  Calls:
  Called By:  
  Input:
  Output: dReferenceOffset;
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::setReferenceOffset()
{
	dReferenceOffset=siSeamInfo.dWeldSeamOffset*dHorizontalCalibrationRatio;
}

/******************************************************************************
  Function: on_actionTrackMethod1_triggered
  Description: ��Ӧ���ٷ���һ����
  Calls:
  Called By:  
  Input:
  Output: dReferenceOffset;
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionTrackMethod1_triggered()
{
	ui_main.actionTrackMethod1->setChecked(true);
	ui_main.actionTrackMethod2->setChecked(false);
	ui_main.actionTrackMethod3->setChecked(false);
	ui_main.actionTrackMethod4->setChecked(false);
	intTrackMethod=1;
	emit updateTrackMethod_triggered(intTrackMethod);
}

/******************************************************************************
  Function: on_actionTrackMethod2_triggered
  Description: ��Ӧ���ٷ���2����
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionTrackMethod2_triggered()
{
	ui_main.actionTrackMethod1->setChecked(false);
	ui_main.actionTrackMethod2->setChecked(true);
	ui_main.actionTrackMethod3->setChecked(false);
	ui_main.actionTrackMethod4->setChecked(false);
	intTrackMethod=2;
	emit updateTrackMethod_triggered(intTrackMethod);
}

/******************************************************************************
  Function: on_actionTrackMethod3_triggered
  Description: ��Ӧ���ٷ���3����
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionTrackMethod3_triggered()
{
	ui_main.actionTrackMethod1->setChecked(false);
	ui_main.actionTrackMethod2->setChecked(false);
	ui_main.actionTrackMethod3->setChecked(true);
	ui_main.actionTrackMethod4->setChecked(false);
	intTrackMethod=3;
	emit updateTrackMethod_triggered(intTrackMethod);
}

/******************************************************************************
  Function: on_actionTrackMethod4_triggered
  Description: ��Ӧ���ٷ���2����
  Calls:
  Called By:  
  Input:
  Output: dReferenceOffset;
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionTrackMethod4_triggered()
{
	ui_main.actionTrackMethod1->setChecked(false);
	ui_main.actionTrackMethod2->setChecked(false);
	ui_main.actionTrackMethod3->setChecked(false);
	ui_main.actionTrackMethod4->setChecked(true);
	intTrackMethod=4;
	emit updateTrackMethod_triggered(intTrackMethod);
}

/******************************************************************************
  Function: sendBlockWidthPixel
  Description: ��������emitһ��sendBlockWidthPixel��Ϣ��ui_simpleCalibration
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::sendBlockWidthPixel()
{
	emit sendBlockWidthPixel_triggered(siSeamInfo.dWeldSeamWidth); //��λ������
}

/******************************************************************************
  Function: setDefaultSeamTrackParameter
  Description: ����Ĭ�ϵĺ��Ӹ��ٲ���
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::setDefaultSeamTrackParameters()
{
	spSeamTrackParameter.trackingDistance=100.0;
	spSeamTrackParameter.trackingVelocity=2.0;
	spSeamTrackParameter.trackingDirection=1;
}

/******************************************************************************
  Function: syncSeamTrackParameters
  Description: ����welduiʱ�����ٲ���ҲҪ����
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::syncSeamTrackParameters(SeamTrackParameter parameters)
{
	spSeamTrackParameter=parameters;
}