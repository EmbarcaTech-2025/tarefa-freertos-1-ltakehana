#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12
#define BUZZER_PIN 21
#define BUTTON_A 5
#define BUTTON_B 6

TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t buzzerTaskHandle = NULL;

void led_task(void *params) {
    while (1) {
        gpio_put(LED_R, 1); gpio_put(LED_G, 0); gpio_put(LED_B, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_put(LED_R, 0); gpio_put(LED_G, 1); gpio_put(LED_B, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_put(LED_R, 0); gpio_put(LED_G, 0); gpio_put(LED_B, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void buzzer_init_pwm() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); 
    pwm_init(slice_num, &config, false);

    pwm_set_wrap(slice_num, 500);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER_PIN), 250); 
    pwm_set_enabled(slice_num, false);
}

void buzzer_task(void *params) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    while (1) {
        pwm_set_enabled(slice_num, true);   
        vTaskDelay(pdMS_TO_TICKS(100));
        pwm_set_enabled(slice_num, false);  
        vTaskDelay(pdMS_TO_TICKS(900));
    }
}

void button_task(void *params) {
    bool ledSuspended = false, buzzerSuspended = false;
    while (1) {
        if (!gpio_get(BUTTON_A)) {
            if (!ledSuspended) {
                vTaskSuspend(ledTaskHandle);
                ledSuspended = true;
            } else {
                vTaskResume(ledTaskHandle);
                ledSuspended = false;
            }
            vTaskDelay(pdMS_TO_TICKS(300)); 
        }

        if (!gpio_get(BUTTON_B)) {
            if (!buzzerSuspended) {
                vTaskSuspend(buzzerTaskHandle);
                buzzerSuspended = true;
            } else {
                vTaskResume(buzzerTaskHandle);
                buzzerSuspended = false;
            }
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B); gpio_set_dir(LED_B, GPIO_OUT);
    gpio_init(BUTTON_A); gpio_set_dir(BUTTON_A, GPIO_IN); gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B); gpio_set_dir(BUTTON_B, GPIO_IN); gpio_pull_up(BUTTON_B);

    buzzer_init_pwm();

    xTaskCreate(led_task, "LED Task", 256, NULL, 1, &ledTaskHandle);
    xTaskCreate(buzzer_task, "Buzzer Task", 256, NULL, 1, &buzzerTaskHandle);
    xTaskCreate(button_task, "Button Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();
    while (true) {}
}
