#include "app.hpp"

Application::Application(ImuDriver& imuDriver,
                        HBridgeDriver& motorLeft,
                        HBridgeDriver& motorRight,
                        AttitudeEstimator& estimator,
                        AttitudeController& controller) : 
    m_imu(imuDriver), m_motorLeft(motorLeft), m_motorRight(motorRight),
    m_estimator(estimator), m_controller(controller) { 
        m_imu.calibrateGyro();
}

void Application::loop(const uint64_t &sysTimeUs) {
    m_imu.readAccel(m_accel);
    m_imu.readGyro(m_gyro, true);
    m_speedCommand.reset();
    if (m_accel.has_value() && m_gyro.has_value()) {
        m_estimator.update(*m_accel, *m_gyro, sysTimeUs);
        m_speedCommand = m_controller.run(m_estimator.getRollAngle(), (*m_gyro)[0], sysTimeUs);
        // TODO: logging
    } else {
        m_estimator.reset();
        m_controller.reset();
    }
    m_motorLeft.writeSpeed(m_speedCommand.value_or(0.0f));
    m_motorRight.writeSpeed(-m_speedCommand.value_or(0.0f));
}