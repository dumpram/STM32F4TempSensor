#include "usart.h"
#include "temp.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define NUM_OF_SAMPLES 100

xQueueHandle tempQueue;


void delay() {
	int i = 8000000;
	while(i--);
}

void calcAverageTemperature() {
	int i;
	double avrTemp = 0;
	init_ADC();
	while(1) {
			for (i = 0; i < NUM_OF_SAMPLES; i++) {
				avrTemp = (avrTemp + get_temperature())/2;
				xQueueSendToBack(tempQueue, &avrTemp, 0);
			}
	}
}

void sendTemperature() {
	double avrTemp;
	char temp[10];
	init_USART1(9600);
	while(1) {
		USART_puts(BLUETOOTH_TERMINAL, "Current temperature is: ");
		xQueueReceive(tempQueue, &avrTemp, 0);
		sprintf(temp, "%f", avrTemp);
		USART_puts(BLUETOOTH_TERMINAL, temp);
		USART_puts(BLUETOOTH_TERMINAL, "\n");
		delay();
	}
}

int main(){
	tempQueue = xQueueCreate(1, sizeof(double));
	xTaskCreate(sendTemperature, "temp_send", 128, NULL, 1, NULL);
	xTaskCreate(calcAverageTemperature, "temp_calc", 128, NULL, 1, NULL);
	vTaskStartScheduler();
}
