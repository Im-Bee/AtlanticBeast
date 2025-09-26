#include "Input/MouseMap.hpp"

namespace App
{

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::SetKeyToAction(const AbInputBind& ib, void* pThis, AbMouseAction a)
{
	AB_ASSERT(ib.Type == EAbBindType::Mouse);

	m_vMouseBinds.push_back(DataForActionReplay { pThis, a });
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::UnSetKey(const AbInputBind& ib)
{
	AB_ASSERT(ib.Type == EAbBindType::Mouse);

	static_assert(true, "Not impemented yet");
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::PlayAction(float fX, float fY)
{
	for (const auto& bind : m_vMouseBinds)
		if (bind.pThis)
			bind.action(bind.pThis, fX, fY);
}

} // !App
