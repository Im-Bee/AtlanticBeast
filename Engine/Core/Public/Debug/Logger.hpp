#ifndef AB_LOGGER_H
#define AB_LOGGER_H

#include "CoreMinimal.h"

namespace Core::Debug
{

enum ESeverity 
{
    Info = 1,
    Warning = Info << 1,
    Error = Warning << 1,
};

class Logger
{
    BEAST_API Logger();

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

public:

    BEAST_API static Logger* Get();
    
    BEAST_API ~Logger();

public:

    /**
     * @brief Logs message to the file.
     *
     * @param sev Severity tag of the log message
     * @param pwszFmt Format for the message or message itself, written with `vswprintf` standard
     * @param ... Variables to be used in the format
     */
    BEAST_API void Log(const ESeverity sev, const wchar_t wszFmt[], ...);

private:

    using Clock = ::std::chrono::system_clock;

    struct LogStruct
    {
        ::std::chrono::time_point<Clock> TimeStamp;
        ESeverity Sev;
        wchar_t* Message;
    };

private:

    ::std::string CreateDatePreFix() const;

    const wchar_t* GetTag(const ESeverity sev) const;

    const ::std::wstring Stringify(LogStruct& ls) const; 
    
    void WriteLoop();

private:

    ::std::mutex m_InstanceLock;

    ::std::queue<LogStruct> m_MessageQueue;
    ::std::string m_strTargetPath;
    ::std::string m_strLogName;

    volatile ::std::atomic_bool m_aIsWriteThreadWorking;
    ::std::thread m_tWriteThreadHandle;

};

} // !Core

#endif // !AB_LOGGER_H
