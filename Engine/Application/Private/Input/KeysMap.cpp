#include "Input/KeysMap.hpp"

namespace App
{

// KeysMap // ----------------------------------------------------------------------------------------------------------
KeysMap::KeysMap()
    : m_vKeys(AmountOfBindableKeys)
{
    memset(&m_vKeys[0], 0, sizeof(ActionReplayData) * m_vKeys.size());
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::BindActionImpl(const AbInputBind& ib, void* pThis, AbAction a, AbMouseAction ma)
{
	AB_ASSERT(ib.Type == EAbBindType::Keyboard);
	AB_ASSERT(ib.keyboard.KeyCode > AB_INVALID_KEY && ib.keyboard.KeyCode < AB_KEY_COUNT);
	AB_ASSERT(m_vKeys[ib.keyboard.KeyCode].pThis == nullptr);
    AB_ASSERT(ma == nullptr);
    AB_ASSERT(pThis != nullptr);

	m_vKeys[ib.keyboard.KeyCode] = ActionReplayData { pThis, a };
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::UnbindActionImpl(const AbInputBind& ib, void* pThis)
{
	AB_ASSERT(ib.Type == EAbBindType::Keyboard);
	AB_ASSERT(ib.keyboard.KeyCode > AB_INVALID_KEY && ib.keyboard.KeyCode < AB_KEY_COUNT);
    AB_ASSERT(pThis != nullptr);

	m_vKeys[ib.keyboard.KeyCode].pThis = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
void KeysMap::PlayAction(AbKeyId keyCode)
{
	AB_ASSERT(keyCode > AB_INVALID_KEY && keyCode < AB_KEY_COUNT);

	const auto& playableAction = m_vKeys[keyCode];

    if (playableAction.pThis) {
	    playableAction.action(playableAction.pThis);
    }
}

} // !App
