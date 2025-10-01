#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "Window/IBaseWindow.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"

namespace App
{

/**
 * Basic empty window implementation for rendering and game. Has it's own built in UserInput instance.
 *
 * @param bManualInputUpdate - if true, input update will not be called automatically on every window update,
 * this way you can manualy update the input on another thread or at a different time.
 * */
template<bool bManualInputUpdate = false>
class EmptyCanvas : public ::App::IBaseWindow<EmptyCanvas<bManualInputUpdate>, DefaultGameSystemWindowPolicy>
{

	friend class IBaseWindow<EmptyCanvas<bManualInputUpdate>, DefaultGameSystemWindowPolicy>;

public:

    explicit EmptyCanvas(std::wstring wstrWindowName = L"EmptyCanvas")
        : IBaseWindow<EmptyCanvas, DefaultGameSystemWindowPolicy>(
                CreateWindowDesc(std::move(wstrWindowName), 1200, 700))
		, m_pInput(::std::make_shared<UserInput>(this->GetWindowDesc()))
    { } 

    template<class U>
    explicit EmptyCanvas(U&& desc)
        : IBaseWindow<EmptyCanvas, DefaultGameSystemWindowPolicy>(::std::forward<U>(desc))
    { }
    
    ~EmptyCanvas() = default;

    EmptyCanvas(EmptyCanvas&&) noexcept = default;

    EmptyCanvas(const EmptyCanvas&) = delete;

public:

    const ::std::shared_ptr<UserInput>& GetInput() const
	{ return m_pInput; }

private:

    /**
    * @brief It's called on every update 
    */
    void HandleMessageImpl(EAbWindowEventsFlags events)
    { 
        if constexpr (!bManualInputUpdate) {
            m_pInput->Update();
        }
    }

private:

	::std::shared_ptr<UserInput> m_pInput;

};

} // !App
#endif // !AB_EMPTYCANVAS_H

