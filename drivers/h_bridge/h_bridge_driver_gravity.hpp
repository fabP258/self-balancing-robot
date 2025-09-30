#ifndef H_BRIDGE_DRIVER_GRAVITY_HPP_INCLUDED
#define H_BRIDGE_DRIVER_GRAVITY_HPP_INCLUDED

#include <cstdint>
#include "h_bridge_driver.hpp"
#include "hal_pwm.hpp"

#define PWM_FREQ_HZ 50u
#define PICO_2_CLOCK_FREQ_HZ 150000000u
#define SPEED_SYMMETRIC_DEADZONE 0.005f

class HBridgeDriverGravity : public HBridgeDriver {
    public:
        HBridgeDriverGravity(uint32_t pwmPin, PwmHal& pwmHal);

        void writeSpeed(float speed) override;

    private:
        uint32_t speedToPulseWidth(float speed);

        uint32_t m_pwmPin;
        uint32_t m_pwmWrap;
        PwmHal& m_pwmHal;
};

#endif // H_BRIDGE_DRIVER_GRAVITY_HPP_INCLUDED