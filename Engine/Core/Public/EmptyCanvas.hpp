#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "IWindow.hpp"


namespace Core
{

class EmptyCanvas : public IWindow<EmptyCanvas>
{
    friend IWindow;

#ifdef _WIN32

	template<class T>
	friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

#endif // _WIN32

public:

    EmptyCanvas(::std::wstring Name = L"None")
        : m_WindowDesc({ 0 })
		, m_Name(::std::move(Name))
    { }

    ~EmptyCanvas() = default;

    EmptyCanvas(const EmptyCanvas& other) = default;

    EmptyCanvas(EmptyCanvas&& other) noexcept
        : m_WindowDesc(::std::move(other.m_WindowDesc))
    { 
        memset(&other.m_WindowDesc, 0, sizeof(WindowDesc));
    }

protected:

    void HandleMessage(int)
    { 
		// Special painting or other stuff can be here
		// AB_LOG(Debug::ESeverity::Info, L"EmptyCanvas received a message %p, with name %ls", this, this->m_WindowName.c_str());
    }

private:

    void CreateImpl();

    void ShowImpl();

    void HideImpl();

    void DestroyImpl();

    void UpdateImpl();

    const WindowDesc& GetWindowDescImpl() const
    { return m_WindowDesc; }

private:

    WindowDesc m_WindowDesc;
	::std::wstring m_Name;

};

} // !Core

#endif // !AB_EMPTYCANVAS_H
