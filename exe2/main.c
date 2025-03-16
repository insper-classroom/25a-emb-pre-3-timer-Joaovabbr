#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;
volatile int g_timer_0 = 0;
volatile int g_timer_1 = 0;

bool timer_callback(repeating_timer_t *rt) {
    g_timer_0 = 1;
    return true;
}

bool timer_callback_1(repeating_timer_t *rt) {
    g_timer_1 = 1;
    return true;
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == BTN_PIN_R)
            flag_r = 1;
        else if (gpio == BTN_PIN_G)
            flag_g = 1;
    }
}

int main() {
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    int time_ms_1 = 500;
    int time_ms_2 = 250;
    repeating_timer_t timer_0;
    repeating_timer_t timer_1;
    bool controle_led_1 = false;
    bool controle_led_2 = false;

    while (true) {

        if (flag_r) {
            if (!controle_led_1) {
                if (add_repeating_timer_ms(time_ms_1, timer_callback, NULL, &timer_0)) {
                    controle_led_1 = true;
                    printf("Timer started\n");
                };
            } else {
                printf("Timer stopped\n");
                cancel_repeating_timer(&timer_0);
                controle_led_1 = false;
            }
            flag_r = 0;
        }

        if (flag_g) {
            if (!controle_led_2) {
                if (add_repeating_timer_ms(time_ms_2, timer_callback_1, NULL, &timer_1)) {
                    controle_led_2 = true;
                    printf("Timer started\n");
                };
            } else {
                printf("Timer stopped\n");
                cancel_repeating_timer(&timer_1);
                controle_led_2 = false;
            }
            flag_g = 0;
        }

        if (g_timer_0) {
            printf("Piscou\n");
            gpio_put(LED_PIN_R, 1);
            sleep_ms(100);
            gpio_put(LED_PIN_R, 0);
            g_timer_0 = 0;
        }

        if (g_timer_1) {
            printf("Piscou\n");
            gpio_put(LED_PIN_G, 1);
            sleep_ms(100);
            gpio_put(LED_PIN_G, 0);
            g_timer_1 = 0;
        }
    }
}
