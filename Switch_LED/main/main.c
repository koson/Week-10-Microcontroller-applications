#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BUTTON_GPIO  GPIO_NUM_15   // ปุ่มที่ GPIO15 (active LOW เมื่อกด)
#define LED_GPIO     GPIO_NUM_2    // LED บนบอร์ด (เปลี่ยนได้ตามบอร์ดของคุณ)
#define DEBOUNCE_MS  20            // หน่วงกันเด้งปุ่ม ~20ms

void app_main(void)
{
    // ตั้งค่า LED เป็นเอาต์พุต
    gpio_config_t io_conf_led = {
        .pin_bit_mask = 1ULL << LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf_led);
    gpio_set_level(LED_GPIO, 0); // เริ่มต้น LED ดับ

    // ตั้งค่าปุ่มเป็นอินพุต + pull-up ภายใน
    gpio_config_t io_conf_btn = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,   // ดึงขึ้นภายใน
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf_btn);

    int last_state = 1;     // เริ่ม HIGH (ไม่ได้กด)
    int stable_state = 1;
    TickType_t last_change = xTaskGetTickCount();

    while (1) {
        int level = gpio_get_level(BUTTON_GPIO);

        // ตรวจจับการเปลี่ยน เพื่อเริ่มจับเวลาหน่วงเด้ง
        if (level != last_state) {
            last_state = level;
            last_change = xTaskGetTickCount();
        }

        // ถ้าค่าอยู่คงที่เกิน DEBOUNCE_MS ถือว่าเสถียร
        if ((xTaskGetTickCount() - last_change) * portTICK_PERIOD_MS >= DEBOUNCE_MS) {
            if (level != stable_state) {
                stable_state = level;
                // ปุ่ม active LOW: กด = 0 → LED ติด, ปล่อย = 1 → LED ดับ
                gpio_set_level(LED_GPIO, (stable_state == 0) ? 1 : 0);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
