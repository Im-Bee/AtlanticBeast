#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Body/BodyID.h>

#include "Core.h"
#include "Debug/Logger.hpp"
#include "Raycaster/VoxelGrid.hpp"
#include "Vec3.hpp"

namespace Layers
{
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr uint32_t NUM_LAYERS(2);
};

class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:

	BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	virtual uint32_t GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char*	GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	
                return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		
                return "MOVING";
		default:													
                JPH_ASSERT(false); 
                return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

class Physics
{
public:

    Physics(uint32_t uMaxBodies = 512)
        : m_uMaxBodies(uMaxBodies)
        , m_uMaxBodiesPairs(uMaxBodies)
        , m_uMaxContacts(uMaxBodies * 2)
        , m_Bpii()
        , m_Ovbplfi()
        , m_Olpfi()
        , m_pTempAlloc(nullptr)
        , m_pJobSystem(nullptr)
        , m_pPhysicsSystem(nullptr)
    { }

    ~Physics()
    { 
        DestroyJolt();
    }

public:

    Physics(const Physics&) noexcept = delete;
    Physics& operator=(const Physics&) noexcept = delete;

    Physics(Physics&&) noexcept = delete;
    Physics& operator=(Physics&&) noexcept = delete;

public:

    void InitializeJolt()
    {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory();

        JPH::RegisterTypes();

        m_pTempAlloc = ::std::make_unique<JPH::TempAllocatorImpl>(m_uAllocatedMem);

        m_pJobSystem = ::std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, 
                                                                    JPH::cMaxPhysicsBarriers,
                                                                    ::std::thread::hardware_concurrency() - 1);

        m_pPhysicsSystem = ::std::make_unique<JPH::PhysicsSystem>();
        m_pPhysicsSystem->Init(m_uMaxBodies, 0, m_uMaxBodiesPairs, m_uMaxContacts, m_Bpii, m_Ovbplfi, m_Olpfi);

        CreateFloor(); 
    }

    void Update(float fDelta)
    { 
        // Convert to seconds
        fDelta *= 0.001f;

        // If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = ::std::ceil(fDelta / (1.f / 60.f));

		// Step the world
		m_pPhysicsSystem->Update(fDelta, cCollisionSteps, m_pTempAlloc.get(), m_pJobSystem.get());
    }

    void DestroyJolt()
    {
        if (JPH::Factory::sInstance != nullptr) {
            delete JPH::Factory::sInstance;
            JPH::Factory::sInstance = nullptr;
        }
    }

public:

    void PushCube(const JPH::BodyID& bodyId, const Voxels::Vec3& normal, const float fForceMul) 
    {
        AB_ASSERT(m_pPhysicsSystem != nullptr);

        JPH::BodyInterface &body_interface = m_pPhysicsSystem->GetBodyInterface();

        JPH::Vec3 force = { };
        if (normal.x != 0) 
            force.SetX(-normal.x * fForceMul);
        else if (normal.y != 0) 
            force.SetY(-normal.y * fForceMul);
        else 
            force.SetZ(-normal.z * fForceMul);

        body_interface.SetLinearVelocity(bodyId, force);
    }

    JPH::BodyID CreateCube(const Voxels::Vec3& p)
    {
        using namespace JPH::literals;

        AB_ASSERT(m_pPhysicsSystem != nullptr);

        JPH::BodyInterface &body_interface = m_pPhysicsSystem->GetBodyInterface();
        JPH::BodyCreationSettings boxSettings(new JPH::BoxShape(JPH::Vec3(0.505_r, 0.505_r, 0.505_r)),
                                              JPH::RVec3(p.x, p.y, p.z), 
                                              JPH::Quat::sIdentity(), 
                                              JPH::EMotionType::Dynamic, 
                                              Layers::MOVING);

        return body_interface.CreateAndAddBody(boxSettings, JPH::EActivation::Activate);
    }


    Voxels::Vec3 GetCubePos(const JPH::BodyID& id)
    {
        AB_ASSERT(m_pPhysicsSystem != nullptr);

        JPH::BodyInterface &body_interface = m_pPhysicsSystem->GetBodyInterface();

        auto jv = body_interface.GetCenterOfMassPosition(id);
        
        return Voxels::Vec3(jv.GetX(), jv.GetY(), jv.GetZ());
    }

    Voxels::Rot3 GetCubeRot(const JPH::BodyID& id)
    {
        AB_ASSERT(m_pPhysicsSystem != nullptr);

        JPH::BodyInterface &body_interface = m_pPhysicsSystem->GetBodyInterface();

        auto euler = body_interface.GetRotation(id).GetEulerAngles();
        
        return Voxels::Rot3(euler.GetX(), euler.GetY(), euler.GetZ());
    }

private:

    void CreateFloor()
    {
        using namespace JPH::literals;

        AB_ASSERT(m_pPhysicsSystem != nullptr);

        JPH::BodyInterface &body_interface = m_pPhysicsSystem->GetBodyInterface();

        // Next we can create a rigid body to serve as the floor, we make a large box
        // Create the settings for the collision volume (the shape).
        // Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
        JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0_r, 1.0_r, 100.0_r));
        floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

        // Create the shape
        JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
        JPH::ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

        // Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
        JPH::BodyCreationSettings floor_settings(floor_shape, 
                                                 JPH::RVec3(0.0_r, 1.0_r, 0.0_r), 
                                                 JPH::Quat::sIdentity(),
                                                 JPH::EMotionType::Static,
                                                 Layers::NON_MOVING);
        floor_settings.mFriction = 0.23;

        // Add it to the world
        body_interface.CreateAndAddBody(floor_settings, JPH::EActivation::DontActivate);
    }

