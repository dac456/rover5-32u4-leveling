#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <Wire.h>

#include <AStar32U4Motors.h>
#include <AStar32U4Encoders.h>
#include <AStar32U4Buttons.h>
#include <LSM303.h>
#include <L3G.h>
#include <LPS.h>

#include "MovingAverage.hpp"
#include "CompFilter.hpp"
#include "LowpassFilter.hpp"
#include "Queue.hpp"

#define ENCODER_TPR 333.3f //Ticks per revolution
#define WHEEL_RADIUS 0.03f
#define WHEEL_BASE 0.153f
#define WHEEL_BASE_INV 6.535f;

struct RoverHardware{
    RoverHardware(){
        motors = new AStar32U4Motors;
        encoders = new AStar32U4Encoders;
        compass = new LSM303;
        gyro = new L3G;
        altimeter = new LPS;
    }
    ~RoverHardware(){
        delete motors;
        delete compass;
        delete gyro;
        delete altimeter;
        delete encoders;
    }

    AStar32U4Motors* motors;
    AStar32U4Encoders* encoders;
    LSM303* compass;
    L3G* gyro;
    LPS* altimeter;
};

//Forward declaration
class AlgorithmBase;

#endif
