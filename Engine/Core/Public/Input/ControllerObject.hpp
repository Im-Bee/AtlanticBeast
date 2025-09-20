#ifndef AB_CONTROLLER_OBJECT_H
#define AB_CONTROLLER_OBJECT_H

#include "Input/UserInput.hpp"
#include "KeyBind.hpp"

namespace Core
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

private:

    void SignObject(::std::weak_ptr<UserInput> pUserInput, InputBind kb)
    { 
        // We can be signed by only one UserInput
        if (!m_pUserInput.expired()) {
            AB_ASSERT(m_pUserInput.lock() == pUserInput.lock());
            return;
        }

        m_pUserInput = pUserInput;
        m_KeyBind = kb;
    }

private:

    ::std::weak_ptr<UserInput> m_pUserInput;

    InputBind m_KeyBind;

};

} // !Core

#define AB_DECL_ACTION(baseClass, action, customName, ...)              \
    static ::Core::ActionType UseAction##customName(void* pThis)        \
    {                                                                   \
        AB_ASSERT(pThis != nullptr);                                    \
        AB_LOG(::Core::Debug::Info, L"Pressing " L#customName);          \
                                                                        \
        static_cast<baseClass*>(pThis)->action(__VA_ARGS__);            \
        return ::Core::ActionType();                                    \
    }


// ...
//
// class CameraController : public Camera, public ControllerObject
// { };
//

#endif // !AB_CONTROLLER_OBJECT_H

