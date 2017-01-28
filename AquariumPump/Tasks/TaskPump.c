/*
 * TaskPump.c
 *
 *  Created on: 27/01/2017
 *      Author: Patrizio
 */

#include <FreeRTOS.h>
#include <task.h>
#include "TaskPump.h"

#define MINUTES_OFF 15

static void prvPumpTask(void *arg);

static unsigned char processState;

enum {
	PUMP_STOP,
	PUMP_START,
	PUMP_IDLE
};

void xStartPumpTask(void) {
	// Pin C3 as OUTPUT.
	DDRC |= _BV(DDC3);
	processState = PUMP_STOP;
	xTaskCreate(prvPumpTask, (signed portCHAR *) "PUMP", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
}

static void prvPumpTask(void *arg) {
	uint8_t i;

	for (;;) {
		switch (processState) {
		case PUMP_STOP:
			// Port LOW.
			PORTC &= ~_BV(PORTC3);
			// Delay de MINUTES_OFF minutos.
			for (i = 0; i < MINUTES_OFF; i++) {
				vTaskDelay(60000);
			}
			processState = PUMP_START;
			break;

		case PUMP_START:
			// Port HIGH.
			PORTC |= _BV(PORTC3);
			// Delay de 15 segundos.
			vTaskDelay(15000);
			processState = PUMP_STOP;
			break;
		}
	}
}
