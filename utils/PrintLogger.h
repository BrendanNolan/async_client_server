#ifndef PRINTLOGGER_H
#define PRINTLOGGER_H

#include <mutex>
#include <string>

#include "Logger.h"

namespace utils
{

class PrintLogger : public Logger
{
public:
    void log(const std::string& text) override;

private:
    mutable std::mutex coutMutex_;
};

}// namespace utils

#endif// PRINTLOGGER_H
