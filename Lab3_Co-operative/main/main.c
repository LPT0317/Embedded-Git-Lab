#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

TaskHandle_t Task1 = NULL;
TaskHandle_t Task2 = NULL;
TaskHandle_t Task3 = NULL;

volatile uint32_t Task3Count = 0UL;
SemaphoreHandle_t xSemaphore;
QueueHandle_t Queue;

void vTask2(void *params){
	while(1){
		int *queueData;
		if (xQueueReceive(Queue, &queueData, 10)) {
			printf("Task 2 receive data=%d\n", *queueData);
			Task3Count = 0UL;
			free(queueData);
		}
	}
}

void vTask1(void *params){
	while(1){
		if (xSemaphoreTake(xSemaphore, 10)) {
			printf("Task 1 is running\n");
		}
	}
}

void vTask3(void *params){
	while(1){
		while(Task3Count < 5000) {
			if (Task3Count == 2000) {
				 int *queueData = malloc(sizeof(int));
				 *queueData = 2010665;
				 if(xQueueSend(Queue, (void *) &queueData, portMAX_DELAY))
					 printf("Task 3 write to queue successful!\n");
			}
			if (Task3Count == 4000) {
				xSemaphoreGive(xSemaphore);
				 printf("Task 3 write to semaphore successful!\n");
			}
			Task3Count++;
		}
		printf("Task 3 Count:%lu\n", Task3Count);
		taskYIELD();
	}
}

void app_main(void)
{
	xSemaphore = xSemaphoreCreateBinary();
	Queue = xQueueCreate(8, sizeof(int));
    xTaskCreatePinnedToCore(vTask3, "Task 3", 4096, NULL, 0, &Task3, 0);
    xTaskCreatePinnedToCore(vTask2, "Task 2", 4096, NULL, 1, &Task2, 0);
    xTaskCreatePinnedToCore(vTask1, "Task 1", 4096, NULL, 2, &Task1, 0);
}
