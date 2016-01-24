#include "AlgorithmHillClimb.hpp"

float lerp(float v0, float v1, float t){
    return v0 + t*(v1-v0);
}

AlgorithmHillClimb::AlgorithmHillClimb(RoverHardware* hwd, float maxAngular, float maxLinear, int maxRpm)
    : AlgorithmBase(hwd, maxAngular, maxLinear, maxRpm),
      _inclined(false),
      _reversing(false),
      _timeLastCollide(0),
      _reverseCount(0)
{
    _name = "HillCli1";

    setDesiredLinearVelocity(0.0f);
    setDesiredAngularVelocity(0.0f);

    _headingPid = new PidController<float>(0.15f, 0.025f);
    _headingPid->setReference(60.0f);
}

AlgorithmHillClimb::~AlgorithmHillClimb(){
    delete _headingPid;
}

void AlgorithmHillClimb::senseImpl(uint16_t dt){
    (getAccelZ() < 9.0f) ? _inclined = true : _inclined = false;
}

void AlgorithmHillClimb::actImpl(uint16_t dt){
    _headingPid->step(getYawFiltered(), dt);

    /*if(!isColliding(0.5f) && (_reverseCount = 0)){
        setDesiredLinearVelocity(((fabs(pitchFiltered())/90.0f) * 0.3) + 0.1f);
        setDesiredAngularVelocity(0.0f);
        move();
    }
    else{
        _reverseCount++;
        setDesiredLinearVelocity(-(((fabs(pitchFiltered())/90.0f) * 0.3) + 0.1f));
        setDesiredAngularVelocity(0.0f);
        move();

        if(_reverseCount > 20) _reverseCount = 0;
    }*/
    if(_reverseCount == 0) (isColliding(0.5f) && (millis() - _timeLastCollide > 4000)) ? _reversing = true : _reversing = false;

    if(_reversing){
        _reverseCount++;
        setDesiredLinearVelocity(-(((fabs(getPitchFiltered())/90.0f) * 0.27) + 0.1f));
        setDesiredAngularVelocity(1.5f);
        move();

        if(_reverseCount > 15) {
            _reverseCount = 0;
            _timeLastCollide = millis();
        }
    }
    else{
        setDesiredLinearVelocity(((fabs(getPitchFiltered())/90.0f) * 0.27) + 0.1f);
        setDesiredAngularVelocity(0.0f);
        move();
    }
    //setDesiredAngularVelocity((-_headingPid->getValue()) * (M_PI/180.0f));

    //move();
}
