#include <cmath>
#include "pid_controller.hpp"

static constexpr float DEADZONE_COMP_VALUE = 0.00f;

PIDController::PIDController(const float kp, const float ki, const float kd, const float cycleTimeSec) 
    : m_cycleTimeSec(cycleTimeSec), m_kp(kp), m_ki(ki), m_kd(kd) {}

void PIDController::reset() {
    m_errorIntegrated.reset();
}

float PIDController::run(const std::optional<float> pitchAngleRad, const float pitchOffsetRad, const float pitchAngleVelocityRadPerSec) {
    if (!pitchAngleRad.has_value() || std::abs(*pitchAngleRad) > MAX_PITCH_ANGLE_RAD_F) {
        reset();
        return 0.0f;
    }
    float error = pitchOffsetRad-(*pitchAngleRad);
    if (m_errorIntegrated.has_value()) {
        m_errorIntegrated = (*m_errorIntegrated) + error * m_cycleTimeSec;
    } else {
        m_errorIntegrated = 0.0f;
    }
    m_pPart = m_kp * error;
    m_iPart = m_ki * (*m_errorIntegrated);
    m_dPart = - m_kd * pitchAngleVelocityRadPerSec;

    return compensateDeadzone(m_pPart + m_iPart + m_dPart);
}

float PIDController::compensateDeadzone(float command) const {
    if (command > 0) {
        command += DEADZONE_COMP_VALUE;
    } else if (command < 0) {
        command -= DEADZONE_COMP_VALUE;
    }
    return command;
}

size_t PIDController::getDebugSignals(ControllerDebugArray& debugSignals) {
    debugSignals[0] = m_pPart;
    debugSignals[1] = m_iPart;
    debugSignals[2] = m_dPart;
    return 3;
}

float PIDController::getPPart() const {
    return m_pPart;
}

float PIDController::getIPart() const {
    return m_iPart;
}

float PIDController::getDPart() const {
    return m_dPart;
}