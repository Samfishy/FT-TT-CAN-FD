/*
 * enocoder.c
 *
 *  Created on: Aug 24, 2025
 *      Author: samfishy
 */
#include "main.h"

#include <stdio.h>
#include <string.h>

#include "pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "uart.pb.h"
#include "Main_Funcs.h"

extern UART_HandleTypeDef hlpuart1;

extern struct Data_Loging log1;

pc_comm_master log_mess = pc_comm_master_init_zero;
pc_comm_master log_rec  = pc_comm_master_init_zero;

uint16_t len;
uint8_t sent[25000];
bool status ;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern FDCAN_HandleTypeDef hfdcan1;
extern int rx_data_after_rx[];
extern uint8_t	Rx_can_data[];

void proto_tx()
{
	HAL_TIM_Base_Stop_IT(&htim1);
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_FDCAN_DeactivateNotification(&hfdcan1,  FDCAN_IT_RX_FIFO0_NEW_MESSAGE);

	for(int i = 0;i < 2000;i++)
	{
		if(i <1400)
		{
			log_mess.id_tx[i] 	= (uint32_t)log1.id_tx[i];
			log_mess.tim_tx[i] 	= log1.timestamp_tx[i];
		}
		if(i < 900)
		{
			log_mess.id_rx[i] 	= (uint32_t)log1.id_rx[i];
			log_mess.tim_rx[i] 	= log1.timestamp_rx[i];
		}
		/*if(i < 100)
		{
			log_mess.tim_mc[i] 	= log1.mc_time[i];
		}
		if(i < 200)
		{
			log_mess.tim_bc[i] 	= log1.bc_time[i];
		}

		log_mess.id_total[i]  = log1.id_totla[i];
		log_mess.tim_total[i] = log1.welp[i];
	*/}

	log_mess.id_tx_count = 1700;
	log_mess.id_rx_count = 700;
	log_mess.tim_tx_count = 1700;
	log_mess.tim_rx_count = 700;
	//log_mess.tim_mc_count = 100;
	//log_mess.tim_bc_count = 200;

	pb_ostream_t stream = pb_ostream_from_buffer(sent, 25000);
	status = pb_encode(&stream, pc_comm_master_fields, &log_mess);
	len = stream.bytes_written;

	if (status != true)
    {
        printf("Encoding failed: %s\n",PB_GET_ERROR(&stream));
        return;
    }

	/*for(int i = 0; i <= 7;i++)
	{
		if(i <= 6)
		{
			size_t len_1;
			len_1 = 1000;
			header[0] = (len_1>>8) & 0xFF;
			header[1] = len_1 & 0xFF;

			status = HAL_UART_Transmit(&hlpuart1, header, 2, HAL_MAX_DELAY);
			if(status != HAL_OK)
			{
				printf("Header send failed!\n");
			}
			status = HAL_UART_Transmit(&hlpuart1, &sent[i*1000], len_1, HAL_MAX_DELAY);
			if(status != HAL_OK)
			{
			    printf("Data send failed!\n");
			}
		}
		else
		{
			size_t len_1;
			len_1 = len-6000;
			header[0] = (len_1>>8) & 0xFF;
			header[1] = len_1 & 0xFF;

			status = HAL_UART_Transmit(&hlpuart1, header, 2, HAL_MAX_DELAY);
			if(status != HAL_OK)
			{
				printf("Header send failed!\n");
			}
			status = HAL_UART_Transmit(&hlpuart1, &sent[6000], len_1, HAL_MAX_DELAY);
			if(status != HAL_OK)
			{
			    printf("Data send failed!\n");
			}
		}

		HAL_Delay(100);
	}*/
	uint8_t header[2];
	header[0] = (len >> 8) & 0xFF; // High byte
	header[1] = len & 0xFF;

	status = HAL_UART_Transmit_DMA(&hlpuart1, header, 2);
	if(status != HAL_OK)
	{
		printf("Header send failed!\n");
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	status = HAL_UART_Transmit_DMA(&hlpuart1, sent, len);
	if(status != HAL_OK)
	{
		printf("Data send failed!\n");
	}

	while(1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
	}
}

