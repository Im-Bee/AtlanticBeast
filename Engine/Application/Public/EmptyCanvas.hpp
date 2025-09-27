#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Window/IBaseWindow.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"

namespace App
{

/**
 * Basic empty window implementation, prefer this for renderer.
 * */
class EmptyCanvas : public ::App::IBaseWindow<EmptyCanvas, DefaultGameSystemWindowPolicy>
{
public:

    explicit EmptyCanvas(std::wstring wstrWindowName = L"EmptyCanvas")
        : IBaseWindow<EmptyCanvas, DefaultGameSystemWindowPolicy>(
                CreateWindowDesc(std::move(wstrWindowName), 1200, 700))
    { } 

    template<class U>
    explicit EmptyCanvas(U&& desc)
        : IBaseWindow<EmptyCanvas, DefaultGameSystemWindowPolicy>(::std::forward<U>(desc))
    { }
    
    ~EmptyCanvas() = default;

    EmptyCanvas(EmptyCanvas&&) noexcept = default;

    EmptyCanvas(const EmptyCanvas&) = delete;

public:

    void HandleMessageImpl(int32_t)
    { }

};

} // !App
#endif // !AB_EMPTYCANVAS_H

