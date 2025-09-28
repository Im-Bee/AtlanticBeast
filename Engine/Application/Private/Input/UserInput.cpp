#include "Input/UserInput.hpp"
#include "Input/InputEvents.h"
#include "Input/ControllerObject.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::StartCapturing()
{ 
    if (!m_bIsCapturing && m_pWindowDesc)
        m_bIsCapturing = true;
}

void UserInput::StopCapturing()
{
    if (!m_bIsCapturing)
        return;

    m_bIsCapturing = false;
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Update()
{ 
    // Always replay the continuos keybinds that are currently pressed
    if (m_vCurrentlyPressedKeys.any()) {
        for (AbKeyId i = 0; i < AB_KEY_COUNT; ++i) {
            if (!m_vCurrentlyPressedKeys.test(i)) {
                continue;
            }

            m_KeyContinuous.PlayAction(i);
        }
    }
    
    while (!m_pWindowDesc->InputStruct.empty()) {
        AbInputStruct& is = m_pWindowDesc->InputStruct.front();
        m_pWindowDesc->InputStruct.pop();

        switch (is.Event) {
            case EAbInputEvents::AbKeyPress: {
                AbKeyId key = is.Keyboard.KeyId;

                if (!m_bIsCapturing || key <= AB_INVALID_KEY || key >= AB_KEY_COUNT)
                    continue;

                if (m_vCurrentlyPressedKeys.test(key))
                    continue;

                m_vCurrentlyPressedKeys.set(key);

                m_KeyPressMap.PlayAction(key);
                m_KeyContinuous.PlayAction(key);
                continue;
            }

            case EAbInputEvents::AbKeyRelease: {
                AbKeyId key = is.Keyboard.KeyId;

                if (!m_bIsCapturing || key <= AB_INVALID_KEY || key >= AB_KEY_COUNT)
                    continue;

                if (!m_vCurrentlyPressedKeys.test(key))
                    continue;

                m_vCurrentlyPressedKeys.flip(key);

                m_KeyReleaseMap.PlayAction(key);
                continue;
            }

            case EAbInputEvents::AbButtonPress:
            case EAbInputEvents::AbButtonRelease:
                // AB_LOG(Debug::Info, L"Key press: %d", is.KeyId);
                continue;

            case EAbInputEvents::AbMotion:
                m_MouseMap.PlayAction(is.Mouse.MouseX, is.Mouse.MouseY);
                is.Mouse.MouseX = 0;
                is.Mouse.MouseY = 0;
                continue;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Bind(void* pThis, ControllerObject* pCo, AbAction action, AbMouseAction mouseAction, AbInputBind bind)
{ 
    if (pCo->m_pUserInput.lock().get() != this) {
        AB_LOG(Core::Debug::Info, L"Object must be signed by the UserInput, before binding");
        return;
    }

    if (bind.Type & EAbBindType::Keyboard) 
    {
        if (bind.keyboard.KeyCode <= AB_INVALID_KEY || bind.keyboard.KeyCode >= AB_KEY_COUNT) {
            AB_LOG(Debug::Error, L"Key code is an invalid code (code outside of boundries for keys).");
            AB_LOG(Debug::Error, L"Can't bind the action for the keyboard.");
            AB_LOG(Debug::Error, L"Action wasn't bound.");
            return;
        }

        if (bind.keyboard.KeyState & EAbOnKeyState::Press) {
            m_KeyPressMap.BindAction(bind, pThis, action, nullptr);
        }
        else if (bind.keyboard.KeyState & EAbOnKeyState::Release) {
            m_KeyReleaseMap.BindAction(bind, pThis, action, nullptr);
        }
        else if (bind.keyboard.KeyState & EAbOnKeyState::Continuous) {
            m_KeyContinuous.BindAction(bind, pThis, action, nullptr);
        }

        m_BindsHandles[pThis].push_back(bind);
    }
    else if (bind.Type & EAbBindType::Mouse) {
        m_MouseMap.BindAction(bind, pThis, nullptr, mouseAction);
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void UserInput::Unbind(void* pThis)
{
    const auto& handle = m_BindsHandles.find(pThis);
    
    if (handle == m_BindsHandles.end()) {
        AB_LOG(Debug::Warning, L"Cannot unbind this bind from this UserInput, because UserInput doesn't handles it.");
        return;
    }

    for (const auto& inputBind : handle->second) {

        if (inputBind.Type & EAbBindType::Keyboard) 
        {
            if (inputBind.keyboard.KeyState & EAbOnKeyState::Press) {
                m_KeyPressMap.UnbindAction(inputBind, pThis);
            }
            else if (inputBind.keyboard.KeyState & EAbOnKeyState::Release) {
                m_KeyReleaseMap.UnbindAction(inputBind, pThis);
            }
            else if (inputBind.keyboard.KeyState & EAbOnKeyState::Continuous) {
                m_KeyContinuous.UnbindAction(inputBind, pThis);
            }
        }
        else if (inputBind.Type & EAbBindType::Mouse)
        {
            m_MouseMap.UnbindAction(inputBind, pThis);
        }
    }

    m_BindsHandles.erase(handle);
}

} // !App
