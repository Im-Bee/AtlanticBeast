#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "ExportImport.h"
#include "Window/WindowDesc.h"
#include "CSystem.hpp"

namespace Core
{

class BEAST_API UserInput
{
public:

    UserInput() 
        : m_bIsCapturing(false)
        , m_pWindowDesc(nullptr)
        , m_vHandlers()
    { }

    explicit UserInput(::std::shared_ptr<WindowDesc> pWd) 
        : m_bIsCapturing(false)
        , m_pWindowDesc(pWd)
        , m_vHandlers()
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



private:

    bool m_bIsCapturing;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

    ::std::vector<int> m_vHandlers;

};

} // !Core


#endif // !AB_USER_INPUT_H
