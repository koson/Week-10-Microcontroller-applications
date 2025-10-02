#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED1_GPIO GPIO_NUM_25
#define LED2_GPIO GPIO_NUM_26

void app_main(void){
    gpio_config_t out = {
        .pin_bit_mask = (1ULL<<LED1_GPIO)|(1ULL<<LED2_GPIO),
        .mode=GPIO_MODE_OUTPUT
    };
    gpio_config(&out);

    while(1){
        gpio_set_level(LED1_GPIO, 1);
        gpio_set_level(LED2_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED1_GPIO, 0);
        gpio_set_level(LED2_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
