/*
 * sec_ini1.c
 *
 *  Created on: Nov 5, 2025
 *      Author: samfishy
 */

#include "cmox_init.h"
#include "cmox_low_level.h"
#include "cmox_crypto.h"

#include "stm32g4xx_hal.h"
#include "main.h"
#include "Main_Funcs.h"

#include "encoder.h"
#include "stdio.h"
#include <string.h>
#include "math.h"

cmox_gcm_handle_t Gcm_Ctx;

extern FDCAN_TxHeaderTypeDef Tx_can;
extern FDCAN_RxHeaderTypeDef Rx_can;
extern uint8_t	Tx_can_data1[64];
extern uint8_t	Rx_can_data[64];
extern uint8_t rx_data_decoded[64];
extern int index_rx;
extern int index_1;
extern struct TTCANFD_Mess can_test_matrix[];

const uint8_t Key[] =
{
  0x46, 0x3b, 0x41, 0x29, 0x11, 0x76, 0x7d, 0x57, 0xa0, 0xb3, 0x39, 0x69, 0xe6, 0x74, 0xff, 0xe7,
  0x84, 0x5d, 0x31, 0x3b, 0x88, 0xc6, 0xfe, 0x31, 0x2f, 0x3d, 0x72, 0x4b, 0xe6, 0x8e, 0x1f, 0xca
};

uint8_t key1[] = {} ;

const uint8_t IV[] =
{
  0x61, 0x1c, 0xe6, 0xf9, 0xa6, 0x88, 0x07, 0x50, 0xde, 0x7d, 0xa6, 0xcb
};

uint8_t non_critic_data[1] = {0};
size_t	non_critic_data_size = 0;
size_t  auth_size = 16;


cmox_cipher_retval_t retval;
size_t computed_size;
size_t computed_size_rx;
size_t computed_size_rx_dec;
/* General cipher context */
cmox_cipher_handle_t *cipher_ctx;
/* Fault check verification variable */
uint32_t fault_check = CMOX_CIPHER_AUTH_FAIL;

cmox_init_arg_t init_target = {CMOX_INIT_TARGET_AUTO, NULL};

void aes_init(void)
{
	  if (cmox_initialize(&init_target) != CMOX_INIT_SUCCESS)
	  {
	    Error_Handler();
	  }

}

void aes_encoding()
{


	     size_t len1 = (size_t)can_test_matrix[index_1].dlc + auth_size;


			if(len1 == 0)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_0;
	  			computed_size = 0;
	  		}
	  		if(len1 == 1)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_1;
	  			computed_size = 1;
	  		}
	  		if(len1 == 2)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_2;
	  			computed_size = 2;
	  		}
	  		if(len1 == 3)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_3;
	  			computed_size = 3;
	  		}
	  		if(len1 == 4)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_4;
	  			computed_size = 4;
	  		}
	  		if(len1 == 5)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_5;
	  			computed_size = 5;
	  		}
	  		if(len1 == 6)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_6;
	  			computed_size = 6;
	  		}
	  		if(len1 == 7)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_7;
	  			computed_size = 7;
	  		}
	  		if(len1 == 8)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_8;
	  			computed_size = 8;
	  		}
	  		if(len1 >= 9 && len1 <= 12)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_12;
	  			computed_size = 12;
	  		}
	  		if(len1 >= 13 && len1 <= 16)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_16;
	  			computed_size = 16;
	  		}
	  		if(len1 >= 17 && len1 <= 20)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_20;
	  			computed_size = 20;
	  		}
	  		if(len1 >= 21 && len1 <= 24)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_24;
	  			computed_size = 24;
	  		}
	  		if(len1 >= 25 && len1 <= 32)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_32;
	  			computed_size = 32;
	  		}
	  		if(len1 >= 33 && len1 <= 48)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_48;
	  			computed_size = 48;
	  		}
	  		if(len1 >= 49 && len1 <= 64)
	  		{
	  			Tx_can.DataLength = FDCAN_DLC_BYTES_64;
	  			computed_size = 64;
	  		}

	retval = cmox_aead_encrypt(CMOX_AES_GCM_ENC_ALGO,                    							 /* Use AES GCM algorithm */
							   can_test_matrix[index_1].data, (size_t)can_test_matrix[index_1].dlc,  /* Plaintext to encrypt */
	                           auth_size,                   										 /* Authentication tag size */
	                           Key, sizeof(Key),                       								 /* AES key to use */
	                           IV, sizeof(IV),                         								 /* Initialization vector */
							   non_critic_data, non_critic_data_size,  								 /* Additional authenticated data */
							   Tx_can_data1, &computed_size);   									 /* Data buffer to receive generated ciphertext and authentication tag */

	/* Verify API returned value */
	  if (retval != CMOX_CIPHER_SUCCESS)
	  {
	    Error_Handler();
	  }
	  /* Verify generated data size is the expected one */
	  if (computed_size != ((size_t)can_test_matrix[index_1].dlc + auth_size))
	  {
	    Error_Handler();
	  }
}

void aes_decoding()
{
	computed_size_rx = ((size_t)can_test_matrix[index_rx].dlc + (size_t)auth_size);
	computed_size_rx_dec = (size_t)can_test_matrix[index_rx].dlc  - auth_size;

	retval = cmox_aead_decrypt(CMOX_AES_GCM_DEC_ALGO,                  /* Use AES GCM algorithm */
							   Rx_can_data, computed_size_rx,     /* Ciphertext + tag to decrypt and verify */
							   auth_size,                   /* Authentication tag size */
	                           Key, sizeof(Key),                       /* AES key to use */
	                           IV, sizeof(IV),                         /* Initialization vector */
	                           non_critic_data, non_critic_data_size,               /* Additional authenticated data */
							   rx_data_decoded, &computed_size_rx_dec);    /* Data buffer to receive generated plaintext */

	  /* Verify API returned value */
	  if (retval != CMOX_CIPHER_AUTH_SUCCESS)
	  {
	    Error_Handler();
	  }

	  /* Verify generated data size is the expected one */
	  if (computed_size_rx_dec != (size_t)can_test_matrix[index_rx].dlc)
	  {
	    Error_Handler();
	  }

	  /* Verify generated data are the expected ones */
}
