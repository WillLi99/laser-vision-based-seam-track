/******************************************************************************
  File name: onlinetrack.cpp
  Author: WillLi99		Date:2019-5-21
  Description:
              ���ļ�ʵ��onlinetrack��. onlinetrack�ฺ������ͼ��������߸��١�            
  Others: 
  Function List:
                OnlineTrack			//��������
				~OnlineTrack		//
				start100msTiming	//����100ms��ʱ
				start200msTiming	//����200ms��ʱ
				timeOut200ms		//��ʱ�ﵽ200ms��Ĵ���
				stopTiming			//�رն�ʱ
				allowAutoTrack		//�����Զ����ٹ���
				declineAutoTrack	//�����Զ����ٹ���
				updateHorizontalCalibrationRatio	//����ˮƽ�궨����
				dip					//����ͼ����
				receiveReferenctOffset	//���ղο�ƫ��
				checkRecitificationAvailable		//����Ƿ���Ҫ���о�ƫ
				recitifyWeldTorch					//������ǹλ��
				moveWeldTorch						//�����ƶ���ǹ�����Ǿ�ƫ�ķ���
				recordSeamCenterCoordinate			//��¼��������λ�õľ�������
				
				sendDIPResult_triggered				//����DIP����������һ������ͼ
				sendDIPCalculationResult_triggered	//����DIP������
				allowNextDIP_triggered				//��һ֡��ͼ����
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    ���onlinetrack.hͷ��ע��
		   WillLi99    2019-6-21    ���checkRecitificationAvailable��recordSeam
		                            CenterCoordinate�Ⱥ���. ɾ��sumuplist
		   WillLi99    2019-7-18    ���resetWeldTrackConfiguration
******************************************************************************/
#include "dip.h"
#include "onlinetrack.h"

/******************************************************************************
  Function:OnlineTrack
  Description: ��ʼ�������ʹ����ź�����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
OnlineTrack::OnlineTrack(QObject* parent)
{
	isDIPAllowed=false;
	isAutoTrackTriggered=false;
	isTestModeTriggered=false;

	dTorchSensorDistance=60;
	dSensorFrameRate=5;
	intOffsetCount=0;
	intNthPeriod=0;
	dReferenceOffset=0.0;
	dWeldSpeed=2.0;
	intTrackMethod=2;
	dLastPeriodRemain=0.0;
	dLastWeldTorchCoordinateX=0.0;

	timeoutFlag=true;
	nthFrame=0;

	connect(&sTimer, SIGNAL(timeout()), this, SLOT(timeOut200ms()));
}

/******************************************************************************
  Function:~OnlineTrack
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
OnlineTrack::~OnlineTrack(void)
{
}

/******************************************************************************
  Function:start100msTiming
  Description: ����100ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::start100msTiming()
{
	sTimer.start(100);	//����100ms��ʱ
}

/******************************************************************************
  Function:start200msTiming
  Description: ����200ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::start200msTiming()
{
	sTimer.start(200);	//����200ms��ʱ
}

/******************************************************************************
  Function:stopTiming
  Description: �رն�ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::stopTiming()
{
	sTimer.stop();
}

/******************************************************************************
  Function:timeOut200ms
  Description: ����200ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::timeOut200ms()
{
	timeoutFlag=true;
	stopTiming();
	emit enableNextDIP_triggered();
}


/******************************************************************************
  Function:enableAutoTrack
  Description: ʹ���Զ�����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::enableAutoTrack(SeamTrackParameter spParameter)
{
	isAutoTrackTriggered=true;
	dWeldSpeed=spParameter.trackingVelocity;
	dTrackDistance=spParameter.trackingDistance;
	intBufferNum=dSensorFrameRate*dTorchSensorDistance/dWeldSpeed;
	intTotalTrackPeriodNum=dSensorFrameRate*spParameter.trackingDistance/dWeldSpeed;

	qDebug()<<QStringLiteral("�ܸ�������")<<intTotalTrackPeriodNum;
}

/******************************************************************************
  Function:disableAutoTrack
  Description: �ر��Զ�����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::disableAutoTrack()
{
	isAutoTrackTriggered=false;
	nthFrame=0;
	resetWeldTrackConfiguration();
}

/******************************************************************************
  Function:dip
  Description: ����ִ��һ��ͼ����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::dip(QImage image)
{
	//���
	_image=image.copy();
	start200msTiming();		//����һ������200ms�Ķ�ʱ
	isDIPAllowed=true;
	if(isAutoTrackTriggered) 
	{
		qDebug()<<"weld torch moving";
		moveWeldTorch(dWeldSpeed*1.2,dWeldSpeed*0.2,0.01,2);
	}
}

/******************************************************************************
  Function:syncHorizontalCalibrationRatio
  Description: ͬ��ˮƽ�궨��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::syncHorizontalCalibrationRatio(double dHorizontalCalibrationRatio)
{
	this->dHorizontalCalibrationRatio=dHorizontalCalibrationRatio;
}

/******************************************************************************
  Function:enableTestingMode
  Description: �������ģʽ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::enableTestingMode()
{
	isTestModeTriggered=true;
}

/******************************************************************************
  Function:disableTestingMode
  Description: ��ֹ����ģʽ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::disableTestingMode()
{
	isTestModeTriggered=false;
	nthFrame=0;
}

/******************************************************************************
  Function:run
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: ע�⣡���߸��ٹ��̵Ĳο��㷨ֻ��������ֱ�ߺ���ĸ��١�
******************************************************************************/
void OnlineTrack::run()
{
	while(true)
	{
		if(isDIPAllowed && !isTestModeTriggered) //����ģʽ�£���Ҫ��ͼ������ɺ���о�ƫ
		{
			//ͼ����ʱ��ԼΪ30~50ms

			Mat image=DIP::qImage2Mat(_image);
			if(nthFrame==0)
			{
				DIP::setROISize(400,200);
				DIP::locateROI(image);
			}
			DIP dip(image);
			
			emit sendDIPResult_triggered(dip.qOutputImage);
			emit sendDIPCalculationResult_triggered(dip.seamProfileInfo);

			dOffset=dip.seamProfileInfo.dWeldSeamOffset*dHorizontalCalibrationRatio;
			isDIPAllowed=false;

			double dDeviation;
			/***********�������߸���***************/
			if(isAutoTrackTriggered)
			{
				if(intTrackMethod == TRACKMETHOD_REFERENCE)	//�ο��㷨
				{
					dDeviation=dOffset-dReferenceOffset+dLastPeriodRemain;
					referenceTrack(dDeviation);
				} //end of if intTrackMethod=TRACKMETHOD_REFERENCE

				if(intTrackMethod == TRACKMETHOD_GLOBAL_COORDINATE)
				{
					globalCoordinateTrack();
				}// end of if intTrackMethod=TRACKMETHOD_GLOBAL_COORDINATE

			} // end of if(isAutoTrackTriggered)
			nthFrame++;
		}
		else if(isDIPAllowed && isTestModeTriggered)
		{
			//qDebug()<<"nthFrame="<<nthFrame;
			Mat image=DIP::qImage2Mat(_image);
			if(nthFrame==0)
			{
				DIP::setROISize(400,200);
				DIP::locateROI(image);
			}
			DIP dip(image);

			emit sendDIPResult2_triggered(dip.qOutputImage);
			isDIPAllowed=false;
			nthFrame++;
		}
	}//end of while
}

