#ifndef PICO_I2C_ADAPTER_HPP_INCLUDED
#define PICO_I2C_ADAPTER_HPP_INCLUDED

#include "hal_i2c.hpp"

class PicoI2CHal : public I2CHal {
    public:
        int write(uint8_t addr, uint8_t reg, uint8_t data) override;

        int read(uint8_t addr, uint8_t reg, uint8_t *data, unsigned int len) override;
};

#endif // PICO_I2C_ADAPTER_HPP_INCLUDED