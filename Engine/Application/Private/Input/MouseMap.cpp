#include "Input/MouseMap.hpp"

namespace App
{

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::BindActionImpl(const AbInputBind& ib, void* pThis, AbAction a, AbMouseAction ma)
{
	AB_ASSERT(ib.Type == EAbBindType::Mouse);
    AB_ASSERT(a == nullptr);
    AB_ASSERT(pThis != nullptr);

	m_vMouseBinds.push_back(DataForActionReplay { pThis, ma });
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::UnbindActionImpl(const AbInputBind& ib, void* pThis)
{
	AB_ASSERT(ib.Type == EAbBindType::Mouse);
    AB_ASSERT(pThis != nullptr);

    for (auto it = m_vMouseBinds.begin(); it != m_vMouseBinds.end(); ++it) {
        if (it->pThis == pThis) {
            m_vMouseBinds.erase(it);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void MouseMap::PlayAction(int32_t fX, int32_t fY)
{
	for (const auto& bind : m_vMouseBinds) {
		AB_ASSERT(bind.pThis);

		bind.action(bind.pThis, fX, fY);
    }
}

} // !App
