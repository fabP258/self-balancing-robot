#include "pico_gpio_adapter.hpp"
#include "hardware/gpio.h"

void PicoGpioHal::setDir(unsigned int pin, bool isOutput) {
    gpio_init(pin);
    gpio_set_dir(pin, isOutput);
}

void PicoGpioHal::write(unsigned int pin, bool value) {
    gpio_put(pin, value);
}