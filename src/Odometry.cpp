#include "Odometry.hpp"
#include "AlgorithmBase.hpp"
#include "Logger.hpp"

Odometry::Odometry(AlgorithmBase* alg)
    : _alg(alg),
      _linearVelocity(0.0f),
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

        //float velLeft = distLeft/dtf;
        //float velRight = distRight/dtf;
        float velLeft = (M_PI*(leftCount/ENCODER_TPR))/dtf;
        float velRight = (M_PI*(rightCount/ENCODER_TPR))/dtf;

        _linearVelocity = (WHEEL_RADIUS*(velLeft + velRight)) * 0.5f;
        _angularVelocity = (WHEEL_RADIUS*(velRight - velLeft)) * WHEEL_BASE_INV;

        float r = (WHEEL_BASE*(velLeft + velRight)) / (2.0f*(velRight - velLeft));
        if(!isnan(r)){
            if(isinf(r)){
                float avgDist = (distLeft+distRight)*0.5f;
                _pose.x += avgDist*cos(_pose.theta);
                _pose.y += avgDist*sin(_pose.theta);
            }
            else{
                _pose.x += -r*sin(_pose.theta) + r*sin(_pose.theta + _angularVelocity*dtf);
                _pose.y += r*cos(_pose.theta) - r*cos(_pose.theta + _angularVelocity*dtf);
                _pose.theta += _angularVelocity*dtf;
            }

            //logger.printf('x', _pose.x);
            //logger.printf('y', _pose.y);
        }
    }
}

Pose Odometry::getPose(){
    return _pose;
}

float Odometry::getLinearVelocity(){
    return _linearVelocity;
}

float Odometry::getAngularVelocity(){
    return _angularVelocity;
}
