#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;
volatile int g_timer_0 = 0;

bool timer_callback(repeating_timer_t *rt) {
    g_timer_0 = 1;
    return true;
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        if (gpio == BTN_PIN_R)
            flag_f_r = 1;

    } else if (events == 0x8) { // rise edge
        if (gpio == BTN_PIN_R)
            flag_f_r = 0;
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);
    
    bool apertou = false;
    uint32_t time_diff = 0;
    uint32_t time_start = 0;

    while (true) {
        while(flag_f_r == 1) {
            if (!apertou){
                time_start = get_absolute_time();
                apertou = true;
            }
            time_diff = get_absolute_time() - time_start;
        }
        apertou = false;
        if (time_diff > 500000){
            gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
            time_diff = 0;
        }
        
    }
}
