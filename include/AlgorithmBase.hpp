#ifndef __ALGORITHMBASE_H
#define __ALGORITHMBASE_H

#include "Common.hpp"
#include "Logger.hpp"
#include "Odometry.hpp"

class AlgorithmBase{
private:
    Odometry _odom;

    float _desiredLinearVelocity;
    float _desiredAngularVelocity;
    float _maxLinear;
    float _maxAngular;
    uint16_t _maxRpm;

    LowpassFilter<int16_t>* _accelXFilter;
    LowpassFilter<int16_t>* _accelYFilter;
    LowpassFilter<int16_t>* _accelZFilter;
    int16_t _accelX;
    int16_t _accelY;
    int16_t _accelZ;

    LowpassFilter<int16_t>* _rotXFilter;
    LowpassFilter<int16_t>* _rotYFilter;
    LowpassFilter<int16_t>* _rotZFilter;
    int16_t _rotX;
    int16_t _rotY;
    int16_t _rotZ;

    float _pitch;

    CompFilter<float>* _pitchFiltered;
    CompFilter<float>* _headingFiltered;
    //LowpassFilter<float>* _heading;

    MovingAverage<float>* _altFilter;
    float _altitude;

    uint16_t _timeLastTurn;
    uint16_t _timeLastCollision;

    Queue<float> _xyMag;
    uint16_t _lastSpeed;
    uint16_t _timeLastAccel;
    bool _accelerating;

protected:
    RoverHardware* _hwd;
    char* _name;

    Logger logger;

public:
    AlgorithmBase(RoverHardware* hwd, float maxAngular = 10.0f, float maxLinear = 0.34f, int16_t maxRpm = 320);
    virtual ~AlgorithmBase();

    char* getName();

    float getAccelX();
    float getAccelY();
    float getAccelZ();

    float getRotX();
    float getRotY();
    float getRotZ();

    float getPitch();
    float getRoll();
    float getYaw();

    float getPitchFiltered();
    float getYawFiltered();

    float getAltitude();

    bool isColliding(float threshold);

    void move();
    void stop();

    void setDesiredLinearVelocity(float vel);
    void setDesiredAngularVelocity(float vel);

    void step(uint16_t dt);

    void sense(uint16_t dt);
    void act(uint16_t dt);

    virtual void senseImpl(uint16_t dt) = 0;
    virtual void actImpl(uint16_t dt) = 0;

private:
    void _clampSpeed();

};

#endif
