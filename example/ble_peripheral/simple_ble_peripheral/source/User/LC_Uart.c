/**
 *	@file		LC_Uart.h
 *	@author		YQ(1500861686)
 *	@date		04/10/2021
 *	@version	1.0.0
 *
 */

/*!
 * 	@defgroup	LC_Uart
 *	@brief
 *	@{*/

/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Uart.h"
#include "simpleGATTprofile.h"
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8       LC_Uart_TaskID;
BUP_ctx_t   mBUP_Ctx;
/*------------------------------------------------------------------*/
/* 					 	local variables		 					    */
/*------------------------------------------------------------------*/
// static      uint8   UartData_IDSaved[20];
/*------------------------------------------------------------------*/
/* 					 	local functions                             */
/*------------------------------------------------------------------*/
static  void    on_BUP_Evt(BUP_Evt_t* pev)
{
    switch(pev->ev){

    }
}
/**
 * @brief 
 * 
 * @param pev 
 */
void	LC_USART_Handler_Evt(uart_Evt_t* pev)
{
    BUP_ctx_t*  pctx    =   &mBUP_Ctx;
	// LOG("%s,%s,Line %d\n",__FILE__,__func__,__LINE__);
    switch (pev->type)
    {
        case    UART_EVT_TYPE_RX_DATA:
            if((pctx->hal_uart_rx_size + pev->len) >= UART_RX_BUF_SIZE)
                break;
            uartrx_timeout_tiemr_stop();
            uartrx_timeout_timer_start();
            osal_memcpy(pctx->hal_uart_rx_buf + pctx->hal_uart_rx_size, pev->data, pev->len);
            pctx->hal_uart_rx_size  +=  pev->len;
        break;

        case    UART_EVT_TYPE_RX_DATA_TO:
            if((pctx->hal_uart_rx_size + pev->len) >= UART_RX_BUF_SIZE)
                break;
            uartrx_timeout_tiemr_stop();
            uartrx_timeout_timer_start();
            osal_memcpy(pctx->hal_uart_rx_buf + pctx->hal_uart_rx_size, pev->data, pev->len);
            pctx->hal_uart_rx_size  +=  pev->len;
            
        break;

        case    UART_EVT_TYPE_TX_COMPLETED:
            osal_set_event(LC_Uart_TaskID, UART_EVT_UART_TX_COMPLETE);
        break;

        default:
		
        break;
    }
}
//timer for uart send delay slot
static  void    rx_start_timer(uint16_t timeout)
{
	LOG("uart start Timer\n");
	osal_start_timerEx(LC_Uart_TaskID, UART_EVT_UARTRX_TIMER, timeout);
}
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
void    uartrx_timeout_timer_start(void){
    osal_start_timerEx(LC_Uart_TaskID, UART_EVT_TO_TIMER, 10);
}
void    uartrx_timeout_tiemr_stop(void){
    osal_stop_timerEx(LC_Uart_TaskID, UART_EVT_TO_TIMER);
}
void	LC_UART_TX_Send(uint8 *data, uint16 len){
	osal_memcpy(mBUP_Ctx.tx_buf, data, len);
	mBUP_Ctx.tx_size	=	len;
	mBUP_Ctx.tx_state	=	BUP_TX_ST_SENDING;
	osal_set_event(LC_Uart_TaskID, UART_EVT_UART_TX_COMPLETE);
}
void	user_UART1_IRQHandler()
{
	hal_UART1_IRQHandler();
}
/**
 * @brief 
 * 
 * @param cb 
 * @return int 
 */
int    LC_Uart_Init(BUP_CB_t cb){
    BUP_ctx_t*   pctx   =   &mBUP_Ctx;
	hal_uart_deinit(UART1);
    uart_Cfg_t  CDX_Cfg =   {
        .tx_pin         =   UART_TX_PIN,
        .rx_pin         =   UART_RX_PIN,
        .rts_pin        =   GPIO_DUMMY,
        .cts_pin        =   GPIO_DUMMY,
        .baudrate       =   Uart_Baudrate,
        .use_fifo       =   TRUE,
        .hw_fwctrl      =   FALSE,
        .use_tx_buf     =   FALSE,
        .parity         =   FALSE,
		.fcr			=	FCR_TX_FIFO_RESET | FCR_RX_FIFO_RESET | FCR_FIFO_ENABLE | UART_FIFO_TX_TRIGGER | UART_FIFO_RX_TRIGGER,
        .evt_handler    =   LC_USART_Handler_Evt,
    };
    JUMP_FUNCTION_SET(UART1_IRQ_HANDLER,(uint32_t)&user_UART1_IRQHandler);

    hal_uart_init(CDX_Cfg, UART1);
    hal_uart_set_tx_buf(UART1, pctx->hal_uart_tx_buf, UART_TX_BUF_SIZE);

    // pwrmgr_register(MOD_USR1, NULL, NULL);
	osal_memset(&mBUP_Ctx, 0, sizeof(mBUP_Ctx));
    return  PPlus_SUCCESS;
}

