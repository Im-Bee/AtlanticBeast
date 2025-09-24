#pragma once

#include "Voxels.hpp"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Primitives/Camera.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Raycaster/Rays.hpp"
#include "Math/Vec3.hpp"
#include "Math/Consts.hpp"

class PlayablePaper : public Voxels::Camera, 
                      public App::ControllerObject
{
public:

    template<class... U>
    explicit PlayablePaper(U&&... args)
        : m_vg(nullptr)
        , Camera(::std::forward<U>(args)...)
    { }


    ~PlayablePaper() = default;

public:

    void SetGrid(::std::shared_ptr<Voxels::VoxelGrid> vg)
    {
        m_vg = vg;
    }

public:

    void PlaceBlock()
    { 
        Voxels::Vec3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0., 0., 1. }, rot.x), rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_vg.get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit)
        {
            Voxels::Voxel v;
            v.Type = 1;
            v.RGBA = 0xFFFF00FF;
            m_vg->ModifyVoxel((hr.HitCoords.x + hr.Normal.x) + 
                              (hr.HitCoords.y + hr.Normal.y) * m_vg->GetGridWidth() + 
                              (hr.HitCoords.z + hr.Normal.z) * m_vg->GetGridWidth() * m_vg->GetGridWidth(), v);
        }
    }

    void MoveForwardBackwards(float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0., 0., 1. }, rot.y);

        this->AddPositon(lookDir * fDir);
    }

    void Strafe(float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0., 0., 1. }, rot.y + (90.f * Voxels::AB_DEG_TO_RAD));

        this->AddPositon(lookDir * fDir);
    }

    void MouseMove(float fX, float fY)
    {
        this->AddRotation(Voxels::Rot3 { fY * -0.0015f, fX * -0.0015f, 0.f });
    }

public:
    
    AB_DECL_ACTION(PlayablePaper, Strafe, MoveRight, -0.1f);

    AB_DECL_ACTION(PlayablePaper, Strafe, MoveLeft, 0.1f);

    AB_DECL_ACTION(PlayablePaper, MoveForwardBackwards, MoveFront, 0.1f);

    AB_DECL_ACTION(PlayablePaper, MoveForwardBackwards, MoveBack, -0.1f);

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveUp, Voxels::Vec3{ 0., 0.1, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveDown, Voxels::Vec3{ 0., -0.1, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, UpPitch, Voxels::Vec3{ 0.01, 0, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, DownPitch, Voxels::Vec3{ -0.01, 0, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, RightYaw, Voxels::Vec3{ 0., 0.01, 0. });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, LeftYaw, Voxels::Vec3{ 0., -0.01, 0. });

    AB_DECL_ACTION(Voxels::Camera, IncreaseFov, FovUp, 1.0);

    AB_DECL_ACTION(Voxels::Camera, IncreaseFov, FovDown, -1.0);

    AB_DECL_ACTION(PlayablePaper, PlaceBlock, PlaceBlock);

    AB_DECL_MOUSE_ACTION(PlayablePaper, MouseMove, Mouse);

private:

    std::shared_ptr<Voxels::VoxelGrid> m_vg;

};
