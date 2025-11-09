#pragma once

#include "Raycaster/VoxelGrid.hpp"

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
                    this->SetVoxel(Voxels::iVec3(x, y, z), 0xAA0090FF);
                }
            }
        }
    }
    
};

// --------------------------------------------------------------------------------------------------------------------
class InWorldCube 
{
public:

    InWorldCube(const ::std::shared_ptr<World>& pW, size_t uC) 
        : m_pWorld(pW)
        , m_uCubeId(uC)
        , m_pPhysics(nullptr)
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

public:

    void Update(float fDelta)
    { }

private:

    ::std::weak_ptr<World> m_pWorld;

    size_t m_uCubeId = -1;

    void* m_pPhysics = nullptr;

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

    Game(const Game&) noexcept = default;
    Game& operator=(const Game&) noexcept = default;

    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;

public:

    ::std::shared_ptr<World> GetWorld() const
    { return m_pWorld; }

public:

    void Update(float fDelta)
    { 
        for (auto& inWorldCube : m_vInWorldObjects) 
            inWorldCube.Update(fDelta);
    }

private:

    ::std::shared_ptr<World> m_pWorld = nullptr;
    ::std::vector<InWorldCube> m_vInWorldObjects;

};
