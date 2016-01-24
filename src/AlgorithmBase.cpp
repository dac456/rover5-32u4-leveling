#include "AlgorithmBase.hpp"

AlgorithmBase::AlgorithmBase(RoverHardware* hwd, float maxAngular, float maxLinear, int16_t maxRpm)
    : //_hwd(hwd),
      _maxAngular(maxAngular),
      _maxLinear(maxLinear),
      _maxRpm(maxRpm),
      _accelY(0),
      _accelZ(0),
      _altitude(0.0f),
      _timeLastTurn(0),
      _timeLastCollision(0),
      _lastSpeed(0),
      _timeLastAccel(0),
      _accelerating(false)
{
    _hwd = hwd;

    _accelXFilter = new LowpassFilter<int16_t>(0.75f);
    _accelYFilter = new LowpassFilter<int16_t>(0.75f);
    _accelZFilter = new LowpassFilter<int16_t>(0.75f, -20);

    _rotXFilter = new LowpassFilter<int16_t>(0.98f);
    _rotYFilter = new LowpassFilter<int16_t>(0.98f);
    _rotZFilter = new LowpassFilter<int16_t>(0.98f);

    _pitch = new CompFilter<float>(0.98f);
    _heading = new CompFilter<float>(0.98f);
    //_heading = new LowpassFilter<float>(0.96f);

    _altFilter = new MovingAverage<float>(-92.0f);
}

AlgorithmBase::~AlgorithmBase(){
    delete _accelXFilter;
    delete _accelYFilter;
    delete _accelZFilter;

    delete _rotXFilter;
    delete _rotYFilter;
    delete _rotZFilter;

    delete _pitch;
    delete _heading;

    delete _altFilter;
}

char* AlgorithmBase::getName(){
    return _name;
}

/* http://cegt201.bradley.edu/projects/proj2015/autonomous_underwater_robots/Deliverables/Doxygen_Report_2_13_2015/_l_s_m303_d_8c_source.xhtml */
float AlgorithmBase::getAccelX(){
    return static_cast<float>(_accelX) / 104.41575f;
}

float AlgorithmBase::getAccelY(){
    return static_cast<float>(_accelY) / 104.41575f;
}

float AlgorithmBase::getAccelZ(){
    return static_cast<float>(_accelZ) / 104.41575f;
}

float AlgorithmBase::getRotX(){
    return (static_cast<float>(_rotX) * 8.75f) / 1000.0f;
}

float AlgorithmBase::getRotY(){
    return (static_cast<float>(_rotY) * 8.75f) / 1000.0f;
}

float AlgorithmBase::getRotZ(){
    return (static_cast<float>(_rotZ) * 8.75f) / 1000.0f;
}

float AlgorithmBase::getPitch(){
    //return atan2(_accelX, sqrt(_accelY*_accelY + _accelZ*_accelZ));
    uint16_t y2 = uint16_t(_accelY*_accelY);
    uint16_t z2 = uint16_t(_accelZ*_accelZ);
    return atan(float(_accelX) / sqrt(y2+z2));
}

float AlgorithmBase::getRoll(){
    return atan(float(-_accelY) / float(_accelZ));
    //return atan2(_accelY, sqrt(_accelY*_accelY + _accelX*_accelX));
}

float AlgorithmBase::getYaw(){
    return _hwd->compass->heading(LSM303::vector<int>{1,0,0})*(M_PI/180.0);
}

float AlgorithmBase::getPitchFiltered(){
    return _pitch->getFilteredValue();
}

float AlgorithmBase::getYawFiltered(){
    return _heading->getFilteredValue();
}

float AlgorithmBase::getAltitude(){
    return _altitude;
}

