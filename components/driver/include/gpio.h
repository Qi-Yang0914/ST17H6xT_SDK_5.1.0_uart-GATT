/*******************************************************************************
* @file     gpio.h
* @brief    Contains all functions support for gpio driver
* @version  0.0
* @date     27. Nov. 2017
* @author   qing.han
* 
* 
* 
*
*******************************************************************************/
#ifndef __GPIO_ROM_H__
#define __GPIO_ROM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>
//#include "bus_dev.h"
#include "error.h"

//special gpio:P35~P40,no pull,no retention,no wakeup
//#define SPECIAL_GPIO_MIN P35
//#define SPECIAL_GPIO_NUM 6

typedef enum{
    GPIO_P00   =   0,    P0  =  GPIO_P00,
    GPIO_P01   =   1,    P1  =  GPIO_P01,
    GPIO_P02   =   2,    P2  =  GPIO_P02,
    GPIO_P03   =   3,    P3  =  GPIO_P03,
    GPIO_P07   =   4,    P7  =  GPIO_P07, 
    GPIO_P09   =   5,    P9  =  GPIO_P09,
    GPIO_P10   =   6,    P10  =  GPIO_P10,
    GPIO_P11   =   7,    P11  =  GPIO_P11,   Analog_IO_0 = GPIO_P11,
    GPIO_P14   =   8,    P14  =  GPIO_P14,   Analog_IO_1 = GPIO_P14,
    GPIO_P15   =   9,    P15  =  GPIO_P15,   Analog_IO_2 = GPIO_P15,
    GPIO_P16   =   10,   P16  =  GPIO_P16,   Analog_IO_3 = GPIO_P16,XTALI = GPIO_P16,
    GPIO_P17   =   11,   P17  =  GPIO_P17,   Analog_IO_4 = GPIO_P17,XTALO = GPIO_P17,
    GPIO_P18   =   12,   P18  =  GPIO_P18,   Analog_IO_5 = GPIO_P18,
    GPIO_P20   =   13,   P20  =  GPIO_P20,   Analog_IO_6 = GPIO_P20,
    GPIO_P23   =   14,   P23  =  GPIO_P23,   Analog_IO_7 = GPIO_P23,
    GPIO_P24   =   15,   P24  =  GPIO_P24,   Analog_IO_8 = GPIO_P24,
    GPIO_P25   =   16,   P25  =  GPIO_P25,   Analog_IO_9 = GPIO_P25,
    GPIO_P26   =   17,   P26  =  GPIO_P26,
    GPIO_P27   =   18,   P27  =  GPIO_P27,
    GPIO_P31   =   19,   P31  =  GPIO_P31,
    GPIO_P32   =   20,   P32  =  GPIO_P32,
    GPIO_P33   =   21,   P33  =  GPIO_P33,
    GPIO_P34   =   22,   P34  =  GPIO_P34,
    GPIO_NORMAL = GPIO_P34,
  	GPIO_P35   =   23,   P35  =  GPIO_P35,
  	GPIO_P36   =   24,   P36  =  GPIO_P36,
  	GPIO_P37   =   25,   P37  =  GPIO_P37,
  	GPIO_P38   =   26,   P38  =  GPIO_P38,
  	GPIO_P39   =   27,   P39  =  GPIO_P39,
  	GPIO_P40   =   28,   P40  =  GPIO_P40,
	
    GPIO_NUM   =   29,
    GPIO_DUMMY =  0xff,
}gpio_pin_e;

typedef enum {  
    FMUX_IIC0_SCL= 0,
    FMUX_IIC0_SDA= 1,
    FMUX_IIC1_SCL= 2,
    FMUX_IIC1_SDA= 3,
    FMUX_UART0_TX=4,        FMUX_UART_TX=4,
    FMUX_UART0_RX=5,        FMUX_UART_RX=5,
    FMUX_RF_RX_EN=6,
    FMUX_RF_TX_EN=7,
    FMUX_UART1_TX=8,
    FMUX_UART1_RX=9,
    FMUX_PWM0=10,
    FMUX_PWM1=11,
    FMUX_PWM2=12,
    FMUX_PWM3=13,
    FMUX_PWM4=14,
    FMUX_PWM5=15, 
    FMUX_SPI_0_SCK=16,
    FMUX_SPI_0_SSN=17,
    FMUX_SPI_0_TX=18, 
    FMUX_SPI_0_RX=19, 
    FMUX_SPI_1_SCK=20,
    FMUX_SPI_1_SSN=21,
    FMUX_SPI_1_TX=22, 
    FMUX_SPI_1_RX=23, 
    FMUX_CHAX=24,    
    FMUX_CHBX=25, 
    FMUX_CHIX=26, 
    FMUX_CHAY=27,
    FMUX_CHBY=28, 
    FMUX_CHIY=29, 
    FMUX_CHAZ=30, 
    FMUX_CHBZ=31, 
    FMUX_CHIZ=32, 
    FMUX_CLK1P28M=33,
    FMUX_ADCC=34,
    FMUX_ANT_SEL_0=35,
    FMUX_ANT_SEL_1=36,
    FMUX_ANT_SEL_2=37,
	FMUX_MISO_0=38,
	FMUX_CLK=39,
	FMUX_CSN=40,
	FMUX_MISO_1=41,
	FMUX_WPB_DQ2=42,
	FMUX_HOLDB_QD3=43,

}gpio_fmux_e;
 
