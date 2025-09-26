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
        , m_vCurrentlyPressedKeys()
        , m_vKeysStatusMap(AB_KEY_COUNT)
        , m_MouseMap()
    { 
        memset(&m_vKeysStatusMap[0], 0, sizeof(int8_t) * m_vKeysStatusMap.size());
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
    
    ::std::vector<int8_t> m_vCurrentlyPressedKeys;
    ::std::vector<int8_t> m_vKeysStatusMap;

    MouseMap m_MouseMap;
};

} // !App



#endif // !AB_USER_INPUT_H
