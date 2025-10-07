#ifndef COMPLEMENTARY_FILTER_HPP_INCLUDED
#define COMPLEMENTARY_FILTER_HPP_INCLUDED

#include "attitude_estimator.hpp"

class ComplementaryFilter : public AttitudeEstimator {
    public:
        ComplementaryFilter(const float cycleTimeSec, float alpha = 0.98f) : m_cycleTimeSec(cycleTimeSec), m_alpha(alpha) {}

        void update(const std::array<float,3> &accel, const std::array<float,3> &gyro) override;

        void reset();
        
        std::optional<std::array<float,3>> getEulerAngles() const;

        std::optional<float> getRollAngle() const;

        std::optional<float> getPitchAngle() const;

        std::optional<float> getYawAngle() const;
    private:
        const float m_cycleTimeSec;
        float m_alpha;
        std::optional<std::array<float,3>> m_eulerAngles{};
};

#endif // COMPLEMENTARY_FILTER_HPP_INCLUDED