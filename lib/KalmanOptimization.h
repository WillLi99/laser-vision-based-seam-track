/******************************************************************************
  File name: KalmanOptimization.h
  Author: WillLi99		Date:2019-7-3
  Description:
              ���ļ�������KalmanOptimization��. KalmanOptimization���𺸷����Ŀ������Ż���            
  Others: 
  Function List:
                KalmanOptimization			//��������
				~KalmanOptimization
				initialize
				predict
				correct
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-7-3    ���KalmanOptimization.h�ļ�
******************************************************************************/

#pragma once
#include "common.h"
#include "opencv2/video/tracking.hpp"

class KalmanOptimization :public KalmanFilter
{
public:
	KalmanOptimization(void);
	~KalmanOptimization(void);

public:
	void initialize();
	void optimize(Mat measurement);
	Mat optimizedValue;
private:
	KalmanFilter kf;
	int stateNum;
	int measurementNum;
	Mat processNoise;
	Mat measurementNoise;
	Mat transitionMatrix;
	Mat controlMatrix;
	Mat measurementMatrix;
	
	int iterationNum;

};