#ifdef _WIN32

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
    AB_LOG(Debug::ESeverity::Info, L"Creating canvas");
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::ShowImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Show canvas");
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::HideImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Hide canvas");
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::DestroyImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Destroy canvas");
}

// ---------------------------------------------------------------------------------------------------------------------
void UpdateImpl()
{
}

} // !Core
#endif // !_WIN32
