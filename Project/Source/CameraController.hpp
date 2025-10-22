#pragma once

#include "Voxels.hpp"
#include "Input/ControllerObject.hpp"
#include "Input/UserInput.hpp"
#include "Input/KeyList.hpp"
#include "Primitives/Camera.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Raycaster/Rays.hpp"
#include "Math/Math.hpp"

class PaperCharacter : public Voxels::Camera
{
public:

    template<class... U>
    explicit PaperCharacter(U&&... args)
        : m_vg(nullptr)
        , Camera(::std::forward<U>(args)...)
    { }


    ~PaperCharacter() = default;

public:

    void SetGrid(::std::shared_ptr<Voxels::VoxelGrid> vg)
    {
        m_vg = vg;
    }

public:

    void PlaceBlock()
    {
        Voxels::Vec3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.x), rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_vg.get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit)
        {
            Voxels::Voxel v;
            v.Type = 1;
            v.Id1 = (m_uColor += 12739871) | 0x999999FF;
            static const float f = .5f;
            memcpy(&v.Id2, &f, sizeof(uint32_t));
            m_vg->ModifyVoxel(static_cast<size_t>((hr.iHitCoords.x + hr.Normal.x) +
                                                  (hr.iHitCoords.y + hr.Normal.y) * m_vg->GetGridWidth() +
                                                  (hr.iHitCoords.z + hr.Normal.z) * m_vg->GetGridWidth() * m_vg->GetGridWidth()),
                              v);
        }
    }

    void RemoveBlock()
    {
        Voxels::Vec3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.x), rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_vg.get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit)
        {
            Voxels::Voxel v;
            v.Type = 0;
            m_vg->ModifyVoxel(hr.iHitCoords.x +
                              hr.iHitCoords.y * m_vg->GetGridWidth() +
                              hr.iHitCoords.z * m_vg->GetGridWidth() * m_vg->GetGridWidth(), v);
        }
    }

    void MoveForwardBackwards(float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.y);

        this->AddPositon(lookDir * fDir);
    }

    void Strafe(float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.y + (90.f * Voxels::AB_DEG_TO_RAD));

        this->AddPositon(lookDir * fDir);
    }

    void MouseMove(int32_t fX, int32_t fY)
    {
        this->AddRotation(Voxels::Rot3{ -0.0015f * fY, -0.0015f * fX, 0.f });
    }

private:

    ::std::shared_ptr<Voxels::VoxelGrid> m_vg;

    uint32_t m_uColor;

};

class PaperController : public App::ControllerObject
{
public:

    PaperController() = default;

    ~PaperController() = default;

public:

    AB_DECL_ACTION(PaperCharacter, Strafe, MoveRight, -0.1f);

    AB_DECL_ACTION(PaperCharacter, Strafe, MoveLeft, 0.1f);

    AB_DECL_ACTION(PaperCharacter, MoveForwardBackwards, MoveFront, 0.1f);

    AB_DECL_ACTION(PaperCharacter, MoveForwardBackwards, MoveBack, -0.1f);

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveUp, Voxels::Vec3{ 0.f, 0.1f, 0.f });

    AB_DECL_ACTION(Voxels::Camera, AddPositon, MoveDown, Voxels::Vec3{ 0.f, -0.1f, 0.f });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, UpPitch, Voxels::Vec3{ 0.01f, 0.f, 0.f });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, DownPitch, Voxels::Vec3{ -0.01f, 0.f, 0.f });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, RightYaw, Voxels::Vec3{ 0.f, 0.01f, 0.f });

    AB_DECL_ACTION(Voxels::Camera, AddRotation, LeftYaw, Voxels::Vec3{ 0.f, -0.01f, 0.f });

    AB_DECL_ACTION(Voxels::Camera, IncreaseFov, FovUp, 1.0f);

    AB_DECL_ACTION(Voxels::Camera, IncreaseFov, FovDown, -1.0f);

    AB_DECL_ACTION(PaperCharacter, PlaceBlock, PlaceBlock);

    AB_DECL_ACTION(PaperCharacter, RemoveBlock, RemoveBlock);

    AB_DECL_MOUSE_ACTION(PaperCharacter, MouseMove, Mouse);

};

class PlayablePaper
{
public:

    PlayablePaper()
        : m_Character(::std::make_shared<PaperCharacter>())
        , m_Controller()
    { }

    ~PlayablePaper() = default;

public:

    void BindToInput(const ::std::shared_ptr<App::UserInput>& pInput);

    ::std::shared_ptr<PaperCharacter>& GetCharacter()
    {
        return m_Character;
    }

private:

    ::std::shared_ptr<PaperCharacter> m_Character;
    PaperController m_Controller;

};
