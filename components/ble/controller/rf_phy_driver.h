/**************************************************************************************************
**************************************************************************************************  
**************************************************************************************************/

/*******************************************************************************
* @file		rf_phy_driver.h
* @brief	Contains all functions support for RF_PHY_DRIVER
* @version	1.0
* @date		24. Aug. 2017
* @author	Zhongqi Yang
* 
* 
* 
*
*******************************************************************************/
#ifndef __RF_PHY_DRIVER_H_
#define __RF_PHY_DRIVER_H_




/*******************************************************************************
 * INCLUDES
 */
 
#include "rom_sym_def.h"
#include "clock.h"
#include "ll_hw_drv.h"
#include "ll_common.h"
#include "jump_function.h"



typedef enum  _RF_PHY_CLK_SEL {
    RF_PHY_CLK_SEL_16M_XTAL     = 0,
    RF_PHY_CLK_SEL_32M_DBL_B    = 1,
    RF_PHY_CLK_SEL_32M_DBL      = 2,
    RF_PHY_CLK_SEL_32M_DLL      = 3
 }rfphy_clk_t;

 typedef enum  _RX_ADC_CLK_SEL {
    RX_ADC_CLK_SEL_16M_XTAL     = 0,
    RX_ADC_CLK_SEL_32M_DBL_B    = 1,
    RX_ADC_CLK_SEL_32M_DBL      = 2,
    RX_ADC_CLK_SEL_32M_DLL      = 3
 }rxadc_clk_t;




/*******************************************************************************
 * Global Var
 */
 extern volatile uint8_t g_rfPhyTpCal0;            //** two point calibraion result0            **//
 extern volatile uint8_t g_rfPhyTpCal1;            //** two point calibraion result1            **//
 extern volatile uint8_t g_rfPhyTpCal0_2Mbps;            //** two point calibraion result0            **//
 extern volatile uint8_t g_rfPhyTpCal1_2Mbps;            //** two point calibraion result1            **//
 extern volatile uint8_t g_rfPhyTxPower;           //** rf pa output power setting [0x00 0x1f]  **//
 extern volatile uint8_t g_rfPhyPktFmt;            //** rf_phy pkt format config                **//
 extern volatile uint32  g_rfPhyRxDcIQ;            //** rx dc offset cal result                 **//
 extern volatile int8_t  g_rfPhyFreqOffSet;

extern volatile sysclk_t g_system_clk;
extern volatile rfphy_clk_t g_rfPhyClkSel;
extern volatile uint8_t   g_rc32kCalRes   ;
extern volatile rxadc_clk_t g_rxAdcClkSel;



extern volatile  uint8_t        g_rfPhyDtmCmd[];
extern volatile  uint8_t        g_rfPhyDtmEvt[];
extern volatile  uint8_t        g_dtmModeType ; 
extern volatile  uint8_t        g_dtmCmd      ; 
extern volatile  uint8_t        g_dtmFreq     ; 
extern volatile  uint8_t        g_dtmLength   ; 
extern volatile  uint8_t        g_dtmExtLen   ; 
extern volatile  uint16_t       g_dtmPktIntv  ; 
extern volatile  uint8_t        g_dtmPKT      ; 
extern volatile  uint8_t        g_dtmCtrl     ; 
extern volatile  uint8_t        g_dtmPara     ; 
extern volatile  uint8_t        g_dtmEvt      ; 
extern volatile  uint8_t        g_dtmStatus   ; 
extern volatile  uint16_t       g_dtmPktCount ; 
extern volatile  uint16_t       g_dtmRxCrcNum ; 
extern volatile  uint16_t       g_dtmRxTONum  ; 
extern volatile  uint16_t       g_dtmRsp      ; 
extern volatile  uint8_t        g_dtmTxPower  ; 
extern volatile  uint16_t       g_dtmFoff     ; 
extern volatile  uint8_t        g_dtmRssi     ; 
extern volatile  uint8_t        g_dtmCarrSens ; 
extern volatile  uint8_t        g_dtmTpCalEnable; 
extern volatile  uint32_t       g_dtmTick       ; 
extern volatile  uint32_t       g_dtmPerAutoIntv; 
extern volatile  uint32_t       g_dtmAccessCode ; 

