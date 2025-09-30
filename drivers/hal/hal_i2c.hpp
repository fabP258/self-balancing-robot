#ifndef HAL_I2C_HPP_INCLUDED
#define HAL_I2C_HPP_INCLUDED

#include <cstdint>

class I2CHal {
    public:
        virtual ~I2CHal() = default;

        virtual int write(uint8_t addr, uint8_t reg, uint8_t data) = 0;

        virtual int read(uint8_t addr, uint8_t reg, uint8_t *data, unsigned int len) = 0;
};

#endif // HAL_I2C_HPP_INCLUDED