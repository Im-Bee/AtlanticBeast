#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "IWindow.hpp"


namespace Core
{

class EmptyCanvas : public IWindow<EmptyCanvas>
{
    friend IWindow;

public:

    EmptyCanvas();

    ~EmptyCanvas();

    EmptyCanvas(const EmptyCanvas& other);

    EmptyCanvas(EmptyCanvas&& other) 
        : m_pResources(other.m_pResources)
    { 
        other.m_pResources = nullptr;
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

    struct pResources;

    pResources* m_pResources;

    WindowDesc m_WindowDesc;

};

} // !Core

#endif // !AB_EMPTYCANVAS_H
