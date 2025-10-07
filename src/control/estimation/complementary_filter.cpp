#include <cmath>
#include "complementary_filter.hpp"

void ComplementaryFilter::update(const std::array<float,3> &accel, const std::array<float,3> &gyro) {

    float angle_rad_x = std::atan2(accel[1], accel[2]);
    float angle_rad_y = std::atan2(-accel[0], std::sqrt(accel[1] * accel[1] + accel[2] * accel[2]));
    
    if (m_eulerAngles.has_value()) {
        std::array<float, 3> &angles = *m_eulerAngles;
        angles[0] = m_alpha * (angles[0] + gyro[0] * m_cycleTimeSec) + (1.0f - m_alpha) * angle_rad_x;
        angles[1] = m_alpha * (angles[1] + gyro[1] * m_cycleTimeSec) + (1.0f - m_alpha) * angle_rad_y;
        angles[2] += gyro[2] * m_cycleTimeSec;
    } else {
        m_eulerAngles.emplace(std::array<float,3>{angle_rad_x, angle_rad_y, 0.0f});
    }
}

void ComplementaryFilter::reset() {
    m_eulerAngles.reset();
}

std::optional<std::array<float,3>> ComplementaryFilter::getEulerAngles() const {
    return m_eulerAngles;
}

std::optional<float> ComplementaryFilter::getRollAngle() const {
    if (m_eulerAngles.has_value()) {
        return (*m_eulerAngles)[0];
    }
    return std::nullopt;
}

std::optional<float> ComplementaryFilter::getPitchAngle() const {
    if (m_eulerAngles.has_value()) {
        return (*m_eulerAngles)[1];
    }
    return std::nullopt;
}

std::optional<float> ComplementaryFilter::getYawAngle() const {
    if (m_eulerAngles.has_value()) {
        return (*m_eulerAngles)[2];
    }
    return std::nullopt;
}