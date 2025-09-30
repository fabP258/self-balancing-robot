#ifndef H_BRIDGE_DRIVER_HPP_INCLUDED
#define H_BRIDGE_DRIVER_HPP_INCLUDED

class HBridgeDriver {
    public:
        virtual ~HBridgeDriver() = default;

        virtual void writeSpeed(float speed) = 0;
};

#endif // H_BRIDGE_DRIVER_HPP_INCLUDED