int     LC_Uart_RXData(void){
    BUP_ctx_t*  pctx    =   &mBUP_Ctx;

    osal_memcpy(pctx->rx_buf + pctx->rx_size, pctx->hal_uart_rx_buf, pctx->hal_uart_rx_size);
    if(pctx->rx_offset != 0){
        return  PPlus_ERR_BLE_BUSY;
    }

    pctx->rx_size +=    pctx->hal_uart_rx_size;
    pctx->hal_uart_rx_size  =   0;
    switch(pctx->rx_state){
        case BUP_RX_ST_IDLE:
            pctx->rx_state  =   BUP_RX_ST_DELAY_SLOT;
            rx_start_timer(1);
            break;
        case BUP_RX_ST_DELAY_SLOT:
        case BUP_RX_ST_SENDING:
        default:
            return  PPlus_ERR_INVALID_STATE;
    }
    return  PPlus_SUCCESS;
    
}

int     LC_Uart_TXData(void){
    BUP_ctx_t*  pctx    =   &mBUP_Ctx;
    if(pctx->tx_size && pctx->tx_state != BUP_TX_ST_IDLE){
        hal_uart_send_buff(UART1, pctx->tx_buf, pctx->tx_size);
        pctx->tx_size   =   0;
        pctx->tx_state  =   BUP_TX_ST_SENDING;
        
        return  PPlus_SUCCESS;
    }
    LOG("uart send err state\n");
    return PPlus_ERR_INVALID_STATE;
}

int     LC_Uart_TXDataDown(void){
    BUP_ctx_t* pctx =   &mBUP_Ctx;
    if(pctx->tx_size){
        LC_Uart_TXData();
        return  PPlus_SUCCESS;
    }
    pctx->tx_state  =   BUP_TX_ST_IDLE;
    return  PPlus_SUCCESS;
}
/**
 * @brief 
 * 
 * @param task_id 
 */
void    LC_Uart_Task_Init(uint8 task_id)
{
    LC_Uart_TaskID  =   task_id;
	pwrmgr_register(MOD_USR8, NULL, NULL);
	pwrmgr_lock(MOD_USR8);	
    LC_Uart_Init(on_BUP_Evt);
    LOG("uart task init\n");
}
/**
 * @brief       UI_LED_BUZZER Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 * 
 * @param task_id   :The OSAL assigned task ID.
 * @param events    :events to process.This is a bit map and can
 *									contain more than one event.
 * @return uint16 
 */
uint16	LC_Uart_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id;	// OSAL required parameter that isn't used in this function
	if(events & SYS_EVENT_MSG){
		uint8	*pMsg;
		if((pMsg = osal_msg_receive(LC_Uart_TaskID)) != NULL){
			LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
            // Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}
		return(events ^ SYS_EVENT_MSG);
	}
    if(events & UART_EVT_TO_TIMER){
        LOG("RX_TO\n");
        LC_Uart_RXData();
        return (events ^ UART_EVT_TO_TIMER);
    }
    if(events & UART_EVT_UARTRX_TIMER){
        BUP_ctx_t*  pctx    =   &mBUP_Ctx;
        uint8   size        =   0;
        bool    start_flag  =   FALSE;
        // uint8   readbuff[20]    =   {0, };
        if(pctx->rx_state != BUP_RX_ST_IDLE && pctx->rx_size){

            if(pctx->rx_state == BUP_RX_ST_DELAY_SLOT){
                start_flag = TRUE;
                // FLOW_CTRL_BLE_TX_LOCK();
                pctx->rx_state = BUP_RX_ST_SENDING;
            }

            size    =   ((pctx->rx_size - pctx->rx_offset));
            LOG("uart data:\n");
            LOG_DUMP_BYTE(pctx->rx_buf + pctx->rx_offset, size);
			osal_memcpy(LC_App_Set_Param.app_notify_data, pctx->rx_buf + pctx->rx_offset, size);
			LC_App_Set_Param.app_notify_len	=	size;
			osal_set_event(LC_Uart_TaskID, UART_EVT_TEST_NOTIFY);
            LOG("Success\n");
            pctx->rx_state = BUP_RX_ST_IDLE;
            pctx->rx_offset = 0;
            pctx->rx_size = 0;
            // FLOW_CTRL_BLE_TX_UNLOCK();
            return PPlus_SUCCESS;

        }else{
            LOG("U2B s=:%x,%x",pctx->rx_state,pctx->rx_size);
            pctx->rx_state = BUP_RX_ST_IDLE;
            pctx->rx_offset = 0;
            pctx->rx_size = 0;
            // FLOW_CTRL_BLE_TX_UNLOCK();

        }
        return (events ^ UART_EVT_UARTRX_TIMER);
    }
    
    if(events & UART_EVT_UART_TX_COMPLETE){
        //data uart send
        LOG("TX complete\n");
        LC_Uart_TXDataDown();
        return(events ^ UART_EVT_UART_TX_COMPLETE);
    }
//====================TEST=========================
	if(events & UART_EVT_TEST_NOTIFY)
	{
		SimpleGATTProfile_Notify(0, LC_App_Set_Param.app_notify_len, LC_App_Set_Param.app_notify_data);
		osal_memset(LC_App_Set_Param.app_notify_data, 0, LC_App_Set_Param.app_notify_len);
		LC_App_Set_Param.app_notify_len	=	0;
		return(events ^ UART_EVT_TEST_NOTIFY);
	}


	if(events & UART_EVT_TEST_SEND)
	{
		LC_UART_TX_Send(LC_App_Set_Param.app_write_data, LC_App_Set_Param.app_write_len);
		osal_memset(LC_App_Set_Param.app_write_data, 0, LC_App_Set_Param.app_write_len);
		LC_App_Set_Param.app_write_len	=	0;
		return(events ^ UART_EVT_TEST_SEND);
	}
    // Discard unknown events
    return 0;
}
/** @}*/
