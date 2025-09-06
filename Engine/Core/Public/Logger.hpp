#ifndef AB_LOGGER_H
#define AB_LOGGER_H

namespace Core::Debug
{

class Logger
{
    Logger() = default;

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger* m_pInstance;

public:

    static Logger* Get();
    
    ~Logger();

};

} // !Core

#endif // !AB_LOGGER_H
