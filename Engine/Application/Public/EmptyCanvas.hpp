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

    EmptyCanvas(std::wstring wstrWindowName = L"EmptyCanvas")
        : IBaseWindow<EmptyCanvas, DefaultGameSystemWindowPolicy>(
                CreateWindowDesc(wstrWindowName.c_str(), wstrWindowName.length(), 1200, 700))
    { }
    
    ~EmptyCanvas() = default;

    EmptyCanvas(EmptyCanvas&&) noexcept = default;

    EmptyCanvas(const EmptyCanvas&) = delete;

public:

    void HandleMessageImpl(EAbWindowEvents)
    { }

};

} // !App
#endif // !AB_EMPTYCANVAS_H

