/******************************************************************************
  File name: tracksys.h
  Author: WillLi99		Date:2019-5-18
  Description:
              ������SeamTrackingSys�࣬����̳���QMainWindow���������ݽ��ӣ���Ϣ
			  ��ʾ��������UI�ȹ�����               
  Others: 
  Function List:
                 SeamTrackingSys	// ���ʼ��
                 ~SeamTrackingSys	// ������ʱ���ͷ���Դ
	             setDefaultUiConfiguration // ����Ĭ�ϵĲ˵�����
	             initializeVariable //��ʼ�����˽�б���
				 setReferenceOffset	//�趨�ο�ƫ��
	             on_pushButtonTurnOnCamera_clicked	// �����
	             on_pushButtonTurnOffCamera_clicked	// �����
	             loadDefaultCameraConfiguration	// ���������Ĭ������
	             updateCameraConnectionState // ��������������
	             on_cameraResolutionComBox_currentIndexChanged	// �ı�ֱ���
	             updateCameraResolution	//��������ֱ���
	             display1	// ��ʾ�����ͼ��
	             display2	// ��ʾ�������ݼ���ͼ��
	             saveCalibrationData	// �������У��ͼ��
	             saveValidationData	// �������У����������֤��ͼ��
	             saveSeamLaserStripeData	// ���漤������ͼ��
	             on_pushButtonCameraParametersSetting_clicked	// ����camera����
	             updateFPSValue	// �������FPS
	             saveSingleImage	// ���浥��ͼƬ
	             saveMultiImages // �����ر���ͼ��
	             stopSavingImages // ֹͣ�������ͼƬ
	             saveImageOfflineTrack	// ��������ģʽ�µ�ͼ��
	             startDIP	//����ͼ����
	             updateDIPResult	// ����ʶ�����Ľ��
	             updateDIPCalculationResult // ����ͼ����ļ�����
	             updateHorizontalCalibrationRatio	// ����ˮƽ�궨����
	             updateSeamType // ���º��������
	             on_pushButtonLoadTestData_clicked	// �򿪲������ݼ�
	             on_unloadTestingDataSetPushButton_clicked	// �رղ������ݼ�
	             on_pushButtonQuit_clicked	// �ͷ������Դ���ͷſ��ƿ�
	             on_pushButtonTurnOnRecognition_clicked	// ��ͼ��ʶ����
	             on_pushButtonTurnOffRecognition_clicked	// ��ͼ��ʶ����
	             updateMotionControllerConnectionState	// ���¿��ƿ��������
	             on_actiontrapezoidal_triggered // �����˶�����ΪS�ͼ���
	             on_actionsigmoid_triggered	// �����˶�����Ϊ���μ���
	             on_pushButtonManualControl_clicked	// �ֶ�����
	             on_pushButtonAlign_clicked	// ��ͼ����Ұ�м��׼��������
	             on_pushButtonStartAutoTrack_clicked	// �����Զ�����
	             on_pushButtonStopAutoTrack_clicked	// ֹͣ�Զ����ٵĹ���
	             setDefaultWeldParameters	// ����Ĭ�ϵĺ���ͼ��
	             updateWeldParameters	// ���º��Ӳ���
	             on_pushButtonSetWeldParameters_clicked	// ���ú��Ӳ���
	             closeEvent	// �رմ����¼�
	             on_actionabout_triggered // ���������ڡ���Ϣ
	             on_actionCalibrationMethod1_triggered	// ���б궨����1-���ױ궨
	             on_actionCalibrationMethod2_triggered	// ���б궨����2-�ݰұ궨
	             on_actionCalibrationMethod3_triggered	// ���б궨����3-��ϱ궨

				 saveImages_triggered();	//����ͼ���ź�
				 stopSavingImages_triggered();		//ֹͣ����ͼ���ź�
				 autoTrack_triggered(SeamTrackParameter seamTrackingParas);	//�Զ������ź�
				 updateHorizontalCalibrationRatio_triggered(double rho);		//����ˮƽ�궨�����ź�
				 stopAutoTracking_triggered();		//ֹͣ�Զ������ź�
				 updateWeldParameters_triggered(WeldParameter parameters);		���º��Ӳ����ź�
				 triggerTestingMode_triggered();		//��������ģʽ
				 detriggerTestingMode_triggered();		//�������ģʽ
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     ���tracksys.hͷ��ע��
		   WillLi99    2019-5-19	��testorNotȫ�ֱ���ȥ��
		   WillLi99    2019-6-12    ���setReferenceOffset����
		   Will99	  2019-7-8      ɾ����actionOnlineTrack��actionOfflineTrack�������
		                            ������trackMethodMenu����
******************************************************************************/

#pragma once

#include "ui_tracksys.h"
#include "manual.h"
#include "camera.h"
#include "image.h"
#include "dmc1000.h"
#include "motion.h"
#include "calibration1.h"
#include "weld.h"
#include "onlinetrack.h"
#include "dip.h"
#include "common.h"
#include "offlinetrack.h"
#include "calibration3.h"

