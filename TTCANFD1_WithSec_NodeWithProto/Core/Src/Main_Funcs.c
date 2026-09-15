/*
 * TTCANFD_config.c
 *
 *  Created on: Aug 19, 2025
 *      Author: samfishy
 */
#include "main.h"
#include "Main_Funcs.h"
#include "stdio.h"
#include "math.h"
#include "encoder.h"

#include "pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "packing.pb.h"

extern FDCAN_HandleTypeDef hfdcan1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

int i0=0,i1=0,i2=0,i4= 0,i5 = 0;
int i_tx = 0,i_rx = 0;
int i_m2 = 0,i_m3 = 0,i_m4 = 0,i_m8 = 0,i_m10 = 0,i_m12 = 0;
int i_m1 = 0,i_m5 = 0,i_m6 = 0,i_m7 = 0,i_m9 = 0,i_m11 =0;
int index_log = 0,i_ft = 0,i_ft1 = 0;

FDCAN_TxHeaderTypeDef Tx_can;
FDCAN_RxHeaderTypeDef Rx_can;

FDCAN_TxEventFifoTypeDef TxEvent;

uint8_t	Tx_can_data1[64];
uint8_t	Rx_can_data[64];
uint32_t rx_data[1024];
int32_t tim = 0;
volatile int send_cmpt = 0,fault_mode = 0, mode_switch = 0, mode_switch_flag = 0,fault_messages_flag = 0, fault_free = 0, fault_12  = 0,free_time_flag = 0;
int index_1 = 0,matrix_1_index = 0,matrix_2_index = 0, rx_data_after_rx[64],index_rx = 0;
int index_mc = 0, index_bc = 0,index_log_tx = 0,index_log_rx = 0,index_all = 0 ,index_condi = 0,i3 = 0;
int flag_corrtim = 0,set_flag = 0,exti_flag, back_messages_flag = 0;
float  condi1 =0, condi2 = 0;
double tolerance = 0,mod = 0, period = 0;
double tolerance_mul = 0,mod_mul = 0, period_mul = 0;
uint32_t welp = 0,timer_rev = 0, fault_id = 0, faults[300] = {0};
uint32_t timer =0,debug_time1 =0, debug_time = 0,timer_ini =0,debug_time2 = 0;

uint16_t WCET = 150;
uint8_t mess_no = 13;
volatile int bc_norm = 0, mt_pointer_norm = 0, mess_norm = 0, td_norm = 0, ta_norm = 0,  ta_norm_set = 0, fault_no = 0, first_fault = 0;
volatile int fault_mode_mx_pt = 0, fault_mode_bc_pt = 0,welp_1 = 0,  i_fault = 0, free_tx = 0, fault_message_flag = 0;
int debug1= 0,fault_number = 0, fault_id_sent = 0, once = 0, mess = 0;

int32_t tim1 =0 ;

struct Data_Loging log1;

int fault_array_lo[10] = {0},fault_array_hi[10] = {0},fault_mode_rx = 0,fault_mode_tx = 0,fault_i_lo = 0,fault_i_hi = 0,free_flag = 0;

int Matrix_Cycle[4][13] = {
		{0,1,2,3,4,5,6,7,8,9,10,11,12},
		{0,1,2,13,4,13,13,7,8,13,13,11,13},
		{0,1,2,3,4,5,13,7,8,9,10,11,12},
		{0,1,2,13,4,13,13,7,8,13,13,11,13},
};

uint16_t S_lo[13] = {0,120,90,90,90,100,100,100,100,100,110,100,80};
uint16_t S_hi[13] = {0,120,190,90,190,100,200,100,200,100,210,100,180};