extern volatile uint8_t         g_dtmManualConfig;

/*******************************************************************************
 * MACRO
 */
#define RF_PHY_EXT_PREAMBLE_US                      (8)             // ext ble preamble length

#define PHY_REG_RD(x)                               *(volatile uint32_t *)(x)
#define PHY_REG_WT(x,y)                             {*(volatile uint32_t *)(x) = (y);__asm volatile("nop");}
#define RF_CHN_TO_FREQ(x)
#define DCDC_REF_CLK_SETTING(x)                     subWriteReg(0x4000f014,25,25, (0x01&(x)))
#define DCDC_CONFIG_SETTING(x)                      subWriteReg(0x4000f014,18,15, (0x0f&(x)))
#define XTAL16M_CAP_SETTING(x)                      subWriteReg(0x4000f0bc, 4, 0, (0x1f&(x)))
#define XTAL16M_CURRENT_SETTING(x)                  subWriteReg(0x4000f0bc, 6, 5, (0x03&(x)))
#define DIG_LDO_CURRENT_SETTING(x)                  subWriteReg(0x4000f014,22,21, (0x03&(x)))
#define RF_PHY_LO_LDO_SETTING(x)                    subWriteReg(0x400300cc,11,10, (0x03&(x)))
#define RF_PHY_PA_VTRIM_SETTING(x)                  subWriteReg(0x400300dc, 9, 7, (0x03&(x)))
#define RF_PHY_LNA_LDO_SETTING(x)                   subWriteReg(0x400300dc, 6, 5, (0x03&(x)))
#define RF_PHY_TPCAL_CALC(tp0,tp1,chn)              ((tp0)>(tp1) ?(((tp0<<5)-(tp0-tp1)*(chn)+16)>>5) : tp0 )
//DTM STATE
#define RF_PHY_DTM_IDL                              0
#define RF_PHY_DTM_CMD                              1
#define RF_PHY_DTM_EVT                              2
#define RF_PHY_DTM_TEST                             3

#define RF_PHY_DTM_SYNC_WORD                        0x71764129
#define RF_PHY_DTM_PRBS9_SEED                       0xffffffff
#define RF_PHY_DTM_CRC_WT                           0x00555555

//DTM MODE TYPE
#define RF_PHY_DTM_MODE_RESET                       0
#define RF_PHY_DTM_MODE_TX_BURST                    2
#define RF_PHY_DTM_MODE_TX_CTMOD                    4
#define RF_PHY_DTM_MODE_TX_SINGLE                   6
#define RF_PHY_DTM_MODE_RX_PER                      8
#define RF_PHY_DTM_MODE_TEST_END                    10
#define RF_PHY_DTM_MODE_SET_LENGTH_UP2BIT           12

#define RF_PHY_DTM_MODE_SET_PHY_1M                  16
#define RF_PHY_DTM_MODE_SET_PHY_2M                  18
#define RF_PHY_DTM_MODE_SET_PHY_500K                20
#define RF_PHY_DTM_MODE_SET_PHY_125K                22
#define RF_PHY_DTM_MODE_SET_PHY_ZB                  24

#define RF_PHY_DTM_MODE_ASSUME_TX_MOD_INDX_STANDARD 32
#define RF_PHY_DTM_MODE_ASSUME_TX_MOD_INDX_STABLE   34
#define RF_PHY_DTM_MODE_READ_SUPPORTED_TEST_CASE    36
#define RF_PHY_DTM_MODE_READ_MAX_TX_OCTETS          38
#define RF_PHY_DTM_MODE_READ_MAX_TX_TIME            40
#define RF_PHY_DTM_MODE_READ_MAX_RX_OCTETS          42
#define RF_PHY_DTM_MODE_READ_MAX_RX_TIME            44

