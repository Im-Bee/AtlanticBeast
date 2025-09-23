#ifndef AB_IWINDOW_POLICY_H
#define AB_IWINDOW_POLICY_H

#include "Window/WindowDesc.h"

namespace App
{

template<class Policy>
class IWindowPolicy
{
public:

	IWindowPolicy() = default;
	~IWindowPolicy() = default;

	IWindowPolicy(const IWindowPolicy&) = default;
	IWindowPolicy(IWindowPolicy&&) noexcept = default;

public:

	uint32_t WindowPolicyCreate(WindowDesc* pWd)
	{ return static_cast<Policy*>(this)->CreateImpl(pWd); }

	void WindowPolicyShow(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->ShowImpl(pWd); }

	void WindowPolicyHide(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->HideImpl(pWd); }

	void WindowPolicyDestroy(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->DestroyImpl(pWd); }

	void WindowPolicyUpdate(WindowDesc* pWd)
	{ static_cast<Policy*>(this)->UpdateImpl(pWd); }

};

} // !App

#endif // !AB_IWINDOW_POLICY_H
