/**
 *	@file		LC_Common.c
 *	@author		YQ(1500861686)
 *	@date		09/17/2020
 *	@version	1.0.3
 *
 */

/*!
 * 	@defgroup	LC_Common
 *	@brief
 *	@{*/
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/
static	uint8	LC_Timer_Working_Flag	=	State_Off;
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
lc_app_set_t		LC_App_Set_Param	=
{
	.app_write_data				=	{0},
	.app_notify_data			=	{0},
	.app_write_len				=	0,
	.app_notify_len				=	0,
};

/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/*!
 *	@fn			clock_time_exceed_func
 *	@brief		
 */
uint32 clock_time_exceed_func(uint32 ref, uint32 span_ms)
{
#if 0
	u32 deltTick ,T0 ;
	T0 = hal_read_current_time();
	deltTick =TIME_DELTA(T0,ref);
	if(deltTick>span_ms){
		return 1 ;
	}else {
		return 0 ;
	}
#else 
	uint32 deltTick  = 0 ;
	deltTick = get_ms_intv(ref) ;
	if(deltTick>span_ms){
		return 1 ;
	}else {
		return 0 ;
	}	
#endif
}
/*!
 *	@fn			LC_Common_ProcessOSALMsg
 *	@brief		Process an incoming task message,nothing.
 *	@param[in]	pMsg	:message to process
 *	@return		none.
 */
void LC_Common_ProcessOSALMsg(osal_event_hdr_t *pMsg)
{
	switch(pMsg->event)
	{
		default:
			// do nothing
		break;
	}
}
void LC_Timer_Start(void)
{
	if(LC_Timer_Working_Flag == State_Off)
	{
		// hal_timer_init(LC_RGB_Valeu_Deal);
		hal_timer_set(AP_TIMER_ID_4, 100);
		LC_Timer_Working_Flag	=	State_On;
	}
	else
	{
		return;
	}
}
void LC_Timer_Stop(void)
{
	if(LC_Timer_Working_Flag == State_On)
	{
		hal_timer_stop(AP_TIMER_ID_4);
		LC_Timer_Working_Flag	=	State_Off;
	}
	else
	{
		return;
	}
}
/** @}*/

