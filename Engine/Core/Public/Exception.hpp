#ifndef AB_EXCEPT_H
#define AB_EXCEPT_H

#include "ExportImport.h"
#include "CSystem.hpp"

namespace Core
{

class Exception : public ::std::exception
{
    
    static constexpr int32_t InvalidLine = -1;

public:

    BEAST_API explicit Exception(const char*  szMessage       = nullptr,
                                 size_t       uMesLen         = 0,
                                 int32_t      uLine           = InvalidLine,
                                 const char*  szFileName      = nullptr,
                                 size_t       uFileNameLen    = 0) noexcept;

    template<size_t uMesLen, size_t uFileNameLen>
    explicit constexpr Exception(const char (&pszMessage)[uMesLen]          = 0, 
                                 int32_t    uLine                           = 0,
                                 const char (&pszFileName)[uFileNameLen]    = nullptr) noexcept
        : Exception(pszMessage, uMesLen, uLine, pszFileName, uFileNameLen)
    { }

    BEAST_API ~Exception() noexcept;

    BEAST_API Exception(const Exception&) noexcept;

    BEAST_API Exception(Exception&&) noexcept = default;

public: 

    BEAST_API virtual const char* what() const noexcept final override;

private:

   void LogAndReturnMessage(const char* pszMessage, size_t uMesLen) const noexcept;

private:

   const char* m_pszMessage;
   size_t m_uMesLen;
   const int32_t m_Line;
   const char* m_pszFileName;
   const size_t m_uFileNameLen;

};

} // !Core

#endif // !AB_EXCEPT_H