#define RF_PHY_DTM_MODE_SET_ACCCODE_0               114
#define RF_PHY_DTM_MODE_SET_ACCCODE_1               116
#define RF_PHY_DTM_MODE_SET_ACCCODE_2               118
#define RF_PHY_DTM_MODE_SET_ACCCODE_3               120

#define RF_PHY_DTM_MODE_SET_FREQ_FOFF               122
#define RF_PHY_DTM_MODE_SET_TPCAL_MANUAL            124
#define RF_PHY_DTM_MODE_SET_XTAL_CAP                126
#define RF_PHY_DTM_MODE_SET_TX_POWER                128
#define RF_PHY_DTM_MODE_GET_FOFF                    130
#define RF_PHY_DTM_MODE_GET_TPCAL                   132
#define RF_PHY_DTM_MODE_GET_RSSI                    134
#define RF_PHY_DTM_MODE_GET_CARR_SENS               136
#define RF_PHY_DTM_MODE_GET_PER_AUTO                138

#define RF_PHY_DTM_MODE_ATE_SET_PKTFMT              0xd0 //208
#define RF_PHY_DTM_MODE_ATE_SET_TXPOWER             0xd1

#define RF_PHY_DTM_MODE_ATE_TX_BURST                0xe0 //224
#define RF_PHY_DTM_MODE_ATE_TX_MOD                  0xe1 
#define RF_PHY_DTM_MODE_ATE_TX_CARR                 0xe2 
#define RF_PHY_DTM_MODE_ATE_RX_AUTOGAIN             0xe3 
#define RF_PHY_DTM_MODE_ATE_RX_FIXGAIN              0xe4 
#define RF_PHY_DTM_MODE_ATE_RX_DEMOD                0xe5 
#define RF_PHY_DTM_MODE_ATE_RX2TX                   0xe6 
#define RF_PHY_DTM_MODE_ATE_TX2RX                   0xe7 

#define RF_PHY_DTM_MODE_ATE_RESET                   0xef 

#define RF_PHY_DTM_MODE_ERROR                       254
/*
RF_PHY_DTM_REG_READ:   FE 02 AA BB CC DD -> aabbccdd=read_reg(0xAABBCCDD)
RF_PHY_DTM_REG_RD_RSP: FE 82 aa bb cc dd
RF_PHY_DTM_REG_WRITE:  FE 01 AA BB CC DD aa bb cc dd -> write_reg(0xAABBCCDD,0xaabbccdd)
*/
#define RF_PHY_DTM_REG_RW_OPCODE                    0xFE
#define RF_PHY_DTM_REG_READ                         0x01
#define RF_PHY_DTM_REG_WRITE                        0x02
#define RF_PHY_DTM_REG_RD_RSP                       0x81    

/*******************************************************************************
 * CONSTANTS
 */
#define PKT_FMT_ZIGBEE                              0
#define PKT_FMT_BLE1M                               1
#define PKT_FMT_BLE2M                               2
#define PKT_FMT_BLR500K                             3
#define PKT_FMT_BLR125K                             4

#define RF_PHY_TX_POWER_EXTRA_MAX                   0x3f
#define RF_PHY_TX_POWER_MAX                         0x1f
#define RF_PHY_TX_POWER_MIN                         0x00


#define RF_PHY_TX_POWER_7DBM                        0x1c
#define RF_PHY_TX_POWER_5DBM                        0x12
#define RF_PHY_TX_POWER_4DBM                        0x0f
#define RF_PHY_TX_POWER_3DBM                        0x0d
#define RF_PHY_TX_POWER_0DBM                        0x08

#define RF_PHY_TX_POWER_N3DBM                       0x06
#define RF_PHY_TX_POWER_N5DBM                       0x05
#define RF_PHY_TX_POWER_N6DBM                       0x04

#define RF_PHY_TX_POWER_N10DBM                      0x03
#define RF_PHY_TX_POWER_N15DBM                      0x02
#define RF_PHY_TX_POWER_N20DBM                      0x01

