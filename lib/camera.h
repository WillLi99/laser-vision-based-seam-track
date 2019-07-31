/******************************************************************************
  File name: camera.h
  Author: WillLi99		Date:2019-5-18
  Description: ����Camera�࣬ʵ�ֵ����������
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
				15. initialize						//��ʼ������
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     ���camra.hͷ��ע��
******************************************************************************/

#ifndef Camera_H
#define Camera_H
#include "ui_camera.h"
#include "common.h"

//���������
class Camera:public QWidget
{
	Q_OBJECT
public:
	explicit Camera(QWidget *parent=0);
	~Camera();

private slots:
	void on_pushButtonWhiteBalance_clicked();
	void on_pushButtonLoadCameraConfiguration_clicked();
	void on_pushButtonSaveCameraConfiguration_clicked();
	void on_pushButtonSaveSingleImage_clicked();
	void on_pushButtonSaveMultipleImages_clicked();
	void on_pushButtonStopSavingMultipleImages_clicked();

	void on_horizontalSliderGamma_valueChanged(int value);
	void on_horizontalSliderContrast_valueChanged(int value);
	void on_horizontalSliderGain_valueChanged(int value);
	void on_horizontalSliderExposure_valueChanged(int value);
	
	void on_checkBoxGain_toggled(bool checked);
	void on_checkBoxExposure_toggled(bool checked);

private:
	Ui_Camera ui;
	int intCameraIndex;
	bool bAGC;
	bool bAEC;
	void initialize();

signals:
	void saveSingleImage_triggered();
	void saveMultipleImages_triggered();
	void stopSavingMultipleImages_triggered();
};

#endif