#ifndef IMU_INTERFACE_HPP_INCLUDED
#define IMU_INTERFACE_HPP_INCLUDED

#include <array>
#include <optional>

#define IMU_NUM_GYRO_CALIB_SAMPLES 1000

class ImuDriver {
    public:
        virtual ~ImuDriver() = default;

        /**
         * @brief Read linear accelerations.
         *
         * @param[out] vec  3D vector of accelerations (x, y, z) in g.
         * @return 0 on success, negative error code on failure.
         */
        virtual void readAccel(std::optional<std::array<float,3>> &vec) = 0;

        /**
         * @brief Read angular velocities.
         *
         * @param[out] vec            3D vector of angular rates (x, y, z) in rad/s.
         * @param[in]  correctOffset  If true, subtract static gyro offsets.
         * @return 0 on success, negative error code on failure.
         */
        virtual void readGyro(std::optional<std::array<float,3>> &vec, bool correctOffset) = 0;

        /**
         * @brief Calibrate gyroscope offsets.
         *
         * Computes and stores static offset values in @ref m_gyroOffsets.
         *
         * @return 0 on success, negative error code on failure.
         */
        int calibrateGyro();
    
    protected:
        // static gyroscope offsets in rad/s
        std::array<float,3> m_gyroOffsets;
};

#endif // IMU_INTERFACE_HPP_INCLUDED