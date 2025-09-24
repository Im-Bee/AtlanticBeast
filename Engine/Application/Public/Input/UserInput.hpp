#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "ExportImport.h"
#include "KeyBind.hpp"
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
        , m_CurrentlyPressedKeys()
        , m_KeysStatusMap(AB_KEY_COUNT)
        , m_MouseMap()
    { 
        memset(&m_KeysStatusMap[0], 0, sizeof(int8_t) * m_KeysStatusMap.size());
    }

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

    void Bind(void* pThis, Action pIa, MouseAction mouseAction, InputBind bind);

    void Unbind(void* pThis);
    
    /**
     * Updates are based on key state in m_pWindowDesc
     * */
    void Update();

private:

    bool m_bIsCapturing;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

    ::std::unordered_map<void*, InputBind> m_BindsHandles;

    KeysMap m_KeyReleaseMap;
    KeysMap m_KeyPressMap;
    KeysMap m_KeyContinuous;
    KeysMap m_ButtonReleaseMap;
    KeysMap m_ButtonPressMap;
    
    ::std::vector<int8_t> m_CurrentlyPressedKeys;
    ::std::vector<int8_t> m_KeysStatusMap;

    MouseMap m_MouseMap;
};

} // !App



#endif // !AB_USER_INPUT_H
