/*
 * GPIO.h
 *
 *  Created on: May 15, 2020
 *      Author: D.F.R. / R.G.P.
 */

#ifndef GPIO_H_
#define GPIO_H_


#include <stdint.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
//#include "pin_mux.h"
/* TODO: insert other include files here. */
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LEDR_GPIO 		BOARD_LED_RED_GPIO
#define BOARD_LEDR_GPIO_PIN 	BOARD_LED_RED_GPIO_PIN
#define BOARD_LEDG_GPIO 		BOARD_LED_GREEN_GPIO
#define BOARD_LEDG_GPIO_PIN 	BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LEDB_GPIO 		BOARD_LED_BLUE_GPIO
#define BOARD_LEDB_GPIO_PIN 	BOARD_LED_BLUE_GPIO_PIN

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void config_LED_RGB(void);

void config_SW3(void);
void PORTA_IRQHandler(void);


#endif /* GPIO_H_ */
