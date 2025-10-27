#pragma once

#include "Math/Vec3.hpp"
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
        , m_fSpeed(0.1f)
    { }


    ~PaperCharacter() = default;

public:

    void SetGrid(::std::shared_ptr<Voxels::WorldGrid> vg)
    {
        m_vg = vg;
    }

public:

    void PlaceBlock(const float)
    {
        Voxels::Vec3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.x), rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_vg.get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit) {
            m_vg->ModifyVoxel(Voxels::iVec3(hr.iHitCoords + hr.Normal), Voxels::Cube());
        }
    }

    void RemoveBlock(const float)
    {
        Voxels::Vec3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::Normalize(Voxels::RotateY(Voxels::RotateX(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.x), rot.y));

        Voxels::HitResult hr = Voxels::MarchTheRay(m_vg.get(), this->GetPosition(), lookDir, 10);

        if (hr.bHit)
        {
        }
    }

    void MoveForwardBackwards(const float fDelta, float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.y);

        this->AddPositon(lookDir * fDir * (fDelta * m_fSpeed));
    }

    void Strafe(const float fDelta, float fDir)
    {
        Voxels::Rot3 rot = this->GetRotation();
        Voxels::Vec3 lookDir = Voxels::RotateY(Voxels::Vec3{ 0.f, 0.f, 1.f }, rot.y + (90.f * Voxels::AB_DEG_TO_RAD));

        this->AddPositon(lookDir * fDir * (fDelta * m_fSpeed));
    }

    void MouseMove(const float fDelta, int32_t fX, int32_t fY)
    {
        this->AddRotation(Voxels::Rot3{ -0.0045f * fY * (fDelta * 0.01f), -0.0045f * fX * (fDelta * 0.01f), 0.f });
    }

    void Move(const float fDelta, const Voxels::Vec3& dir)
    {
        this->AddPositon(dir * (fDelta * m_fSpeed));
    }

private:

    ::std::shared_ptr<Voxels::WorldGrid> m_vg;

    uint32_t m_uColor;

    const float m_fSpeed = -1.f;

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

    AB_DECL_ACTION(PaperCharacter, Move, MoveUp, Voxels::Vec3{ 0.f, 0.1f, 0.f });

    AB_DECL_ACTION(PaperCharacter, Move, MoveDown, Voxels::Vec3{ 0.f, -0.1f, 0.f });

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
