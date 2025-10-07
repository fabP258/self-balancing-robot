#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include <string>
#include "hal_i2c.hpp"
#include "hal_pwm.hpp"
#include "imu_driver.hpp"
#include "h_bridge_driver.hpp"
#include "attitude_estimator.hpp"
#include "attitude_controller.hpp"

#define STATIC_PITCH_OFFSET 0.0189f

class Application {
    public:
        Application(ImuDriver& imuDriver,
                    HBridgeDriver& motorLeft,
                    HBridgeDriver& motorRight,
                    AttitudeEstimator& estimator,
                    AttitudeController& controller);

        void loop(const uint64_t &sysTimeUs);

        std::string getLogString(const uint64_t &sysTimeUs);

    private:
        ImuDriver& m_imu;
        HBridgeDriver& m_motorLeft;
        HBridgeDriver& m_motorRight;
        AttitudeEstimator& m_estimator;
        AttitudeController& m_controller;
        std::optional<std::array<float,3>> m_accel;
        std::optional<std::array<float,3>> m_gyro;
        std::optional<float> m_speedCommand;
};

#endif // APP_HPP_INCLUDED