/*****************************************************************************
Author: Li Yangjin
Date: 2018-8-21
Description:Motion���װ�˻���ƽ̨�����˶���Ҫ�õ��ĺ�����Run�ຯ���������˶�������Move
�ຯ���Ƕ����˶��������˶����ƻ���laisai dmc1000���ƿ����ƣ����п��ƿ�0��ӻ���ƽ̨x�ᣬ
���ƿ�2��ӻ���ƽ̨z�ᣬ���ƿ�3��ӻ���ƽ̨y�ᡣ����ƽ̨�ϸ���ͷ���������ʾ��
                      ^z+
                      |
                      |-----> y+
                     /
                    /
                   <x+

*****************************************************************************/
#include "motion.h"
#include "dmc1000.h"
#define minVecPPS 100	//���ƿ��˶����ٶȣ���λ��������ÿ��

Motion::Motion(void)
{
	_coeffX=0.00625;		
	_coeffY=0.0125;		
	_coeffZ=0.00625;	

	//�����������ģʽ
	d1000_set_pls_outmode(0,1);
	d1000_set_pls_outmode(2,0);
	d1000_set_pls_outmode(3,0);
}

Motion::~Motion(void)
{

}


/*****************************************************************************
Function:void Motion::xRun(double maxVec,double accTime,int accMode)
Description:x��������
Call:SysTrackingSys��ManuanlControl
Input:maxVec-��������ٶȣ���λmm/s��;accTime-����ʱ�䣨��λs);accMode-��������
Output:
Return:
Others:����������ߵĲ��죬��ʱҪ��maxVecȡ����
*****************************************************************************/
void Motion::xRun(double maxVec,double accTime,int accMode)
{
	//maxVec=-maxVec;	
	switch(accMode)
	{
	case 0:d1000_start_tv_move(AXISX,minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	case 1:d1000_start_sv_move(AXISX,minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	}
}

void Motion::yRun(double maxVec,double accTime,int accMode)
{
	//maxVec=-maxVec;
	switch(accMode){
	case 0:d1000_start_tv_move(AXISY,minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	case 1:d1000_start_sv_move(AXISY,minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	}	
}


void Motion::zRun(double maxVec,double accTime,int accMode)
{
	maxVec=-maxVec;
	switch(accMode)
	{
	case 0:d1000_start_tv_move(AXISZ,minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	case 1:d1000_start_sv_move(AXISZ,minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	}
}

/*****************************************************************************
Function:void Motion::xMove(double maxVec,double distance,double accTime,int accMode)
Description:x����ָ��·��
Call:SysTrackingSys��ManuanlControl
Input:maxVec-��������ٶȣ���λmm/s��;distance-�˶����룬���������򣨵�λmm��;accTime-
      ����ʱ�䣨��λs);accMode-��������
Output:
Return:
Others:����������ߵĲ��죬��ʱҪ��distanceȡ����
*****************************************************************************/
void Motion::xMove(double maxVec,double distance,double accTime,int accMode)
{
	//distance=-distance;
	switch(accMode)
	{
	case 0:d1000_start_t_move(AXISX,long(distance/_coeffX),minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	case 1:d1000_start_s_move(AXISX,long(distance/_coeffX),minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	}
}

void Motion::yMove(double maxVec,double distance,double accTime,int accMode)
{
	switch(accMode)
	{
	case 0:d1000_start_t_move(AXISY,long(distance/_coeffY),minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	case 1:d1000_start_s_move(AXISY,long(distance/_coeffY),minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	}
}

void Motion::zMove(double maxVec,double distance,double accTime,int accMode)
{
	distance=-distance;
	switch(accMode)
	{
	case 0:d1000_start_t_move(AXISZ,long(distance/_coeffZ),minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	case 1:d1000_start_s_move(AXISZ,long(distance/_coeffZ),minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	}
}

/*****************************************************************************
Function:Motion::xStop()
Description:x��ͣ��
Call:SysTrackingSys��ManuanlControl
Input:
Output:
Return:
Others:
*****************************************************************************/
void Motion::xStop()
{
	d1000_decel_stop(AXISX);
}


void Motion::yStop()
{
	d1000_decel_stop(AXISY);
}

void Motion::zStop()
{
	d1000_decel_stop(AXISZ);
}

void Motion::allStop()
{
	d1000_decel_stop(0);
	d1000_decel_stop(1);
	d1000_decel_stop(2);
	d1000_decel_stop(3);
}


void Motion::run()
{
	while(true)
	{
		sleep(500);
	}
}
