#include "Input/UserInput.hpp"
#include "Input/InputEvents.h"

namespace Core
{

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Update()
{ 
    AbInputStruct& is = m_pWindowDesc->InputStruct;

    if (is.Handled || !m_bIsCapturing) {
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
            // AB_LOG(Debug::Info, L"Mouse movement: %d %d", is.MouseX, is.MouseY);
            break;
    }

    is.Handled = true;
}

} // !Core
