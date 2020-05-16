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
	uint8_t  cont_flanco_bajada = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	parameters_task_t parameters_task = *((parameters_task_t*)data);
	static uint32_t toggle = false;

	while(1)
	{
//		xSemaphoreGive(parameters_task.Sempahore_serial_clock);
//		xSemaphoreGive(parameters_task.Semaphore_Serial_data);

		printf("SCK is running\n");
		num_task++;
		PRINTF("num_task: %i\n", num_task);

		if (toggle == false) { 					// RED = ON
			GPIO_PortClear(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
			GPIO_PinWrite(GPIOA, pin6, UNO);	// PTA6 = 1U
			toggle = true;
		} else { 								// RED = OFF
			GPIO_PortSet(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
			GPIO_PinWrite(GPIOA, pin6, CERO);	// PTA6 = 0U
			toggle = false;
			cont_flanco_bajada++;
			if(cont_flanco_bajada==4){
				xSemaphoreGive(parameters_task.Semaphore_task_word);
				cont_flanco_bajada=0;
			}

		}

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SCK_TIME_MS));
	}
}

void task_WordSelect_WS( void*data )
{
	uint32_t num_task = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	parameters_task_t parameters_task = *((parameters_task_t*)data);
	static uint32_t toggle = false;

	while(1)
	{
		xSemaphoreTake(parameters_task.Semaphore_task_word, portMAX_DELAY);

		printf("WS is running\n");
		num_task++;
		PRINTF("num_task_ws: %i\n", num_task);
		if (toggle == false) { 					// RED = ON
			GPIO_PortClear(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
			GPIO_PinWrite(GPIOA, pin7, UNO);	// PTA7 = 1U
			toggle = true;
		} else { 								// RED = OFF
			GPIO_PortSet(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
			GPIO_PinWrite(GPIOA, pin7, CERO);	// PTA7 = 0U
			toggle = false;
		}
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

/////////////////////////////////////////////////////////////////////////////

void I2S_config_SCK(uint32_t pin)
{
	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0 };

	/* Port A Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortA);

	const port_pin_config_t pinOutput_config = { kPORT_PullDisable,				// 0U
												 kPORT_FastSlewRate,			// 0U
												 kPORT_PassiveFilterDisable,	// 0U
												 kPORT_OpenDrainDisable,		// 0U
												 kPORT_LowDriveStrength,		// 0U
												 	 kPORT_MuxAsGpio,				// 1U
												 kPORT_UnlockRegister 			// 0U
												};

	/* PORTA (pin n) is configured as PTAn */
	PORT_SetPinConfig(PORTA, pin, &pinOutput_config);

	/* Init output GPIO. */
	GPIO_PinInit(GPIOA, 6U, &led_config);
	GPIO_PortSet(GPIOA, 1u << pin);
	GPIO_PortClear(GPIOA, 1u << pin);
}

void I2S_config_WS(uint32_t pin)
{
	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0 };

	/* Port A Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortA);

	const port_pin_config_t pinOutput_config = { kPORT_PullDisable,				// 0U
												 kPORT_FastSlewRate,			// 0U
												 kPORT_PassiveFilterDisable,	// 0U
												 kPORT_OpenDrainDisable,		// 0U
												 kPORT_LowDriveStrength,		// 0U
												 	 kPORT_MuxAsGpio,				// 1U
												 kPORT_UnlockRegister 			// 0U
												};

	/* PORTA (pin n) is configured as PTAn */
	PORT_SetPinConfig(PORTA, pin, &pinOutput_config);

	/* Init output GPIO. */
	GPIO_PinInit(GPIOA, 7U, &led_config);
	GPIO_PortSet(GPIOA, 1u << pin);
	GPIO_PortClear(GPIOA, 1u << pin);
}

void I2S_config_SD(uint32_t pin)
{
	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0 };

	/* Port A Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortA);

	const port_pin_config_t pinOutput_config = { kPORT_PullDisable,				// 0U
												 kPORT_FastSlewRate,			// 0U
												 kPORT_PassiveFilterDisable,	// 0U
												 kPORT_OpenDrainDisable,		// 0U
												 kPORT_LowDriveStrength,		// 0U
												 	 kPORT_MuxAsGpio,				// 1U
												 kPORT_UnlockRegister 			// 0U
												};

	/* PORTA (pin n) is configured as PTAn */
	PORT_SetPinConfig(PORTA, pin, &pinOutput_config);

	/* Init output GPIO. */
	GPIO_PinInit(GPIOA, 8U, &led_config);
	GPIO_PortSet(GPIOA, 1u << pin);
	GPIO_PortClear(GPIOA, 1u << pin);
}
