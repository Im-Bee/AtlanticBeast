#include "Input/UserInput.hpp"
#include "Input/InputEvents.h"
#include "Input/ControllerObject.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Update()
{ 
    // Always replay the continuos keybinds that are currently pressed
    if (m_vCurrentlyPressedKeys.any())
        for (size_t i = 0; i < AB_KEY_COUNT; ++i)
            if (m_vCurrentlyPressedKeys.test(i))
                m_KeyContinuous.PlayAction(i);
    
    AbInputStruct& is = m_pWindowDesc->InputStruct;
    AbKeyId key = is.KeyId;

    if (is.Handled || !m_bIsCapturing || key <= AB_INVALID_KEY || key >= AB_KEY_COUNT)
        return;

    switch (is.Event) {
        case EAbInputEvents::AbKeyPress:
            if (m_vCurrentlyPressedKeys.test(key))
                break;

            m_vCurrentlyPressedKeys.set(key);

            m_KeyPressMap.PlayAction(key);
            m_KeyContinuous.PlayAction(key);
            break;

        case EAbInputEvents::AbKeyRelease:
            if (!m_vCurrentlyPressedKeys.test(key))
                break;
            
            m_vCurrentlyPressedKeys.flip(key);

            m_KeyReleaseMap.PlayAction(key);
            break;

        case EAbInputEvents::AbButtonPress:
        case EAbInputEvents::AbButtonRelease:
            // AB_LOG(Debug::Info, L"Key press: %d", is.KeyId);
            break;

        case EAbInputEvents::AbMotion:
            m_MouseMap.PlayAction(is.MouseX, is.MouseY);
            is.MouseX = 0;
            is.MouseY = 0;

            break;
    }

    is.Handled = true;
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Bind(void* pThis, AbAction action, AbMouseAction mouseAction, AbInputBind bind)
{ 
    if (bind.Type & EAbBindType::Keyboard) {
        if (bind.keyboard.KeyCode <= AB_INVALID_KEY || bind.keyboard.KeyCode >= AB_KEY_COUNT)
        {
            AB_LOG(Debug::Error, L"Key code is an invalid code (code outside of boundries for keys).");
            AB_LOG(Debug::Error, L"Can't bind the action for the keyboard.");
            AB_LOG(Debug::Error, L"Action wasn't bound.");
            return;
        }

        if (bind.keyboard.KeyState & EAbOnKeyState::Press) {
            m_KeyPressMap.SetKeyToAction(bind, pThis, action);
        }
        else if (bind.keyboard.KeyState & EAbOnKeyState::Release) {
            m_KeyReleaseMap.SetKeyToAction(bind, pThis, action);
        }
        else if (bind.keyboard.KeyState & EAbOnKeyState::Continuous) {
            m_KeyContinuous.SetKeyToAction(bind, pThis, action);
        }

        m_BindsHandles[pThis] = bind;

        return;
    }

    if (bind.Type & EAbBindType::Mouse)
        m_MouseMap.SetKeyToAction(bind, pThis, mouseAction);
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Unbind(void* pThis)
{
    const auto& bind = m_BindsHandles.find(pThis);

    if (bind == m_BindsHandles.end()) {
        AB_LOG(Debug::Warning, L"Cannot unbind this bind from this UserInput, because UserInput doesn't handles it.");
        return;
    }

    const auto& inputBind = bind->second;

    if (inputBind.Type & EAbBindType::Keyboard) 
    {
        if (inputBind.keyboard.KeyState & EAbOnKeyState::Press) {
            m_KeyPressMap.UnSetKey(inputBind);
        }
        else if (inputBind.keyboard.KeyState & EAbOnKeyState::Release) {
            m_KeyReleaseMap.UnSetKey(inputBind);
        }
        else if (inputBind.keyboard.KeyState & EAbOnKeyState::Continuous) {
            m_KeyContinuous.UnSetKey(inputBind);
        }
    }
}

} // !App
