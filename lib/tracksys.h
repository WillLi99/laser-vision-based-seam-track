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
	             on_turnOnCameraPushButton_clicked	// �����
	             on_turnOffCameraPushButton_clicked	// �����
	             loadDefaultCameraConfiguration	// ���������Ĭ������
	             updateCameraConnectionState // ��������������
	             on_cameraResolutionComBox_currentIndexChanged	// �ı�ֱ���
	             updateCameraResolution	//��������ֱ���
	             display1	// ��ʾ�����ͼ��
	             display2	// ��ʾ�������ݼ���ͼ��
	             saveCalibrationData	// �������У��ͼ��
	             saveValidationData	// �������У����������֤��ͼ��
	             saveSeamLaserStripeData	// ���漤������ͼ��
	             on_cameraParametersSettingPushButton_clicked	// ����camera����
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
	             on_loadTestingDataSetsPushButton_clicked	// �򿪲������ݼ�
	             on_unloadTestingDataSetPushButton_clicked	// �رղ������ݼ�
	             on_quitPushButton_clicked	// �ͷ������Դ���ͷſ��ƿ�
	             on_turnOnRecognitionPushButton_clicked	// ��ͼ��ʶ����
	             on_turnOffRecognitonPushButton_clicked	// ��ͼ��ʶ����
	             updateMotionControllerConnectionState	// ���¿��ƿ��������
	             on_actiontrapezoidal_triggered // �����˶�����ΪS�ͼ���
	             on_actionsigmoid_triggered	// �����˶�����Ϊ���μ���
	             on_manualControlPushButton_clicked	// �ֶ�����
	             on_alignPushButton_clicked	// ��ͼ����Ұ�м��׼��������
	             on_startAutoTrackPushButton_clicked	// �����Զ�����
	             on_stopAutoTrackPushButton_clicked	// ֹͣ�Զ����ٵĹ���
	             setDefaultWeldParameters	// ����Ĭ�ϵĺ���ͼ��
	             updateWeldParameters	// ���º��Ӳ���
	             on_setWeldParametersPushButton_clicked	// ���ú��Ӳ���
	             closeEvent	// �رմ����¼�
	             on_actionOfflineTrack_triggered	// ���ߺ���
	             on_actionOnlineTrack_triggered	// ���ߺ���
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
#include "var.h"
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
	int cameraIndex;//���������
	int cardCount; 

	QImage frame;

	int imageWidth;
	int imageHeight;

	double fpsValue;
	int m_totalFrame;
	
	int multiImagesNo;
	QString multiImagesFilePath;
	QString multiImagesName;

	double horizontalCalibrationRatio;
	double currentSeamOffset;
	double lastSeamOffset;

	WeldParameter weldparameter;		//���Ӳ���
	SeamTrackParameter seamtrackparameter;	//���ٲ���
	
	LONG64 receivedFrameNumber;

	SeamType weldSeamType;

	QImage checkico;
	QImage wrongico;
	QImage unknownico;

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

// slots
private slots:
	
	// pushbutton-related
	void on_quitPushButton_clicked();
	void on_startAutoTrackPushButton_clicked();
	void on_stopAutoTrackPushButton_clicked();
	void on_alignPushButton_clicked();
	void on_turnOnCameraPushButton_clicked();
	void on_turnOffCameraPushButton_clicked();
	void on_turnOnRecognitionPushButton_clicked();
	void on_turnOffRecognitonPushButton_clicked();
	void on_loadTestingDataSetsPushButton_clicked();
	void on_unloadTestingDataSetsPushButton_clicked();
	void on_cameraParametersSettingPushButton_clicked();
	void on_manualControlPushButton_clicked();
	void on_setWeldParametersPushButton_clicked();

	// combox-related
	void on_cameraResolutionComBox_currentIndexChanged(int index);
	
	// munu_related
	void on_actionOnlineTrack_triggered();
	void on_actionOfflineTrack_triggered();
	void on_actionTrapezoidal_triggered();
	void on_actionSigmoid_triggered();
	void on_actionAbout_triggered();
	void on_actionCalibrationMethod1_triggered();
	void on_actionCalibrationMethod2_triggered();
	void on_actionCalibrationMethod3_triggered();
	
	// self-defining
	void display1(QImage img, unsigned char* buffer);
	void display2(QImage image);
	void updateFPSValue();

	void saveSingleImage();
	void saveMultiImages();
	void stopSavingImages();
	
	void updateDIPResult(QImage image);
	void updateDIPCalculationResult(SeamInfo seaminfo);
	void updateHorizontalCalibrationRatio(double ratio);

	void updateWeldParameters(WeldParameter paramters);
	
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
	void updateCalibrationState();

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
};