/*
 * Main_Funcs.h
 *
 *  Created on: Aug 19, 2025
 *      Author: samfishy
 */

#ifndef INC_MAIN_FUNCS_H_
#define INC_MAIN_FUNCS_H_

struct TTCANFD_Mess{

	uint8_t  rx_tx;
	uint32_t id;
    char*    msg;
    uint32_t dlc;
    uint32_t ttcan_def;
    uint32_t period_ms;
    uint8_t  data[64];
    int last_sent;
    int times_sent;
} ;

struct FT_TT_CANFD_mess{

	uint32_t id;
    uint32_t Td;
    uint32_t Ta;
    uint8_t  Fault;
};

struct Data_Loging{

	uint16_t id_tx[1000];
	uint16_t id_rx[1000];
	uint32_t timestamp_tx[1000];
	uint32_t timestamp_rx[1000];
};

void MainLoop(void);
void MainInit(void);
void TTCANFD_MainInit(void);

#endif /* INC_MAIN_FUNCS_H_ */