struct FT_TT_CANFD_mess canfd_S_LO[4][13] = {
		{{0,0,300,0},{1,770,910,1},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{6,2920,3040,0},{8,3190,3310,0},{10,3460,3590,0},{12,3740,3840,0}},
	    {{0,0,300,0},{1,770,910,1},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,1},{4,1860,1970,1},{8,2120,2240,1},{13,2390,2940,0},{13,3000,3240,0},{13,3440,3640,0},{13,3840,4040,0},{13,4240,4440,0},{13,4690,4840,0}},
		{{0,0,300,0},{1,770,910,1},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{8,2920,3040,0},{10,3190,3320,0},{12,3470,3570,0},{13,3720,4840,0}},
	    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,0},{4,1860,1970,0},{8,2120,2240,0},{13,2390,2940,0},{13,3000,3240,0},{13,3440,3640,0},{13,3840,4040,0},{13,4240,4440,0},{13,4690,4840,0}}
};
struct FT_TT_CANFD_mess canfd_S_HI[4][13] = {
		{{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{6,3120,3340,0},{8,3490,3710,0},{10,3860,4090,0},{12,4240,4440,0}},
	    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,1},{4,1960,2170,1},{8,2320,2540,1},{13,2690,2940,0},{13,3000,3240,0},{13,3440,3640,0},{13,3840,4040,0},{13,4240,4440,0},{13,4690,4840,0}},
		{{0,0,300,0},{1,770,910,1},{3,1060,1170,0},{5,1320,1440,1},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,1},{4,2760,2970,0},{8,3120,3340,0},{10,3490,3720,0},{12,3870,4070,0},{13,4220,4070,0}},
	    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,0},{4,1960,2170,0},{8,2320,2540,0},{13,2690,2940,0},{13,3000,3240,0},{13,3440,3640,0},{13,3840,4040,0},{13,4240,4440,0},{13,4690,4840,0}},
};
struct FT_TT_CANFD_mess canfd_Silo_BC0[12][14] = {
	    {{0,0,300,0},{1,770,910,0},{1,1060,1200,0},{3,1350,1460,0},{5,1610,1730,1},{7,1880,2000,0},{9,2150,2270,0},{11,2420,2540,0},{2,2690,2800,1},{4,2950,3060,1},{6,3210,3330,0},{8,3480,3600,1},{10,3750,3880,0},{12,4030,4130,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{3,1320,1430,0},{5,1580,1700,0},{7,1850,1970,0},{9,2120,2240,0},{11,2390,2510,0},{2,2660,2770,0},{4,2920,3030,0},{6,3180,3300,0},{8,3450,3570,0},{10,3720,3850,0},{12,4000,4100,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{5,1590,1710,0},{7,1860,1980,0},{9,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{6,3190,3310,0},{8,3460,3580,0},{10,3730,3860,0},{12,4010,4110,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{7,1860,1980,0},{9,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{6,3190,3310,0},{8,3460,3580,0},{10,3730,3860,0},{12,4010,4110,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{9,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{6,3190,3310,0},{8,3460,3580,0},{10,3730,3860,0},{12,4010,4110,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{6,3190,3310,0},{8,3460,3580,0},{10,3730,3860,0},{12,4010,4110,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{2,2660,2770,0},{4,2920,3030,0},{6,3180,3300,0},{8,3450,3570,0},{10,3720,3850,0},{12,4000,4100,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{4,2920,3030,0},{6,3180,3300,0},{8,3450,3570,0},{10,3720,3850,0},{12,4000,4100,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{6,2920,3040,0},{6,3190,3310,0},{8,3460,3580,0},{10,3730,3860,0},{12,4010,4110,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{6,2920,3040,0},{8,3190,3310,0},{8,3460,3580,0},{10,3730,3860,0},{12,4010,4110,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{6,2920,3040,0},{8,3190,3310,0},{10,3460,3590,0},{10,3740,3870,0},{12,4019,4120,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{6,2920,3040,0},{8,3190,3310,0},{10,3460,3590,0},{12,3740,3840,0},{12,3990,4090,0}}
	};
struct FT_TT_CANFD_mess canfd_SiHi_BC0[12][14] = {
		{{0,0,300,0},{1,770,910,0},{3,1110,1220,0},{5,1420,1540,1},{7,1740,1860,0},{9,2060,2180,0},{11,2380,2500,0},{2,2700,2910,0},{4,3110,3320,0},{6,3520,3740,1},{8,3940,4160,0},{10,4360,4590,0},{12,4790,4990,0},{13,5100,6000,0}},
		{{0,0,300,0},{1,770,910,0},{3,1110,1220,0},{5,1420,1540,0},{7,1740,1860,0},{9,2060,2180,0},{11,2380,2500,0},{2,2700,2910,0},{4,3110,3320,0},{6,3520,3740,0},{8,3940,4160,0},{10,4360,4590,0},{12,4790,4990,0},{13,5100,6000,0}},
		{{0,0,300,0},{1,770,910,0},{3,1110,1220,0},{5,1420,1540,0},{7,1740,1860,0},{9,2060,2180,0},{11,2380,2500,0},{2,2700,2910,0},{4,3110,3320,0},{6,3520,3740,0},{8,3940,4160,0},{10,4360,4590,0},{12,4790,4990,0},{13,5100,6000,0}},
		{{0,0,300,0},{1,770,910,0},{3,1110,1220,0},{5,1420,1540,0},{7,1740,1860,0},{9,2060,2180,0},{11,2380,2500,0},{2,2700,2910,0},{4,3110,3320,0},{6,3520,3740,0},{8,3940,4160,0},{10,4360,4590,0},{12,4790,4990,0},{13,5100,6000,0}},
		{{0,0,300,0},{1,770,910,0},{3,1110,1220,0},{5,1420,1540,0},{7,1740,1860,0},{9,2060,2180,0},{11,2380,2500,0},{2,2700,2910,0},{4,3110,3320,0},{6,3520,3740,0},{8,3940,4160,0},{10,4360,4590,0},{12,4790,4990,0},{13,5100,6000,0}},
		{{0,0,300,0},{1,770,910,0},{3,1110,1220,0},{5,1420,1540,0},{7,1740,1860,0},{9,2060,2180,0},{11,2380,2500,0},{2,2700,2910,0},{4,3110,3320,0},{6,3520,3740,0},{8,3940,4160,0},{10,4360,4590,0},{12,4790,4990,0},{13,5100,6000,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{2,2760,2970,0},{4,3120,3330,0},{6,3480,3700,0},{8,3850,4070,0},{10,4220,4450,0},{12,4600,4800,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{4,3120,3330,0},{6,3480,3700,0},{8,3850,4070,0},{10,4220,4450,0},{12,4600,4800,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{6,3120,3340,0},{6,3490,3710,0},{8,3860,4080,0},{10,4230,4460,0},{12,4610,4810,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{6,3120,3340,0},{8,3490,3710,0},{8,3860,4080,0},{10,4230,4460,0},{12,4610,4810,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{6,3120,3340,0},{8,3490,3710,0},{10,3860,4090,0},{10,4240,4470,0},{12,4620,4820,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{6,3120,3340,0},{8,3490,3710,0},{10,3860,4090,0},{12,4240,4440,0},{12,4590,4790,0}}
	};

struct FT_TT_CANFD_mess canfd_Silo_BC1[6][14] = {
		 {{0,0,300,0},{1,770,910,0},{1,1060,1200,0},{7,1350,1470,0},{11,1620,1740,0},{2,1890,2000,1},{4,2150,2260,0},{8,2410,2530,1},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{7,1330,1450,0},{11,1600,1720,0},{2,1870,1980,0},{4,2130,2240,0},{8,2390,2510,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{11,1600,1720,0},{2,1870,1980,0},{4,2130,2240,0},{8,2390,2510,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,0},{2,1860,1970,0},{4,2120,2230,0},{8,2380,2500,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,0},{4,1860,1970,0},{4,2120,2230,0},{8,2380,2500,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,0},{4,1860,1970,0},{8,2120,2240,0},{8,2390,2510,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}}
};
struct FT_TT_CANFD_mess canfd_SiHi_BC1[6][14] = {
	{{0,0,300,0},{1,770,910,0},{7,1010,1130,0},{11,1230,1350,0},{2,1450,1660,1},{4,1760,1970,1},{8,2070,2290,1},{13,2350,2400,0},{13,2500,3000,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0}},
	{{0,0,300,0},{1,770,910,0},{7,1010,1130,0},{11,1230,1350,0},{2,1450,1660,0},{4,1760,1970,0},{8,2070,2290,0},{13,2350,2400,0},{13,2500,3000,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0}},
	{{0,0,300,0},{1,770,910,0},{7,1010,1130,0},{11,1230,1350,0},{2,1450,1660,0},{4,1760,1970,0},{8,2070,2290,0},{13,2350,2400,0},{13,2500,3000,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0}},
    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,0},{2,1960,2170,0},{4,2320,2530,1},{8,2680,2900,1},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0},{13,4300,4400,0}},
    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,0},{4,1960,2170,0},{4,2320,2530,0},{8,2680,2900,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0},{13,4300,4400,0}},
    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,0},{4,1960,2170,0},{8,2320,2540,0},{8,2690,2910,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0},{13,4300,4400,0}},
};

struct FT_TT_CANFD_mess canfd_Silo_BC2[11][14] = {
	    {{0,0,300,0},{1,770,910,0},{1,1060,1200,0},{3,1350,1460,0},{5,1610,1730,0},{7,1880,2000,0},{9,2150,2270,0},{11,2420,2540,0},{2,2690,2800,0},{4,2950,3060,0},{8,3210,3330,0},{10,3480,3610,0},{12,3760,3860,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{3,1320,1430,0},{5,1580,1700,0},{7,1850,1970,0},{9,2120,2240,0},{11,2390,2510,0},{2,2660,2770,0},{4,2920,3030,0},{8,3180,3300,0},{10,3450,3580,0},{12,3730,3830,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{5,1590,1710,0},{7,1860,1980,0},{9,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{8,3190,3310,0},{10,3460,3590,0},{12,3740,3840,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{7,1860,1980,0},{9,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{8,3190,3310,0},{10,3460,3590,0},{12,3740,3840,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{9,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{8,3190,3310,0},{10,3460,3590,0},{12,3740,3840,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{11,2400,2520,0},{2,2670,2780,0},{4,2930,3040,0},{8,3190,3310,0},{10,3460,3590,0},{12,3740,3840,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{2,2660,2770,0},{4,2920,3030,1},{8,3180,3300,1},{10,3450,3580,0},{12,3730,3830,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{4,2920,3030,0},{8,3180,3300,0},{10,3450,3580,0},{12,3730,3830,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{8,2920,3040,0},{8,3190,3310,0},{10,3460,3590,0},{12,3740,3840,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{8,2920,3040,0},{10,3190,3320,0},{10,3470,3600,0},{12,3750,3850,0},{13,4010,4200,0}},
	    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2510,0},{4,2660,2770,0},{8,2920,3040,0},{10,3190,3320,0},{12,3470,3570,0},{12,3720,3820,0},{13,4010,4200,0}}
	};
struct FT_TT_CANFD_mess canfd_SiHi_BC2[11][14] = {
	{{0,0,300,0},{1,770,910,0},{3,1010,1120,0},{5,1220,1340,0},{7,1440,1560,0},{9,1660,1780,0},{11,1880,2000,0},{2,2100,2310,0},{4,2410,2620,0},{8,2720,2940,0},{10,3040,3270,0},{12,3370,3570,0},{13,3570,3570,0},{13,3600,3700,0}},
	{{0,0,300,0},{1,770,910,0},{3,1010,1120,0},{5,1220,1340,0},{7,1440,1560,0},{9,1660,1780,0},{11,1880,2000,0},{2,2100,2310,0},{4,2410,2620,0},{8,2720,2940,0},{10,3040,3270,0},{12,3370,3570,0},{13,3570,3570,0},{13,3600,3700,0}},
	{{0,0,300,0},{1,770,910,0},{3,1010,1120,0},{5,1220,1340,0},{7,1440,1560,0},{9,1660,1780,0},{11,1880,2000,0},{2,2100,2310,0},{4,2410,2620,0},{8,2720,2940,0},{10,3040,3270,0},{12,3370,3570,0},{13,3570,3570,0},{13,3600,3700,0}},
	{{0,0,300,0},{1,770,910,0},{3,1010,1120,0},{5,1220,1340,0},{7,1440,1560,0},{9,1660,1780,0},{11,1880,2000,0},{2,2100,2310,0},{4,2410,2620,0},{8,2720,2940,0},{10,3040,3270,0},{12,3370,3570,0},{13,3570,3570,0},{13,3600,3700,0}},
	{{0,0,300,0},{1,770,910,0},{3,1010,1120,0},{5,1220,1340,0},{7,1440,1560,0},{9,1660,1780,0},{11,1880,2000,0},{2,2100,2310,0},{4,2410,2620,0},{8,2720,2940,0},{10,3040,3270,0},{12,3370,3570,0},{13,3570,3570,0},{13,3600,3700,0}},
	{{0,0,300,0},{1,770,910,0},{3,1010,1120,0},{5,1220,1340,0},{7,1440,1560,0},{9,1660,1780,0},{11,1880,2000,0},{2,2100,2310,0},{4,2410,2620,0},{8,2720,2940,0},{10,3040,3270,0},{12,3370,3570,0},{13,3570,3570,0},{13,3600,3700,0}},
    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{2,2760,2970,0},{4,3120,3330,1},{8,3480,3700,1},{10,3850,4080,0},{12,4230,4430,0},{13,4580,5000,0}},
    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{4,3120,3330,0},{8,3480,3700,0},{10,3850,4080,0},{12,4230,4430,0},{13,4580,5000,0}},
    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{8,3120,3340,0},{8,3490,3710,0},{10,3860,4090,0},{12,4240,4440,0},{13,4580,5000,0}},
    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{8,3120,3340,0},{10,3490,3720,0},{10,3870,4100,0},{12,4250,4450,0},{13,4580,5000,0}},
    {{0,0,300,0},{1,770,910,0},{3,1060,1170,0},{5,1320,1440,0},{7,1590,1710,0},{9,1860,1980,0},{11,2130,2250,0},{2,2400,2610,0},{4,2760,2970,0},{8,3120,3340,0},{10,3490,3720,0},{12,3870,4070,0},{12,4220,4420,0},{13,4580,5000,0}}
};

struct FT_TT_CANFD_mess canfd_Silo_BC3[6][14] = {
		 {{0,0,300,0},{1,770,910,0},{1,1060,1200,0},{7,1350,1470,0},{11,1620,1740,0},{2,1890,2000,0},{4,2150,2260,0},{8,2410,2530,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{7,1330,1450,0},{11,1600,1720,0},{2,1870,1980,0},{4,2130,2240,0},{8,2390,2510,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{11,1600,1720,0},{2,1870,1980,0},{4,2130,2240,0},{8,2390,2510,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,0},{2,1860,1970,0},{4,2120,2230,0},{8,2380,2500,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,0},{4,1860,1970,0},{4,2120,2230,0},{8,2380,2500,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}},
		 {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1710,0},{4,1860,1970,0},{8,2120,2240,0},{8,2390,2510,0},{13,2700,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,3900,4000,0},{13,4100,4200,0}}
};
struct FT_TT_CANFD_mess canfd_SiHi_BC3[6][14] = {
	{{0,0,300,0},{1,770,910,0},{7,1010,1130,0},{11,1230,1350,0},{2,1450,1660,0},{4,1760,1970,0},{8,2070,2290,0},{13,2350,2400,0},{13,2500,3000,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0}},
	{{0,0,300,0},{1,770,910,0},{7,1010,1130,0},{11,1230,1350,0},{2,1450,1660,0},{4,1760,1970,0},{8,2070,2290,0},{13,2350,2400,0},{13,2500,3000,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0}},
	{{0,0,300,0},{1,770,910,0},{7,1010,1130,0},{11,1230,1350,0},{2,1450,1660,0},{4,1760,1970,0},{8,2070,2290,0},{13,2350,2400,0},{13,2500,3000,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0}},
    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,0},{2,1960,2170,0},{4,2320,2530,0},{8,2680,2900,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0},{13,4300,4400,0}},
    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,0},{4,1960,2170,0},{4,2320,2530,0},{8,2680,2900,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0},{13,4300,4400,0}},
    {{0,0,300,0},{1,770,910,0},{7,1060,1180,0},{11,1330,1450,0},{2,1600,1810,0},{4,1960,2170,0},{8,2320,2540,0},{8,2690,2910,0},{13,3100,3200,0},{13,3300,3400,0},{13,3500,3600,0},{13,3700,3800,0},{13,4100,4200,0},{13,4300,4400,0}},
};

struct TTCANFD_Mess can_test_matrix[] = {
		{ 1,0x00,"REF", 4,FDCAN_DLC_BYTES_4, 10,  {0},0,0},
	    { 0,0x01,"M1",  8,FDCAN_DLC_BYTES_8, 10,  {0x22, 0x23, 0x24,0x22, 0x23, 0x24,0x22, 0x23},0,0},
	    { 1,0x02,"M2",  3,FDCAN_DLC_BYTES_3, 14,  {0x22, 0x23, 0x24},0,0},
	    { 1,0x03,"M3",  3,FDCAN_DLC_BYTES_3, 20,  {0x26,0x01, 0x02, 0x03, 0x04, 0x05},0,0},
	    { 1,0x04,"M4",  2,FDCAN_DLC_BYTES_2, 15,  {0x04, 0x05},0,0},
	    { 0,0x05,"M5",  5,FDCAN_DLC_BYTES_5, 20,  {0x22, 0x23, 0x24, 0x25, 0x26},0,0},
	    { 0,0x06,"M6",  5,FDCAN_DLC_BYTES_5, 40,  {0x22,0x22, 0x23, 0x24, 0x25, 0x26,0x01, 0x02, 0x03},0,0},
	    { 1,0x07,"M7",  4,FDCAN_DLC_BYTES_4, 15,  {0x22, 0x23, 0x24, 0x25},0,0},
	    { 0,0x08,"M8",  5,FDCAN_DLC_BYTES_5, 50,  {0x22, 0x23, 0x24, 0x25, 0x26},0,0},
	    { 0,0x09,"M9",  4,FDCAN_DLC_BYTES_4, 20,  {0x26, 0x01, 0x02, 0x03},0,0},
	    { 1,0xA ,"M10", 7,FDCAN_DLC_BYTES_7, 100, {0x22, 0x23, 0x24, 0x25, 0x22, 0x23, 0x24},0,0},
	    { 0,0xB ,"M11", 5,FDCAN_DLC_BYTES_5, 50,  {0x22, 0x23, 0x24, 0x25, 0x26},0,0},
	    { 1,0xC ,"M12", 2,FDCAN_DLC_BYTES_2, 100, {0x26,0x26},0,0},
		{ 1,0xD ,"FREE",2,FDCAN_DLC_BYTES_2, 10,  {0x53, 0x54},0,0},
		{ 1,0xF ,"LOW"  ,2,FDCAN_DLC_BYTES_2, 10,{0x1, 0x1},0,0},
		{ 1,0xE ,"HIGH" ,2,FDCAN_DLC_BYTES_2, 10,{0x0, 0x0},0,0},
		{ 1,0x10,"N_FLT",2,FDCAN_DLC_BYTES_2, 10,{0x52, 0x52},0,0},
		{ 1,0x11 ,"FLT"  ,2,FDCAN_DLC_BYTES_2, 10,{0x52, 0x53},0,0},
	    { 0,0xff,"BCK_MSG",8,FDCAN_DLC_BYTES_8, 10,{0x1, 0x2, 0x3,0x4, 0x5, 0x6, 0x7, 0x8},0,0}
};

void MainInit(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
	__HAL_TIM_SET_COUNTER(&htim2,0);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
	TTCANFD_MainInit();
}

void TTCANFD_MainInit()
{
	if(HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	  {
		  Error_Handler();
	  }
	if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	  {
		  Error_Handler();
	  }
	if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_TX_EVT_FIFO_NEW_DATA, 0) != HAL_OK)
	  {
		  Error_Handler();
	  }

	Tx_can.IdType				 = FDCAN_STANDARD_ID;
	Tx_can.TxFrameType		 	 = FDCAN_DATA_FRAME;
	Tx_can.ErrorStateIndicator 	 = FDCAN_ESI_ACTIVE;
	Tx_can.BitRateSwitch		 = FDCAN_BRS_ON;
	Tx_can.FDFormat			 	 = FDCAN_FD_CAN;
	Tx_can.TxEventFifoControl	 = FDCAN_STORE_TX_EVENTS;
	Tx_can.MessageMarker		 = 0;
}

void TTCAN_loop(int seq)
{
	Tx_can_data1[0] =  ((uint16_t)seq >>  8) & 0xFF;
	Tx_can_data1[1] =  ((uint16_t)seq) & 0xFF;

	for(int i = 2; i < can_test_matrix[index_1].dlc; i++)
	{
		Tx_can_data1[i] =  can_test_matrix[index_1].data[i];
	}
}

void TTCAN_tx(int mess_norm, int seq)
{
	Tx_can.DataLength = can_test_matrix[mess_norm].ttcan_def;
	Tx_can.Identifier = can_test_matrix[mess_norm].id;

	if(Tx_can.Identifier == can_test_matrix[18].id)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

	if( Tx_can.Identifier !=  can_test_matrix[13].id)
	{
		debug_time2 = __HAL_TIM_GET_COUNTER(&htim2);

		TTCAN_loop(seq);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);

		debug_time1 = __HAL_TIM_GET_COUNTER(&htim2);
		debug_time1 = debug_time1-debug_time2;
	}

    if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) > 0)
    {
    	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &Tx_can, Tx_can_data1) != HAL_OK)
    	{
    		Error_Handler();
    	}
    }
}

void TTCAN_rx(void)
{
	welp = (uint8_t)Rx_can.Identifier;

	if(welp == can_test_matrix[0].id)
		{

			uint16_t tim =
					((uint32_t)Rx_can_data[0] << 8)  |
					((uint32_t)Rx_can_data[1]);
			tim = tim+87;

	      	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,1);

			__HAL_TIM_SET_COUNTER(&htim1,tim);
	        mt_pointer_norm = 0;

	        if(mode_switch == 0)
	        {
				ta_norm   = canfd_S_LO[bc_norm][mt_pointer_norm].Ta;
	        }
	        else if (mode_switch == 1)
	        {
				ta_norm   = canfd_S_HI[bc_norm][mt_pointer_norm].Ta;
	        }

	        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 9999);
	        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, ta_norm);
	        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ta_norm+WCET);

	        mt_pointer_norm = 1;

	        if(mode_switch == 0)
	        {
				td_norm   = canfd_S_LO[bc_norm][mt_pointer_norm].Td;
				mess_norm = canfd_S_LO[bc_norm][mt_pointer_norm].id;
	        }
	        else if (mode_switch == 1)
	        {
				td_norm   = canfd_S_HI[bc_norm][mt_pointer_norm].Td;
				mess_norm = canfd_S_HI[bc_norm][mt_pointer_norm].id;
	        }

	        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, td_norm);
	        send_cmpt = 1;

			if(can_test_matrix[0].last_sent == 0)
			{
				HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_2);
				HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_3);
				HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_4);
				HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
				can_test_matrix[0].last_sent = 1;
			}
		}
	else if(welp == can_test_matrix[15].id)
	{

 	     if(fault_mode == 0)
 	     {

     	     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, 1);
        	mode_switch = 1;
 	     }
 	     else
 	     {
 	    	 mode_switch_flag = 1;
 	     }
	}
	else if(welp == can_test_matrix[17].id)
	{
		for(int i = 0; i < (int)Rx_can.DataLength;i++)
		{
			rx_data_after_rx[i] = (int)Rx_can_data[i];
		}

		uint16_t fault_id = ((uint16_t)rx_data_after_rx[0] << 8) | ((uint16_t)rx_data_after_rx[1]);

    	if(fault_mode == 1)
    	{
    		faults[fault_no] = fault_id;
    		fault_no ++;
    	}
    	else if(fault_mode == 0)
     	{
		    int mess = fault_id/1000;
    		fault_id = fault_id%1000;

    		if(mode_switch == 0)
    		{
        		fault_mode = 1;
        		mess_no = 14;
        		fault_mode_bc_pt = fault_id/100;
        		fault_mode_mx_pt = fault_id%100;
    		}
    		else if(mode_switch == 1)
    		{
        		fault_mode_bc_pt = fault_id/100;
        		fault_mode_mx_pt = fault_id%100;

        		if(mess%2 == 0)
        		{
            		fault_mode = 1;
        		    mess_no = 14;
        		}
        		else
        		{
    	    		faults[fault_no] = mess*1000;
    	    		fault_no ++;
        		}
    		}
     	}
	}
	else if(welp == can_test_matrix[14].id)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	else if(welp == can_test_matrix[16].id)
	{
    	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	else if(welp != can_test_matrix[14].id && welp != can_test_matrix[15].id && welp != can_test_matrix[0].id && welp != can_test_matrix[18].id)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, 1);
	  	send_cmpt = 1;
	  	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, 0);

