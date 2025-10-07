#ifndef PID_CONTROLLER_HPP_INCLUDED
#define PID_CONTROLLER_HPP_INCLUDED

#include "attitude_controller.hpp"

#define MAX_PITCH_ANGLE_RAD_F 0.52f

class PIDController : public AttitudeController {
    public:
        PIDController(const float kp, const float ki, const float kd, const float m_cycleTimeSec);

        void reset() override;

        float run(const std::optional<float> pitchAngleRad, const float pitchOffsetRad, const float pitchAngleVelocityRadPerSec) override;

        size_t getDebugSignals(ControllerDebugArray& debugSignals) override;

        float getPPart() const;
        float getIPart() const;
        float getDPart() const;

    private:
        const float m_cycleTimeSec;
        std::optional<float> m_errorIntegrated;
        float m_kp;
        float m_ki;
        float m_kd;
        float m_pPart;
        float m_iPart;
        float m_dPart;

        float compensateDeadzone(float command) const;
};

#endif // PID_CONTROLLER_HPP_INCLUDED