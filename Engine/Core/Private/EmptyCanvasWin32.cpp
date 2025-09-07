#ifdef _WIN32

#include "Core.h"
#include "EmptyCanvas.hpp"

namespace Core
{

// EmptyCanvas // ------------------------------------------------------------------------------------------------------
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
