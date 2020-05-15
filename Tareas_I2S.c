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
#include "Tareas_I2S.h"
/*
 * Hasta el momento simplementa pasa por las tres tareas de manera repetitiva
 */

void task_Serial_clock( void*data )
{
	parameters_task_t parameters_task = *((parameters_task_t*)data);
	TickType_t xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		printf("TOGLE\n");

		xSemaphoreGive(parameters_task.Sempahore_serial_clock);
		xSemaphoreGive(parameters_task.Semaphore_Serial_data);

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));
	}
}

void task_Serial_data( void*data )
{
	parameters_task_t parameters_task = *((parameters_task_t*)data);
		TickType_t xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		xSemaphoreTake(parameters_task.Semaphore_Serial_data, portMAX_DELAY);
		printf("DE bajo a alto\n");
		xSemaphoreGive(parameters_task.Semaphore_Serial_data);
		xSemaphoreGive(parameters_task.Semaphore_task_word);

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));
	}
}

void task_word_select( void*data )
{
	parameters_task_t parameters_task = *((parameters_task_t*)data);
		TickType_t xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		xSemaphoreTake(parameters_task.Semaphore_task_word, portMAX_DELAY);
		printf("task word\n");
		xSemaphoreGive(parameters_task.Semaphore_task_word);

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));

	}
}
