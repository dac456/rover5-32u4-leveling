#include "Odometry.hpp"
#include "Logger.hpp"

Odometry::Odometry()
    : _linearVelocity(0.0f),
      _angularVelocity(0.0f)
{

}

Odometry::~Odometry(){

}

void Odometry::integrate(int leftCount, int rightCount, uint16_t dt){
    Logger logger;
    float dtf = dt/1000.0f;

    if(dtf != 0.0f){
        float distLeft = 2.0f*M_PI*WHEEL_RADIUS*(leftCount/ENCODER_TPR);
        float distRight = 2.0*M_PI*WHEEL_RADIUS*(rightCount/ENCODER_TPR);

        float velLeft = distLeft/dtf;
        float velRight = distRight/dtf;

        _linearVelocity = (velLeft + velRight) / 2.0f;
        _angularVelocity = (velRight - velLeft) / WHEEL_BASE;

        float r = (WHEEL_BASE*(velLeft + velRight)) / (2.0f*(velRight - velLeft));
        if(!isnan(r)){
            if(isinf(r)){
                //TODO: why distLeft and distRight? does it matter if both are the same?
                _poseRobot.x += distLeft*cos(_poseRobot.theta);
                _poseRobot.y += distRight*sin(_poseRobot.theta);
            }
            else{
                _poseRobot.x += -r*sin(_poseRobot.theta) + r*sin(_poseRobot.theta + _angularVelocity*dtf);
                _poseRobot.y += r*cos(_poseRobot.theta) - r*cos(_poseRobot.theta + _angularVelocity*dtf);
                _poseRobot.theta += _angularVelocity*dtf;
            }

            logger.printf('x', _poseRobot.x);
            logger.printf('y', _poseRobot.y);
        }
    }
}
