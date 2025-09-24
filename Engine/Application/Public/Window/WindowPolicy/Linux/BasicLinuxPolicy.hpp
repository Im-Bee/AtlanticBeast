#ifndef AB_WINDOW_POLICY_H
#define AB_WINDOW_POLICY_H

#ifdef __linux__

#include "Window/WindowPolicy/IWindowPolicy.hpp"

namespace App
{

class BEAST_API BasicLinuxWindowPolicy : public IWindowPolicy<BasicLinuxWindowPolicy>
{
public:

	BasicLinuxWindowPolicy() = default;
	~BasicLinuxWindowPolicy() = default;

	BasicLinuxWindowPolicy(const BasicLinuxWindowPolicy&) = default;
	BasicLinuxWindowPolicy(BasicLinuxWindowPolicy&&) noexcept = default;

public:

	uint32_t CreateImpl(WindowDesc* pWd);

	void ShowImpl(WindowDesc* pWd);

	void HideImpl(WindowDesc* pWd);

	void DestroyImpl(WindowDesc* pWd);

    void UpdateImpl(WindowDesc* pWd);

    virtual uint32_t UpdateEvent(WindowDesc* pWd, XEvent& event);

};

} // !App
#endif // __linux__
#endif // !AB_WINDOW_POLICY_H
