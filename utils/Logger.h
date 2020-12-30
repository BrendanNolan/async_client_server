#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace utils
{

class Logger
{
public:
    virtual ~Logger() = default;
    
    virtual void log(const std::string& text) = 0; 
};

}// namespace utils

#endif// LOGGER_H

