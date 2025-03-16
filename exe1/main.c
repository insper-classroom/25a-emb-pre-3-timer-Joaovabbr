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
        flag_f_r = 1;
    } else if (events == 0x8) { // rise edge
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);

    repeating_timer_t timer_0;
    bool controle_led = false;
    

    while (true) {
        if (flag_f_r) {
            if (!controle_led){
                if (add_repeating_timer_us(500000, timer_callback, NULL, &timer_0)){
                    controle_led = true;
                };
                
            }else{
                cancel_repeating_timer(&timer_0);
                controle_led = false;
            }
            flag_f_r = 0;
        }
        if (g_timer_0) {
            printf("Piscou\n");
            gpio_put(LED_PIN_R, 1);
            sleep_ms(150);
            gpio_put(LED_PIN_R, 0);
            g_timer_0 = 0;
        }
    }
}
