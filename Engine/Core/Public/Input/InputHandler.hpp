#ifndef AB_INPUT_HANDLER_H
#define AB_INPUT_HANDLER_H

#include "Input/UserInput.hpp"

namespace Core
{

class InputBindableObject
{

    friend UserInput;

public:

    InputBindableObject() = default;

    ~InputBindableObject()
    {
        if (!m_pUserInput.expired())
            m_pUserInput.lock()->Unbind(this);
    }

private:

    void SignObject(::std::weak_ptr<UserInput> pUserInput)
    { 
        // We can be signed by only one UserInput
        if (m_pUserInput.use_count() != 0) {
            AB_ASSERT(m_pUserInput.lock() == pUserInput.lock());
        }

        m_pUserInput = pUserInput;
    }

private:

    ::std::weak_ptr<UserInput> m_pUserInput;

};

} // !Core

// ...
//
// class BindableCamera : public Camera, public InputBindableObject
// { };
//

#endif // !AB_INPUT_HANDLER_H

