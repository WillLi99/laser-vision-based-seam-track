#ifndef _LEISAI_
#define _LEISAI_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DMC1000_EXPORTS
#define DMC1000_API __declspec(dllexport)
#else
#define DMC1000_API __declspec(dllimport)
#endif
	// This class is exported from the dmc1000.dll
	class DMC1000_API CDmc1000 {
	public:
		CDmc1000(void);
		// TODO: add your methods here.
	};
#define WORD unsigned short int
#define WINAPI      __stdcall
typedef unsigned long       DWORD;

	extern DMC1000_API int nDmc1000;

	//DMC1000_API int fnDmc1000(void);

	///////////////////////////////////////////////////////////
	/////////////////////DMC1000 V1.1�����б�//////////////////
	///////////////////////////////////////////////////////////

	//////////////////��ʼ������////////////////////
	DMC1000_API DWORD WINAPI d1000_board_init(void);
	DMC1000_API DWORD WINAPI d1000_board_close(void);
	DMC1000_API DWORD WINAPI d1000_get_base_addr(short CardNo);
	DMC1000_API DWORD WINAPI d1000_get_irq_channel(short CardNo);

	//////////////////����������ú���//////////////
	DMC1000_API DWORD WINAPI d1000_set_pls_outmode(short axis, short pls_outmode);

	//////////////////�ٶ�ģʽ�˶�����//////////////
	DMC1000_API DWORD WINAPI d1000_start_tv_move(short axis, long StrVel, long MaxVel, double Tacc);
	DMC1000_API DWORD WINAPI d1000_start_sv_move(short axis, long StrVel, long MaxVel, double Tacc);
	DMC1000_API DWORD WINAPI d1000_get_speed(short axis);
	DMC1000_API DWORD WINAPI d1000_change_speed(short axis, long MaxVel);
	DMC1000_API DWORD WINAPI d1000_decel_stop(short axis);

	//////////////////����λ��ģʽ����//////////////
	DMC1000_API DWORD WINAPI d1000_start_t_move(short axis, long Dist, long StrVel, long MaxVel, double Tacc);
	DMC1000_API DWORD WINAPI d1000_start_ta_move(short axis, long Pos, long StrVel, long MaxVel, double Tacc);
	DMC1000_API DWORD WINAPI d1000_start_s_move(short axis, long Dist, long StrVel, long MaxVel, double Tacc);
	DMC1000_API DWORD WINAPI d1000_start_sa_move(short axis, long Pos, long StrVel, long MaxVel, double Tacc);

	//////////////////���Բ岹����//////////////////
	DMC1000_API DWORD WINAPI d1000_start_t_line(short TotalAxis, short *AxisArray, long *DistArray, long StrVel, long MaxVel, double Tacc);
	DMC1000_API DWORD WINAPI d1000_start_ta_line(short TotalAxis, short *AxisArray, long *PosArray, long StrVel, long MaxVel, double Tacc);

	//////////////////Բ���岹����//////////////////
	DMC1000_API DWORD WINAPI d1000_start_t_arc(short *AxisArray, long OffsetC1, long OffsetC2, double Angle, long StrVel, long MaxVel, double Tacc);

	//////////////////��ԭ�㺯��////////////////////
	DMC1000_API DWORD WINAPI d1000_home_move(short axis, long StrVel, long MaxVel, double Tacc);

	//////////////////�˶�״̬��⺯��//////////////
	DMC1000_API DWORD WINAPI d1000_check_done(short axis);

	//////////////////λ���趨�Ͷ�ȡ����////////////
	DMC1000_API DWORD WINAPI d1000_get_command_pos(short axis);
	DMC1000_API DWORD WINAPI d1000_set_command_pos(short axis, long Pos);

	//////////////////ͨ��I/O����///////////////////
	DMC1000_API DWORD WINAPI d1000_out_bit(short BitNo, short BitData);
	DMC1000_API DWORD WINAPI d1000_in_bit(short BitNo);
	DMC1000_API DWORD WINAPI d1000_get_outbit(short BitNo);

	//////////////////ר��I/O�ӿں���///////////////
	DMC1000_API DWORD WINAPI d1000_set_sd(short axis, short SdMode);
	DMC1000_API DWORD WINAPI d1000_get_axis_status(short axis);


#ifdef __cplusplus
}
#endif

#endif 
