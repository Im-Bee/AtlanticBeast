#include "MainCharacter.hpp"

void PlayablePaper::BindToInput(const ::std::shared_ptr<App::UserInput>& pInput)
{
    m_Controller.SignObject(pInput);

    AbInputBind ib;
    ib.Type = EAbBindType::Keyboard;
    ib.Keyboard = AbKeyboardBind{ EAbOnState::Continuous, App::AB_KEY_A };

    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveLeft, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_D;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveRight, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_W;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveFront, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_S;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveBack, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_Q;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveUp, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_E;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveDown, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_SPACE;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionPlaceBlock, nullptr, ib);

    ib.Keyboard.KeyCode = App::AB_KEY_X;
    ib.Keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionRemoveBlock, nullptr, ib);

    ib.Type = Mouse; 
    pInput->Bind(m_Character.get(), &m_Controller, nullptr, &PaperController::UseActionMouse, ib);
}
