#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "IWindow.hpp"


namespace Core
{

class EmptyCanvas : public IWindow<EmptyCanvas>
{
    friend IWindow;

public:

    EmptyCanvas() = default;

    ~EmptyCanvas() = default;

    EmptyCanvas(const EmptyCanvas& other) = default;

    EmptyCanvas(EmptyCanvas&& other) 
        : m_WindowDesc(::std::move(other.m_WindowDesc))
    { 
        memset(&other.m_WindowDesc, 0, sizeof(WindowDesc));
    }

private:

    void CreateImpl();

    void ShowImpl();

    void HideImpl();

    void DestroyImpl();

    void UpdateImpl();

    const WindowDesc& GetWindowDescImpl() const
    { return m_WindowDesc; }

private:

    WindowDesc m_WindowDesc;

};

} // !Core

#endif // !AB_EMPTYCANVAS_H
