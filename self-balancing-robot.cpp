#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "app.hpp"
#include "pico_i2c_adapter.hpp"
#include "pico_pwm_adapter.hpp"
#include "mpu6050_driver.hpp"
#include "complementary_filter.hpp"
#include "pid_controller.hpp"
#include "h_bridge_driver_gravity.hpp"

#define PIN_MOTOR_LEFT 14u
#define PIN_MOTOR_RIGHT 15u

static constexpr int MPU6050_I2C_ADDRESS = 0x68;

void pico_led_init(void) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void pico_set_led(bool led_on) {
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
}

int main()
{
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // configure I2C pins
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    PicoI2CHal i2cHal;
    MPU6050Driver imu(MPU6050_I2C_ADDRESS, i2cHal);

    ComplementaryFilter estimator(0.995f);
    PIDController controller(0.82f, 0.001f, 0.0216f);

    PicoPwmHal pwmHal;
    HBridgeDriverGravity motorLeft(PIN_MOTOR_LEFT, pwmHal);
    HBridgeDriverGravity motorRight(PIN_MOTOR_RIGHT, pwmHal);

    Application app(imu, motorLeft, motorRight, estimator, controller);

    gpio_put(PICO_DEFAULT_LED_PIN, true);

    uint64_t sysTimeUs;
    while (true) {
        sysTimeUs = time_us_64();
        app.loop(sysTimeUs);
        printf(app.getLogString(sysTimeUs).c_str());
    }
}
