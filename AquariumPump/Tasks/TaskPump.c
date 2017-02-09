/*
 * TaskPump.c
 *
 *  Created on: 27/01/2017
 *      Author: Patrizio
 */

#include <FreeRTOS.h>
#include <task.h>
#include "TaskPump.h"

static void prvPumpTask(void *arg);

static unsigned char processState;

enum {
	PUMP_STOP,
	PUMP_START
};

void xStartPumpTask(void) {
	// Pin C3 as OUTPUT.
	DDRC |= _BV(DDC3);
	// Port LOW.
	PORTC &= ~_BV(PORTC3);
	processState = PUMP_STOP;
	xTaskCreate(prvPumpTask, (signed portCHAR *) "PUMP", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
}

static void prvPumpTask(void *arg) {
	const portTickType MINUTES_OFF = 1;
	const portTickType SECONDS_ON = 15;

	portTickType xLastWakeTime;
	uint8_t i;

	xLastWakeTime = xTaskGetTickCount();

	for (;;) {
		switch (processState) {
		case PUMP_STOP:
			// Port LOW.
			PORTC &= ~_BV(PORTC3);
			// Delay de MINUTES_OFF minutos.
			for (i = 0; i < MINUTES_OFF; i++) {
				vTaskDelayUntil(&xLastWakeTime, 60000 / portTICK_RATE_MS);
			}
			processState = PUMP_START;
			break;

		case PUMP_START:
			// Port HIGH.
			PORTC |= _BV(PORTC3);
			// Delay de 15 segundos.
			vTaskDelayUntil(&xLastWakeTime, (SECONDS_ON * 1000) / portTICK_RATE_MS);
			processState = PUMP_STOP;
			break;
		}
	}
}
