#ifndef __LOGGER_H
#define __LOGGER_H

#include "Common.hpp"

typedef enum{
    TURN_LEFT,
    TURN_RIGHT,
    COLLISION
} ACTION;

typedef enum{
    PRINT_PITCH = 'a',
    PRINT_HEADING,
    PRINT_DESIRED_VEL
} LOG_TYPE;

class Logger{
private:


public:
    Logger();
    ~Logger();

    void printf(const char type, float val);
    void printi(const char type, int val);

};

#endif
