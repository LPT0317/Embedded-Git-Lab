#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/event_groups.h"

TaskHandle_t Task1 = NULL;
TaskHandle_t Task2 = NULL;

volatile uint32_t ulIdleCycleCount = 0UL;

EventGroupHandle_t event_group;
const int trigger_Task1 = BIT0;
int flag = 0;

void vApplicationIdleHook ( void )
{
	ulIdleCycleCount++;
	flag = 1;
	if (ulIdleCycleCount == 1000)
		xEventGroupSetBits(event_group, trigger_Task1);
}

void vTask2(void *params){
	while(1){
		if (flag == 1 && ulIdleCycleCount % 100 == 0) {
			printf("Idle Count: %lu\n", ulIdleCycleCount);
			flag = 0;
		}
	}
}

void vTask1(void *params){
	while(1){
		xEventGroupWaitBits(event_group, trigger_Task1, true, true, portMAX_DELAY);
		printf("Task 1 is running\n");
	}
}

void app_main(void)
{
	event_group = xEventGroupCreate();
    xTaskCreatePinnedToCore(vTask2, "Task 2", 4096, NULL, 0, &Task2, 0);
    xTaskCreatePinnedToCore(vTask1, "Task 1", 4096, NULL, 1, &Task1, 0);
}
