#ifndef ATTITUDE_ESTIMATOR_HPP_INCLUDED
#define ATTITUDE_ESTIMATOR_HPP_INCLUDED

#include <array>
#include <cstdint>
#include <optional>

class AttitudeEstimator {
    public:
        virtual ~AttitudeEstimator() = default;

        virtual void update(const std::array<float,3> &accel, const std::array<float,3> &gyro) = 0;

        virtual void reset() = 0;

        virtual std::optional<std::array<float,3>> getEulerAngles() const = 0;

        virtual std::optional<float> getRollAngle() const = 0;

        virtual std::optional<float> getPitchAngle() const = 0;

        virtual std::optional<float> getYawAngle() const = 0;
};

#endif // ATTITUDE_ESTIMATOR_HPP_INCLUDED