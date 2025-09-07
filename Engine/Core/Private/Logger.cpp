#include "Core.h"

#include <iostream>
#include <cstdarg>
#include <cwchar>

namespace Core::Debug
{
 
using namespace std;
using namespace chrono_literals;

// Logger // -----------------------------------------------------------------------------------------------------------
Logger::Logger()
    : m_MessageQueue()
    , m_strTargetPath(filesystem::current_path().string() + "/Logs/")
    , m_strLogName(CreateDatePreFix() + szLogPostfix)
    , m_aIsWriteThreadWorking(true)
    , m_tWriteThreadHandle(&Logger::WriteLoop, this)
{ }

// Statics // ----------------------------------------------------------------------------------------------------------
Logger* Logger::m_pInstance     = new Logger();
mutex   Logger::m_InstanceLock  = { };

// ---------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
    m_aIsWriteThreadWorking.store(false);
    m_tWriteThreadHandle.join();

    lock_guard<mutex> lock(m_InstanceLock);

    m_pInstance = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
Logger* Logger::Get()
{
    lock_guard<mutex> lock(m_InstanceLock);

    if (!m_pInstance) {
        m_pInstance = new Logger();
    }

    return m_pInstance;
}

// ---------------------------------------------------------------------------------------------------------------------
void Logger::Log(const ESeverity sev, const wchar_t pwszFmt[], ...)
{
    wchar_t* pwszMessage = new wchar_t[MaxLongString];

    va_list args;
    va_start(args, pwszFmt);
    vswprintf(pwszMessage, MaxLongString, pwszFmt, args);
    va_end(args);

    lock_guard<mutex> lock(m_InstanceLock);
    m_MessageQueue.push({ chrono::system_clock::now(), sev, pwszMessage });
}

// ---------------------------------------------------------------------------------------------------------------------
string Logger::CreateDatePreFix() const
{
    stringstream    prefix      = { };
    time_t          timeStamp   = time(NULL);
    tm              lTime       = *localtime(&timeStamp);

    prefix << put_time(&lTime, "%Y-%m-%d_%H-%M-%S");

    return prefix.str();
}

// ---------------------------------------------------------------------------------------------------------------------
const wchar_t* Logger::GetTag(const ESeverity sev) const
{
    switch (sev)
    {
        case ESeverity::EInfo:
            return L"Info";

        case ESeverity::EWarning:
            return L"Warning";

        case ESeverity::EError:
            return L"Error";

        default:
            return L"Unknown";
    }
}

const ::std::wstring Logger::Stringify(LogStruct& ls) const
{
    time_t  timeStamp   = chrono::system_clock::to_time_t(ls.TimeStamp);
    tm      lTime       = *localtime(&timeStamp);

    return (wstringstream() << L'[' << put_time(&lTime, L"%H:%M:%S") << L"][" 
            << GetTag(ls.Sev) << "]: " << ls.Message).str();
}

// ---------------------------------------------------------------------------------------------------------------------
void Logger::WriteLoop()
{   
    string      outputPath;
    wofstream   woFile;

    m_InstanceLock.lock();
    
    filesystem::create_directory(this->m_strTargetPath);
    outputPath = this->m_strTargetPath + this->m_strLogName;
    woFile.open(outputPath);
    woFile.close();

    m_InstanceLock.unlock();

    do
    {
        m_InstanceLock.lock();
        
        if (m_MessageQueue.empty()) {
            m_InstanceLock.unlock();
            this_thread::sleep_for(1ms);
            continue;
        }

        auto& stamp = m_MessageQueue.front();
        m_MessageQueue.pop();

        m_InstanceLock.unlock();

        wcout << Stringify(stamp) << endl;

        woFile.open(outputPath);
        woFile << Stringify(stamp) << endl;
        woFile.close();

        delete[] stamp.Message;
    }
    while (m_aIsWriteThreadWorking.load());
}

} // !Core
