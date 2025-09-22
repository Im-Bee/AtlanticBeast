#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Window/IBaseWindow.hpp"

namespace Core
{

/**
 * Basic empty window implementation.
 * */
class EmptyWindow : public ::Core::IBaseWindow<EmptyWindow>
{
public:

    EmptyWindow()
        : IBaseWindow<EmptyWindow>(CreateWindowDesc(L"EmptyWindow", 1200, 700))
    { }
    
    template<class U>
    explicit EmptyWindow(U&& windowDesc)
        : IBaseWindow<EmptyWindow>(::std::forward<U>(windowDesc))
    { }

    ~EmptyWindow() = default;

    EmptyWindow(EmptyWindow&&) noexcept = default;

    EmptyWindow(const EmptyWindow&) = delete;

public:

    virtual void HandleMessageImpl(EAbWindowEvents)
    {
        // AB_LOG(Debug::Info, L"Proccessing a message! ... %u", msg);
    }

};

} // !Core




#endif // !AB_EMPTYCANVAS_H
