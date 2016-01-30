#ifndef __ODOMETRY_HPP
#define __ODOMETRY_HPP

#include "Common.hpp"

struct Pose{
    Mat33<float> R;
    float x, y, z;
    float theta;

    Pose()
        : x(0.0f),
          y(0.0f),
          z(0.0f),
          theta(0.0f)
    {
    }
};

class Odometry{
private:
    AlgorithmBase* _alg;
    Pose _pose;

    float _linearVelocity;
    float _angularVelocity;

public:
    Odometry(AlgorithmBase* alg);
    ~Odometry();

    void integrate(int leftCount, int rightCount, uint16_t dt);

    float getLinearVelocity();
    float getAngularVelocity();

};

#endif
