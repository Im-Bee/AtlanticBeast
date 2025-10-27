#ifndef AB_EMPTYWINDOW_H
#define AB_EMPTYWINDOW_H

#include "Window/IBaseWindow.hpp"

namespace App
{

/**
 * Basic empty window implementation.
 * */
class EmptyWindow : public ::App::IBaseWindow<EmptyWindow>
{
	friend class IBaseWindow<EmptyWindow>;

public:

    explicit EmptyWindow(::std::wstring wstrName = L"EmptyWindow")
        : IBaseWindow<EmptyWindow>(CreateWindowDesc(std::move(wstrName), 1200, 700))
    { }
    
    template<class U>
    explicit EmptyWindow(U&& windowDesc)
        : IBaseWindow<EmptyWindow>(::std::forward<U>(windowDesc))
    { }

    ~EmptyWindow() = default;

    EmptyWindow(EmptyWindow&&) noexcept = default;

    EmptyWindow(const EmptyWindow&) = delete;

private:

    virtual void HandleMessageImpl(const float, EAbWindowEventsFlags)
    {
        // AB_LOG(Debug::Info, L"Proccessing a message! ... %u", msg);
    }

};

} // !App
#endif // !AB_EMPTYWINDOW_H
