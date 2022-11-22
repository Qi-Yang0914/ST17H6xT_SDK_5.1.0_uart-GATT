/**
 *	@file		LC_Common.h
 *	@author		YQ(1500861686)
 *	@date		10/20/2020
 *	@version	1.0.1
 *
 */

/*!
 * 	@defgroup	LC_Common
 *	@brief
 *	@{*/
#ifndef		LC_COMMON_H_
#define		LC_COMMON_H_
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
	 extern  "C" {
#endif
/*------------------------------------------------------------------*/
/* 				 		head files include 							*/
/*------------------------------------------------------------------*/

#include "att.h"
#include "bcomdef.h"
#include "gapbondmgr.h"
#include "gapgattserver.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "global_config.h"
#include "hci.h"
#include "hci_tl.h"
#include "linkdb.h"
#include "ll.h"
#include "ll_def.h"
#include "ll_hw_drv.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "OSAL_Clock.h"
#include "pwm.h"
#include "pwrmgr.h"
#include "rf_phy_driver.h"
#include "osal_snv.h"
#include "gpio.h"
#include "watchdog.h"
#include "adc.h"

#include "simpleGATTprofile.h"
/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define	UART_BUFFER_LEN		200
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/
/* 					 	Data structures							 	*/
/*------------------------------------------------------------------*/
typedef uint8_t				u8;
typedef uint16_t			u16;
typedef uint32_t			u32;
typedef signed   char		int8_t;			//!< Signed 8 bit integer
typedef unsigned char		uint8_t;		//!< Unsigned 8 bit integer
typedef signed   short		int16_t;		//!< Signed 16 bit integer
typedef unsigned short		uint16_t;		//!< Unsigned 16 bit integer
typedef signed   int		int32_t;		//!< Signed 32 bit integer
typedef unsigned int		uint32_t;		//!< Unsigned 32 bit integer
typedef signed   char		int8;			//!< Signed 8 bit integer
typedef unsigned char		uint8;			//!< Unsigned 8 bit integer
typedef signed   short		int16;			//!< Signed 16 bit integer
typedef unsigned short		uint16;			//!< Unsigned 16 bit integer
typedef signed   long		int32;			//!< Signed 32 bit integer
typedef unsigned long		uint32;			//!< Unsigned 32 bit integer

typedef		enum	
{
	LC_DEV_BLE_DISCONNECTION	=	0,
	LC_DEV_BLE_CONNECTION,
}lc_dev_ble_state;

typedef		enum	
{
	State_Off	=	0,
	State_On	=	1,
}lc_state_bool;

typedef		enum	{
	SYSTEM_POWEROFF	=	0,	SYSTEM_STANDBY	=	0,
	SYSTEM_WORKING	=	1,
	SYSTEM_SUSPEND	=	2,
}lc_sys_run_t;

typedef	struct
{
	uint8		app_write_data[UART_BUFFER_LEN];
	uint8		app_notify_data[UART_BUFFER_LEN];
	uint8		app_write_len;
	uint8		app_notify_len;
}lc_app_set_t;



/*------------------------------------------------------------------*/
/* 					 external variables							 	*/
/*------------------------------------------------------------------*/
extern	lc_app_set_t		LC_App_Set_Param;
/*------------------------------------------------------------------*/
/* 					 User function prototypes					 	*/
/*------------------------------------------------------------------*/
extern	uint32	clock_time_exceed_func			(uint32 ref,		uint32 span_ms	);
extern	void	LC_Common_ProcessOSALMsg	 	(osal_event_hdr_t *pMsg				);
extern	void	LC_Timer_Start					(void);
extern	void	LC_Timer_Stop					(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LC_COMMON_H_ */
/** @}*/
