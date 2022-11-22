/**
 *	@file		LC_Uart.h
 *	@author		YQ(1500861686)
 *	@date		04/10/2021
 *	@version	1.0.0
 */

/*!
 *	@defgroup	LC_Uart
 *	@brief
 *	@{*/

#ifndef		LC_UART_H_
#define		LC_UART_H_
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
	extern "C"	{
#endif
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/
#define     UART_TX_PIN     P15
#define     UART_RX_PIN     P11
/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define     Uart_Baudrate       115200
#define     UART_RX_BUF_SIZE    1024
#define     UART_TX_BUF_SIZE    512

#define		UART_CMD_LEN		16
/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/
#define     UART_EVT_UARTRX_TIMER       0x0001
#define     UART_EVT_UART_TX_COMPLETE   0x0002
#define     UART_EVT_TO_TIMER           0x0004
#define		UART_EVT_ID_SAVE			0x0008
#define		UART_EVT_TX_DATA			0x0010
#define		UART_EVT_TEST_NOTIFY		0x0020
#define		UART_EVT_TEST_SEND			0x0040
/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/
typedef struct{
	bool    conn_state;
	//uart_rx
	uint8 rx_state;
	uint8 rx_size;
	uint8 rx_offset;
	uint8 rx_buf[UART_RX_BUF_SIZE];

	//uart tx
	uint8 tx_state;
	uint8 tx_size;
	uint8 tx_buf[UART_TX_BUF_SIZE];


	uint8 hal_uart_rx_size;
	uint8 hal_uart_rx_buf[UART_RX_BUF_SIZE];
	uint8 hal_uart_tx_buf[UART_TX_BUF_SIZE];
  
}BUP_ctx_t;

typedef struct {
  uint8 ev;

}BUP_Evt_t;

typedef     void    (*BUP_CB_t)(BUP_Evt_t* pev);

enum{
	BUP_RX_ST_IDLE = 0,
	BUP_RX_ST_DELAY_SLOT,
	BUP_RX_ST_SENDING
};

enum{
	BUP_TX_ST_IDLE = 0,
	BUP_TX_ST_DELAY_SLOT,
	BUP_TX_ST_SENDING
};

/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/
extern		uint8       LC_Uart_TaskID;
extern      BUP_ctx_t   mBUP_Ctx;
/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
void    uartrx_timeout_timer_start	(void);
void    uartrx_timeout_tiemr_stop	(void);
void	LC_UART_TX_Send				(uint8 *data, uint16 len);
int		LC_Uart_Init				(BUP_CB_t cb);
void    LC_Uart_Task_Init			(uint8 task_id);
uint16	LC_Uart_ProcessEvent		(uint8 task_id, uint16 events);
#ifdef	__cplusplus
	}
#endif

#endif	/**	LC_Uart.h **/
/**	@}*/
