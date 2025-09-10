#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Core.h"
#include "IBaseWindow.hpp"

namespace Core
{

class EmptyCanvas : public IBaseWindow<EmptyCanvas>
{
public:

    EmptyCanvas()
        : IBaseWindow<EmptyCanvas>(CreateWindowDesc(L"EmptyCanvas", sizeof(L"EmptyCanvas"), 1200, 700))
    { }

    void HandleMessageImpl(uint32_t msg) 
    {
        // AB_LOG(Debug::Info, L"Proccessing a message! ... %u", msg);
    }

};

} // !Core

#endif // !AB_EMPTYCANVAS_H
