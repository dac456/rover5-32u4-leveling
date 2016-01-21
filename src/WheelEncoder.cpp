#include <Arduino.h>
#include "WheelEncoder.hpp"

WheelEncoder::WheelEncoder(int m1a, int m1b, int m2a, int m2b)
    : _m1Count(0),
      _m2Count(0),
      _m1a(m1a),
      _m1b(m1b),
      _m2a(m2a),
      _m2b(m2b)
{
    pinMode(m1a, INPUT);
    //pinMode(m1b, INPUT);
    //pinMode(m2a, INPUT);
    //pinMode(m2b, INPUT);

    digitalWrite(m1a, HIGH);
    //digitalWrite(m1b, HIGH);
    //digitalWrite(m2a, HIGH);
    //digitalWrite(m2b, HIGH);

}

void WheelEncoder::handleM1Interrupt(){
    if(digitalRead(_m1a) == digitalRead(_m1b)){
        _m1Count++;
    }
    else{
        _m1Count--;
    }
}

void WheelEncoder::handleM2Interrupt(){
    if(digitalRead(_m2a) == digitalRead(_m2b)){
        _m2Count++;
    }
    else{
        _m2Count--;
    }
}