		for(int i = 0; i < (int)Rx_can.DataLength;i++)
		{
			rx_data_after_rx[i] = (int)Rx_can_data[i];
		}

		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);

		uint16_t idx = ((uint16_t)rx_data_after_rx[0] << 8) | ((uint16_t)rx_data_after_rx[1]);

		/*if(welp == can_test_matrix[2].id)
			{
				uint32_t time_lost = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[2].last_sent = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[2].times_sent++;

				//if(time_lost > 14005)
				{
					log1.id_rx[i_rx]  = idx;
					log1.timestamp_rx[i_rx] = time_lost;
					i_rx++;
				}
			}
		else if(welp == can_test_matrix[3].id)
			{
			uint32_t time_lost = __HAL_TIM_GET_COUNTER(&htim2);
			can_test_matrix[3].last_sent = __HAL_TIM_GET_COUNTER(&htim2);
			can_test_matrix[3].times_sent++;

			//if(time_lost > 20005)
			{
				log1.id_rx[i_rx]  = idx;
				log1.timestamp_rx[i_rx] = time_lost;
				i_rx++;

			}
			}
		else if(welp == can_test_matrix[4].id)
			{
			uint32_t time_lost = __HAL_TIM_GET_COUNTER(&htim2);
			can_test_matrix[4].last_sent = __HAL_TIM_GET_COUNTER(&htim2);
			can_test_matrix[4].times_sent++;

			//if(time_lost > 15005)
			{
				log1.id_rx[i_rx]  = idx;
				log1.timestamp_rx[i_rx] = time_lost;
				i_rx++;
			}
			}
		else if(welp == can_test_matrix[7].id)
			{
				uint32_t time_lost = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[7].last_sent = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[7].times_sent++;

				//if(time_lost > 50005)
				{
					log1.id_rx[i_rx]  = idx;
					log1.timestamp_rx[i_rx] = time_lost;
					i_rx++;
				}
			}
		else if(welp == can_test_matrix[10].id)
			{
				uint32_t time_lost = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[10].last_sent = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[10].times_sent++;

				//if(time_lost > 100005)
				{
					log1.id_rx[i_rx]  = idx;
					log1.timestamp_rx[i_rx] = time_lost;
					i_rx++;
				}
			}
		else if(welp == can_test_matrix[12].id)
			{
				uint32_t time_lost = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[12].last_sent = __HAL_TIM_GET_COUNTER(&htim2);
				can_test_matrix[12].times_sent++;

				//if(time_lost > 100005)
				{
					log1.id_rx[i_rx]  = idx;
					log1.timestamp_rx[i_rx] = time_lost;
					i_rx++;
				}
			}*/

