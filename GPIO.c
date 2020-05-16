/*
 * GPIO.c
 *
 *  Created on: May 15, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "GPIO.h"



	/* The tasks to be created. */
	static void vHandlerTask(void *pvParameters);

	/* Declare a variable of type SemaphoreHandle_t.  This is used to reference the
	semaphore that is used to synchronize a task with an interrupt. */
	SemaphoreHandle_t xBinarySemaphore;



void config_LED_RGB(void)
{
	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0 };

	/* Port A Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortA);
	/* Port C Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortC);
	/* Port E Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortE);

	const port_pin_config_t pinOutput_config = { kPORT_PullDisable,				// 0U
												 kPORT_FastSlewRate,			// 0U
												 kPORT_PassiveFilterDisable,	// 0U
												 kPORT_OpenDrainDisable,		// 0U
												 kPORT_LowDriveStrength,		// 0U
												 	 kPORT_MuxAsGpio,				// 1U
												 kPORT_UnlockRegister 			// 0U
												};

    /* PORTC (pin  9) is configured as PTC9  */
    PORT_SetPinConfig(PORTC,  9U, &pinOutput_config);

    /* PORTE (pin  6) is configured as PTE6  */
    PORT_SetPinConfig(PORTE,  6U, &pinOutput_config);

    /* PORTA (pin 11) is configured as PTA11 */
    PORT_SetPinConfig(PORTA, 11U, &pinOutput_config);

	/* Init output LED GPIO. */
	GPIO_PinInit(BOARD_LEDR_GPIO, BOARD_LEDR_GPIO_PIN, &led_config);
	GPIO_PortSet(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
	GPIO_PinInit(BOARD_LEDG_GPIO, BOARD_LEDG_GPIO_PIN, &led_config);
	GPIO_PortSet(BOARD_LEDG_GPIO, 1u << BOARD_LEDG_GPIO_PIN);
	GPIO_PinInit(BOARD_LEDB_GPIO, BOARD_LEDB_GPIO_PIN, &led_config);
	GPIO_PortSet(BOARD_LEDB_GPIO, 1u << BOARD_LEDB_GPIO_PIN);

	/* Print a note to terminal. */
	PRINTF("\r\nGPIO RGB Driver \r\n");
}

void config_SW3(void) {
	/* Define the init structure for the input switch pin */
	gpio_pin_config_t sw_config = { kGPIO_DigitalInput, 0 };

    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    const port_pin_config_t porta10_pinM9_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTA10 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA10 (pin M9) is configured as PTA10 */
    PORT_SetPinConfig(PORTA, 10U, &porta10_pinM9_config);

    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);

    NVIC_SetPriority(PORTA_IRQn, 0x3);

   	EnableIRQ(BOARD_SW_IRQ);
	GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

	///////////////////////////////////////////////////////////
	 /* Before a semaphore is used it must be explicitly created.  In this
		example	a binary semaphore is created. */
	    xBinarySemaphore = xSemaphoreCreateBinary();

		/* Check the semaphore was created successfully. */
		if( xBinarySemaphore != NULL )
		{
			/* Create the 'handler' task, which is the task to which interrupt
			processing is deferred, and so is the task that will be synchronized
			with the interrupt.  The handler task is created with a high priority to
			ensure it runs immediately after the interrupt exits.  In this case a
			priority of 3 is chosen. */
			xTaskCreate( vHandlerTask, "Handler", 400, NULL, 3, NULL );
		}
}

static void vHandlerTask( void *pvParameters )
{
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Use the semaphore to wait for the event.  The semaphore was created
		before the scheduler was started so before this task ran for the first
		time.  The task blocks indefinitely meaning this function call will only
		return once the semaphore has been successfully obtained - so there is
		no need to check the returned value. */
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );

		/* To get here the event must have occurred.  Process the event (in this
		case just print out a message). */
		PRINTF( "Handler task - Processing event.\r\n" );
	}
}

void PORTA_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);

	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	it will get set to pdTRUE inside the interrupt safe API function if a
	context switch is required. */
	xHigherPriorityTaskWoken = pdFALSE;

	/* 'Give' the semaphore to unblock the task. */
	xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );

	/* Pass the xHigherPriorityTaskWoken value into portYIELD_FROM_ISR().  If
	xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR()
	then calling portYIELD_FROM_ISR() will request a context switch.  If
	xHigherPriorityTaskWoken is still pdFALSE then calling
	portYIELD_FROM_ISR() will have no effect.  The implementation of
	portYIELD_FROM_ISR() used by the Windows port includes a return statement,
	which is why this function does not explicitly return a value. */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
