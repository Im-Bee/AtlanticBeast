#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "Window/WindowDesc.hpp"
#include "Window/WindowListener.hpp"
#include "Input/Bind.h"
#include "Input/KeyList.hpp"

namespace App
{

class ControllerObject;

class UserInput : public WindowListener
{

public:

    BEAST_API explicit UserInput(::std::shared_ptr<WindowDesc> pWd = nullptr);

    BEAST_API ~UserInput();

public:

    BEAST_API void StartCapturing();

    BEAST_API void StopCapturing();

    /**
     * @brief Binds an action or mouse action to a key or mouse.
     *
     * @param pThis - pointer to an object on which we are performing action
     * @param pCo - pointer to an object that controlls life time of pThis
     * @param action - action to be performed, should be null, if we are performing mouse action instead
     * @param mouseAction - mouse action to be performed, should be null, if we are performing action instead 
     * @param bind - AbInputBind struct describing the bind
     **/
    BEAST_API void Bind(void* pThis, ControllerObject* pCo, AbAction action, AbMouseAction mouseAction, AbInputBind bind);

    BEAST_API void Unbind(ControllerObject* pCo);
    
    /**
     * Reads and consumes the input queue from WindowDesc.
     * Plays continues binds.
     */
    BEAST_API void Update();

private:

    struct BindHandle
    {
        AbInputBind Ib;
        void* pThis;
    };

private:

    bool m_bIsCapturing;

    ::std::unordered_map<void*, ::std::vector<BindHandle>> m_BindsHandles;

    ::std::bitset<AB_KEY_COUNT> m_vCurrentlyPressedKeys;

    struct Impl;
    Impl* m_pImpl = nullptr;

};

} // !App



#endif // !AB_USER_INPUT_H
