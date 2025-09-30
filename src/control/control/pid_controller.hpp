#ifndef PID_CONTROLLER_HPP_INCLUDED
#define PID_CONTROLLER_HPP_INCLUDED

#include "attitude_controller.hpp"

#define MAX_PITCH_ANGLE_RAD_F 0.52f

class PIDController : public AttitudeController {
    public:
        PIDController(float kp, float ki, float kd);

        void reset() override;

        float run(const std::optional<float> pitchAngleRad, const float pitchAngleVelocityRadPerSec, const uint64_t systemTimeUs) override;

        float getPPart() const;
        float getIPart() const;
        float getDPart() const;
        float getDt() const;

    private:
        bool m_isInitialized;
        float m_errorIntegrated;
        float m_kp;
        float m_ki;
        float m_kd;
        float m_pPart;
        float m_iPart;
        float m_dPart;
        float m_dt;

        float compensateDeadzone(float command) const;
};

#endif // PID_CONTROLLER_HPP_INCLUDED