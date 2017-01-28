/*
 * main.c
 *
 *  Created on: 27/01/2017
 *      Author: Patrizio
 */

#include <task.h>
#include <TaskPump.h>

int main(void) {
	xStartPumpTask();
	vTaskStartScheduler();
	return 0;
}
