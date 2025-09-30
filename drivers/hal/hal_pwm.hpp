#ifndef HAL_PWM_HPP_INCLUDED
#define HAL_PWM_HPP_INCLUDED

#include <cstdint>

class PwmHal {
    public:
        virtual ~PwmHal() = default;

        virtual void init(unsigned int pin, unsigned int wrap, float clkDivider) = 0;

        virtual void setLevel(unsigned int pin, uint16_t level) = 0;
};

#endif // HAL_PWM_HPP_INCLUDED