class SeamTrackingSys : public QMainWindow
{
	Q_OBJECT

public:
	SeamTrackingSys(QWidget *parent = Q_NULLPTR);
	~SeamTrackingSys();
	void closeEvent(QCloseEvent *);

// some ui
private:
	Ui::SeamTrackingSysClass ui_main;
	Manual ui_manual;
	Camera ui_camera;
	SimpleCalibration ui_simplecalibration;
	FittingCalibration ui_fittingcalibration;
	Weld ui_weld;
	OfflineTrack ui_offlinetrack;

// self-defining object
private:

	ImageAcquisition *obj_imageaquisition;		
	OnlineTrack *obj_onlinetrack;		
	Motion obj_automotion;

// Qt Object
private:
	QGraphicsScene *scene1;
	QGraphicsScene *scene2;
	QGraphicsPixmapItem *video0PixItem;
	QGraphicsPixmapItem *video1PixItem;

	QLabel *fpsLabel;	//��ʾ֡�ʵı�ǩ
	QTimer *m_timer;	//��ʱ��

	QTimer sTimer;

// normal variables
private:
	int intCameraIndex;//���������
	int intCardCount; 

	QImage frame;

	int intImageWidth;
	int intImageHeight;

	double dFpsValue;
	int intTotalFrame;
	
	int intMultipleImagesNo;
	QString qsMultipleImagesFilePath;
	QString qsMultipleImagesName;

	double dHorizontalCalibrationRatio;
	double dReferenceOffset;
	double dCurrentOffset;

	WeldParameter wpWeldParameter;
	SeamTrackParameter spSeamTrackParameter;
	SeamInfo siSeamInfo;
	
	LONG64 longReceivedFrameNumber;

	SeamType stWeldSeamType;

	QImage checkico;
	QImage wrongico;
	QImage unknownico;

	int intTrackMethod;
// ���ء���־��
private:
	bool isCameraConnected;	
	bool isControlCardConnected;
	bool cmbNotResponding;
	bool isDIPStarted;
	bool isWeldParametersUpdated;
	bool isSavingMultiImagesCommandTriggered;
	bool isFirstDIPStarted;
	bool isWeldTorchTurnedOn;
	bool isSheldingGasTurnedOn;
	bool isCalibrationFinished;
	bool isAlignmentFinished;
	bool isAutoTrackTriggered;
	bool isTestModeTriggered;

// ��ͨ˽�к���
private:
	void initializeVariable();
	void setReferenceOffset();
	void setDefaultSeamTrackParameters();
// slots
private slots:
	
	// pushbutton-related
	void on_pushButtonQuit_clicked();
	void on_pushButtonStartAutoTrack_clicked();
	void on_pushButtonStopAutoTrack_clicked();
	void on_pushButtonAlign_clicked();
	void on_pushButtonTurnOnCamera_clicked();
	void on_pushButtonTurnOffCamera_clicked();
	void on_pushButtonTurnOnRecognition_clicked();
	void on_pushButtonTurnOffRecognition_clicked();
	void on_pushButtonLoadTestData_clicked();
	void on_pushButtonUnloadTestDataSets_clicked();
	void on_pushButtonCameraParametersSetting_clicked();
	void on_pushButtonManualControl_clicked();
	void on_pushButtonSetWeldParameters_clicked();

	// combox-related
	void on_comboxCameraResolution_currentIndexChanged(int index);
	
	// munu_related
	void on_actionTrackMethod1_triggered();
	void on_actionTrackMethod2_triggered();
	void on_actionTrackMethod3_triggered();
	void on_actionTrackMethod4_triggered();
	void on_actionTrapezoidal_triggered();
	void on_actionSigmoid_triggered();
	void on_actionAbout_triggered();
	void on_actionCalibration1_triggered();
	void on_actionCalibration2_triggered();
	void on_actionCalibration3_triggered();
	
	// self-defining
	void display1(QImage img, unsigned char* buffer);
	void display2(QImage image);
	void updateFPSValue();

	void saveSingleImage();
	void saveMultiImages();
	void stopSavingImages();
	
	void updateDIPResult(QImage image);
	void updateDIPCalculationResult(SeamInfo seaminfo);
	void syncHorizontalCalibrationRatio(double horizontalCalibrationRate);
	void syncWeldParameters(WeldParameter parameters);
	void syncSeamTrackParameters(SeamTrackParameter parameters);
	
	void saveImageOfflineTrack(QString fileName);

	void saveCalibrationData(QString fileName);
	void saveValidationData(QString fileName);
	void saveSeamLaserStripeData(QString fileName);

	void startDIP();

	void updateSeamType();
	void updateMotionControllerConnectionState();
	void updateCameraConnectionState();
	void updateCameraResolution();
	void setDefaultUiConfiguration();
	void setDefaultWeldParameters();
	void loadDefaultCameraConfiguration();
	void syncCalibrationState();
	void sendBlockWidthPixel();

// �ź�
signals:
	void saveImages_triggered();
	void stopSavingImages_triggered();
	void autoTrack_triggered(SeamTrackParameter seamTrackingParas);
	void updateHorizontalCalibrationRatio_triggered(double rho);
	void stopAutoTracking_triggered();
	void updateWeldParameters_triggered(WeldParameter parameters);
	void triggerTestingMode_triggered();
	void detriggerTestingMode_triggered();
	void sendReferenceOffset_triggered(double dReferenceOffset);
	void updateTrackMethod_triggered(int intTrackMethod);
	void sendBlockWidthPixel_triggered(double dBlockWidthPixel);
};