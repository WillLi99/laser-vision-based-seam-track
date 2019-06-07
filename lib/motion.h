/******************************************************************************
  File name: motion.h
  Author: WillLi99		Date:2019-5-23
  Description:
              ������Motion�࣬����̳���QThread��������˶�ƽ̨����������п���
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
           WillLi99    2019-5-23     ���motion.hͷ��ע��
******************************************************************************/


#ifndef MOTION_H
#define MOTION_H

#include "var.h"

#define AXISX 0
#define AXISY 3
#define AXISZ 2

//�˶���
class Motion:public QThread
{
	Q_OBJECT
public:
	Motion(void);
	~Motion(void);
public:
	void xRun(double maxVec,double accTime,int accMode=0);
	void yRun(double maxVec,double accTime,int accMode=0);
	void zRun(double maxVec,double accTime,int accMode=0);
	void xMove(double maxVec,double distance,double accTime,int accMode=0);
	void yMove(double maxVec,double distance,double accTime,int accMode=0);
	void zMove(double maxVec,double distance,double accTime,int accMode=0);
	void xStop();
	void yStop();
	void zStop();
	void allStop();
	void run();

private:
	double _coeffX;	//x�� ����Ƶ���˶��ٶ�ת��ϵ������pp/s->mm/s
	double _coeffZ;	//z�� ����Ƶ���˶��ٶ�ת��ϵ������pp/s->mm/s
	double _coeffY;	//y�� ����Ƶ���˶��ٶ�ת��ϵ������pp/s->mm/s

};

#endif