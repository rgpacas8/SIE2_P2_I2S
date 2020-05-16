/*
 * Tareas_I2S.h
 *
 *  Created on: May 15, 2020
 *      Author: D.F.R. / R.G.P.
 */

#ifndef TAREAS_I2S_H_
#define TAREAS_I2S_H_

/* TODO: insert other include files here. */
#include <stdint.h>
#include <stdbool.h>
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"
#include "GPIO.h"

/* TODO: insert other definitions and declarations here. */
#define SCK_TIME_MS	500  // t = 500ms    ---   f= 1/t*2 Hz = 1Hz

#define pin6 	6U	// Pin output in K66F
#define pin7 	7U
#define pin8 	8U

#define CERO 0U // State bit to transmit
#define UNO  1U

#define EVENT_SD    (1 << 0)


#define MSB_bit7	0U	// Control for Switch case
#define bit6		1U
#define bit5		2U
#define bit4		3U
#define bit3		4U
#define bit2		5U
#define bit1		6U
#define LSB_bit0	7U

#define BYTE_bit7	0U	// Bit value to send for one BYTE
#define BYTE_bit6	0U	// BYTE = 0x0F
#define BYTE_bit5	0U
#define BYTE_bit4	0U
#define BYTE_bit3	1U
#define BYTE_bit2	1U
#define BYTE_bit1	1U
#define BYTE_bit0	1U


typedef struct {
	SemaphoreHandle_t Semaphore_task_word;
	EventGroupHandle_t event_FreeRTOs;
} parameters_task_t;


void task_SerialClock_SCK(void *data);
void task_WordSelect_WS(void *data);
void task_SerialData_SD(void *data);

void I2S_config_SCK(uint32_t pin);
void I2S_config_WS(uint32_t pin);
void I2S_config_SD(uint32_t pin);


#endif /* TAREAS_I2S_H_ */
