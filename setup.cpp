#include "setup.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace HCR04
{
    class setup
    {
    public:
        setup(gpio_num_t t, gpio_num_t e) : trig(t), echo(e) {}

        void Setup()
        {
            gpio_reset_pin(trig);
            gpio_reset_pin(echo);

            gpio_set_direction(echo, GPIO_MODE_INPUT);
            gpio_set_direction(trig, GPIO_MODE_OUTPUT);
        }

    private:
        gpio_num_t trig;
        gpio_num_t echo;
    };

    class Measure
    {
    public:
        Measure(gpio_num_t t, gpio_num_t e) : trig(t), echo(e) {}

        float getValue_C()
        {

            gpio_set_level(trig, 0);
            esp_rom_delay_us(2);
            gpio_set_level(trig, 1);
            esp_rom_delay_us(10);
            gpio_set_level(trig, 0);

            int64_t gpioStart = esp_timer_get_time();
            while (gpio_get_level(echo) == 0)
            {
                if (esp_timer_get_time() - gpioStart >= 30000)
                {
                    ESP_LOGE(TAG, "Durantion reach at echo high");
                    return -1;
                }
            }

            int64_t echoStart = esp_timer_get_time();
            while (gpio_get_level(echo) == 1)
            {
                if (esp_timer_get_time() - echoStart >= 30000)
                {
                    ESP_LOGE(TAG, "Duration reach at echo low");
                    return -1;
                }
            }

            int64_t echoEnd = esp_timer_get_time();
            float res = ((echoEnd - echoStart) * 0.0343) / 2;

            if (res >= 0)
            {
                return res;
            }
            else
            {
                return -1;
            }
        }

    private:
        gpio_num_t trig;
        gpio_num_t echo;
    };
};

void hcr04_init(gpio_num_t trig, gpio_num_t echo)
{
    HCR04::setup st(trig, echo);
    st.Setup();
}

float hcr04_get_value_c(gpio_num_t trig, gpio_num_t echo)
{
    HCR04::Measure m(trig, echo);
    return m.getValue_C();
}

float hcr04_get_value_f(gpio_num_t trig, gpio_num_t echo)
{
    HCR04::Measure m(trig, echo);
    return (m.getValue_C() * 9 / 5) + 32;
}