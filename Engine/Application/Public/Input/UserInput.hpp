#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "ExportImport.h"
#include "Bind.h"
#include "Input/KeysMap.hpp"
#include "Window/WindowDesc.h"
#include "CSystem.hpp"
#include "MouseMap.hpp"

namespace App
{

class BEAST_API UserInput
{

public:

    explicit UserInput(::std::shared_ptr<WindowDesc> pWd = nullptr) 
        : m_bIsCapturing(false)
        , m_pWindowDesc(pWd)
        , m_KeyReleaseMap()
        , m_KeyPressMap()
        , m_KeyContinuous()
        , m_ButtonReleaseMap()
        , m_ButtonPressMap()
        , m_vCurrentlyPressedKeys()
        , m_MouseMap()
    { }

    ~UserInput() 
    { this->StopCapturing(); }

public:

    void StartCapturing()
    { 
        if (!m_bIsCapturing && m_pWindowDesc)
            m_bIsCapturing = true;
    }

    void StopCapturing()
    {
        if (!m_bIsCapturing)
            return;

        m_bIsCapturing = false;
    }

    void Bind(void* pThis, AbAction pIa, AbMouseAction mouseAction, AbInputBind bind);

    void Unbind(void* pThis);
    
    /**
     * Updates are based on key state in m_pWindowDesc
     * */
    void Update();

private:

    bool m_bIsCapturing;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

    ::std::unordered_map<void*, AbInputBind> m_BindsHandles;

    KeysMap m_KeyReleaseMap;
    KeysMap m_KeyPressMap;
    KeysMap m_KeyContinuous;
    KeysMap m_ButtonReleaseMap;
    KeysMap m_ButtonPressMap;
    
    ::std::bitset<AB_KEY_COUNT> m_vCurrentlyPressedKeys;

    MouseMap m_MouseMap;
};

} // !App



#endif // !AB_USER_INPUT_H
