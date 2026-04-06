#pragma once

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DISTANCE";

#ifdef __cplusplus
extern "C"
{
#endif

    void hcr04_init(gpio_num_t trig, gpio_num_t echo);
    float hcr04_get_value_c(gpio_num_t trig, gpio_num_t echo);
    float hcr04_get_value_f(gpio_num_t trig, gpio_num_t echo);

#ifdef __cplusplus
}

#endif