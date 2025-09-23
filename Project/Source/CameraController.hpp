#pragma once

#include "Voxels.hpp"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Primitives/Camera.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Raycaster/Rays.hpp"
#include "Math/Vec3.hpp"

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
        Voxels::HitResult hr = Voxels::MarchTheRay(m_vg.get(), this->GetPosition(), Voxels::Vec3(1., 0., 0.), 5);

        if (hr.bHit)
        {
            Voxels::Voxel v;
            v.Type = 1;
            v.RGBA = 0xFFFF00FF;
            m_vg->ModifyVoxel(hr.HitIndex + hr.Normal.x + hr.Normal.y + hr.Normal.z, v);
        }

    }

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

    AB_DECL_ACTION(PlayablePaper, PlaceBlock, PlaceBlock);

private:

    std::shared_ptr<Voxels::VoxelGrid> m_vg;

};
