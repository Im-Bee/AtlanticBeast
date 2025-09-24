#ifndef AB_CONTROLLER_OBJECT_H
#define AB_CONTROLLER_OBJECT_H

#include "Core.h"
#include "Debug/Assert.h"
#include "Input/UserInput.hpp"
#include "KeyBind.hpp"

namespace App
{

class ControllerObject
{

    friend UserInput;

public:

    ControllerObject() = default;

    ~ControllerObject()
    {
        if (!m_pUserInput.expired())
            m_pUserInput.lock()->Unbind(this);
    }

    ControllerObject(const ControllerObject&) = delete;

    ControllerObject(ControllerObject&& other) noexcept
        : m_pUserInput(std::move(other.m_pUserInput))
    { }

public:

    void SignObject(::std::weak_ptr<UserInput> pUserInput)
    { 
        // We can be signed by only one UserInput
        if (!m_pUserInput.expired()) {
            AB_ASSERT(m_pUserInput.lock() == pUserInput.lock());
            return;
        }

        m_pUserInput = pUserInput;
    }

private:

    ::std::weak_ptr<UserInput> m_pUserInput;

};

} // !App

#define AB_DECL_ACTION(baseClass, action, customName, ...)              \
    static ::App::ActionType UseAction##customName(void* pThis)         \
    {                                                                   \
        AB_ASSERT(pThis != nullptr);                                    \
                                                                        \
        static_cast<baseClass*>(pThis)->action(__VA_ARGS__);            \
        return ::App::ActionType();                                     \
    }


#define AB_DECL_MOUSE_ACTION(baseClass, action, customName)                             \
    static ::App::ActionType UseAction##customName(void* pThis, float fX, float fY)     \
    {                                                                                   \
        AB_ASSERT(pThis != nullptr);                                                    \
                                                                                        \
        static_cast<baseClass*>(pThis)->action(fX, fY);                                 \
        return ::App::ActionType();                                                     \
    }


// ...
//
// class CameraController : public Camera, public ControllerObject
// { };
//

#endif // !AB_CONTROLLER_OBJECT_H

