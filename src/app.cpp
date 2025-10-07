#include <sstream>
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

void Application::loop() {
    m_imu.readAccel(m_accel);
    m_imu.readGyro(m_gyro, true);
    m_speedCommand.reset();
    if (m_accel.has_value() && m_gyro.has_value()) {
        m_estimator.update(*m_accel, *m_gyro);
        m_speedCommand = m_controller.run(m_estimator.getRollAngle(), STATIC_PITCH_OFFSET, (*m_gyro)[0]);
    } else {
        m_estimator.reset();
        m_controller.reset();
        m_accel.reset();
        m_gyro.reset();
    }
    m_motorLeft.writeSpeed(m_speedCommand.value_or(0.0f));
    m_motorRight.writeSpeed(-m_speedCommand.value_or(0.0f));
}

std::string Application::getLogString(const uint64_t &sysTimeUs) {
    auto eulerAngles = m_estimator.getEulerAngles();
    if (!m_accel || !m_gyro || !eulerAngles) {
        return {};
    }
    std::array<float,3>& accel = *m_accel;
    std::array<float,3>& gyro = *m_gyro;
    std::ostringstream oss;
    oss << sysTimeUs << ",";
    oss << accel[0] << ",";
    oss << accel[1] << ",";
    oss << accel[2] << ",";
    oss << gyro[0] << ",";
    oss << gyro[1] << ",";
    oss << gyro[2] << ",";
    oss << (*eulerAngles)[0] << ",";
    oss << (*eulerAngles)[1] << ",";
    oss << (*eulerAngles)[2] << ",";
    ControllerDebugArray debugSignals{};
    size_t nControllerSignals = m_controller.getDebugSignals(debugSignals);
    for (unsigned int i = 0; i < nControllerSignals; i++) {
        oss << debugSignals[i];
        if (i == nControllerSignals-1) {
            break;
        }
        oss << ",";
    }
    oss << "\n";
    return oss.str();
}