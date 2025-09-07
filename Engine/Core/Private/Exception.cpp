#include "Core.h"

namespace Core
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
Exception::Exception(const char* szMessage,
                     size_t uMesLen,
                     int32_t uLine,
                     const char* szFileName,
                     size_t uFileNameLen) noexcept 
    : m_pszMessage(szMessage) 
    , m_uMesLen(uMesLen)
    , m_Line(uLine)
    , m_pszFileName(szFileName)
    , m_uFileNameLen(uFileNameLen)
{
    if (!m_pszMessage) {
        return;
    }

    char* szErrorMessage = new char[MaxLongString];
    char szLine[MaxSmallString] = { 0 };
    char szAtLine[] = "\nAt line: ";
    char szInFile[] = "\nIn file: ";
    size_t uIndex = m_uMesLen;
    size_t uOldIndex = uIndex;
 
    if (uIndex >= MaxLongString) {
        LogAndReturnMessage(m_pszMessage, uOldIndex);
        return;
    }
    strcpy(szErrorMessage, m_pszMessage);
    m_pszMessage = szErrorMessage;
    
    uOldIndex = uIndex;
    uIndex += sprintf(szLine, "\nAt line: %d", static_cast<int>(m_Line));
    if (uIndex >= MaxLongString || m_Line == InvalidLine) {
        LogAndReturnMessage(szErrorMessage, uOldIndex);
        return;
    }
    strcat(szErrorMessage, szLine);

    uOldIndex = uIndex;
    uIndex += sizeof(szInFile) + m_uFileNameLen;
    if (uIndex >= MaxLongString || !m_pszFileName) {
        LogAndReturnMessage(szErrorMessage, uOldIndex);
        return;
    }
    strcat(szErrorMessage, szInFile);
    strcat(szErrorMessage, m_pszFileName);

    LogAndReturnMessage(szErrorMessage, uIndex);
}

// ---------------------------------------------------------------------------------------------------------------------
const char* Exception::what() const noexcept 
{
    if (m_pszMessage) {
        return m_pszMessage;
    } else {
        return ::std::exception::what();
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void Exception::LogAndReturnMessage(const char* pszMessage, size_t uMesLen) const noexcept
{
    wchar_t* wc = new wchar_t[uMesLen + 1];
    mbstowcs (wc, pszMessage, uMesLen);
    
    try {
        ::Core::Debug::Logger::Get()->Log(Debug::ESeverity::EError, wc);
        delete ::Core::Debug::Logger::Get();
    }
    catch (...) 
    { }
}

} // !Core