		if(fault_messages_flag == 1)
		//if(welp == 3)
		{
			log1.id_rx[i_rx]  = idx;
			log1.timestamp_rx[i_rx] = __HAL_TIM_GET_COUNTER(&htim2);
			i_rx++;

         	if(welp == 2)
         	{
         		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
         	}
         	else if(welp == 4)
         	{
         		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
         	}
		}
		else if(first_fault == 1)
		{
			log1.id_rx[i_rx]  = idx;
			log1.timestamp_rx[i_rx] =__HAL_TIM_GET_COUNTER(&htim1);
			i_rx++;
			first_fault = 0;

         	if(welp == 2)
         	{
         		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
         	}
         	else if(welp == 4)
         	{
         		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
         	}
		}
	}
}

void HAL_FDCAN_TxEventFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxEventFifoITs)
{
	if(HAL_FDCAN_GetTxEvent(hfdcan, &TxEvent)==HAL_OK)
	{
		if(TxEvent.EventType == 0x00400000)
		{
			debug_time1 = __HAL_TIM_GET_COUNTER(&htim2)-(uint32_t)timer_ini*10;
			debug_time1 = debug_time1-debug_time;
		}
	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &Rx_can, Rx_can_data) != HAL_OK)
    {
    	Error_Handler();
    }

	TTCAN_rx();
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_4)
	{
		HAL_TIM_Base_Start_IT(&htim2);
		__HAL_TIM_SET_COUNTER(&htim2,0);

	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		back_messages_flag = 1;

		free_tx = 10000 -__HAL_TIM_GET_COUNTER(&htim1);
	}
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
   if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
	   send_cmpt =0;
	   HAL_TIM_Base_Stop_IT(&htim3);
	   fault_messages_flag = 0;

        if(bc_norm < 3)
        {
            mess_no = 13;
            fault_mode = 0;

            if(mode_switch_flag == 1)
            {

         	     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, 1);
            	mode_switch_flag = 0;
            	mode_switch = 1;
            }

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
            bc_norm++;
        }
        else
        {
        	//fault_number = 0;
        	mode_switch = 0;
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
        	fault_mode = 0;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
            bc_norm = 0;
        }

       	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,0);
        mt_pointer_norm = 0;

        if(mode_switch == 0)
        {
            td_norm = canfd_S_LO[bc_norm][0].Td;
            ta_norm = canfd_S_LO[bc_norm][0].Ta;
        }
        else
        {
            td_norm = canfd_S_HI[bc_norm][0].Td;
            ta_norm = canfd_S_HI[bc_norm][0].Ta;
        }

        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 9999);
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, td_norm+10);

    }

    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
    	int fault = 0;

        if(mt_pointer_norm < mess_no)
        {
            if(fault_mode == 0)
            {

            	if(mode_switch == 0)
            	{
            		td_norm   = canfd_S_LO[bc_norm][mt_pointer_norm].Td;
            		ta_norm   = canfd_S_LO[bc_norm][mt_pointer_norm].Ta;
            		mess_norm = canfd_S_LO[bc_norm][mt_pointer_norm].id;
            		fault 	  = canfd_S_LO[bc_norm][mt_pointer_norm].Fault;
            	}
            	else
            	{
            		td_norm   = canfd_S_HI[bc_norm][mt_pointer_norm].Td;
            		ta_norm   = canfd_S_HI[bc_norm][mt_pointer_norm].Ta;
            		mess_norm = canfd_S_HI[bc_norm][mt_pointer_norm].id;
            		fault 	  = canfd_S_HI[bc_norm][mt_pointer_norm].Fault;
            	}
            }

            else if(fault_mode == 1)
            {
            	if(mode_switch == 0)
            	{
            		if(fault_mode_bc_pt == 0)
            		{
                		td_norm   = canfd_Silo_BC0[fault_mode_mx_pt][mt_pointer_norm].Td;
                		ta_norm   = canfd_Silo_BC0[fault_mode_mx_pt][mt_pointer_norm].Ta;
                		mess_norm = canfd_Silo_BC0[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_Silo_BC0[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            		if(fault_mode_bc_pt == 1)
            		{
            			td_norm   = canfd_Silo_BC1[fault_mode_mx_pt][mt_pointer_norm].Td;
            			ta_norm   = canfd_Silo_BC1[fault_mode_mx_pt][mt_pointer_norm].Ta;
            			mess_norm = canfd_Silo_BC1[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_Silo_BC1[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            		if(fault_mode_bc_pt == 2)
            		{
            			td_norm   = canfd_Silo_BC2[fault_mode_mx_pt][mt_pointer_norm].Td;
            			ta_norm   = canfd_Silo_BC2[fault_mode_mx_pt][mt_pointer_norm].Ta;
            		    mess_norm = canfd_Silo_BC2[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_Silo_BC2[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            		if(fault_mode_bc_pt == 3)
            		{
            	    	td_norm   = canfd_Silo_BC3[fault_mode_mx_pt][mt_pointer_norm].Td;
            		    ta_norm   = canfd_Silo_BC3[fault_mode_mx_pt][mt_pointer_norm].Ta;
            		    mess_norm = canfd_Silo_BC3[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_Silo_BC3[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            	}
            	else
            	{
            		if(fault_mode_bc_pt == 0)
            		{
                		td_norm   = canfd_SiHi_BC0[fault_mode_mx_pt][mt_pointer_norm].Td;
                		ta_norm   = canfd_SiHi_BC0[fault_mode_mx_pt][mt_pointer_norm].Ta;
                		mess_norm = canfd_SiHi_BC0[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_SiHi_BC0[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            		if(fault_mode_bc_pt == 1)
            		{
            			td_norm   = canfd_SiHi_BC1[fault_mode_mx_pt][mt_pointer_norm].Td;
            			ta_norm   = canfd_SiHi_BC1[fault_mode_mx_pt][mt_pointer_norm].Ta;
            			mess_norm = canfd_SiHi_BC1[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_SiHi_BC1[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            		if(fault_mode_bc_pt == 2)
            		{
            			td_norm   = canfd_SiHi_BC2[fault_mode_mx_pt][mt_pointer_norm].Td;
            			ta_norm   = canfd_SiHi_BC2[fault_mode_mx_pt][mt_pointer_norm].Ta;
            		    mess_norm = canfd_SiHi_BC2[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_SiHi_BC2[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            		if(fault_mode_bc_pt == 3)
            		{
            			td_norm   = canfd_SiHi_BC3[fault_mode_mx_pt][mt_pointer_norm].Td;
            		    ta_norm   = canfd_SiHi_BC3[fault_mode_mx_pt][mt_pointer_norm].Ta;
            		    mess_norm = canfd_SiHi_BC3[fault_mode_mx_pt][mt_pointer_norm].id;
                		fault 	  = canfd_SiHi_BC3[fault_mode_mx_pt][mt_pointer_norm].Fault;
            		}
            	}
            }

            fault_message_flag = fault;

            i0++;
            if(i0 == 2000)
            {
        		//HAL_Delay(2000);
        		proto_tx();
        		while(1);
            }

        	/*if(can_test_matrix[mess_norm].id == can_test_matrix[1].id)
        	{
        		index_log = i_m1;
        		i_m1++;
        	}
        	else if(can_test_matrix[mess_norm].id == can_test_matrix[5].id)
        	{
        		index_log = i_m5;
        		i_m5++;
        	}
        	else if(can_test_matrix[mess_norm].id == can_test_matrix[6].id)
        	{
        		index_log = i_m6;
        		i_m6++;
        	}
        	else if(can_test_matrix[mess_norm].id == can_test_matrix[8].id)
        	{
        		index_log = i_m8;
        		i_m8++;
        	}
        	else if(can_test_matrix[mess_norm].id == can_test_matrix[9].id)
        	{
        		index_log = i_m9;
        		i_m9++;
        	}
        	else if(can_test_matrix[mess_norm].id == can_test_matrix[11].id)
        	{
        		index_log = i_m11;
        		i_m11++;
        	}*/

            if(can_test_matrix[mess_norm].rx_tx == 0 && can_test_matrix[mess_norm].id != 13)
            {

            	/*HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,1);
            	back_messages_flag = 0;

                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);

            	int idx = mess_norm*1000+index_log;
            	log1.id_tx[i_tx] = idx;
            	log1.timestamp_tx[i_tx]  =  __HAL_TIM_GET_COUNTER(&htim2);
            	TTCAN_tx(mess_norm,idx);
            	i_tx++;

            	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,0);*/

            	int idx = mess_norm*1000;

            	if(fault == 0)
            	{
                	TTCAN_tx(mess_norm,idx);
            	}
            }
            else if(can_test_matrix[mess_norm].id != 13)
            {

                __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ta_norm);
                ta_norm_set = ta_norm;

                fault_id = can_test_matrix[mess_norm].id ;
                uint16_t mode_time = (uint16_t)ta_norm+WCET;
        		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1, mode_time);

        		if(can_test_matrix[mess_norm].id == 12)
                 {
                     fault_free = 1;
                 }
            }

            if(can_test_matrix[mess_norm].id == 13)
            {
            	fault_messages_flag = 1;

        		HAL_TIM_Base_Start_IT(&htim3);
        		__HAL_TIM_SET_COUNTER(&htim3,0);
            }

            mt_pointer_norm++;

            if(mt_pointer_norm < mess_no)
            {
            	if(fault_mode == 0)
                {
            		if(mode_switch == 0)
                    {
            			uint16_t next_td = canfd_S_LO[bc_norm][mt_pointer_norm].Td;
                        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, next_td);
                    }
                    else
                    {
                    	uint16_t next_td = canfd_S_HI[bc_norm][mt_pointer_norm].Td;
                        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, next_td);
                    }
                }
                else if(fault_mode == 1)
                {
                	if(mode_switch == 0)
                	{
                		uint16_t next_td = 0;

                		if(fault_mode_bc_pt == 0)
                        {
                			next_td   = canfd_Silo_BC0[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }
                        if(fault_mode_bc_pt == 1)
                        {
                        	next_td   = canfd_Silo_BC1[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }
                        if(fault_mode_bc_pt == 2)
                        {
                        	next_td   = canfd_Silo_BC2[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }
                        if(fault_mode_bc_pt == 3)
                        {
                        	next_td   = canfd_Silo_BC3[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }

                        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, next_td);
                	}
                    else
                    {
                    	uint16_t next_td = 0;

                        if(fault_mode_bc_pt == 0)
                        {
                        	next_td   = canfd_SiHi_BC0[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }
                        if(fault_mode_bc_pt == 1)
                        {
                        	next_td   = canfd_SiHi_BC1[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }
                        if(fault_mode_bc_pt == 2)
                        {
                        	next_td   = canfd_SiHi_BC2[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }
                        if(fault_mode_bc_pt == 3)
                        {
                        	next_td   = canfd_SiHi_BC3[fault_mode_mx_pt][mt_pointer_norm].Td;
                        }

                        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, next_td);
                    }
                }
            }
        }
    }

    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
    	if(send_cmpt == 1)
    	{
       		//TTCAN_tx(16,0);
    	}
    	else if(send_cmpt == 0)
    	{
    		fault_id_sent = fault_id;

         	if(fault_mode == 0)
         	{
         		log1.id_tx[index_log_tx] = fault_id*1000;
         		log1.timestamp_tx[index_log_tx] = __HAL_TIM_GET_COUNTER(&htim2);
         		index_log_tx++;

         		if(mode_switch == 1)
         		{
             		if(fault_id%2 == 0)
             		{
                 		first_fault = 1;
                		fault_mode = 1;
            			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
                		mess_no = 14;

                		fault_mode_bc_pt = bc_norm;
                		fault_mode_mx_pt = mt_pointer_norm-2;
             		}
         		}
         		else
         		{
                 	first_fault = 1;
                	fault_mode = 1;
        			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
                	mess_no = 14;

                	fault_mode_bc_pt = bc_norm;
                	fault_mode_mx_pt = mt_pointer_norm-2;
         		}

         		if(fault_id_sent == 12 && once == 0)
        		{
        			fault_12 = 1;
        			once++;
        			if(mode_switch == 0)
        			{
                		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3,3150);
        			}
        			else
        			{
                		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3,3750);
        			}
        		}

        		int welp = fault_id*1000+fault_mode_bc_pt*100+fault_mode_mx_pt;
                TTCAN_tx(17,welp);
         	}
         	else
         	{
         		logging_tx();

         		int welp1 = fault_id*1000+index_log;

         		log1.id_tx[index_log_tx] = welp1;
         		log1.timestamp_tx[index_log_tx] = __HAL_TIM_GET_COUNTER(&htim2);
         		index_log_tx++;

             	TTCAN_tx(17,welp1);
         	}

         	if(fault_id_sent == 2)
         	{
         		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
         	}
         	else if(fault_id_sent == 4)
         	{
         		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
         	}
    	}

    }
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
    	if(send_cmpt == 1)
    	{
          	send_cmpt = 0;
    	}
    	else if(send_cmpt == 0)
    	{
    		if(fault_message_flag == 0)
    		{
            	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);

          	     if(fault_mode == 0)
          	     {

              	     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, 1);
                 	mode_switch = 1;
          	     }
          	     else
          	     {
          	    	 mode_switch_flag = 1;
          	     }

        		TTCAN_tx(15,0);
    		}
    	}

        if(fault_free == 1 && fault_12 == 0)
        {

            free_time_flag = 1;
        	tim1 = __HAL_TIM_GET_COUNTER(&htim1);
        	fault_free = 0;
            fault_messages_flag = 1;

        }

        if(once == 1)
		{
			fault_12 = 0;
			once = 0;
		}
    }
}


void MainLoop(void)
{
	if(fault_messages_flag == 1 )
	{
	    if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) > 0)
	    {
	    	if(i_fault < fault_no)
	    	{
	    		int fault = faults[i_fault];
	    		int id_not = fault/1000;
	    		int id = (int)can_test_matrix[id_not].id;
	    		TTCAN_tx(id,fault);
	    		i_fault++;
	    	}
	   		else
	   		{
	   			fault_no =0;
	   			i_fault = 0;
	   		}
	    }
	}
	if(back_messages_flag == 1 && free_tx > 150)
	{
		if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) > 0)
		{
			TTCAN_tx(18,0);
			__HAL_TIM_SET_COUNTER(&htim3,0);
			back_messages_flag = 0;
			i3++;
		}
	}
	if(free_time_flag == 1)
	{
		free_time_flag = 0;
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, 1);
		HAL_TIM_Base_Start_IT(&htim3);
		__HAL_TIM_SET_COUNTER(&htim3,0);
	}
}

void logging_tx()
{
	if(fault_id == 2)
	{
		index_log = i_m2;
	    i_m2++;
	}
	else if(fault_id == 3)
	{
		index_log = i_m3;
		i_m3++;
	}
	else if(fault_id == 4)
	{
		index_log = i_m4;
		i_m4++;
	}
	else if(fault_id == 8)
	{
		index_log = i_m8;
		i_m8++;
	}
	else if(fault_id == 10)
	{
		index_log = i_m10;
		i_m10++;
	}
	else if(fault_id == 12)
	{
	   index_log = i_m12;
	   i_m12++;
	}
}

