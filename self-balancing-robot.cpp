#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "app.hpp"
#include "pico_i2c_adapter.hpp"
#include "pico_pwm_adapter.hpp"
#include "mpu6050_driver.hpp"
#include "complementary_filter.hpp"
#include "pid_controller.hpp"
#include "h_bridge_driver_gravity.hpp"

#define PIN_MOTOR_LEFT 14u
#define PIN_MOTOR_RIGHT 15u
#define CONTROL_LOOP_CYCLE_TIME_MS 5

// Note: uncomment this line to enable logging
// #define LOGGING_ENABLED

static constexpr int MPU6050_I2C_ADDRESS = 0x68;

volatile bool control_flag = false;

void pico_led_init(void) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

void pico_set_led(bool led_on) {
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
}

bool timer_callback(__unused repeating_timer_t *t) {
    control_flag = true;
    return true;
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

    repeating_timer_t control_timer;
    add_repeating_timer_ms(-CONTROL_LOOP_CYCLE_TIME_MS, timer_callback, NULL, &control_timer);

    gpio_put(PICO_DEFAULT_LED_PIN, true);

    uint64_t sysTimeUs;
    while (true) {
        if (control_flag) {
            control_flag = false;
            sysTimeUs = time_us_64();
            app.loop(sysTimeUs);
#ifdef LOGGING_ENABLED
            printf(app.getLogString(sysTimeUs).c_str());
#endif
        }
    }
}
