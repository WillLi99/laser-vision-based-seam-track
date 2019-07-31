/******************************************************************************
  File name: offlinetrack.cpp
  Author: WillLi99		Date:2019-5-22
  Description:
              ʵ����OfflineTrack�࣬����̳���QWidget���������ߺ���ģʽ����������
  Others: 
  Function List:
               OfflineTrack
			   ~OfflineTrack
			   start200msTiming		//200ms��ʱ
			   stop200msTiming		//200ms��ʱ��ֹ
			   start100msTimming	//100ms��ʱ
			   stop100msTiming		//100ms��ʱ��ֹ
			   start150msTiming		//150ms��ʱ
			   stop150msTiming		//150ms��ʱ
			   switchOnWeld			//��������
			   switchOffWeld		//�رպ���

			   synchoriWeldParameters				//ͬ�����Ӳ���
			   syncHorizontalCalibrationRatio		//ͬ��ˮƽ�궨����
			   on_pushButtonCollectData_clicked		//�ɼ�����
			   on_pushButtonAnalyzeData_clicked		//���ݴ���
			   on_pushButtonRecoverSensorPosition_clicked	//��λ������λ��
			   on_pushButtonSaveData_clicked		//��������
			   on_pushButtonClearData_clicked		//�������
			   on_pushButtonLoadData_clicked		//��������
			   on_pushButtonStart_clicked			//����
			   on_pushButtonStop_clicked			//ֹͣ
			   timeOut200ms							//��ʱ������һЩ����
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-20     ���offlinetrack.cppͷ��ע��
******************************************************************************/

#include "offlinetrack.h"

