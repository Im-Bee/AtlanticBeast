#include "Input/KeysMap.hpp"

namespace App
{

// KeysMap // ----------------------------------------------------------------------------------------------------------
KeysMap::KeysMap()
    : m_Keys(AmountOfBindableKeys)
{
    memset(&m_Keys[0], 0, sizeof(DataForActionReplay) * m_Keys.size());
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::SetKeyToAction(const InputBind& ib, void* pThis, Action a)
{
	AB_ASSERT(ib.Type == EBindType::Keyboard);
	AB_ASSERT(ib.keyboard.KeyCode > AB_INVALID_KEY && ib.keyboard.KeyCode < AB_KEY_COUNT);
	AB_ASSERT(m_Keys[ib.keyboard.KeyCode].pThis == nullptr);

	m_Keys[ib.keyboard.KeyCode] = DataForActionReplay { pThis, a };
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::UnSetKey(const InputBind& ib)
{
	AB_ASSERT(ib.Type == EBindType::Keyboard);
	AB_ASSERT(ib.keyboard.KeyCode > AB_INVALID_KEY && ib.keyboard.KeyCode < AB_KEY_COUNT);

	m_Keys[ib.keyboard.KeyCode] = DataForActionReplay { 0, 0 };
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::PlayAction(int8_t keyCode)
{
	AB_ASSERT(keyCode > AB_INVALID_KEY && keyCode < AB_KEY_COUNT);

	const auto& playableAction = m_Keys[keyCode];

	if (playableAction.pThis) {
		playableAction.action(playableAction.pThis);
	}
}

} // !App
