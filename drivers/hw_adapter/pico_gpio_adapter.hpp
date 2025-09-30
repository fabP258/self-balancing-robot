#ifndef PICO_GPIO_ADAPTER_HPP_INCLUDED
#define PICO_GPIO_ADAPTER_HPP_INCLUDED

#include "hal_gpio.hpp"

class PicoGpioHal : public GpioHal {
    public:
        void setDir(unsigned int pin, bool isOutput) override;
        void write(unsigned int pin, bool value) override;
};

#endif // PICO_GPIO_ADAPTER_HPP_INCLUDED