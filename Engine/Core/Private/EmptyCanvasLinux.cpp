#ifdef __linux__

#include "Core.h"
#include "EmptyCanvas.hpp"

namespace Core
{

// EmptyCanvas::pResources // ------------------------------------------------------------------------------------------
struct EmptyCanvas::pResources
{
    pResources() = default;
    ~pResources() = default;

    pResources(const pResources&) = default;
    pResources(pResources&&) = default;
};

// EmptyCanvas // ------------------------------------------------------------------------------------------------------
EmptyCanvas::EmptyCanvas()
    : m_pResources(new pResources())
    , m_WindowDesc()
{ }

// ---------------------------------------------------------------------------------------------------------------------
EmptyCanvas::~EmptyCanvas()
{
    delete m_pResources;
}

// ---------------------------------------------------------------------------------------------------------------------
EmptyCanvas::EmptyCanvas(const EmptyCanvas& other)
    : m_pResources(new pResources(*other.m_pResources))
    , m_WindowDesc(other.m_WindowDesc)
{ }

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::CreateImpl()
{
    if (m_WindowDesc.Display) {
        return;
    }

    AB_LOG(Debug::ESeverity::Info, L"Creating canvas");

    m_WindowDesc.Display = AskForDisplayLinux(NULL);
    if (m_WindowDesc.Display == NULL) {
        throw AB_EXCEPT("Couldn't open a connection to X server");
    }

    int screen = DefaultScreen(m_WindowDesc.Display);
    auto* display = m_WindowDesc.Display;
    Window window = XCreateSimpleWindow(
        m_WindowDesc.Display,
        RootWindow(display, screen),
        100, 100, 
        400, 300, 
        1,
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    m_WindowDesc.Screen = DefaultScreen(display);
    m_WindowDesc.Window = window;
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::ShowImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Show canvas");

    XMapWindow(m_WindowDesc.Display, m_WindowDesc.Window);
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::HideImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Hide canvas");

    XUnmapWindow(m_WindowDesc.Display, m_WindowDesc.Window);
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::DestroyImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Destroy canvas");

    XUnmapWindow(m_WindowDesc.Display, m_WindowDesc.Window);

    AskToCloseDisplayLinux(NULL);
    m_WindowDesc.Display = NULL;
}
 
// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::UpdateImpl()
{
    XEvent event;
    ::Display* display = m_WindowDesc.Display;
    ::Window window = m_WindowDesc.Window;
    int screen = m_WindowDesc.Screen;

    if (!display) {
        return;
    }

    XNextEvent(display, &event);
    if (event.type == Expose)
    {
        XDrawString(display, 
                window,
                DefaultGC(display, screen),
                10,
                20,
                "Hello, explicit display!", 
                24);
    }
    else if (event.type == KeyPress) 
    {
        this->Destroy();
        return;
    }
}

} // !Core
#endif // !__linux__
