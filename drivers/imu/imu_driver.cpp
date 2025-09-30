#include "imu_driver.hpp"

int ImuDriver::calibrateGyro() {
    std::array<float,3> sum{};
    std::optional<std::array<float,3>> rates{};

    int numSamples = IMU_NUM_GYRO_CALIB_SAMPLES;
    for (int i = 0; i < IMU_NUM_GYRO_CALIB_SAMPLES; i++) {
        readGyro(rates, false);
        if (!rates.has_value()) {
            numSamples--;
            continue;
        }
        sum[0] += rates->at(0);
        sum[1] += rates->at(1);
        sum[2] += rates->at(2);
        // TODO: sleep
    }

    m_gyroOffsets[0] = sum[0] / IMU_NUM_GYRO_CALIB_SAMPLES;
    m_gyroOffsets[1] = sum[1] / IMU_NUM_GYRO_CALIB_SAMPLES;
    m_gyroOffsets[2] = sum[2] / IMU_NUM_GYRO_CALIB_SAMPLES;

    return 0;
}