typedef enum{
    FRE_HCLK_DIV8 = 0,
    FRE_PCLK_DIV4 = 1,
    FRE_CLK_1P28M = 2,
    FRE_CLK_RC32K = 6,
    FRE_XTAL_CLK32768 = 7,
}Freq_Type_e;

typedef enum{
    GPIO_INPUT  = 0,IE = 0,
    GPIO_OUTPUT = 1,OEN = 1,
}gpio_dir_t;


typedef enum{
    POL_FALLING = 0, POL_ACT_LOW  = 0,NEGEDGE = 0,
    POL_RISING  = 1, POL_ACT_HIGH = 1,POSEDGE = 1,
}gpio_polarity_e;


typedef enum
{
    GPIO_FLOATING   = 0x00, FLOATING = 0x00,       //no pull
    GPIO_PULL_UP    = 0x02, WEAK_PULL_UP = 0x02,   //1M
    GPIO_PULL_UP_S  = 0x01, STRONG_PULL_UP = 0x01, //10K
    GPIO_PULL_DOWN  = 0x03, PULL_DOWN = 0x03,      //150K
} gpio_pupd_e;
	

typedef enum{
    Bit_DISABLE = 0,
    Bit_ENABLE = 1,
}bit_action_e;

enum{
    GPIO_EVT_EDGE_RISING = 1,
    GPIO_EVT_EDGE_FALLING
};

typedef struct
{
    gpio_pin_e pin;
    gpio_pupd_e type;
} ioinit_cfg_t;

#define IO_Wakeup_Pol_e     gpio_polarity_e
#define GPIO_Pin_e          gpio_pin_e
#define Fmux_Type_e         gpio_fmux_e
#define GPIO_Wakeup_Pol_e   gpio_polarity_e
#define BitAction_e         bit_action_e

//gpio pin callback,pos or neg
typedef void (*gpioin_Hdl_t)(gpio_pin_e pin,gpio_polarity_e type);

typedef struct 
{	
	bool          enable;
	gpio_pin_e    pin;
	int           pin_state;
	gpioin_Hdl_t  posedgeHdl;
	gpioin_Hdl_t  negedgeHdl;
    
}gpioin_t;



typedef struct 
{
  int             state;
  uint32_t        retention_map;
  int             gpioin_nums;
  gpioin_t*  	  gpioin_ctx;
}gpio_Ctx_t;

/*
 * rom use:
 * *_r
 *
*/
void gpio_init_r(void);
void gpio_dir_r(gpio_pin_e pin, gpio_dir_t dir);
int gpio_in_trigger_r(gpio_pin_e pin, comm_cb_t in_cb);
void gpio_interrupt_set_r(gpio_pin_e pin, bit_action_e en);
void GPIO_IRQHandler_r(void);

/*
 * 
 * */
int gpio_init(void);
int rom_gpio_init(void);
int gpio_dir(gpio_pin_e pin, gpio_dir_t dir);
void gpio_write(gpio_pin_e pin, bit_action_e en);
void gpio_fast_write(gpio_pin_e pin, uint8_t en);
bool gpio_read(gpio_pin_e pin);

void gpio_pull_set(gpio_pin_e pin, gpio_pupd_e type);
void gpio_wakeup_set(gpio_pin_e pin, gpio_polarity_e type);
void gpio_ds_control(gpio_pin_e pin, bit_action_e value);

void gpio_pin2pin3_control(gpio_pin_e pin, bool en);//0:sw,1:other func
int gpio_cfg_analog_io(gpio_pin_e pin, bit_action_e value);

void gpio_fmux_control(gpio_pin_e pin, bit_action_e value);
void gpio_fmux_set(gpio_pin_e pin,gpio_fmux_e type);


int gpioin_init(gpioin_t* gpioin, int pin_nums);
int gpioin_disable(gpioin_t* p_gpioin_ctx);
int gpioin_enable(gpioin_t* p_gpioin_ctx);
int gpioin_register(gpio_pin_e pin, gpioin_Hdl_t posedgeHdl, gpioin_Hdl_t negedgeHdl);
int gpioin_unregister(gpio_pin_e pin);

void gpio_debug_mux(Freq_Type_e fre,bool en);
void gpio_retention(gpio_pin_e pin, bool en);

gpio_Ctx_t* gpio_get_handler(void);

void GPIO_IRQHandler(void);


#ifdef __cplusplus
}
#endif


#endif
