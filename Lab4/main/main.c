#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "time.h"

#define NUM_TASK	3

QueueHandle_t Queue;
TaskHandle_t taskcreateQueue = NULL;
TaskHandle_t taskreceptionTask = NULL;
TaskHandle_t taskfunctionalTask1 = NULL;
TaskHandle_t taskfunctionalTask2 = NULL;
TaskHandle_t taskfunctionalTask3 = NULL;

int idTask1 = 0;
int idTask2 = 1;
int idTask3 = 2;

struct Data_t
{
	int DeviceID;
	uint32_t DataValue;
};

void queueCreate(void *pvParameter) {
	Queue = xQueueCreate(8, sizeof(struct Data_t));
	if (Queue == NULL)
		printf("Error: Fail to create queue!\n");
	else
		printf("Queue create successful!\n");
	vTaskDelete(NULL);
}

void receptionTask(void *pvParameter) {
	time_t t;
	srand((unsigned)time(&t));
	if (Queue == NULL)
		printf("Warning: Please create Queue!\n");
	else
		while(1) {
			struct Data_t *queueData = malloc(sizeof(struct Data_t));
			queueData->DeviceID = (int)(rand() % 3);
			queueData->DataValue = (uint32_t)(rand());
			if (xQueueSend(Queue, (void *) &queueData, portMAX_DELAY)){
				printf("Reception send data=0x%lx to device ID=%d to queue successful!\n", queueData->DataValue, queueData->DeviceID);
			}
			else
				printf("Send data to queue fail!\n");
			vTaskDelay(pdMS_TO_TICKS(2000));
		}
	vTaskDelete(NULL);
}

void functionalTask(void *pvParameter) {
	if (Queue == NULL)
		printf("Warning: Please create Queue!\n");
	else
		while(1) {
			struct Data_t *queueData;
			int *deviceID = (int *)pvParameter;
			if (xQueuePeek(Queue, &queueData, 10)) {
				if (queueData->DeviceID == *deviceID) {
					xQueueReceive(Queue, &queueData, 10);
					printf("Device: ID=%d receive data=0x%lx\n", queueData->DeviceID, queueData->DataValue);
					free(queueData);
				}
			}
			vTaskDelay(pdMS_TO_TICKS(10));
		}
	vTaskDelete(NULL);
}

void app_main(void)
{
	 xTaskCreate(&queueCreate, "Create Queue", 4096, NULL, 0, &taskcreateQueue);
	 xTaskCreate(&receptionTask, "Create Data and Push to Queue", 4096, NULL, 0, &taskreceptionTask);
	 xTaskCreate(&functionalTask, "Receive Data to Task 1", 4096, (void *)&idTask1, 0, &taskfunctionalTask1);
	 xTaskCreate(&functionalTask, "Receive Data to Task 2", 4096, (void *)&idTask2, 0, &taskfunctionalTask2);
	 xTaskCreate(&functionalTask, "Receive Data to Task 3", 4096, (void *)&idTask3, 0, &taskfunctionalTask3);
}
