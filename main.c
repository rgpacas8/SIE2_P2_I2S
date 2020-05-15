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
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "Tareas_I2S.h"
/* TODO: insert other definitions and declarations here. */
#define EVENT_PRODUCER    (1 << 0)
#define EVENT_CONSUMER    (1 << 1)









/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n\r");

    static parameters_task_t parameters_task;


    parameters_task.Semaphore_task_word = xSemaphoreCreateBinary();
    parameters_task.Sempahore_serial_clock = xSemaphoreCreateBinary();
    parameters_task.Semaphore_Serial_data= xSemaphoreCreateBinary();
    parameters_task.counter = 0;

    parameters_task.event_FreeRTOs = xEventGroupCreate();

    xTaskCreate( task_Serial_clock, "task_Serial_clock", 200, (void*)&parameters_task, configMAX_PRIORITIES, NULL );
    xTaskCreate( task_word_select, "task_word_select", 200, (void*)&parameters_task, configMAX_PRIORITIES, NULL );
    xTaskCreate( task_Serial_data, "task_Serial_data", 200, (void*)&parameters_task, configMAX_PRIORITIES, NULL );

    vTaskStartScheduler();

    while( 1 )
    {

    };

    return 0 ;
}