#define RF_PHY_FREQ_FOFF_00KHZ                      0
#define RF_PHY_FREQ_FOFF_20KHZ                      5
#define RF_PHY_FREQ_FOFF_40KHZ                      10
#define RF_PHY_FREQ_FOFF_60KHZ                      15
#define RF_PHY_FREQ_FOFF_80KHZ                      20
#define RF_PHY_FREQ_FOFF_100KHZ                     25
#define RF_PHY_FREQ_FOFF_120KHZ                     30
#define RF_PHY_FREQ_FOFF_140KHZ                     35
#define RF_PHY_FREQ_FOFF_160KHZ                     40
#define RF_PHY_FREQ_FOFF_180KHZ                     45
#define RF_PHY_FREQ_FOFF_200KHZ                     50
#define RF_PHY_FREQ_FOFF_N20KHZ                     -5
#define RF_PHY_FREQ_FOFF_N40KHZ                     -10
#define RF_PHY_FREQ_FOFF_N60KHZ                     -15
#define RF_PHY_FREQ_FOFF_N80KHZ                     -20
#define RF_PHY_FREQ_FOFF_N100KHZ                    -25
#define RF_PHY_FREQ_FOFF_N120KHZ                    -30
#define RF_PHY_FREQ_FOFF_N140KHZ                    -35
#define RF_PHY_FREQ_FOFF_N160KHZ                    -40
#define RF_PHY_FREQ_FOFF_N180KHZ                    -45
#define RF_PHY_FREQ_FOFF_N200KHZ                    -50


#define RF_PHY_DTM_MANUL_NULL                        0x00
#define RF_PHY_DTM_MANUL_FOFF                        0x01
#define RF_PHY_DTM_MANUL_TXPOWER                     0x02
#define RF_PHY_DTM_MANUL_XTAL_CAP                    0x04
#define RF_PHY_DTM_MANUL_MAX_GAIN                    0x08

#define RF_PHY_DTM_MANUL_ALL                         0xFF
/*******************************************************************************
 * FUNCION DEFINE
 */
void        rf_phy_ini      (void);
void        rf_phy_ini1      (void);
void        rf_phy_ana_cfg  (void);
void        rf_phy_bb_cfg   (uint8_t pktFmt);
void        rf_phy_bb_cfg1   (uint8_t pktFmt);
void        rf_phy_change_cfg0(uint8_t pktFmt);
void        rf_tpCal_cfg    (uint8_t rfChn);
void        rf_tpCal_cfg_avg(uint8 rfChn,uint8 avgNum);
uint8_t     rf_tp_cal       (uint8_t rfChn ,uint8_t fDev);
void        rf_rxDcoc_cfg   (uint8_t rfChn ,uint8_t bwSet,volatile uint32* dcCal);
void        rf_tpCal_gen_cap_arrary(void);

void        rf_phy_direct_test  (void);
void        rf_phy_dtm_cmd_parse(void);
void        rf_phy_dtm_evt_send (uint8_t dtmType);
void        rf_phy_dtm_trigged  (void);
void        rf_phy_get_pktFoot  (uint8* rssi, uint16* foff,uint8* carrSens);

void        rf_phy_set_txPower  (uint8 txPower);

uint8_t     rf_phy_direct_test_ate(uint32_t cmdWord,uint8_t regPatchNum,uint32_t* regPatchAddr,uint8_t* regPatchHigh,uint8_t* regPatchLow,uint32_t* regPatchVal,uint8_t* dOut);
void rf_phy_get_pktFoot_fromPkt(uint32 pktFoot0, uint32 pktFoot1,
                                uint8* rssi, uint16* foff,uint8* carrSens);
