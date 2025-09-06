#include "Logger.hpp"

namespace Core::Debug
{
    
Logger* Logger::m_pInstance = nullptr;

Logger::~Logger()
{
    m_pInstance = nullptr;
}

Logger* Logger::Get()
{
    if (!m_pInstance) {
        m_pInstance = new Logger();
    }

    return m_pInstance;
}

} // !Core
