#include "pico_i2c_adapter.hpp"
#include "hardware/i2c.h"

int PicoI2CHal::write(uint8_t addr, uint8_t reg, uint8_t data) {
    uint8_t buf[2] = {reg, data};
    int ret = i2c_write_blocking(i2c_default, addr, buf, 2, false);
    return (ret < 0) ? ret : 0;
}

int PicoI2CHal::read(uint8_t addr, uint8_t reg, uint8_t *data, unsigned int len) {
    int ret = i2c_write_blocking(i2c_default, addr, &reg, 1, true);
    if (ret < 0) return ret;
    ret = i2c_read_blocking(i2c_default, addr, data, len, false);
    return (ret < 0) ? ret : 0;
}