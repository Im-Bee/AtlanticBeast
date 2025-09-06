#ifndef AB_CORE_H
#define AB_CORE_H

#include "CSystem.hpp"

namespace Core
{

static const char szLogPostfix[] = "_AtlanticBeast.log";

} // !Core


#include "Logger.hpp"

#ifdef _DEBUG
#   define AB_LOG(...) ::Core::Debug::Logger::Get()->Log(__VA_ARGS__)
#else
#   define AB_LOG(...) 
#endif // !_DEBUG


int TestFun();

#endif // !AB_CORE_H
