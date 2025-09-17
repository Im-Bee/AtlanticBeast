#include "Input/UserInput.hpp"
#include "Input/InputEvents.h"

namespace Core
{

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Update()
{ 
    AbInputStruct& is = m_pWindowDesc->InputStruct;

    if (is.Handled) {
        return;
    }

    switch (is.Event) {
        case EAbInputEvents::AbKeyPress:
        case EAbInputEvents::AbKeyRelease:
        case EAbInputEvents::AbButtonPress:
        case EAbInputEvents::AbButtonRelease:
            AB_LOG(Debug::Info, L"Key press: %d", is.KeyId);
            break;

        case EAbInputEvents::AbMotion:
            AB_LOG(Debug::Info, L"Mouse movement: %lf %lf", is.MouseX, is.MouseY);
    }

    is.Handled = true;
}

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
bool UserInput::StartCapturingInternal()
{
    if (m_pWindowDesc->bInputSupport == 0) {
        AB_LOG(Debug::Error, L"XI2 isn't supported! Cannot start capturing user input.");

        // Some fallback needed?
        return false;
    }

    unsigned char maskBytes[(XI_LASTEVENT + 7) / 8] = { 0 };
    XISetMask(maskBytes, XI_KeyPress);
    XISetMask(maskBytes, XI_KeyRelease);
    XISetMask(maskBytes, XI_ButtonPress);
    XISetMask(maskBytes, XI_ButtonRelease);
    XISetMask(maskBytes, XI_Motion);

    XIEventMask evmask;

    evmask.deviceid = XIAllDevices;
    evmask.mask_len = sizeof(maskBytes);
    evmask.mask     = maskBytes;

    XISelectEvents(m_pWindowDesc->DisplayHandle, 
                   m_pWindowDesc->WindowHandle,
                   &evmask, 1);

    XFlush(m_pWindowDesc->DisplayHandle);

    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::StopCapturingInternal()
{
}

#elif _WIN32

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::StartCapturingInternal()
{
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::StopCapturingInternal()
{
}

#endif

} // !Core
