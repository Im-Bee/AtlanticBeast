#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "ExportImport.h"
#include "KeyBind.hpp"
#include "Input/KeysMap.hpp"
#include "Window/WindowDesc.h"
#include "CSystem.hpp"
namespace Core
{

class BEAST_API UserInput
{

public:

    explicit UserInput(::std::shared_ptr<WindowDesc> pWd = nullptr) 
        : m_bIsCapturing(false)
        , m_pWindowDesc(pWd)
        , m_KeyReleaseMap()
        , m_KeyPressMap()
        , m_ButtonReleaseMap()
        , m_ButtonPressMap()
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

    void Bind(void* pThis, Action pIa, InputBind bind);

    void Unbind(void* pThis);

    void Update();

private:

    bool m_bIsCapturing;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

    ::std::unordered_map<void*, InputBind> m_BindsHandles;

    KeysMap m_KeyReleaseMap;
    KeysMap m_KeyPressMap;
    KeysMap m_ButtonReleaseMap;
    KeysMap m_ButtonPressMap;

};

} // !Core


#endif // !AB_USER_INPUT_H
