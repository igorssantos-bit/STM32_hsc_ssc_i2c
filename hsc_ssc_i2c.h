/**
  ******************************************************************************
  * @file           : hsc_ssc_i2c.h
  * @brief          : This file contains the headers of honeywell pressure
  *         sensor SSCDANN150PG2A3
  * @autor          : igorssantos-bit
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef INC_HSC_SSC_I2C_H_
#define INC_HSC_SSC_I2C_H_

/* Includes -------------------------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Defines --------------------------------------------------------------------------------------*/

/* Default communication value timeout */
#define DEFAULT_I2C_TIMEOUT			  100

/* Measurement error value */
#define HSC_SSC_MEASURE_FAILED		0xFFFF

/* Temperature resolution */
#define HSC_SSC_TEMP_8BIT_RES     3			// 2 for bridge + 1 for temp
#define HSC_SSC_TEMP_11BIT_RES    4			// 2 for bridge + 2 for temp

/* I2C communication address
 * Slave Pressure sensors address can be found based on the part number: _SC_________XA_.
 * Where X define the following adresses in 7-bit format:
 * S  - spi (this is not the right library for spi opperation)
 * 2  - i2c slave address 0x28 (used in the tests)
 * 3  - i2c slave address 0x38
 * 5  - i2c slave address 0x58
 * 6  - i2c slave address 0x68
 * 7  - i2c slave address 0x78
 * */
#define HSC_SSC_PRESS_ADDR			   ((0x28 << 1) | 0x01) // Only read option available

/* Datasheet specifications for SSC-D-AN-N-150PG-2-A-3 sensor:
 * SSC = Standard Accuracy, Compensated/Amplified
 * D = DIP (Dual Inline Pin)
 * AN = Single axial barbed port (layout)
 * N = Dry gases only, no diagnostics
 * 150PG = 0 psi to 150 psi
 * 2 = Address 0x28
 * A = 10% to 90% of Vsupply (analog), 214 counts (digital)
 * 3 = 3.3 Vdd voltage supply
 * */
#define OUTPUT_MIN 					        0x666       // 10%
#define OUTPUT_MAX 					        0x3999      // 90% of 2^14 - 1
#define PRESSURE_MIN 				        0
#define PRESSURE_MAX_PSI 			      150.0
#define PRESSURE_MAX_PASCAL 		    1034213.6 	// 150 psi in pascal

/* Functions declarations -----------------------------------------------------------------------*/

/**
 * @brief		Reads pressure measurement and temperature from the sensor via i2c
 * @param		hi2c: I2C bus handler.
 * @retval		Current pressure measured in psi and temperature em C.
 * 				HSC_SSC_MEASURE_FAILED
 */
void hsc_ssc_i2c_get_data(I2C_HandleTypeDef *hi2c, float *pressure, float *temperature);

/**
 * @brief		Reads pressure measurement from the sensor via i2c
 * @param		hi2c: I2C bus handler.
 * @retval		Current pressure measured in psi.
 * 				HSC_SSC_MEASURE_FAILED
 */
float hsc_ssc_i2c_read_pressure(I2C_HandleTypeDef *hi2c);

#endif /* INC_HSC_SSC_I2C_H_ */
