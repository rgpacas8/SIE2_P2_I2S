/*
 * Tareas_I2S.h
 *
 *  Created on: May 15, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "Tareas_I2S.h"



void task_SerialClock_SCK( void*data )
{
	uint32_t num_task = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	parameters_task_t parameters_task = *((parameters_task_t*)data);
	static uint32_t toggle = true;

	while(1)
	{
		xSemaphoreGive(parameters_task.Sempahore_serial_clock);
		xSemaphoreGive(parameters_task.Semaphore_Serial_data);

		printf("SCK is running\n");
		num_task++;
		PRINTF("num_task: %i\n", num_task);

		if (toggle == false) { 				// Set LED 0
			GPIO_PortClear(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
			toggle = true;
		} else { 							// Clear LED 0
			GPIO_PortSet(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
			toggle = false;
		}

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SCK_TIME_MS));
	}
}

void task_WordSelect_WS( void*data )
{
	parameters_task_t parameters_task = *((parameters_task_t*)data);
		TickType_t xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		xSemaphoreTake(parameters_task.Semaphore_task_word, portMAX_DELAY);
		printf("WS is running\n");
		xSemaphoreGive(parameters_task.Semaphore_task_word);

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));

	}
}

void task_SerialData_SD( void*data )
{
	parameters_task_t parameters_task = *((parameters_task_t*)data);
		TickType_t xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		xSemaphoreTake(parameters_task.Semaphore_Serial_data, portMAX_DELAY);
		printf("SD is running\n");
		xSemaphoreGive(parameters_task.Semaphore_Serial_data);
		xSemaphoreGive(parameters_task.Semaphore_task_word);

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));
	}
}


