#include "Input/MouseMap.hpp"

namespace App
{

// MouseMap // ---------------------------------------------------------------------------------------------------------
MouseMap::MouseMap()
{ }

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::SetKeyToAction(const InputBind& ib, void* pThis, MouseAction a)
{
	AB_ASSERT(ib.Type == EBindType::Mouse);

	m_MouseBinds.push_back(DataForActionReplay { pThis, a });
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::UnSetKey(const InputBind& ib)
{
	AB_ASSERT(ib.Type == EBindType::Mouse);

	static_assert(true, "Not impemented yet");
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::PlayAction(float fX, float fY)
{
	for (const auto& bind : m_MouseBinds)
	{
		if (bind.pThis) {
			bind.action(bind.pThis, fX, fY);
		}
	}
}

} // !App
