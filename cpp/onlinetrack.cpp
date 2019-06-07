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
				sumUpList			//�б����
				allowAutoTrack		//�����Զ����ٹ���
				declineAutoTrack	//�����Զ����ٹ���
				updateHorizontalCalibrationRatio	//����ˮƽ�궨����
				dip					//����ͼ����
				
				sendDIPResult_triggered				//����DIP����������һ������ͼ
				sendDIPCalculationResult_triggered	//����DIP������
				allowNextDIP_triggered				//��һ֡��ͼ����
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    ���onlinetrack.hͷ��ע��
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
	
	dTorchSensorDistance=42;	//42mm
	dSensorFrameRate=5;
	intROffsetCount=0;
	intOffsetCount=0;
	dLastAbsOffset=0.0;
	dTinyOffset=0.0;
	timeoutFlag=true;
	isTestModeTriggered=true;

	//û�и��µ�����£��������²�������
	//����intBufferNum.֡��Ϊ10Hz,y���ƶ��ٶ�Ϊ
	dWeldSpeed=2;	//�����ٶ�Ϊ2mm/s
	intBufferNum=dSensorFrameRate*dTorchSensorDistance/dWeldSpeed;
	qDebug()<<"intBufferNum="<<intBufferNum;
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
  Function:sumUpList
  Description: ���ƶ���list����������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
double OnlineTrack::sumUpList(vector<double>lst,int start,int end)
{
	double value=0.0;
	for(int i=start;i<end;i++)
	{
		value=value+lst[i];
	}
	return value;
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
	dWeldSpeed=spParameter.weldingVelocity;
	intBufferNum=dSensorFrameRate*dTorchSensorDistance/dWeldSpeed;
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
		motionRectification.yMove(dWeldSpeed,dWeldSpeed*0.2,0.01,0);
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
void OnlineTrack::syncHorizontalCalibrationRatio(double hcr)
{
	dHorizontalCalibrationRatio=hcr;
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
}

/******************************************************************************
  Function:run
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::run()
{
	while(true)
	{
		if(isDIPAllowed && !isTestModeTriggered) //����ģʽ�£���Ҫ��ͼ������ɺ���о�ƫ
		{
			Mat imgOnProcessing=DIP::qImage2Mat(_image);
			DIP::getROIPosition(imgOnProcessing,&DIP::roiX,&DIP::roiY);
			DIP dip(imgOnProcessing);
			
			//ͼ����ʱ��ԼΪ30~50ms
			
			emit sendDIPResult_triggered(dip.out);
			emit sendDIPCalculationResult_triggered(dip.seaminfo);

			dAbsOffset=dip.seaminfo.dWeldSeamOffset*dHorizontalCalibrationRatio;
			isDIPAllowed=false;

			/***********�������߸���***************/
			if(isAutoTrackTriggered)
			{
				absOffsetList.push_back(dAbsOffset);	//��Ӿ���ƫ��
				qDebug()<<"adding "<<intOffsetCount<<"th absolute offset:"<<dAbsOffset;
				
				if(intOffsetCount<intBufferNum)
				{
					dNthROffset=dAbsOffset-dLastAbsOffset;
					rOffsetList.push_back(dNthROffset);
					qDebug()<<"adding "<<intOffsetCount<<"th"<<"rectifying offseta;"<<dNthROffset;
					dLastAbsOffset=dAbsOffset;
					intOffsetCount++;
				}
				else if(intOffsetCount>=intBufferNum)//��ʼ��ƫ
				{
					dNthROffset=dAbsOffset-sumUpList(rOffsetList,intROffsetCount+1,intOffsetCount);
					rOffsetList.push_back(dNthROffset);
					qDebug()<<"adding "<<intOffsetCount<<"th rectifying offset:"<<dNthROffset;
					
					dCROffset=rOffsetList[intROffsetCount]+dTinyOffset;					
					if(abs(dCROffset)>0.2 && abs(dCROffset)<3.0)
					{
						motionRectification.xMove(5*dCROffset,dCROffset,0.01,0);
						qDebug()<<"correcting "<<intROffsetCount<<"th. "<<dCROffset;
						dTinyOffset=0.0;
					}
					else if(abs(dCROffset)<=0.2)
					{
						dTinyOffset=dTinyOffset+dCROffset;
					}
					intROffsetCount++;
					intOffsetCount++;
				}
			} // end of if(isAutoTrackTriggered)

		} //end of if(isDIPAllowed && !testOrNot)
	}// end of while(true)
}
