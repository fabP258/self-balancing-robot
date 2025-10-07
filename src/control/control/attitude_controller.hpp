#ifndef ATTITUDE_CONTROLLER_HPP_INCLUDED
#define ATTITUDE_CONTROLLER_HPP_INCLUDED

#include <optional>
#include <array>

constexpr size_t MAX_DEBUG_SIGNALS = 3;

using ControllerDebugArray = std::array<float, MAX_DEBUG_SIGNALS>;

class AttitudeController {
public:
    virtual ~AttitudeController() = default;

    virtual void reset() = 0;

    virtual float run(const std::optional<float> pitchAngleRad, const float pitchOffsetRad, const float pitchAngleVelocityRadPerSec) = 0;

    virtual size_t getDebugSignals(ControllerDebugArray& debugSignals) = 0;

};

#endif // ATTITUDE_CONTROLLER_HPP_INCLUDED