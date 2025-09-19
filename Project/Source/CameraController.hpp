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

class PaperWeightController : public PaperWeightCamera, 
                              public Core::ControllerObject
{
public:
    
    AB_DECL_ACTION(PaperWeightCamera, Move);

};
