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
volatile uint32_t ulTask2CycleCount = 0UL;

EventGroupHandle_t event_group;
const int trigger_Task1 = BIT0;
int flag = 0;

void vApplicationIdleHook ( void )
{
	ulIdleCycleCount++;
	if (ulIdleCycleCount % 1000 == 0) {
		printf("Task Idle\n");
		xEventGroupSetBits(event_group, trigger_Task1);
	}
}

void vTask2(void *params){
	while(1){
		ulTask2CycleCount++;
		if (flag == 1 && ulTask2CycleCount % 1000 == 0) {
			printf("Task 2\n");
			flag = 0;
			xEventGroupSetBits(event_group, trigger_Task1);
		}
	}
}

void vTask1(void *params){
	while(1){
		xEventGroupWaitBits(event_group, trigger_Task1, true, true, portMAX_DELAY);
		flag = 1;
		printf("Task 1 is running\n");
	}
}

void app_main(void)
{
	event_group = xEventGroupCreate();
    xTaskCreatePinnedToCore(vTask2, "Task 2", 4096, NULL, 0, &Task2, 0);
    xTaskCreatePinnedToCore(vTask1, "Task 1", 4096, NULL, 1, &Task1, 0);
}
