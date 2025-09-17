#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "Core.h"

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
        if (!m_bIsCapturing && m_pWindowDesc && StartCapturingInternal())
            m_bIsCapturing = true;
        else
            return;

    }

    void StopCapturing()
    {
        if (m_bIsCapturing)
            m_bIsCapturing = false;
        else
            return;

        StopCapturingInternal();
    }

    void AddHandler()
    { }

    void Update();

private:

    bool StartCapturingInternal();

    void StopCapturingInternal();

private:

    bool m_bIsCapturing;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

    ::std::vector<int> m_vHandlers;

};

} // !Core


#endif // !AB_USER_INPUT_H
