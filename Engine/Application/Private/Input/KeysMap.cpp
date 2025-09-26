#include "Input/KeysMap.hpp"

namespace App
{

// KeysMap // ----------------------------------------------------------------------------------------------------------
KeysMap::KeysMap()
    : m_vKeys(AmountOfBindableKeys)
{
    memset(&m_vKeys[0], 0, sizeof(DataForActionReplay) * m_vKeys.size());
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::SetKeyToAction(const AbInputBind& ib, void* pThis, AbAction a)
{
	AB_ASSERT(ib.Type == EAbBindType::Keyboard);
	AB_ASSERT(ib.keyboard.KeyCode > AB_INVALID_KEY && ib.keyboard.KeyCode < AB_KEY_COUNT);
	AB_ASSERT(m_vKeys[ib.keyboard.KeyCode].pThis == nullptr);

	m_vKeys[ib.keyboard.KeyCode] = DataForActionReplay { pThis, a };
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::UnSetKey(const AbInputBind& ib)
{
	AB_ASSERT(ib.Type == EAbBindType::Keyboard);
	AB_ASSERT(ib.keyboard.KeyCode > AB_INVALID_KEY || ib.keyboard.KeyCode < AB_KEY_COUNT);

	m_vKeys[ib.keyboard.KeyCode] = DataForActionReplay { 0, 0 };
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::PlayAction(AbKeyId keyCode)
{
	AB_ASSERT(keyCode > AB_INVALID_KEY && keyCode < AB_KEY_COUNT);

	const auto& playableAction = m_vKeys[keyCode];

	if (playableAction.pThis)
		playableAction.action(playableAction.pThis);
}

} // !App
