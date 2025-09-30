#ifndef COMPLEMENTARY_FILTER_HPP_INCLUDED
#define COMPLEMENTARY_FILTER_HPP_INCLUDED

#include "attitude_estimator.hpp"

class ComplementaryFilter : public AttitudeEstimator {
    public:
        ComplementaryFilter(float alpha = 0.98f) : m_alpha(alpha) {}

        void update(const std::array<float,3> &accel, const std::array<float,3> &gyro, const uint64_t systemTimeUs) override;

        void reset();
        
        std::optional<std::array<float,3>> getEulerAngles() const;

        std::optional<float> getRollAngle() const;

        std::optional<float> getPitchAngle() const;

        std::optional<float> getYawAngle() const;
    private:
        float m_alpha;
        std::optional<uint64_t> m_sysTimeLastUpdateUs;
        std::optional<std::array<float,3>> m_eulerAngles{};
};

#endif // COMPLEMENTARY_FILTER_HPP_INCLUDED