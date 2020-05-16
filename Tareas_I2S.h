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
#define SCK_TIME_MS	1000  // t = 1000 ms    ---   f= 1/t Hz

#define pin6 	6U	// Pin output in K66F
#define pin7 	7U
#define pin8 	8U

#define CERO 0U // State bit to transmit
#define UNO  1U

#define EVENT_PRODUCER    (1 << 0)
#define EVENT_CONSUMER    (1 << 1)

typedef struct
{
	SemaphoreHandle_t Sempahore_serial_clock;
	SemaphoreHandle_t Semaphore_task_word;
	SemaphoreHandle_t Semaphore_Serial_data;
	EventGroupHandle_t event_FreeRTOs;
//	uint32_t counter;
} parameters_task_t;

void task_SerialClock_SCK(void*data);
void task_WordSelect_WS(void*data);
void task_SerialData_SD(void*data);

void I2S_config_SCK(uint32_t pin);
void I2S_config_WS(uint32_t pin);
void I2S_config_SD(uint32_t pin);


#endif /* TAREAS_I2S_H_ */
