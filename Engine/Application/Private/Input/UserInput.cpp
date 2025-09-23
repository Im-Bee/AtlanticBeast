#include "Input/UserInput.hpp"
#include "Input/InputEvents.h"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Update()
{ 
    // Always replay the continuos keybinds that are currently pressed
    for (const auto& keyId : m_CurrentlyPressedKeys)
        m_KeyContinuous.PlayAction(keyId);
    
    AbInputStruct& is = m_pWindowDesc->InputStruct;

    if (is.Handled || !m_bIsCapturing || (is.KeyId < AB_INVALID_KEY && is.KeyId >= AB_KEY_COUNT)) {
        return;
    }

    switch (is.Event) {
        case EAbInputEvents::AbKeyPress:
            if (m_KeysStatusMap[is.KeyId] == EKeyState::IsPressed) {
                break;
            }
            m_KeysStatusMap[is.KeyId] = EKeyState::IsPressed;

            m_KeyPressMap.PlayAction(is.KeyId);
       
            m_CurrentlyPressedKeys.push_back(is.KeyId);
            m_KeyContinuous.PlayAction(is.KeyId);
            break;

        case EAbInputEvents::AbKeyRelease:
            if (m_KeysStatusMap[is.KeyId] == EKeyState::IsReleased) {
                break;
            }
            m_KeysStatusMap[is.KeyId] = EKeyState::IsReleased;

            m_KeyReleaseMap.PlayAction(is.KeyId);

            for (size_t i = 0; i < m_CurrentlyPressedKeys.size(); ++i) {
                if (m_CurrentlyPressedKeys[i] == is.KeyId) {
                    m_CurrentlyPressedKeys.erase(m_CurrentlyPressedKeys.begin() + i,
                                                 m_CurrentlyPressedKeys.begin() + i + 1);
                }
            }
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

        if (bind.keyboard.KeyState & EOnKeyState::Press) {
            m_KeyPressMap.SetKeyToAction(bind, pThis, action);
        }
        else if (bind.keyboard.KeyState & EOnKeyState::Release) {
            m_KeyReleaseMap.SetKeyToAction(bind, pThis, action);
        }
        else if (bind.keyboard.KeyState & EOnKeyState::Continuous) {
            m_KeyContinuous.SetKeyToAction(bind, pThis, action);
        }

        m_BindsHandles[pThis] = bind;

        return;
    }

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

    if (inputBind.Type & EBindType::Keyboard) {
        if (inputBind.keyboard.KeyState & EOnKeyState::Press) {
            m_KeyPressMap.UnSetKey(inputBind);

            return;
        }

        if (inputBind.keyboard.KeyState & EOnKeyState::Release) {
            m_KeyReleaseMap.UnSetKey(inputBind);

            return;
        }

        if (inputBind.keyboard.KeyState & EOnKeyState::Continuous) {
            m_KeyContinuous.UnSetKey(inputBind);

            return;
        }
    }
}

} // !App
