#ifndef AB_X11_ERROR_HANDLING_H
#define AB_X11_ERROR_HANDLING_H

#include "Debug/Logger.hpp"
namespace Core 
{

int X11HandleError(Display *display, XErrorEvent *error) 
{
    char szErrorMessage[256];

    XGetErrorText(display, error->error_code, szErrorMessage, sizeof(szErrorMessage));
    ::Core::Debug::Logger::Get()->Log(Debug::Error, L"X11 Error: %s", szErrorMessage);

    return 0; 
}

} // !Core


#endif // !AB_X11_ERROR_HANDLING_H
