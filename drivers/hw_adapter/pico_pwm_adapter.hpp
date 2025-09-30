#ifndef PICO_PWM_ADAPTER_HPP_INCLUDED
#define PICO_PWM_ADAPTER_HPP_INCLUDED

#include "hal_pwm.hpp"

class PicoPwmHal : public PwmHal {
    public:
        void init(unsigned int pin, unsigned int wrap, float clkDivider) override;

        void setLevel(unsigned int pin, uint16_t level) override;
};

#endif // PICO_PWM_ADAPTER_HPP_INCLUDED