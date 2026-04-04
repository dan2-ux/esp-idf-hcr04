#include "setup.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define trig GPIO_NUM_5
#define echo GPIO_NUM_18

void distanceDisplay(void *arg)
{
    while (1)
    {
        printf("Distance: %f C\n", hcr04_get_value_c(trig, echo));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

extern "C" void app_main()
{
    hcr04_init(trig, echo);
    xTaskCreatePinnedToCore(distanceDisplay, "displaying distance", 2048, NULL, 5, NULL, 1);
}
