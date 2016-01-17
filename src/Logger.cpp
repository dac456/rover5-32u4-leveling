#include "Logger.hpp"

Logger::Logger(){

}

Logger::~Logger(){

}

void Logger::printf(const char type, float val){
    char buf[16];
    sprintf(buf, "%8.6f", val);

    Serial1.print(type);
    Serial1.println(buf);
}

void Logger::printi(const char type, int val){
    char buf[16];
    sprintf(buf, "%i", val);

    Serial1.print(type);
    Serial1.println(buf);
}