//void        rf_phy_dtm_zigbee_pkt_gen(void);
/**************************************************************************************
    @fn          rf_phy_dtm_ext_tx_singleTone

    @brief       This function process for rf phy direct test, test mode interup

    input parameters

    @param       txPower    :   rf tx power
                rfChnIdx    :   rf channel = 2402+(rfChnIdx<<1)
                rfFoff      :   rf freq offset = rfFoff*4KHz
                xtal_cap    :   XTAL16M_CAP_SETTING
                testTimeUs  :   test loop active time(ms)

    output parameters

    @param       none

    @return      none
*/
void    rf_phy_dtm_ext_tx_singleTone(uint8_t txPower, uint8_t rfChnIdx,uint8_t xtal_cap,int8_t rfFoff,uint32 testTimeUs);


/**************************************************************************************
    @fn          rf_phy_dtm_ext_tx_modulation

    @brief       This function process for rf phy direct test, test mode interup

    input parameters

    @param       txPower    :   rf tx power
                rfChnIdx    :   rf channel = 2402+(rfChnIdx<<1)
                xtal_cap    :   XTAL16M_CAP_SETTING
                rfFoff      :   rf freq offset = rfFoff*4KHz
                pktType     :   modulaiton data type, 0: prbs9, 1: 1111000: 2 10101010
                testTimeUs  :   test loop active time(ms)

    output parameters

    @param       none

    @return      none
*/
void    rf_phy_dtm_ext_tx_modulation(uint8_t txPower, uint8_t rfChnIdx,uint8_t xtal_cap,int8_t rfFoff,uint8_t pktType,uint32 testTimeUs);

/**************************************************************************************
    @fn          rf_phy_dtm_ext_tx_mt_burst

    @brief       This function process for rf phy direct test, test mode interup

    input parameters

    @param      txPower     :   rf tx power
                rfChnIdx    :   rf channel = 2402+(rfChnIdx<<1)
                xtal_cap    :   XTAL16M_CAP_SETTING                
                rfFoff      :   rf freq offset = rfFoff*4KHz
                pktType     :   modulaiton data type, 0: prbs9, 1: 1111000: 2 10101010
                pktLength   :   pkt length(Byte)

                txPktNum    :   burst pkt tx number
                txPktIntv   :   txPkt intv,0 txPkt intv is pkt interval =  ceil((L+249)/625) * 625

    output parameters

    @param       none

    @return      none
*/
void    rf_phy_dtm_ext_tx_mod_burst(uint8_t txPower, uint8_t rfChnIdx,uint8_t xtal_cap,int8_t rfFoff,
                                    uint8_t pktType, uint8_t pktLength,uint32 txPktNum,uint32 txPktIntv);

/**************************************************************************************
    @fn          rf_phy_dtm_ext_rx_demod_burst

    @brief       This function process for rf phy direct test, test mode interup

    input parameters

    @param       rfChnIdx       :   rf channel = 2402+(rfChnIdx<<1)
                rfFoff          :   rf freq offset = rfFoff*4KHz
                xtal_cap        :   XTAL16M_CAP_SETTING                    
                pktLength       :   pkt length(Byte)
                rxWindow        :   rx demod window length(us)
                rxTimeOut       :   rx on time (ms)

    output parameters

    @param       rxEstFoff      :   rx demod estimated frequency offset
                rxEstRssi       :   rx demod estimated rssi
                rxEstCarrSens   :   rx demod estimated carrier sense
                rxPktNum        :   rx demod received pkt number

    @return      none
*/
void    rf_phy_dtm_ext_rx_demod_burst(uint8_t rfChnIdx,int8_t rfFoff,uint8_t xtal_cap,uint8_t pktLength,uint32 rxTimeOut,uint32 rxWindow,
                                      uint16_t* rxEstFoff,uint8_t* rxEstRssi,uint8_t* rxEstCarrSens,uint16_t* rxPktNum);   


/**************************************************************************************
    @fn          rf_phy_dtm_ext_acc_code_set

    @brief       config the acc_code in rf phy dtm

    input parameters

    @param       acc_code        :   sync word

    output parameters
    @return      none
*/
void    rf_phy_dtm_ext_acc_code_set(uint32 accCode);      
#endif
