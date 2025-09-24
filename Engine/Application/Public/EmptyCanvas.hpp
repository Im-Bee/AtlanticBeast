#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Window/IBaseWindow.hpp"
#include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"

namespace App
{

/**
 * Basic empty window implementation, prefer this for renderer.
 * */
class EmptyCanvas : public ::App::IBaseWindow<EmptyCanvas, GameLinuxWindowPolicy>
{
public:

    EmptyCanvas(std::wstring wstrWindowName = L"EmptyCanvas")
        : IBaseWindow<EmptyCanvas, GameLinuxWindowPolicy>(
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

