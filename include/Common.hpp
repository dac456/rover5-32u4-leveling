#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <Wire.h>

#include <AStar32U4Motors.h>

struct RoverHardware{
    RoverHardware(){
        motors = new AStar32U4Motors;
    }
    ~RoverHardware(){
        delete motors;
    }

    AStar32U4Motors* motors;
};

#endif
