#ifndef AB_WINDOW_LISTENER_H
#define AB_WINDOW_LISTENER_H

#include "Core.h"
#include "Window/WindowDesc.hpp"

namespace App
{

class WindowListener
{
public:

	explicit WindowListener(::std::shared_ptr<WindowDesc> pWd = nullptr)
		: m_pWindowDesc(pWd)
	{ }
	
	~WindowListener() = default;

public:

	void ListenToWindow(::std::shared_ptr<WindowDesc> pWd)
	{
		AB_ASSERT(pWd != nullptr);
		m_pWindowDesc = pWd;
	}

protected:

	const ::std::shared_ptr<WindowDesc>& GetWindowDesc() const
	{
		AB_ASSERT(m_pWindowDesc != nullptr);
		return m_pWindowDesc;
	}

private:

	::std::shared_ptr<WindowDesc> m_pWindowDesc;

};

} // !App

#endif //!AB_WINDOW_LISTENER_H