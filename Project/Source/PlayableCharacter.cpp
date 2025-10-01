#include "CameraController.hpp"

void PlayablePaper::BindToInput(const ::std::shared_ptr<App::UserInput>& pInput)
{
    m_Controller.SignObject(pInput);

    AbInputBind ib;
    ib.Type = EAbBindType::Keyboard;
    ib.keyboard = AbKeyboardBind{ EAbOnKeyState::Continuous, App::AB_KEY_A };

    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveLeft, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_D;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveRight, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_W;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveFront, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_S;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveBack, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_Q;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveUp, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_E;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionMoveDown, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_1;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionLeftYaw, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_2;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionRightYaw, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_3;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionUpPitch, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_4;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionDownPitch, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_5;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionFovUp, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_6;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionFovDown, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_SPACE;
    ib.keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionPlaceBlock, nullptr, ib);

    ib.keyboard.KeyCode = App::AB_KEY_X;
    ib.keyboard.KeyState = Press;
    pInput->Bind(m_Character.get(), &m_Controller, &PaperController::UseActionRemoveBlock, nullptr, ib);

    ib.Type = Mouse; 
    pInput->Bind(m_Character.get(), &m_Controller, nullptr, &PaperController::UseActionMouse, ib);
}