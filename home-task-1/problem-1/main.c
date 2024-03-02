#include <stdio.h>

#include "board.h"
#include "periph_conf.h"

#include "periph/gpio.h"
#include "periph/gpio_ll_irq.h"
#include "xtimer.h"

static bool led_state = 0;

void problem_1_i(gpio_t led_pin, gpio_t btn_pin);
void problem_1_ii(gpio_t led_pin, gpio_t btn_pin);
static void button_is_pressed(void *arg);

static void led_toggle_on_click(gpio_t led_pin, gpio_t btn_pin);
static void led_state_handler(gpio_t led_pin);

int main(void) {
    problem_1_ii(LED0_PIN, BTN0_PIN);

    return 0;
}

void problem_1_i(gpio_t led_pin, gpio_t btn_pin)
{
    gpio_init(led_pin, GPIO_OUT);
    gpio_init(btn_pin, GPIO_IN_PD);

    while (1) 
    {
        led_toggle_on_click(led_pin, btn_pin);
    }
}

static void led_toggle_on_click(gpio_t led_pin, gpio_t btn_pin)
{
    static bool new_btn_state = 0;
    static bool prev_btn_state = 0;

    new_btn_state = gpio_read(btn_pin) > 0 ? 1 : 0;
    if (new_btn_state == 1 && prev_btn_state == 0)
    {
        gpio_toggle(led_pin);
    }
    prev_btn_state = new_btn_state;
}

void problem_1_ii(gpio_t led_pin, gpio_t btn_pin)
{
    gpio_init(led_pin, GPIO_OUT);
    gpio_init(btn_pin, GPIO_IN_PD);
    gpio_init_int(btn_pin, GPIO_IN, GPIO_RISING, button_is_pressed, NULL);
    gpio_irq_enable(btn_pin);

    while (1)
    {
        led_state_handler(led_pin);
    }
}

static void button_is_pressed(void *arg)
{
    static uint32_t last_time = 0;
    static uint32_t short_click_counter = 0;

    gpio_irq_disable(BTN0_PIN);
    puts(arg);

    if (xtimer_now_usec() - last_time > 150)
    {
        (short_click_counter % 2 == 0) ? (led_state = 1) : (led_state = 0);
        short_click_counter++;
        last_time = xtimer_now_usec();
    }

    gpio_irq_enable(BTN0_PIN);
}

static void led_state_handler(gpio_t led_pin)
{
    static uint32_t led_last_time = 0;

    if (led_state == 1 && xtimer_now_usec() - led_last_time > 500000)
    {
        gpio_toggle(led_pin);
        led_last_time = xtimer_now_usec();
    }
    else if (led_state == 0)
    {
        gpio_clear(led_pin);
    }
}