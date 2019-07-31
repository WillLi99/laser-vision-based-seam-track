/******************************************************************************
  File name: var.h
  Author: WillLi99		Date:2019-5-21
  Description:
              ���ļ�������������Ŀ��ģ�鹲�õ�ͷ�ļ������Զ�����������͡�             
  Others: 
  Function List:
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    ���var.hͷ��ע��
******************************************************************************/

#ifndef _VAR_H
#define _VAR_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <QWidget>
#include <QImage>
#include <QThread>
#include "windows.h"
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QIODevice>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QTime>
#include <QtWidgets/QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene> 
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "time.h"
#include <QMetaType>

using namespace std;
using namespace cv;

#define TRAPEZOIDAL_ACCELERATION 1
#define SIGMOID_ACCELERATION 2

#define FILTER_MED_33 1
#define FILTER_MED_55 2
#define FILTER_OPEN_15 3
#define FILTER_OPEN_37 4

#define INTENSITY_GRAVITY 1
#define INTENSITY_MAXIMUM 2

#define EXTRACTION_ITERATION 1
#define	EXTRACTION_DIFFERENTIAL 2

#define TRACKMETHOD_REFERENCE 1
#define TRACKMETHOD_GLOBAL_COORDINATE 2
#define TRACKMETHOD_REFERENCE_KALMAN 3
#define TRACKMETHOD_GLOBAL_COORDINATE_KALMAN 3

/******************************************************************************
  Name:AcclerationMode
  Description: ���ƿ��ļ���ģʽ,S�ͼ��ٱ����μ��ٸ�ƽ��.
  Used By: tracksys.h tracksys.cpp   
  Others: 
******************************************************************************/
enum AcclerationMode
{
	TRAPEZOIDAL,	//���μ���
	SIGMOID			//S�ͼ���
};

/******************************************************************************
  Name:TrackingDirection
  Description: ������ٹ�����,��ǹ��Y�����ƶ�����.
  Used By: tracksys.h tracksys.cpp
  Others: 
******************************************************************************/
enum TrackingDirection
{
	POSITIVE,
	NEGATIVE
};

/******************************************************************************
  Name:WeldParameter
  Description: ���ӹ��ղ���
  Used By: tracksys.h tracksys.cpp weld.h weld.cpp
  Others: 
******************************************************************************/
struct WeldParameter
{
	double dWeldSpeed;			//�����ٶ�
	double dWireRate;			//��˿�ٶ�
	double dGasFlow;			//����������
	double dWeldVoltage;		//���ӵ�ѹ
	double dWeldCurrent;		//���ӵ���
	double dWeldDistance;		//���Ӿ���
	double dGasPressure;		//����ѹ��
};

/******************************************************************************
  Name:SeamType
  Description: ��������
  Used By: tracksys.h tracksys.cpp var.h
  Others: 
******************************************************************************/
enum SeamType
{
	BUTT_WELD_SEAM,		//I�ͶԽӺ���
	LAP_WELD_SEAM,		//��Ӻ���
	GROOVE_WELD_SEAM	//V�ͶԽӺ���
};

/******************************************************************************
  Name:SeamInfo
  Description: ������Ϣ
  Used By: tracksys.h tracksys.cpp dip.h dip.cpp
  Others: 
******************************************************************************/
struct SeamInfo
{
	double dWeldSeamOffset;
	double dWeldSeamWidth;
	double dRemainingHeight;	//�������
	SeamType weldSeamType;
};

typedef vector<vector<Point2i>> PointMat;
typedef vector<Point2i> Points;
typedef Vec4f Line;

typedef vector<Point2d> CoordinateSet;


/******************************************************************************
  Name:SeamTrackParameter
  Description: ������ٲ���
  Used By: tracksys.h tracksys.cpp dip.h dip.cpp
  Others: 
******************************************************************************/
struct SeamTrackParameter
{
	double trackingVelocity;
	double trackingDistance;
	char trackingDirection;
};

#endif