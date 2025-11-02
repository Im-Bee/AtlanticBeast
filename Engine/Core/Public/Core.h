#ifndef AB_CORE_H
#define AB_CORE_H

#include "CoreMinimal.h"
#include "AbLimits.h"

#ifdef __cplusplus
#include "Exception.hpp"

namespace Core::Debug
{

static const char szLogPostfix[] = "_AtlanticBeast.log";

} // !Core

#undef AB_EXCEPT
#define AB_EXCEPT(message) ::Core::Exception(message, __LINE__, __FILE__)

#include "Debug/Assert.hpp"
#include "Debug/Logger.hpp"

#ifdef _DEBUG
#   define AB_LOG(...) ::Core::Debug::Logger::Get()->Log(__VA_ARGS__)
#else
#   define AB_LOG(...) 
#endif // !_DEBUG

#endif // !__cplusplus
#endif // !AB_CORE_H
