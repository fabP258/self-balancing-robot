#ifndef HAL_GPIO_HPP_INCLUDED
#define HAL_GPIO_HPP_INCLUDED

class GpioHal {
    public:
        virtual ~GpioHal() = default;

        virtual void setDir(unsigned int pin, bool isOutput) = 0;

        virtual void write(unsigned int pin, bool value) = 0;
};

#endif // HAL_GPIO_HPP_INCLUDED