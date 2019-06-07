/******************************************************************************
  File name: motion.cpp
  Author: WillLi99		Date:2019-5-23
  Description:
              Motion���װ�˻���ƽ̨�����˶���Ҫ�õ��ĺ�����Run�ຯ���������˶�������
			  Move�ຯ���Ƕ����˶��������˶����ƻ���laisai dmc1000���ƿ����ƣ�����
			  ���ƿ�0��ӻ���ƽ̨x�ᣬ���ƿ�2��ӻ���ƽ̨z�ᣬ���ƿ�3��ӻ���ƽ̨y�ᡣ
			  ����ƽ̨�ϸ���ͷ���������ʾ��
                                 ^z+
                                 |
                                 |-----> y+
                                 /
                                /
                             <x+
  Others: 
  Function List:
                Motion
				~Motion
				xRun	//X�������˶�
				yRun	
				zRun	
				xMove	//X�ᶨ���˶�
				yMove	
				zMove	
				xStop	//X��ֹͣ
				yStop	
				zStop	
				allStop	//������ֹͣ
				run	
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-23     ���motion.cppͷ��ע��
******************************************************************************/
#include "motion.h"
#include "dmc1000.h"
#define minVecPPS 100	//���ƿ��˶����ٶȣ���λ��������ÿ��

/******************************************************************************
  Function:Motion::Motion
  Description: ��ʼ������
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
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

/******************************************************************************
  Function:Motion::~Motion(void)
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Motion::~Motion(void)
{

}

/******************************************************************************
  Function:xRun
  Description: x�������˶�
  Calls: 
  Called By: 
  Input:maxVec-��������ٶȣ���λmm/s��;accTime-����ʱ�䣨��λs);accMode-�������ͣ�=1
		Ϊ���μ��٣�=2ΪS�ͼ���
  Output: 
  Return:       
  Others: ����������ߵĲ��죬��ʱҪ��maxVecȡ����
******************************************************************************/
void Motion::xRun(double maxVec,double accTime,int accMode)
{
	//maxVec=-maxVec;	
	switch(accMode)
	{
	case 1:d1000_start_tv_move(AXISX,minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	case 2:d1000_start_sv_move(AXISX,minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	}
}

/******************************************************************************
  Function:yRun
  Description: y�������˶�
  Calls: 
  Called By: 
  Input:  maxVec-��������ٶȣ���λmm/s��;accTime-����ʱ�䣨��λs);accMode-�������ͣ�=1
		Ϊ���μ��٣�=2ΪS�ͼ���        
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::yRun(double maxVec,double accTime,int accMode)
{
	//maxVec=-maxVec;
	switch(accMode){
	case 1:d1000_start_tv_move(AXISY,minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	case 2:d1000_start_sv_move(AXISY,minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	}	
}

/******************************************************************************
  Function:zRun
  Description: z�������˶�
  Calls: 
  Called By: 
  Input:  maxVec-��������ٶȣ���λmm/s��;accTime-����ʱ�䣨��λs);accMode-�������ͣ�=1
		Ϊ���μ��٣�=2ΪS�ͼ���        
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::zRun(double maxVec,double accTime,int accMode)
{
	maxVec=-maxVec;
	switch(accMode)
	{
	case 1:d1000_start_tv_move(AXISZ,minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	case 2:d1000_start_sv_move(AXISZ,minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	}
}


/******************************************************************************
  Function:xMove
  Description: x�ᶨ���˶�
  Calls: 
  Called By: 
  Input: maxVec-��������ٶȣ���λmm/s��;distance-�˶����룬���������򣨵�λmm��;
		accTime-����ʱ�䣨��λs);accMode-��������         
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::xMove(double maxVec,double distance,double accTime,int accMode)
{
	//distance=-distance;
	switch(accMode)
	{
	case 1:d1000_start_t_move(AXISX,long(distance/_coeffX),minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	case 2:d1000_start_s_move(AXISX,long(distance/_coeffX),minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	}
}

/******************************************************************************
  Function:yMove
  Description: y�ᶨ���˶�
  Calls: 
  Called By: 
  Input: maxVec-��������ٶȣ���λmm/s��;distance-�˶����룬���������򣨵�λmm��;
		accTime-����ʱ�䣨��λs);accMode-��������     
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::yMove(double maxVec,double distance,double accTime,int accMode)
{
	switch(accMode)
	{
	case 1:d1000_start_t_move(AXISY,long(distance/_coeffY),minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	case 2:d1000_start_s_move(AXISY,long(distance/_coeffY),minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	}
}

/******************************************************************************
  Function:zMove
  Description: z�ᶨ���˶�
  Calls: 
  Called By: 
  Input:   maxVec-��������ٶȣ���λmm/s��;distance-�˶����룬���������򣨵�λmm��;
  accTime-����ʱ�䣨��λs);accMode-��������       
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::zMove(double maxVec,double distance,double accTime,int accMode)
{
	distance=-distance;
	switch(accMode)
	{
	case 1:d1000_start_t_move(AXISZ,long(distance/_coeffZ),minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	case 2:d1000_start_s_move(AXISZ,long(distance/_coeffZ),minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	}
}

/******************************************************************************
  Function:xStop
  Description: x��ֹͣ����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::xStop()
{
	d1000_decel_stop(AXISX);
}

/******************************************************************************
  Function:yStop
  Description: y��ֹͣ����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::yStop()
{
	d1000_decel_stop(AXISY);
}

/******************************************************************************
  Function:zStop
  Description: z��ֹͣ����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::zStop()
{
	d1000_decel_stop(AXISZ);
}

/******************************************************************************
  Function:allStop
  Description: ������ֹͣ����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::allStop()
{
	d1000_decel_stop(0);
	d1000_decel_stop(1);
	d1000_decel_stop(2);
	d1000_decel_stop(3);
}

/******************************************************************************
  Function:run
  Description: �߳�����
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::run()
{
	while(true)
	{
		sleep(500);
	}
}