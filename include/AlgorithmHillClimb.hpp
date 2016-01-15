#ifndef __ALGORITHMHILLCLIMB_H
#define __ALGORITHMHILLCLIMB_H

#include "AlgorithmBase.hpp"
#include "PidController.hpp"

class AlgorithmHillClimb : public AlgorithmBase{
private:
    bool _inclined;
    bool _reversing;
    uint16_t _timeLastCollide;
    uint16_t _reverseCount;

    PidController<float>* _headingPid;

public:
    AlgorithmHillClimb(RoverHardware* hwd, float maxAngular = 10.0, float maxLinear = 0.34, int maxRpm = 320);
    ~AlgorithmHillClimb();


    void senseImpl(uint16_t dt);
    void actImpl(uint16_t dt);
};

#endif