/******************************************************************************
  Function:OfflineTrack
  Description:��ʼ������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
OfflineTrack::OfflineTrack(QWidget *parent)
{
	ui.setupUi(this);
	isWeldCmdTriggered=false;
	isDataCollectionFinished=false;
	isDataAnalysisFinished=false;
	haveDataOrNot=false;
	hasTrackStarted=false;
	notAllowTrack=false;

	dataCount=0;
	trackCount=0;
	dataNum=250;
	filePath="..\\images\\offlinetrack\\";
	fileName="";
	trackTime=0;
	ro=0.0;
	lnro=0.0;
	connect(&sTimer200ms, SIGNAL(timeout()), this, SLOT(timeOut200ms()));

}

/******************************************************************************
  Function:~OfflineTrack
  Description:
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
OfflineTrack::~OfflineTrack()
{

}

/******************************************************************************
  Function:start200msTiming
  Description:200ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::start200msTiming()
{
	sTimer200ms.start(200);
	motionOffline.yMove(weldSpeed,weldSpeed*0.2,0.01,0);
}


/******************************************************************************
  Function:stop200msTiming
  Description:��ֹ200ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::stop200msTiming()
{
	sTimer200ms.stop();
}

/******************************************************************************
  Function:start100msTiming
  Description:100ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::start100msTiming()
{
}


/******************************************************************************
  Function:stop100msTiming
  Description:��ֹ100ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::stop100msTiming()
{
}

/******************************************************************************
  Function:start150msTiming
  Description:150ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::start150msTiming()
{
}


/******************************************************************************
  Function:stop150msTiming
  Description:��ֹ150ms��ʱ
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::stop150msTiming()
{
}

/******************************************************************************
  Function:switchOnWeld
  Description: ��������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::switchOnWeld()
{
	d1000_out_bit(4,0);
}

/******************************************************************************
  Function:switchOffWeld
  Description: �رպ���
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::switchOffWeld()
{
	d1000_out_bit(4,1);
}

/******************************************************************************
  Function:syncWeldParameters
  Description: ͬ�����Ӳ���
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::syncWeldParameters(WeldParameter parameters)
{
	weldSpeed=parameters.dWeldSpeed;
	dWeldDistance=parameters.dWeldDistance;
}

/******************************************************************************
  Function:syncHorizontalCalibrationRatio
  Description: ͬ��ˮƽ�궨����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::syncHorizontalCalibrationRatio(double horizontalCalibrationRatio)
{
	this->horizontalCalibrationRatio=horizontalCalibrationRatio;
}


/******************************************************************************
  Function:on_pushButtonCollectData_clicked
  Description: �ɼ�����ṹ������ͼ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonCollectData_clicked()
{

	trackTime=dWeldDistance/weldSpeed;
	cycleCount=trackTime/0.2;
	dataNum=cycleCount;

	if(!g_isCameraWorking)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("��ȷ���Ѵ������"));
		msgbox.exec();
		return;
	}

	start200msTiming();
}

/******************************************************************************
  Function:on_pushButtonAnalyzeData_clicked
  Description: ������ṹ������ͼ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonAnalyzeData_clicked()
{
	if(ui.radioButtonFromSensor->isChecked())	//�������ռ����ɼ�
	{
		if(!isDataCollectionFinished)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("��ȷ�����ݲɼ���ɣ�"));
			msgbox.exec();
			return;
		}
	}
	else if(ui.radioButtonFromSensor->isChecked())
	{
		dataNum=250;
	}
	QString dirName="..\\images\\offline\\";
	QString fileName;
	string stdFileName;
	
	for(int i=1;i<=dataNum;i++)
	{
		fileName=dirName+QString::number(i)+".bmp";
		stdFileName=fileName.toStdString();
		cv::Mat image=imread(stdFileName);

		if(i==0)
		{
			DIP::locateROI(image);

		}
		DIP dip(image);
		double offset=dip.seamProfileInfo.dWeldSeamOffset;
		absOffsetList.push_back(dip.seamProfileInfo.dWeldSeamOffset);
	}

	qDebug()<<"the last abs offset is"<<absOffsetList[dataNum-1];
	qDebug()<<"the first abs offset is"<<absOffsetList[0];
	qDebug()<<"the difference of the last and the first is:"<<horizontalCalibrationRatio*
		(absOffsetList[dataNum-1]-absOffsetList[0]);

	double lastAbsOffset;
	double sum=0.0;
	for(int i=0;i<absOffsetList.length();i++)
	{
		if(i==0)
		{
			lastAbsOffset=absOffsetList[i];
			relOffsetList.push_back(0.0);
			continue;
		}
		double reloffset=horizontalCalibrationRatio*(absOffsetList[i]-lastAbsOffset);
		relOffsetList.push_back(reloffset);
		lastAbsOffset=absOffsetList[i];
		sum=sum+reloffset;
	}

	qDebug()<<"the sum of all the rel offset is:"<<sum;

	
	isDataAnalysisFinished=true;
	if(isDataAnalysisFinished)
	{
		haveDataOrNot=true;
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("��������ݷ�����"));
		msgbox.exec();
		return;
	}
}

/******************************************************************************
  Function:on_pushButtonRecoverSensorPosition_clicked
  Description: �ָ���ǹ��λ��
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonRecoverSensorPosition_clicked()
{
	if(!haveDataOrNot)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("����ȷ�������Ѿ��ɼ��ͷ�����ϣ�"));
		msgbox.exec();
		return;
	}
	motionOffline.yMove(3*weldSpeed,-dWeldDistance,0.2,1);
}

/******************************************************************************
  Function:on_pushButtonStart_clicked
  Description: �������߸���
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonStart_clicked()
{
	if(!haveDataOrNot)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("����ȷ�����ݴ��ڣ�"));
		msgbox.exec();
		return;
	}
	hasTrackStarted=true;

	if(ui.radioButtonOn->isChecked())
	{
		switchOnWeld();
	}

	start200msTiming();
}

/******************************************************************************
  Function:on_pushButtonClearData_clicked
  Description: �������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonClearData_clicked()
{
	if(ui.radioButtonOnlyMemory->isChecked())	//�������ڴ�
	{
		dataCount=0;
		trackCount=0;
		absOffsetList.clear();
		relOffsetList.clear();

		notAllowTrack=false;
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("������ݳɹ���"));
		msgbox.exec();
		return;
	}
	else if(ui.radioButtonTotally->isChecked())		//�����ڴ���ļ�������
	{
		dataCount=0;
		trackCount=0;
		absOffsetList.clear();
		relOffsetList.clear();

		QString dir="..\\Images\\offline\\";
		QDir del(dir);

		del.setFilter(QDir::Files);
		int i,j=del.count()-1;
		for (i=0;i<=j;i++)
			del.remove(del[i]);


		notAllowTrack=false;
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("������ݳɹ���"));
		msgbox.exec();
		return;
	}

	
}

/******************************************************************************
  Function:on_pushButtonStop_clicked
  Description: ֹͣ���߸���
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonStop_clicked()
{
	notAllowTrack=true;
	motionOffline.allStop();
	
	switchOffWeld();
	ui.radioButtonOn->setChecked(true);
}

/******************************************************************************
  Function:on_pushButtonSaveData_clicked
  Description: ��������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonSaveData_clicked()
{
	dataCount=dataNum;weldSpeed=2.0;dWeldDistance=100.0;

	if(relOffsetList.length()==0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("��ǰ�����ݣ�"));
		msgbox.exec();
		return;
	}
	else
	{
		QString fileName=QFileDialog::getSaveFileName(this,"Save TXT",".","txt files (*.txt)");
		if(fileName.isEmpty()==false)
		{
			QFile file(fileName);
			bool isok=file.open(QIODevice::WriteOnly);
			if(isok==true)	//д������
			{
				QTextStream txtStream(&file);
				
				QString begin_str="$$$";
				txtStream<<begin_str<<endl;
				
				QString vitalInfo;
				vitalInfo=QString::number(dataCount)+" "+QString::number(weldSpeed)+" "+
					QString::number(dWeldDistance);
				txtStream<<vitalInfo<<endl;

				QString offsetString;
				for(int i=0;i<dataCount;i++)
				{
					offsetString=QString::number(relOffsetList[i]);
					txtStream<<offsetString<<endl;
				}
			}
			file.close();
			
		}
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("�������ݳɹ���"));
		msgbox.exec();
		return;

	}
}

/******************************************************************************
  Function:on_pushButtonLoadData_clicked
  Description: ��������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::on_pushButtonLoadData_clicked()
{
	QString fileName=QFileDialog::getOpenFileName(this,"open txt","..\\data",
		"txt file(*.txt)");
	if(fileName.isEmpty()==false)
	{
		QFile file(fileName);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
		{	
			std::cerr<<"Error::Cannot read file"<<qPrintable(fileName)
				<<":"<<qPrintable(file.errorString())<<std::endl;
			return;
		}
		
		QTextStream textStream(&file);
		
		QString firstLine=textStream.readLine();
		if(firstLine!="$$$")
		{
			qDebug()<<"Wrong txt file";
			file.close();
			return;
		}
		QString secondLine=textStream.readLine();
		QStringList strList=secondLine.split(" ");

		dataNum=strList[0].toInt();
		weldSpeed=strList[1].toDouble();
		dWeldDistance=strList[2].toDouble();
		
		int i=0;
		QString lineStr;
		double number;
		while(!textStream.atEnd() && i<dataNum)
		{
			lineStr=textStream.readLine();
			number=lineStr.toDouble();
			relOffsetList.push_back(number);
			//qDebug()<<number;
			i++;
		}
		file.close();

		qDebug()<<"the length of relOffsetList="<<relOffsetList.length();

		haveDataOrNot=true;

		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("�������ݳɹ���"));
		msgbox.exec();
		return;
	}
}

/******************************************************************************
  Function:timeOut200ms
  Description: 200ms��ʱ������Ķ���
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OfflineTrack::timeOut200ms()
{
	stop200msTiming();
	if(!notAllowTrack)
	{
		if(!hasTrackStarted)
		{
			fileName=filePath+QString::number(dataCount)+".bmp";
			emit saveImage_triggered(fileName);
			dataCount++;
			if(dataCount<dataNum)
			{
				start200msTiming();
			}
			else
			{
				isDataCollectionFinished=true;
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("���ݲɼ���ϣ�"));
				msgbox.exec();
				return;
			}
		}
		else if(hasTrackStarted)
		{
			if(trackCount<dataNum)
			{
				ro=relOffsetList[trackCount];
				ro=ro+lnro;
				if(abs(ro)<0.2)
				{
					lnro=ro;
				}
				else
				{
					motionOffline.xMove(abs(6*ro),ro,0.01,1);
					lnro=0.0;
				}

				trackCount++;
				start200msTiming();
			} // end of if(trackCount<dataNum)
			else
			{
				switchOffWeld();
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("������ɣ�"));
				msgbox.exec();
				return;
			}
		}
	}
	else if(notAllowTrack)	//ֹͣʱ�������
	{
		dataCount=0;
		trackCount=0;
		absOffsetList.clear();
		relOffsetList.clear();

		motionOffline.allStop();


		switchOffWeld();
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("��ͣ������������ݣ�"));
		msgbox.exec();
		return;
	}
}