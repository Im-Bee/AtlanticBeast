#pragma once

#include "Core.h"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"

class PaperWeightCamera
{

public:

    void Move()
    {
        AB_LOG(Core::Debug::Info, L"I'm moving");
    }

};

class PlayablePaper : public PaperWeightCamera, 
                              public Core::ControllerObject
{
public:

    template<class... U>
    explicit PlayablePaper(U&&... args)
        : PaperWeightCamera(::std::forward<U>(args)...)
    { }


    ~PlayablePaper() = default;

public:
    
    AB_DECL_ACTION(PaperWeightCamera, Move);

};
