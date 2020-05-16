/*
 * Tareas_I2S.h
 *
 *  Created on: May 15, 2020
 *      Author: D.F.R. / R.G.P.
 */

#include "Tareas_I2S.h"


#define DEBUG_RED_SCK	 1
//#define DEBUG_GREEN_WS	 1	// Visualizamos led GREEN la señal de "WS"
//#define DEBUG_BLUE_SD		 1	// ... Junto con led BLUE la señal de "SD"


void task_SerialClock_SCK( void*data )
{
	uint32_t num_task = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	parameters_task_t parameters_task = *((parameters_task_t*)data);
	static uint32_t toggle = false;
	static uint8_t  cont_flanco_bajada = 0;

	while(1)
	{
		PRINTF("SCK is running\n");
		num_task++;
		PRINTF("num_task: %i\n", num_task);

		if (toggle == false) { 					// RED = ON
#ifndef DEBUG_RED_SCK
			GPIO_PortSet(BOARD_LEDG_GPIO, 1u << BOARD_LEDG_GPIO_PIN);
			GPIO_PortClear(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
#endif
			GPIO_PinWrite(GPIOA, pin6, UNO);	// PTA6 = 1U
			toggle = true;
		} else { 								// RED = OFF
#ifndef DEBUG_RED_SCK
			GPIO_PortSet(BOARD_LEDG_GPIO, 1u << BOARD_LEDG_GPIO_PIN);
			GPIO_PortSet(BOARD_LEDR_GPIO, 1u << BOARD_LEDR_GPIO_PIN);
#endif
			GPIO_PinWrite(GPIOA, pin6, CERO);	// PTA6 = 0U
			toggle = false;
			cont_flanco_bajada++;
			xEventGroupSetBits(parameters_task.event_FreeRTOs, EVENT_SD);
			if (cont_flanco_bajada == 8) {
				xSemaphoreGive(parameters_task.Semaphore_task_word);
				cont_flanco_bajada = 0;
			}

		}

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SCK_TIME_MS));
	}
}

void task_WordSelect_WS( void*data )
{
	uint32_t num_task = 0;

	parameters_task_t parameters_task = *((parameters_task_t*)data);
	static uint32_t toggle = true;

	while(1)
	{
		xSemaphoreTake(parameters_task.Semaphore_task_word, portMAX_DELAY);

		printf("\nWS is running\n");
		num_task++;
		PRINTF("num_task: %i\n\n", num_task);

		if (toggle == false) { 					// GREEN = ON
#ifndef DEBUG_GREEN_WS
			GPIO_PortClear(BOARD_LEDG_GPIO, 1u << BOARD_LEDG_GPIO_PIN);
#endif
			GPIO_PinWrite(GPIOA, pin7, UNO);	// PTA7 = 1U
			toggle = true;
		} else { 								// GREEN = OFF
#ifndef DEBUG_GREEN_WS
			GPIO_PortSet(BOARD_LEDG_GPIO, 1u << BOARD_LEDG_GPIO_PIN);
#endif
			GPIO_PinWrite(GPIOA, pin7, CERO);	// PTA7 = 0U
			toggle = false;
		}
	}
}

void task_SerialData_SD( void*data )
{
	parameters_task_t parameters_task = *((parameters_task_t*)data);
	static uint8_t  num_bit = 0;
	uint8_t bit_send_value = 0;
	uint8_t BLUE_ON_OFF = 0;

	while(1)
	{
		xEventGroupWaitBits(parameters_task.event_FreeRTOs, EVENT_SD, pdTRUE, pdTRUE, portMAX_DELAY);

		/* Transmit I2S byte: 0x0F (see I2S.h) in Serial Data (First bit after WS is LSB from the previous byte)*/
		switch (num_bit) {
			case MSB_bit7:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit7);	// PTA8 = 0U
				bit_send_value = BYTE_bit7;
				break;
			case bit6:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit6);	// PTA8 = 0U
				bit_send_value = BYTE_bit6;
				break;
			case bit5:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit5);	// PTA8 = 0U
				bit_send_value = BYTE_bit5;
				break;
			case bit4:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit4);	// PTA8 = 0U
				bit_send_value = BYTE_bit4;
				break;
			case bit3:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit3);	// PTA8 = 1U
				bit_send_value = BYTE_bit3;
				break;
			case bit2:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit2);	// PTA8 = 1U
				bit_send_value = BYTE_bit2;
				break;
			case bit1:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit1);	// PTA8 = 1U
				bit_send_value = BYTE_bit1;
				break;
			case LSB_bit0:
				GPIO_PinWrite(GPIOA, pin8, BYTE_bit0);	// PTA8 = 1U
				bit_send_value = BYTE_bit0;
				break;

			default:
				break;
			}

		num_bit++;

		if (num_bit == 8) {
			num_bit = 0;
		}

		//a = (flag == 1) ? c : d;	OPERADOR TERNARIO
		BLUE_ON_OFF = (bit_send_value == 1) ? 0 : 1;

		PRINTF("- - - SD is sending a bit - - -\n");
#ifndef DEBUG_BLUE_SD
		GPIO_PinWrite(BOARD_LEDB_GPIO, BOARD_LEDB_GPIO_PIN, BLUE_ON_OFF);	// PTA8 = bit_send_value
#endif
		PRINTF("		Bit to Send: %i\n\n", bit_send_value);
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
	GPIO_PinInit(GPIOA, pin, &led_config);
	GPIO_PortSet(GPIOA, 1u << pin);
	GPIO_PortClear(GPIOA, 1u << pin);

	/* Print a note to terminal. */
	PRINTF("GPIO SCK->PTA6  Driver \n");
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
	GPIO_PinInit(GPIOA, pin, &led_config);
	GPIO_PortSet(GPIOA, 1u << pin);		// Inicia encendido WS
//	GPIO_PortClear(GPIOA, 1u << pin);

	/* Print a note to terminal. */
	PRINTF("GPIO  WS->PTA7  Driver \n");
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
	GPIO_PinInit(GPIOA, pin, &led_config);
	GPIO_PortSet(GPIOA, 1u << pin);
	GPIO_PortClear(GPIOA, 1u << pin);

	/* Print a note to terminal. */
	PRINTF("GPIO  SD->PTA8  Driver \n\n");
}
