#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/projdefs.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "sys/time.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "esp_log.h"

#define NUM_TIMERS 2
#define TIMER_ID_1 0
#define TIMER_ID_2 1

TaskHandle_t taskcreateTimer = NULL;
TimerHandle_t xTimers[NUM_TIMERS];
uint32_t ulCount1 = 0;
uint32_t ulCount2 = 0;

static const char *timer1_message = "ahihi";
static const char *timer2_message = "ihaha";

static const int timer_period[NUM_TIMERS] = {2000, 3000};

void getTime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	printf("Time: %lld ", tv.tv_sec);
}

void vTimerCallback(TimerHandle_t xTimer) {
	const uint32_t ulMaxExpiryCountBeforeStopping_timer1 = 10;
	const uint32_t ulMaxExpiryCountBeforeStopping_timer2 = 5;
	uint32_t timer_ID = (uint32_t)pvTimerGetTimerID(xTimer);
	if(timer_ID == TIMER_ID_1) {
		getTime();
		printf("Message: %s\r\n", timer1_message);
		ulCount1++;
		if(ulCount1 >= ulMaxExpiryCountBeforeStopping_timer1) {
			xTimerStop(xTimer, 0);
		}
	}
	else if(timer_ID == TIMER_ID_2) {
		getTime();
		printf("Message: %s\r\n", timer2_message);
		ulCount2++;
		if(ulCount2 >= ulMaxExpiryCountBeforeStopping_timer2) {
			xTimerStop(xTimer, 0);
		}
	}
}

void vTask_CreateTimer(void) {
	for(int i = 0; i < NUM_TIMERS; i++) {
		xTimers[i] = xTimerCreate("Timer", pdMS_TO_TICKS(timer_period[i]), pdTRUE, (void *) i, vTimerCallback);
		if(xTimers[i] == NULL) {
			printf("Timer %d failed to be created\n", i);
		}
		else {
			if(xTimerStart(xTimers[i], 0) != pdPASS)
				printf("Timer %d failed to start\n", i);
		}
	}
	vTaskDelete(NULL);
}

void app_main(void)
{
	xTaskCreate(&vTask_CreateTimer, "Create Timer", 4096, NULL, 0, &taskcreateTimer);
}
