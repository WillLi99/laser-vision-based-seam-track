/*****************************************************************************
Copyright: Guangdong Provincial Welding Technology Research Center
Author: Li Yangjin
Date: 2018-8-27
Description:Dip_AutoTracking���װ��ͼ����Ĺ��ܺ�ʵʱ���ٵĹ��ܡ�����Dip_Auto
			Trackingʵ����processImage����ͼ��ʶ�������ʶ���emit���ͼƬ��
			ƫ��bias. �Զ���ƫ���ܵĿ���������_autoTrackingSwitch�Ŀ�������set-
			AutoTrackingSwitch��������
*****************************************************************************/
#include "dip.h"
#include "onlinetrack.h"

OnlineTrack::OnlineTrack(QObject* parent)
{
	dipSwitch=false;
	_autoTrackingSwitch=false;
	torchSensorDistance=42;
	sensorFrameRate=5;
	nROffsetCount=0;
	offsetCount=0;
	lAbsOffset=0.0;
	tinyOffset=0.0;
	timeoutFlag=true;

	//û�и��µ�����£��������²�������
	//����bufferNum.֡��Ϊ10Hz,y���ƶ��ٶ�Ϊ
	weldVelocity=2;	//�����ٶ�Ϊ2mm/s
	bufferNum=sensorFrameRate*torchSensorDistance/weldVelocity;
	qDebug()<<"bufferNum="<<bufferNum;
	connect(&sTimer, SIGNAL(timeout()), this, SLOT(timeOut200ms()));

}


OnlineTrack::~OnlineTrack(void)
{
}

void OnlineTrack::run()
{
	while(true)
	{
		if(dipSwitch && !testOrNot) //����ģʽ�£���Ҫ��ͼ������ɺ���о�ƫ
		{
			Mat imgOnProcessing=DIP::qImage2Mat(_image);
			DIP::getROIPosition(imgOnProcessing,&DIP::roiX,&DIP::roiY);
			DIP dip(imgOnProcessing);
			
			//ͼ����ʱ��ԼΪ30~50ms
			
			emit sendDIPResult_triggered(dip.out);
			emit sendDIPCalculationResult_triggered(dip.seaminfo);

			absOffset=dip.seaminfo.weldSeamOffset*_rho;
			dipSwitch=false;

			/***********�������߸���***************/
			if(AutoTrackingTriggledFlag)
			{
				absOffsetList.push_back(absOffset);	//��Ӿ���ƫ��
				qDebug()<<"adding "<<offsetCount<<"th absolute offset:"<<absOffset;
				
				if(offsetCount<bufferNum)
				{
					nROffset=absOffset-lAbsOffset;
					rOffsetList.push_back(nROffset);
					qDebug()<<"adding "<<offsetCount<<"th"<<"rectifying offseta;"<<nROffset;
					lAbsOffset=absOffset;
					offsetCount++;
				}
				else if(offsetCount>=bufferNum)//��ʼ��ƫ
				{
					nROffset=absOffset-sumUpList(rOffsetList,nROffsetCount+1,offsetCount);
					rOffsetList.push_back(nROffset);
					qDebug()<<"adding "<<offsetCount<<"th rectifying offset:"<<nROffset;
					
					cROffset=rOffsetList[nROffsetCount]+tinyOffset;					
					if(abs(cROffset)>0.2 && abs(cROffset)<3.0)
					{
						_rectificationObject.xMove(5*cROffset,cROffset,0.01,0);
						qDebug()<<"correcting "<<nROffsetCount<<"th. "<<cROffset;
						tinyOffset=0.0;
					}
					else if(abs(cROffset)<=0.2)
					{
						tinyOffset=tinyOffset+cROffset;
					}
					nROffsetCount++;
					offsetCount++;
				}
			}

		}
	}
}

void OnlineTrack::dip(QImage image)
{
	//���
	_image=image.copy();
	start200msTiming();		//����һ������200ms�Ķ�ʱ
	dipSwitch=true;
	if(AutoTrackingTriggledFlag)
	{
		_rectificationObject.yMove(weldVelocity,weldVelocity*0.2,0.01,0);
	}
}

void OnlineTrack::setAutoTrackingSwitch(SeamTrackParameter seamTrackingParas)
{
	_autoTrackingSwitch=true;
	weldVelocity=seamTrackingParas.weldingVelocity;
	bufferNum=sensorFrameRate*torchSensorDistance/weldVelocity;
}


void OnlineTrack::updateHorizontalCalibrationRatio(double rho)
{
	_rho=rho;
}

void OnlineTrack::timeOut200ms()
{
	timeoutFlag=true;
	stopTiming();
	emit allowNextDIP_triggered();
}

void OnlineTrack::start100msTiming()
{
	sTimer.start(100);	//����100ms��ʱ
}

void OnlineTrack::start200msTiming()
{
	sTimer.start(200);	//����100ms��ʱ
}

void OnlineTrack::stopTiming()
{
	sTimer.stop();
}

void OnlineTrack::stopAutoTracking()
{
	_autoTrackingSwitch=false;
}

double OnlineTrack::sumUpList(vector<double>lst,int start,int end)
{
	double value=0.0;
	for(int i=start;i<end;i++)
	{
		value=value+lst[i];
	}
	return value;
}