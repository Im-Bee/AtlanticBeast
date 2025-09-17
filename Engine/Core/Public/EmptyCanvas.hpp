#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Window/IBaseWindow.hpp"

namespace Core
{

class EmptyCanvas : public ::Core::IBaseWindow<EmptyCanvas>
{
public:

    EmptyCanvas()
        : IBaseWindow<EmptyCanvas>(CreateWindowDesc(L"EmptyCanvas",
                                                    sizeof(L"EmptyCanvas"), 
                                                    1200, 700))
    { }
    
    explicit EmptyCanvas(const wchar_t* pwszName)
        : IBaseWindow<EmptyCanvas>(CreateWindowDesc(pwszName,
                                                    sizeof(L"EmptyCanvas"),
                                                    1200, 700))
    { }
    
    void HandleMessageImpl(EAbWindowEvents)
    {
        // AB_LOG(Debug::Info, L"Proccessing a message! ... %u", msg);
    }

};

} // !Core




#endif // !AB_EMPTYCANVAS_H