/******************************************************************************
  Function:receiveReferenceOffset
  Description: ���²ο�ƫ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: dReferenceOffset����tracksys
******************************************************************************/
void OnlineTrack::receiveReferenceOffset(double dReferenceOffset)
{
	this->dReferenceOffset=dReferenceOffset;
	qDebug()<<QStringLiteral("�ο�ƫ��")<<dReferenceOffset;
}

/******************************************************************************
  Function:checkRecitificationAvailable
  Description: �ж�ƫ������С����ƫ����С��thresh,��������о�ƫ������false��������Ҫ
               ���о�ƫ���ƣ�����true
  Calls: 
  Called By: 
  Input: deviation-ƫ����
  Output:
  Return:       
  Others: 
******************************************************************************/
bool OnlineTrack::checkRecitificationAvailable(double deviation)
{
	double dThreshold=0.2;
	if(abs(deviation)<dThreshold)
	{
		dLastPeriodRemain=deviation;
		return false;
	}
	else
	{
		dLastPeriodRemain=0.0;
		return true;
	}
}

/******************************************************************************
  Function:recitifyWeldTorch
  Description: ������ǹ��λ��
  Calls: 
  Called By: 
  Input: dSpeed-��ƫ�ٶȣ�dDistance-��ƫ���룻dAccTime-����ʱ�䣻dAccMethod-���ٷ���
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::recitifyWeldTorch(double dSpeed,double dDistance,
	double dAccelerationTime,double dAccelerationMethod)
{
	motionRectification.xMove(dSpeed,dDistance,dAccelerationTime,dAccelerationMethod);
}

/******************************************************************************
  Function:moveWeldTorch
  Description: �ƶ���ǹ
  Calls: 
  Called By: 
  Input: dSpeed-��ƫ�ٶȣ�dDistance-��ƫ���룻dAccTime-����ʱ�䣻dAccMethod-���ٷ���
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::moveWeldTorch(double dSpeed,double dDistance,
	double dAccelerationTime,double dAccelerationMethod)
{
	motionRectification.yMove(dSpeed,dDistance,0.01,dAccelerationMethod);
}

/******************************************************************************
  Function:recordSeamCenterCoordinate
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::recordSeamCenterCoordinate(double coordinateX,double coordinateY)
{
	Point2d p=Point2d(coordinateX,coordinateY);
	seamCenterCoordinateSet.push_back(p);
}

/******************************************************************************
  Function:recordWeldTorchCoordinate
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::recordWeldTorchCoordinate(double coordinateX,double coordinateY)
{
	Point2d p=Point2d(dWeldTorchCoordinateX,dWeldTorchCoordinateY);
	weldTorchCoordinateSet.push_back(p);
}

/******************************************************************************
  Function:syncTrackMethod
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::syncTrackMethod(int intTrackMethod)
{
	this->intTrackMethod=intTrackMethod;
}

/******************************************************************************
  Function:resetWeldTrackConfiguration
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::resetWeldTrackConfiguration()
{
	dWeldSpeed=0.0;
	dReferenceOffset=0.0;
	dTrackDistance=0.0;
	intBufferNum=0;
	intNthPeriod=0;
	intTotalTrackPeriodNum=0;

	//����������꼯�ͺ�ǹλ������
	weldTorchCoordinateSet.clear();
	seamCenterCoordinateSet.clear();
}

/******************************************************************************
  Function:tirggerTestingMode
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::tirggerTestingMode()
{
	isTestModeTriggered=true;
}

/******************************************************************************
  Function:detriggerTestingMode
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::detriggerTestingMode()
{
	isTestModeTriggered=false;
}

/******************************************************************************
  Function:referenceTrack
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::referenceTrack(double dDeviation)
{
	//qDebug()<<intNthPeriod<<"nth track";
	dDeviation=dOffset-dReferenceOffset+dLastPeriodRemain;
	if(checkRecitificationAvailable(dDeviation))
	{
		recitifyWeldTorch(5*dDeviation,dDeviation,0.01,SIGMOID_ACCELERATION);
	}
	intOffsetCount++;
	intNthPeriod++;

	if(intNthPeriod>intTotalTrackPeriodNum)	//�������
	{
		isAutoTrackTriggered=false;
	}
}

/******************************************************************************
  Function:globalCoordinateTrack
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::globalCoordinateTrack()
{
	if(intNthPeriod<intBufferNum)	//ֻ��¼������ƫ
	{
		dWeldTorchCoordinateX=0;
		dWeldTorchCoordinateY=intNthPeriod*dWeldSpeed/dSensorFrameRate-dTorchSensorDistance;

		dWeldSeamCoordinateX=dOffset;
		dWeldSeamCoordinateY=intNthPeriod*dWeldSpeed/dSensorFrameRate;

		recordSeamCenterCoordinate(dWeldSeamCoordinateX,dWeldSeamCoordinateY);
		recordWeldTorchCoordinate(dWeldTorchCoordinateX,dWeldTorchCoordinateY);
	}
	else if(intNthPeriod>=intBufferNum) //�߼�¼���߾�ƫ
	{
		dWeldSeamCoordinateX = dLastWeldTorchCoordinateX + dOffset;
		dWeldSeamCoordinateY = intNthPeriod*dWeldSpeed/dSensorFrameRate;
		dWeldTorchCoordinateX = dLastWeldTorchCoordinateX;
		dWeldTorchCoordinateY = intOffsetCount*dWeldSpeed/dSensorFrameRate-dTorchSensorDistance;

		recordSeamCenterCoordinate(dWeldSeamCoordinateX,dWeldSeamCoordinateY);
		recordWeldTorchCoordinate(dWeldTorchCoordinateX,dWeldTorchCoordinateY);

		double dDeviation=seamCenterCoordinateSet[intNthPeriod-intBufferNum].x-
			weldTorchCoordinateSet[intNthPeriod].x + dLastPeriodRemain;

		if(!checkRecitificationAvailable(dDeviation))
		{
			dLastPeriodRemain=dOffset;
		}
		else
		{
			recitifyWeldTorch(5*dOffset,dOffset,0.01,2);
			dLastWeldTorchCoordinateX=dWeldSeamCoordinateX + dOffset;
			dLastPeriodRemain=0.0;
		}
	}
	else if(intNthPeriod>=intTotalTrackPeriodNum-intBufferNum) //ֻ��ƫ������¼
	{
		dWeldTorchCoordinateX = dLastWeldTorchCoordinateX;
		dWeldTorchCoordinateY = intNthPeriod*dWeldSpeed/dSensorFrameRate-dTorchSensorDistance;

		recordWeldTorchCoordinate(dWeldTorchCoordinateX,dWeldTorchCoordinateX);

		double dOffset=seamCenterCoordinateSet[2*intNthPeriod-intTotalTrackPeriodNum].x-
			weldTorchCoordinateSet[intNthPeriod].x;

		if(!checkRecitificationAvailable(dOffset))
		{
			dLastPeriodRemain=dOffset;
		}
		else
		{
			recitifyWeldTorch(5*dOffset,dOffset,0.01,2);
			dLastWeldTorchCoordinateX=dWeldSeamCoordinateX + dOffset;
			dLastPeriodRemain=0.0;
		}
	}

	if(intNthPeriod==intTotalTrackPeriodNum+intBufferNum)	//���ٽ���
	{
		isAutoTrackTriggered=false;
	}

	intOffsetCount++;
	intNthPeriod++;
}