/**
  ******************************************************************************
  * @file           : hsc_ssc_i2c.h
  * @brief          : This file contains the headers of honeywell pressure
  * 				sensor SSCDANN150PG2A3
  * @autor          : igorssantos-bit
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------*/
#include "main.h"
#include "hsc_ssc_i2c.h"

/* Variables declarations -----------------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c2;

/* Functions definitions ------------------------------------------------------------------------*/

void hsc_ssc_i2c_get_data(I2C_HandleTypeDef *hi2c, float *pressure, float *temperature){
	HAL_StatusTypeDef i2c_comm;
	uint8_t	hsc_ssc_buffer[4];
	uint8_t status = 0;
	uint16_t bridge_data = 0;
	uint16_t temp_data = 0;

	i2c_comm = HAL_I2C_Master_Receive(hi2c, HSC_SSC_PRESS_ADDR, hsc_ssc_buffer, HSC_SSC_TEMP_8BIT_RES, DEFAULT_I2C_TIMEOUT);
	if(i2c_comm == HAL_OK){
		HAL_Delay(10);

		// first 2 bits from first byte (S1, S0):
		// 00 = normal operation, valid data.
		// 01 = device in command mode.
		// 10 = stale data: data that has already been fetched since the last measurement cycle, or data
		//      fetched before the first measurement has been completed.
		// 11 = diagnostic condition
		status = (hsc_ssc_buffer[0] & 0xc0) >> 6;

		if(status == 0){
			// resultant bits for bridge data
			bridge_data = ((hsc_ssc_buffer[0] & 0x3f) << 8) + hsc_ssc_buffer[1];
			*pressure = (1.0 * ((((bridge_data - OUTPUT_MIN) * (PRESSURE_MAX_PSI - PRESSURE_MIN)) / (OUTPUT_MAX - OUTPUT_MIN)) + PRESSURE_MIN));
			temp_data = ((hsc_ssc_buffer[2] << 8) + (hsc_ssc_buffer[3] & 0xe0)) >> 5;
			*temperature = (temp_data * 0.0977) - 50;
		}
	}
}

float hsc_ssc_i2c_read_pressure(I2C_HandleTypeDef *hi2c){
	HAL_StatusTypeDef i2c_comm;
	uint8_t	hsc_ssc_buffer[2];
	uint8_t status = 0;
	uint16_t bridge_data = 0;

	i2c_comm = HAL_I2C_Master_Receive(hi2c, HSC_SSC_PRESS_ADDR, hsc_ssc_buffer, 2, DEFAULT_I2C_TIMEOUT);
	if(i2c_comm == HAL_OK){
		HAL_Delay(10);

		// first 2 bits from first byte (S1, S0):
		// 00 = normal operation, valid data.
		// 01 = device in command mode.
		// 10 = stale data: data that has already been fetched since the last measurement cycle, or data
		//      fetched before the first measurement has been completed.
		// 11 = diagnostic condition
		status = (hsc_ssc_buffer[0] & 0xc0) >> 6;

		if(status == 0){
			// resultant bits for bridge data
			bridge_data = ((hsc_ssc_buffer[0] & 0x3f) << 8) + hsc_ssc_buffer[1];
			return (1.0 * ((((bridge_data - OUTPUT_MIN) * (PRESSURE_MAX_PSI - PRESSURE_MIN)) / (OUTPUT_MAX - OUTPUT_MIN)) + PRESSURE_MIN));
		}
		else return HSC_SSC_MEASURE_FAILED;
	}
	else return HSC_SSC_MEASURE_FAILED;
}
