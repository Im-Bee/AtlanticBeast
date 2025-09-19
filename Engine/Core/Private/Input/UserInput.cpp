#include "Input/UserInput.hpp"
#include "Input/InputEvents.h"

namespace Core
{

void* _pThis;
InputAction ia;

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Update()
{ 
    AbInputStruct& is = m_pWindowDesc->InputStruct;

    if (is.Handled || !m_bIsCapturing) {
        return;
    }

    switch (is.Event) {
        case EAbInputEvents::AbKeyPress:
            if (is.KeyId == AB_KEY_A) {
                ia(_pThis);
            }
        case EAbInputEvents::AbKeyRelease:
        case EAbInputEvents::AbButtonPress:
        case EAbInputEvents::AbButtonRelease:
            // AB_LOG(Debug::Info, L"Key press: %d", is.KeyId);
            break;

        case EAbInputEvents::AbMotion:
            // AB_LOG(Debug::Info, L"Mouse movement: %d %d", is.MouseX, is.MouseY);
            break;
    }

    is.Handled = true;
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Bind(void* pThis, InputAction pIa, InputBind bind)
{ 
    _pThis = pThis;
    ia = pIa;
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Unbind(void* pThis)
{ 
    ia = nullptr;
}

} // !Core
