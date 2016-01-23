#include "AlgorithmHillClimb.hpp"
#include "Logger.hpp"

RoverHardware* hwd;
AlgorithmHillClimb* alg;

AStar32U4ButtonA buttonA;

void initialize(){
    init(); //AVR init - timers and things

    Serial1.begin(57600);
    Wire.begin(); //Initialize I2C bus for compass/gyro

    hwd = new RoverHardware();
    hwd->compass->init();
    hwd->compass->enableDefault();

    hwd->compass->m_min = (LSM303::vector<int16_t>){-5450, -5633, -1190};
    hwd->compass->m_max = (LSM303::vector<int16_t>){-912, -2516, +1164};

    hwd->gyro->init();
    hwd->gyro->enableDefault();

    hwd->altimeter->init();
    hwd->altimeter->enableDefault();

    hwd->encoders->init();

    alg = new AlgorithmHillClimb(hwd, 10.0f, 0.27f, 86);

    randomSeed(analogRead(0));
}

int main(int argc, char* argv[])
{
    initialize();

    static uint16_t last_time = 0;
    static uint16_t last_time_print = 0;
    static bool stop = false;

    //buttonA.waitForButton();

    while(1){
        uint16_t dt = millis() - last_time;
        if(dt >= 10){
            alg->step(dt);
            last_time = millis();
        }

        uint16_t dt_print = millis() - last_time_print;
        if(dt_print >= 100){
            char buf[32];
            sprintf(buf, "L: %i R: %i", hwd->encoders->getCountsLeft(), hwd->encoders->getCountsRight());
            Serial1.println(buf);

            last_time_print = millis();
        }
    }

    delete alg;
    delete hwd;
}
