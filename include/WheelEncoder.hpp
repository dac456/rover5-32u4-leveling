#ifndef __WHEELENCODER_HPP
#define __WHEELENCODER_HPP

class WheelEncoder{
private:
    int _m1Count;
    int _m2Count;

    int _m1a;
    int _m1b;
    int _m2a;
    int _m2b;

public:
    WheelEncoder(int m1a, int m1b, int m2a, int m2b);
    ~WheelEncoder();

    void handleM1Interrupt();
    void handleM2Interrupt();
};

#endif
