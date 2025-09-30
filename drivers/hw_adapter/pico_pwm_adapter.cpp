#include "pico_pwm_adapter.hpp"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

void PicoPwmHal::init(unsigned int pin, unsigned int wrap, float clkDivider) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, clkDivider);
    pwm_set_wrap(slice, wrap);
    pwm_set_enabled(slice, true);
}

void PicoPwmHal::setLevel(unsigned int pin, uint16_t level) {
    pwm_set_gpio_level(pin, level);
}