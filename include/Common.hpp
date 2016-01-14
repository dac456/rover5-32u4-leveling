#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <Wire.h>

#include <AStar32U4Motors.h>
#include <AStar32U4Buttons.h>
#include <LSM303.h>
#include <L3G.h>

#include "MovingAverage.hpp"
#include "CompFilter.hpp"
#include "LowpassFilter.hpp"
#include "Queue.hpp"

struct RoverHardware{
    RoverHardware(){
        motors = new AStar32U4Motors;
        compass = new LSM303;
        gyro = new L3G;
    }
    ~RoverHardware(){
        delete motors;
        delete compass;
        delete gyro;
    }

    AStar32U4Motors* motors;
    LSM303* compass;
    L3G* gyro;
};

#endif
