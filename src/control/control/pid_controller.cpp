#include <cmath>
#include "pid_controller.hpp"

static constexpr float DEADZONE_COMP_VALUE = 0.00f;

PIDController::PIDController(float kp, float ki, float kd) {
    m_isInitialized = false;
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
    m_errorIntegrated = 0.0f;
    m_dt = 0.0f;
}

void PIDController::reset() {
    m_isInitialized = false;
}

float PIDController::run(const std::optional<float> pitchAngleRad, const float pitchOffsetRad, const float pitchAngleVelocityRadPerSec, const uint64_t systemTimeUs) {
    if (!pitchAngleRad.has_value() || std::abs(*pitchAngleRad) > MAX_PITCH_ANGLE_RAD_F) {
        reset();
        return 0.0f;
    }
    float error = pitchOffsetRad-(*pitchAngleRad);
    if (m_isInitialized) {
        float dt = (systemTimeUs - m_sysTimeLastUpdateUs) / 1e6f;
        m_errorIntegrated += error * dt;
        m_dt = dt;
    } else {
        m_errorIntegrated = 0.0f;
        m_isInitialized = true;
    }
    m_pPart = m_kp * error;
    m_iPart = m_ki * m_errorIntegrated;
    m_dPart = - m_kd * pitchAngleVelocityRadPerSec;
    m_sysTimeLastUpdateUs = systemTimeUs;

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

float PIDController::getDt() const {
    return m_dt;
}