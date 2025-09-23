#pragma once

#include "Voxels.hpp"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Primitives/Camera.hpp"

class PlayablePaper : public Voxels::Camera, 
                      public App::ControllerObject
{
public:

    template<class... U>
    explicit PlayablePaper(U&&... args)
        : Camera(::std::forward<U>(args)...)
    { }


    ~PlayablePaper() = default;

public:
    
    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveRight, Voxels::Vec3 { 0.1, 0, 0 });

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveLeft, Voxels::Vec3 { -0.1, 0, 0 });

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveFront, Voxels::Vec3 { 0., 0, 0.1 });

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveBack, Voxels::Vec3{ 0., 0, -0.1 });

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveUp, Voxels::Vec3{ 0., 0.1, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveDown, Voxels::Vec3{ 0., -0.1, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, UpPitch, Voxels::Vec3{ 0.01, 0, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, DownPitch, Voxels::Vec3{ -0.01, 0, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, RightYaw, Voxels::Vec3{ 0., 0.01, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, LeftYaw, Voxels::Vec3{ 0., -0.01, 0. });

    AB_DECL_ACTION(Voxels::Camera, IncreaseFov, FovUp, 1.0);

    AB_DECL_ACTION(Voxels::Camera, IncreaseFov, FovDown, -1.0);

};
