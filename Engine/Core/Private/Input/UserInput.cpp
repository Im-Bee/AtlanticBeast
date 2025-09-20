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
            m_KeyPressMap.PlayAction(is.KeyId);
            break;

        case EAbInputEvents::AbKeyRelease:
            m_KeyReleaseMap.PlayAction(is.KeyId);
            break;

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
void UserInput::Bind(void* pThis, Action action, InputBind bind)
{ 
    if (bind.Type & EBindType::Keyboard) {
        if (bind.keyboard.KeyCode <= AB_INVALID_KEY || bind.keyboard.KeyCode >= AB_KEY_COUNT)
        {
            AB_LOG(Debug::Error, L"Key code is an invalid code (code outside of boundries for keys).");
            AB_LOG(Debug::Error, L"Can't bind the action for the keyboard.");
            AB_LOG(Debug::Error, L"Action wasn't bound.");
            return;
        }

        if (bind.keyboard.KeyState & EKeyState::Press) {
            m_KeyPressMap.SetKeyToAction(bind, pThis, action);
            m_BindsHandles[pThis] = bind;
        }

        return;
    }

}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Unbind(void* pThis)
{
    auto& bind = m_BindsHandles.find(pThis);

    if (bind == m_BindsHandles.end()) {
        AB_LOG(Debug::Warning, L"Cannot unbind this bind from this UserInput, because UserInput doesn't handles it.");
        return;
    }

    const auto& inputBind = bind->second;

    if (inputBind.Type & EBindType::Keyboard) {
        if (inputBind.keyboard.KeyState & EKeyState::Press) {
            // TODO
        }
    }
}

} // !Core
