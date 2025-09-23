#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Window/IBaseWindow.hpp"

namespace Core
{

/**
 * Basic empty window implementation, prefer this for renderer.
 * */
class EmptyCanvas : public ::Core::IBaseWindow<EmptyCanvas>
{
public:

    EmptyCanvas(std::wstring wstrWindowName = L"EmptyCanvas")
        : IBaseWindow<EmptyCanvas>(CreateWindowDesc(wstrWindowName.c_str(), wstrWindowName.length(), 1200, 700))
    { }
    
    ~EmptyCanvas() = default;

    EmptyCanvas(EmptyCanvas&&) noexcept = default;

    EmptyCanvas(const EmptyCanvas&) = delete;

public:

    void HandleMessageImpl(EAbWindowEvents)
    { }

};

} // !Core




#endif // !AB_EMPTYCANVAS_H

