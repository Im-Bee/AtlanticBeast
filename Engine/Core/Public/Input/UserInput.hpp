#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "ExportImport.h"
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
        , m_vHandlers() 
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

    void AddHandler()
    { }

    void Update();

private:

    bool m_bIsCapturing;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

    ::std::vector<int> m_vHandlers;

    KeysMap m_KeyReleaseMap;
    KeysMap m_KeyPressMap;
    KeysMap m_ButtonReleaseMap;
    KeysMap m_ButtonPressMap;

};

} // !Core


#endif // !AB_USER_INPUT_H
