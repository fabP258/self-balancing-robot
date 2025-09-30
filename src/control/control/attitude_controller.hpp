#ifndef ATTITUDE_CONTROLLER_HPP_INCLUDED
#define ATTITUDE_CONTROLLER_HPP_INCLUDED

#include <cstdint>
#include <optional>

class AttitudeController {
public:
    virtual ~AttitudeController() = default;

    virtual void reset() = 0;

    virtual float run(const std::optional<float> pitchAngleRad, const float pitchAngleVelocityRadPerSec, const uint64_t systemTimeUs) = 0;

protected:
    uint64_t m_sysTimeLastUpdateUs;
};

#endif // ATTITUDE_CONTROLLER_HPP_INCLUDED