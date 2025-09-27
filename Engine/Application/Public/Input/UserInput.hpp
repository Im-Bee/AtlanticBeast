#ifndef AB_USER_INPUT_H
#define AB_USER_INPUT_H

#include "Window/WindowDesc.hpp"
#include "Input/KeysMap.hpp"
#include "Input/Bind.h"
#include "Input/MouseMap.hpp"

namespace App
{

class ControllerObject;

class BEAST_API UserInput
{

public:

    explicit UserInput(::std::shared_ptr<WindowDesc> pWd = nullptr) 
        : m_bIsCapturing(false)
        , m_pWindowDesc(pWd)
        , m_KeyReleaseMap()
        , m_KeyPressMap()
        , m_KeyContinuous()
        , m_ButtonReleaseMap()
        , m_ButtonPressMap()
        , m_vCurrentlyPressedKeys()
        , m_MouseMap()
    { }

    ~UserInput() = default;

public:

    void StartCapturing();

    void StopCapturing();

    /**
     * @param pThis - pointer to an object on which we are performing action
     * @param pCo - pointer to an object that controlls life time of pThis
     * @param action - action to be performed, should be null, if we are performing mouse action instead
     * @param mouseAction - mouse action to be performed, should be null, if we are performing action instead 
     * @param bind - AbInputBind struct describing the bind
     **/
    void Bind(void* pThis, ControllerObject* pCo, AbAction action, AbMouseAction mouseAction, AbInputBind bind);

    void Unbind(void* pThis);
    
    /**
     * Reads and consumes the input queue from WindowDesc.
     * Plays continues binds.
     */
    void Update();

private:

    bool m_bIsCapturing;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

    ::std::unordered_map<void*, ::std::vector<AbInputBind>> m_BindsHandles;

    ::std::bitset<AB_KEY_COUNT> m_vCurrentlyPressedKeys;

    KeysMap m_KeyReleaseMap;
    KeysMap m_KeyPressMap;
    KeysMap m_KeyContinuous;
    KeysMap m_ButtonReleaseMap;
    KeysMap m_ButtonPressMap;

    MouseMap m_MouseMap;

};

} // !App



#endif // !AB_USER_INPUT_H