private:

    constexpr static const uint32_t m_uAllocatedMem = 10 * 1024 * 1024;
    const uint32_t m_uMaxBodies = -1;
    const uint32_t m_uMaxBodiesPairs = -1;
    const uint32_t m_uMaxContacts = -1;

    BPLayerInterfaceImpl m_Bpii;
    ObjectVsBroadPhaseLayerFilterImpl m_Ovbplfi;
    ObjectLayerPairFilterImpl m_Olpfi;

    ::std::unique_ptr<JPH::TempAllocatorImpl> m_pTempAlloc;
    ::std::unique_ptr<JPH::JobSystemThreadPool> m_pJobSystem;
    ::std::unique_ptr<JPH::PhysicsSystem> m_pPhysicsSystem;
};

// --------------------------------------------------------------------------------------------------------------------
class World : public ::Voxels::CubeWorld 
{
public:

    World()
        : ::Voxels::CubeWorld()
    { GenerateFloor(); }

private:

    void GenerateFloor()
    {
        const size_t uDim = this->GetGridWidth();
        ::Voxels::ColoredCube cc = { };

        for (uint32_t z = 0; z < uDim; ++z) {
            for (uint32_t y = 0; y < 2; ++y) {
                for (uint32_t x = 0; x < uDim; ++x) {
                    this->SetVoxel(Voxels::iVec3(x, y, z), 0x101010FF);
                }
            }
        }
    }
    
};

// --------------------------------------------------------------------------------------------------------------------
class InWorldCube 
{
public:

    InWorldCube(const ::std::shared_ptr<World>& pW, 
                size_t uC,
                Physics* pP,
                JPH::BodyID physicsId) 
        : m_pWorld(pW)
        , m_uCubeId(uC)
        , m_pPhysics(pP)
        , m_PhysicsId(std::move(physicsId))
    { }

    ~InWorldCube() = default;

public:

    InWorldCube(const InWorldCube&) noexcept = default;
    InWorldCube& operator=(const InWorldCube&) noexcept = default;

    InWorldCube(InWorldCube&&) noexcept = default;
    InWorldCube& operator=(InWorldCube&&) noexcept = default;

public:

    ::std::shared_ptr<World> GetWorld() const
    { return m_pWorld.lock(); }

    size_t GetId() 
    { return m_uCubeId; }

    const JPH::BodyID& GetPhysicsId() const
    { return m_PhysicsId; }

public:

    void Update(float fDelta)
    { 
        auto newPos = m_pPhysics->GetCubePos(m_PhysicsId);
        auto newRot = m_pPhysics->GetCubeRot(m_PhysicsId);

        if (auto pLock = m_pWorld.lock()) {
            pLock->UpdatePos(newPos, m_uCubeId);
            pLock->UpdateRot(newRot, m_uCubeId);
        }
    }

    Voxels::Vec3 GetPos()
    {
        if (auto pLock = m_pWorld.lock()) {
            return pLock->GetById(m_uCubeId).GetPosition();
        }

        throw AB_EXCEPT("Couldn't lock the world or the cube has wrong id.");
    }

private:

    ::std::weak_ptr<World> m_pWorld;

    size_t m_uCubeId = -1;

    Physics* m_pPhysics = nullptr;

    JPH::BodyID m_PhysicsId;

};

// --------------------------------------------------------------------------------------------------------------------
class Game
{
public:

    Game()
        : m_pWorld(::std::make_shared<World>())
        , m_vInWorldObjects()
    { }

    ~Game() = default;

public:

    Game(const Game&) noexcept = delete;
    Game& operator=(const Game&) noexcept = delete;

    Game(Game&&) noexcept = delete;
    Game& operator=(Game&&) noexcept = delete;

public:

    ::std::shared_ptr<World> GetWorld() const
    { return m_pWorld; }

public:

    void Initialize()
    {
        m_Physics.InitializeJolt();
    }

    void Update(float fDelta)
    { 
        for (auto& inWorldCube : m_vInWorldObjects) 
            inWorldCube.Update(fDelta);

        m_Physics.Update(fDelta);
    }

public:

    void GenerateCube(const Voxels::iVec3& p)
    {
        size_t uCubeId = m_pWorld->GenerateObjectAtVoxel(p, ::Voxels::ColoredCube());
        Voxels::Vec3 setP(0.5f + p.x, 0.5f + p.y, 0.5f + p.z); 
        
        m_vInWorldObjects.push_back(InWorldCube(m_pWorld, uCubeId, &m_Physics, m_Physics.CreateCube(setP)));
    }

public:

    size_t GetIdFromPos(const Voxels::iVec3& pos)
    {
        for (auto& c : m_vInWorldObjects) 
        {
            if (Voxels::iVec3::ToiVec3(c.GetPos()) == pos) 
                return c.GetId();
        }

        return -1;
    }

    void PushCube(size_t uCubeId, const Voxels::Vec3& normal, const float fForceMul) 
    {
        InWorldCube* pWC = nullptr;

        for (auto& c : m_vInWorldObjects) 
        {
            if (c.GetId() != uCubeId) 
                continue;

            pWC = &c;
            break;
        }

        if (!pWC) {
            AB_LOG(Core::Debug::Info, L"Invalid id on push");
            return;
        }

        m_Physics.PushCube(pWC->GetPhysicsId(), normal, fForceMul);
    }

private:

    ::std::shared_ptr<World> m_pWorld = nullptr;
    ::std::vector<InWorldCube> m_vInWorldObjects;
    Physics m_Physics;

};
