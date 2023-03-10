/**
*	@file	LC_Key.h
*	@author		YQ(1500861686)
*	@date	12/14/2022
*
*/

/*!
 * 	@defgroup	LC_Key
 *	@brief
 *	@{*/
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Key.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/
// static uint8 key_err = 0;
// static uint8 Key_Press_Once_Enable = 0;
// static uint8 Key_Long_Press_3s_Enable = 0;

/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8 LC_Key_TaskID;
gpioin_t	pin_test[4];
/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/
static	void	LC_Key_Pin_IntHandler_Pos(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
	LOG("wakeup0\n");
	if(pin == GPIO_KEY_PWR)
	{
		gpioin_unregister(GPIO_KEY_PWR);
		osal_start_timerEx(LC_Key_TaskID, KEY_RELEASE_EVT, 30);
	}
}
static	void	LC_Key_Pin_IntHandler_Neg(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
	LOG("wakeup1\n");
	if(pin == GPIO_KEY_PWR)
		osal_start_timerEx(LC_Key_TaskID, KEY_PRESS_EVT, 30);

}
/**
 * @brief	
 * 
 */
static	void	Key_Pin_Config(void)
{
	//	key
	gpioin_init(pin_test,sizeof(pin_test)/sizeof(pin_test[0]));

	gpio_dir(GPIO_KEY_PWR, IE);						//set gpio input
    gpio_pull_set(GPIO_KEY_PWR, STRONG_PULL_UP);	//pull up 10k
}
/*!
 *	@fn			LC_Switch_Poweron
 *	@brief		press switch to power on.
 *	@param[in]	cur_state	:
 *	@param[in]	poweron_key_time	:set time for long press to poweron,
 *									power_start_tick*delay_ms
 *	@return		none.
 */
static	void LC_Switch_Poweron(uint8 cur_state, uint8 poweron_key_time)
{
	if(LC_Dev_System_Param.dev_poweron_switch_flag)
	{
		LC_Dev_System_Param.dev_power_flag		=	SYSTEM_WORKING;
		return;
	}
	uint8	poweron_start_num	=	poweron_key_time;

	if(!cur_state)
	{
		while(poweron_start_num)
		{
			if(gpio_read(GPIO_KEY_PWR) == 0)
			{
				poweron_start_num--;
				WaitMs(50);
				watchdog_feed();
				LOG("press first %d\n", poweron_start_num);
			}
			else
			{
				LOG("release \n");
				poweron_start_num	=	poweron_key_time;
				LC_Dev_System_Param.dev_power_flag		=	SYSTEM_POWEROFF;
				return ;
			}
		}
		LC_Dev_System_Param.dev_power_flag		=	SYSTEM_WORKING;
	}
}
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/*!
 *	@fn			LC_Key_Gpio_Init
 *	@brief		Initialize the key pins. 
 *	@param[in]	none.
 *	@return		none.
 */
void LC_KeyPowerProcess(void)
{
    Key_Pin_Config();

	LC_Switch_Poweron(0, 2);

}
/*!
 *	@fn			LC_Dev_Poweroff
 *	@brief		the process of power off,need to disable adv and all events.
 *	@param[in]	none.
 *	@return		none.
 */
void LC_Dev_Poweroff(void)
{

	// GAPRole_TerminateConnection();
	LOG("power off\n");
	// #if(DEBUG_INFO == 0)

	pwroff_cfg_t	User_Set_Wakeup[1]	=	
	{
		{
			.pin	=	GPIO_KEY_PWR,
			.type	=	NEGEDGE,
		},
	};

	// pwrmgr_unlock(MOD_USR8);
	// AP_WDT->CRR	=	0x76;	//	feed watch dog
	// while(gpio_read(GPIO_KEY_PWR) == 0)
	// {
	// 	WaitUs(10*1000);
	// 	AP_WDT->CRR	=	0x76;	//	feed watch dog
	// }
	pwrmgr_poweroff(&User_Set_Wakeup[0], 1);
	// #endif
}

/*!
 *	@fn			LC_Key_Task_Init 
 *	@brief		Initialize function for the KEY Task. 
 *	@param[in]	task_id		: 	the ID assigned by OSAL,
 *								used to send message and set timer.
 *	@retrurn	none.
 */
void LC_Key_Task_Init(uint8 task_id)
{
    LC_Key_TaskID = task_id;
	if(LC_Dev_System_Param.dev_power_flag == SYSTEM_WORKING)
	{
		gpioin_register(GPIO_KEY_PWR, LC_Key_Pin_IntHandler_Pos, NULL);
	}
	else if(LC_Dev_System_Param.dev_power_flag == SYSTEM_POWEROFF)
	{
		osal_start_timerEx(LC_Key_TaskID, KEY_POWEROFF_EVT, 100);
	}
	LOG("Key task init down%d\n", LC_Dev_System_Param.dev_power_flag);
}
/*!
 *	@fn			LC_Key_ProcessEvent
 *	@brief		KEY Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 *	@param[in]	task_id			:The OSAL assigned task ID.
 *	@param[in]	events			:events to process.This is a bit map and can
 *									contain more than one event.
 */
uint16 LC_Key_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id; // OSAL required parameter that isn't used in this function
	if (events & SYS_EVENT_MSG)
	{
		uint8 *pMsg;
		if ((pMsg = osal_msg_receive(LC_Key_TaskID)) != NULL)
		{
			LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
			// Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}
		return (events ^ SYS_EVENT_MSG);
	}

	if(events & KEY_POWEROFF_EVT)
	{
		LC_Dev_Poweroff();
		return(events ^ KEY_POWEROFF_EVT);
	}

	if(events & KEY_RELEASE_EVT)
	{
		if(gpio_read(GPIO_KEY_PWR) == 1)
		{
			osal_start_timerEx(LC_Key_TaskID, KEY_POWEROFF_EVT, 100);
		}
		else
		{
			gpioin_register(GPIO_KEY_PWR, LC_Key_Pin_IntHandler_Pos, NULL);
		}
		return(events ^ KEY_RELEASE_EVT);
	}

    // Discard unknown events
    return 0;
}
/** @}*/
