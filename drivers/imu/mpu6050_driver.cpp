#include "mpu6050_driver.hpp"
#include <cmath>

static const float DEG2RADF = (float)M_PI / 180.0f;

MPU6050Driver::MPU6050Driver(uint8_t i2cAddr, I2CHal &i2cHal) : m_i2cAddress(i2cAddr), m_i2cHal(i2cHal) {
    m_gyroOffsets = std::array<float,3>{};
    m_isInitialized = false;
    int ret = m_i2cHal.write(m_i2cAddress, MPU6050_RA_PWR_MGMT_1, 0x00);
    if (ret < 0) return;
    ret = readDeviceConfig();
    if (ret < 0) return;
    ret = writeDlpfMode(MPU6050_DLPF_0);
    if (ret < 0) return;
    m_isInitialized = true;
}

int MPU6050Driver::writeBits(uint8_t reg, uint8_t mask, uint8_t shift, uint8_t val) {
    uint8_t cur;
    int ret = m_i2cHal.read(m_i2cAddress, reg, &cur, 1);
    if (ret < 0) return ret;
    cur = (cur & ~mask) | ((val << shift) & mask);
    return m_i2cHal.write(m_i2cAddress, reg, cur);
}

int MPU6050Driver::readDeviceConfig() {
    int ret;
    ret = readGyroRange();
    if (ret < 0) { return ret; }
    ret = readAccelRange();
    if (ret < 0) { return ret; }
    ret = readDlpfMode();
    if (ret < 0) { return ret; }
    ret = readSampleRateDivider();
    return ret;
}

int MPU6050Driver::writeGyroRange(mpu6050_gyro_fs_t range) {
    int res = writeBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_MASK, MPU6050_GYRO_FS_SHIFT, range);
    if (res < 0) return res;
    return readGyroRange();
}

int MPU6050Driver::readGyroRange() {
    uint8_t value;
    int res = m_i2cHal.read(m_i2cAddress, MPU6050_RA_GYRO_CONFIG, &value, 1);
    if (res < 0) return res;
    m_gyroRange = static_cast<mpu6050_gyro_fs_t>((value >> 3) & 0x03);
    return 0;
}

float MPU6050Driver::getGyroSensitivity() {
    switch (m_gyroRange)
    {
        case MPU6050_GFS_250DPS: return 131.0f;
        case MPU6050_GFS_500DPS: return 65.5f;
        case MPU6050_GFS_1000DPS: return 32.8f;
        case MPU6050_GFS_2000DPS: return 16.4f;
        default: return 131.0f;
    }
}

int MPU6050Driver::writeAccelRange(mpu6050_accel_fs_t range) {
    int res = writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_MASK, MPU6050_ACCEL_FS_SHIFT, range);
    if (res < 0) return res;
    return readAccelRange();
}

int MPU6050Driver::readAccelRange() {
    uint8_t value;
    int res = m_i2cHal.read(m_i2cAddress, MPU6050_RA_ACCEL_CONFIG, &value, 1);
    if (res < 0) return res;
    m_accelRange = static_cast<mpu6050_accel_fs_t>((value >> 3) & 0x03);
    return 0;
}

float MPU6050Driver::getAccelSensitivity() {
    switch (m_accelRange)
    {
        case MPU6050_AFS_2G: return 16384.0f;
        case MPU6050_AFS_4G: return 8192.0f;
        case MPU6050_AFS_8G: return 4096.0f;
        case MPU6050_AFS_16G: return 2048.0f;
        default: return 16384.0f;
    }
}

int MPU6050Driver::writeDlpfMode(mpu6050_dlpf_mode_t mode) {
    int res = writeBits(MPU6050_RA_CONFIG, MPU6050_DLPF_MASK, MPU6050_DLPF_SHIFT, (uint8_t)mode);
    if (res < 0) return res;
    return readDlpfMode();
}

int MPU6050Driver::readDlpfMode() {
    uint8_t value;
    int res = m_i2cHal.read(m_i2cAddress, MPU6050_RA_CONFIG, &value, 1);
    if (res < 0) return res;
    m_dlpfMode = static_cast<mpu6050_dlpf_mode_t>(value & 0x07);
    return 0;
}

int MPU6050Driver::writeSampleRateDivider(uint8_t divider) {
    int res = m_i2cHal.write(m_i2cAddress, MPU6050_RA_SMPRT_DIV, divider);
    if (res < 0) return res;
    return readSampleRateDivider();
}

int MPU6050Driver::readSampleRateDivider() {
    uint8_t value;
    int res = m_i2cHal.read(m_i2cAddress, MPU6050_RA_SMPRT_DIV, &value, 1);
    if (res < 0) return res;
    m_sampleRateDivider = value;
    return 0;
}

void MPU6050Driver::readAccel(std::optional<std::array<float,3>> &vec) {
    if (!m_isInitialized) {
        vec.reset();
        return;
    }
    uint8_t buf[6];
    int res = m_i2cHal.read(m_i2cAddress, MPU6050_RA_ACCEL_XOUT_H, buf, 6);
    if (res < 0) {
        vec.reset();
        return;
    }

    int16_t raw_x = (buf[0] << 8) | buf[1];
    int16_t raw_y = (buf[2] << 8) | buf[3];
    int16_t raw_z = (buf[4] << 8) | buf[5];

    float sensitivity = getAccelSensitivity();
    vec = std::array<float,3>{(float)raw_x / sensitivity, 
            (float)raw_y / sensitivity, 
            (float)raw_z / sensitivity
        };
}

void MPU6050Driver::readGyro(std::optional<std::array<float,3>> &vec, bool correctOffset) {
    if (!m_isInitialized) {
        vec.reset();
        return;
    }
    uint8_t buf[6];
    int res = m_i2cHal.read(m_i2cAddress, MPU6050_RA_GYRO_XOUT_H, buf, 6);
    if (res < 0) {
        vec.reset();
        return;
    }

    int16_t raw_x = (buf[0] << 8) | buf[1];
    int16_t raw_y = (buf[2] << 8) | buf[3];
    int16_t raw_z = (buf[4] << 8) | buf[5];

    float sensitivity = getGyroSensitivity();
    float rate_x = ((float)raw_x / sensitivity) * DEG2RADF;
    float rate_y = ((float)raw_y / sensitivity) * DEG2RADF;
    float rate_z = ((float)raw_z / sensitivity) * DEG2RADF;

    if (correctOffset) {
        rate_x -= m_gyroOffsets[0];
        rate_y -= m_gyroOffsets[1];
        rate_z -= m_gyroOffsets[2];
    }

    vec = std::array<float,3>{rate_x,rate_y,rate_z};
}