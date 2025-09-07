#ifndef AB_CORE_H
#define AB_CORE_H

#ifdef _WIN32
#   define _CRT_SECURE_NO_WARNINGS
#endif // !_WIN32

#include "CSystem.hpp"
#include "AbLimits.h"
#include "Exception.hpp"

namespace Core::Debug
{

static const char szLogPostfix[] = "_AtlanticBeast.log";

} // !Core


#include "Logger.hpp"

#ifdef _DEBUG
#   define AB_LOG(...) ::Core::Debug::Logger::Get()->Log(__VA_ARGS__)
#else
#   define AB_LOG(...) 
#endif // !_DEBUG
       

#undef AB_EXCEPT
#define AB_EXCEPT(message) ::Core::Exception(message, __LINE__, __FILE__)
       

#endif // !AB_CORE_H
