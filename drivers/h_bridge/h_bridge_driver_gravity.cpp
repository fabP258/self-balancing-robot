#include "h_bridge_driver_gravity.hpp"
#include <algorithm>
#include <cmath>

static constexpr uint32_t NEUTRAL_PULSE_US = 1500u;
static constexpr uint32_t MIN_PULSE_US     = 500u;
static constexpr uint32_t MAX_PULSE_US     = 2500u;

HBridgeDriverGravity::HBridgeDriverGravity(uint32_t pwmPin, PwmHal& pwmHal) 
    : m_pwmPin(pwmPin), m_pwmHal(pwmHal) {
        // configure PWM such that 1 clock tick == 1 microsecond
        const float clkDiv = static_cast<float>(PICO_2_CLOCK_FREQ_HZ) / 1'000'000.0f;
        const uint32_t wrap = (1000000u / PWM_FREQ_HZ) - 1u;
        m_pwmHal.init(pwmPin, wrap, clkDiv);
        m_pwmWrap = wrap;
}

uint32_t HBridgeDriverGravity::speedToPulseWidth(float speed) {
    if (!std::isfinite(speed)) speed = 0.0f;
    speed = std::clamp(speed, -1.0f, 1.0f);

    float pulseWidthF = static_cast<float>(NEUTRAL_PULSE_US);
    if (std::abs(speed) < SPEED_SYMMETRIC_DEADZONE) {
        /* do nothing */
    } else if (speed <= -SPEED_SYMMETRIC_DEADZONE) {
        pulseWidthF = 900.0f * speed + 1400.0f;
    } else {
        pulseWidthF = 900.0f * speed + 1600.0f;
    }

    return static_cast<uint32_t>(std::clamp(static_cast<int32_t>(std::lround(pulseWidthF)), 
                                            static_cast<int32_t>(MIN_PULSE_US),
                                            static_cast<int32_t>(MAX_PULSE_US)));
}

void HBridgeDriverGravity::writeSpeed(float speed) {
    uint32_t pulseWidthUs = speedToPulseWidth(speed);
    m_pwmHal.setLevel(m_pwmPin, pulseWidthUs);
}