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

    alg = new AlgorithmHillClimb(hwd, 10.0f, 0.27f, 86);

    randomSeed(analogRead(0));
}



int main(int argc, char* argv[])
{
    initialize();

    static uint16_t last_time = 0;
    static bool stop = false;

    buttonA.waitForButton();

    while(1){
        uint16_t dt = millis() - last_time;
        if(dt >= 100){
            alg->step(dt);

            //Dump the Z reading from the accelerometer for debugging
            char buf[8];
            sprintf(buf, "%8.6f", alg->yawFiltered());
            Serial1.println(buf);
            //hwd->lcd->gotoXY(0,1);
            //hwd->lcd->print(buf);


            last_time = millis();
        }
    }

    delete alg;
    delete hwd;
}
