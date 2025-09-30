#ifndef MPU6050_DRIVER_HPP_INCLUDED
#define MPU6050_DRIVER_HPP_INCLUDED

#include <stdint.h>
#include "imu_driver.hpp"
#include "hal_i2c.hpp"

#define MPU6050_RA_PWR_MGMT_1 0x6B
#define MPU6050_RA_SMPRT_DIV 0x19
#define MPU6050_RA_CONFIG 0x1a
#define MPU6050_DLPF_MASK 0x07u
#define MPU6050_DLPF_SHIFT 0
#define MPU6050_RA_GYRO_CONFIG 0x1b
#define MPU6050_GYRO_FS_MASK 0x18u
#define MPU6050_GYRO_FS_SHIFT 3
#define MPU6050_RA_ACCEL_CONFIG 0x1c
#define MPU6050_ACCEL_FS_MASK 0x18u
#define MPU6050_ACCEL_FS_SHIFT 3

#define MPU6050_RA_ACCEL_XOUT_L 0x3c
#define MPU6050_RA_ACCEL_XOUT_H 0x3b
#define MPU6050_RA_ACCEL_YOUT_L 0x3e
#define MPU6050_RA_ACCEL_YOUT_H 0x3d
#define MPU6050_RA_ACCEL_ZOUT_L 0x40
#define MPU6050_RA_ACCEL_ZOUT_H 0x3f

#define MPU6050_RA_GYRO_XOUT_L 0x44
#define MPU6050_RA_GYRO_XOUT_H 0x43
#define MPU6050_RA_GYRO_YOUT_L 0x46
#define MPU6050_RA_GYRO_YOUT_H 0x45
#define MPU6050_RA_GYRO_ZOUT_L 0x48
#define MPU6050_RA_GYRO_ZOUT_H 0x47

typedef enum {
    MPU6050_GFS_250DPS = 0,
    MPU6050_GFS_500DPS = 1,
    MPU6050_GFS_1000DPS = 2,
    MPU6050_GFS_2000DPS = 3,
} mpu6050_gyro_fs_t;

typedef enum {
    MPU6050_AFS_2G = 0,
    MPU6050_AFS_4G = 1,
    MPU6050_AFS_8G = 2,
    MPU6050_AFS_16G = 3,
} mpu6050_accel_fs_t;

typedef enum {
    MPU6050_DLPF_0 = 0,     // 260 Hz Accel. Bandwidth
    MPU6050_DLPF_1 = 1,     // 184 Hz
    MPU6050_DLPF_2 = 2,     // 94 Hz
    MPU6050_DLPF_3 = 3,     // 44 Hz
    MPU6050_DLPF_4 = 4,     // 21 Hz
    MPU6050_DLPF_5 = 5,     // 10 Hz
    MPU6050_DLPF_6 = 6      // 5 Hz
} mpu6050_dlpf_mode_t;

class MPU6050Driver : public ImuDriver {
    public:
        MPU6050Driver(uint8_t i2cAddr, I2CHal &i2cHal);
        ~MPU6050Driver() override = default;

        void readAccel(std::optional<std::array<float,3>> &vec) override;
        void readGyro(std::optional<std::array<float,3>> &vec, bool correctOffset) override;

    private:
        bool m_isInitialized;
        uint8_t m_i2cAddress;
        I2CHal& m_i2cHal;
        mpu6050_gyro_fs_t m_gyroRange;
        mpu6050_accel_fs_t m_accelRange;
        mpu6050_dlpf_mode_t m_dlpfMode;
        uint8_t m_sampleRateDivider;

        int writeBits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t val);

        int readDeviceConfig();

        int writeGyroRange(mpu6050_gyro_fs_t range);
        int readGyroRange();
        float getGyroSensitivity();

        int writeAccelRange(mpu6050_accel_fs_t range);
        int readAccelRange();
        float getAccelSensitivity();

        int writeDlpfMode(mpu6050_dlpf_mode_t mode);
        int readDlpfMode();

        int writeSampleRateDivider(uint8_t divider);
        int readSampleRateDivider();
};

#endif // MPU6050_DRIVER_HPP_INCLUDED