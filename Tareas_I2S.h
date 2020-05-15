/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 */

/*
 * @file:			main.c
 * @brief:   		Practica 2 simulacion de I2S
 *
 * @company:			  ITESO
 * @Engineer Team:	 D.F.R. / R.G.P.
 * @contact:		ie717807@iteso.mx
 * @contact:		ie706818@iteso.mx
 */
#ifndef TAREAS_I2S_H_
#define TAREAS_I2S_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"

typedef struct
{
	SemaphoreHandle_t Sempahore_serial_clock;
	SemaphoreHandle_t Semaphore_task_word;
	SemaphoreHandle_t Semaphore_Serial_data;
	EventGroupHandle_t event_FreeRTOs;
	uint32_t counter;
} parameters_task_t;

void task_Serial_clock(void*data);
void task_word_select(void*data);
void task_Serial_data(void*data);


#endif /* TAREAS_I2S_H_ */