bool AlgorithmBase::isColliding(float threshold){
    //int16_t thresh2 = threshold*threshold;
    //int16_t xyDot = _accelX*_accelX + _accelY*_accelY;

    if(_xyMag.size() > 1){
        if( ((_xyMag[0] - _xyMag[1]) <= 0.0f) && (_xyMag[0] >= 0.0f) && (fabs(_xyMag[0] - _xyMag[1]) >= threshold) ){

            _timeLastCollision = millis();
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

void AlgorithmBase::move(){
    const float r = 0.03f; //wheel radius (m)
    const float L = 0.153f; //wheel base (m)

    float vLeft = ((2.0f*_desiredLinearVelocity) - (L*_desiredAngularVelocity)) / (2.0f*r); //rad/s
    float vRight = ((2.0f*_desiredLinearVelocity) + (L*_desiredAngularVelocity)) / (2.0f*r); //rad/s

    //rad/s -> rpm
    int16_t rpmLeft = static_cast<int16_t>(floor(vLeft * 9.551f));
    int16_t rpmRight = static_cast<int16_t>(floor(vRight * 9.551f));

    //TODO: assumes full 9V. need to account for battery degredation
    float dutyCycleLeft = static_cast<float>(rpmLeft) / static_cast<float>(_maxRpm);
    int16_t pwmLeft = static_cast<int16_t>(floor(dutyCycleLeft * 400.0f));

    float dutyCycleRight = static_cast<float>(rpmRight) / static_cast<float>(_maxRpm);
    int16_t pwmRight = static_cast<int16_t>(floor(dutyCycleRight * 400.0f));

    _hwd->motors->setSpeeds(pwmLeft, pwmRight);
}

void AlgorithmBase::stop(){
    _hwd->motors->setSpeeds(0,0);
}

void AlgorithmBase::setDesiredAngularVelocity(float vel){
    _desiredAngularVelocity = vel;
    _clampSpeed();
}

void AlgorithmBase::setDesiredLinearVelocity(float vel){
    //(speed > _lastSpeed) ? _accelerating = true : _accelerating = false;
    //_lastSpeed = speed;
    //if(_accelerating) _timeLastAccel = millis();

    _desiredLinearVelocity = vel;
    _clampSpeed();
}

void AlgorithmBase::step(uint16_t dt){
    sense(dt);
    act(dt);
}

void AlgorithmBase::sense(uint16_t dt){
    _hwd->compass->read();
    _hwd->gyro->read();
    //_hwd->proxSensors->read();

    //logger.printMagX(_hwd->compass->m.y);
    //logger.printAccX(_hwd->compass->m.z);

    _accelX = _accelXFilter->getFilteredValue(_hwd->compass->a.x/16);
    _accelY = _accelYFilter->getFilteredValue(_hwd->compass->a.y/16);
    _accelZ = _accelZFilter->getFilteredValue(_hwd->compass->a.z/16);

    _rotX = _rotXFilter->getFilteredValue(_hwd->gyro->g.x);
    _rotY = _rotYFilter->getFilteredValue(_hwd->gyro->g.y);
    _rotZ = _rotZFilter->getFilteredValue(_hwd->gyro->g.z);

    if(!isnan(this->getPitch())){
        _pitch->integrateValues(this->getRotY(), this->getPitch() * (180.0f/M_PI), dt);
    }
    _heading->integrateValues(this->getRotZ(), this->getYaw() * (180.0f/M_PI), dt);

    _altitude = _altFilter->getFilteredValue(_hwd->altimeter->pressureToAltitudeMeters(_hwd->altimeter->readPressureMillibars()));

    float Xf = getAccelX();
    float Yf = getAccelY();
    float mag = sqrt(Xf*Xf + Yf*Yf);
    _xyMag.push(mag);
    if(_xyMag.size() > 10){
        _xyMag.pop();
    }

    senseImpl(dt);
}

void AlgorithmBase::act(uint16_t dt){
    actImpl(dt);
}

void AlgorithmBase::_clampSpeed(){
    if(_desiredLinearVelocity > _maxLinear){
        _desiredLinearVelocity = _maxLinear;
    }

    if(_desiredAngularVelocity > _maxAngular){
        _desiredAngularVelocity = _maxAngular;
    }
}
