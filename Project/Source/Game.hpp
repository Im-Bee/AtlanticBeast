#pragma once

#include "Voxels.hpp"
#include "Primitives/Cube.hpp"
#include "Primitives/MaterialProperties/ColorProperty.hpp"
#include "Raycaster/VoxelGrid.hpp"

class alignas(16) ColoredCube : public Voxels::Cube
                              , public Voxels::ColorProperty
{ };

class World : public ::Voxels::WorldGrid<ColoredCube> 
{
public:

    World()
        : ::Voxels::WorldGrid<ColoredCube>()
    { GenerateFloor(); }

private:

    void GenerateFloor()
    {
        const size_t uDim = this->GetGridWidth();
        ColoredCube cc = { };
        cc.SetColor(0xAA0090FF);

        for (uint32_t z = 0; z < uDim; ++z) {
            for (uint32_t y = 0; y < 2; ++y) {
                for (uint32_t x = 0; x < uDim; ++x) {
                    this->GenerateAtVoxel(Voxels::iVec3(x, y, z), cc);
                }
            }
        }
    }
    
};

class InWorldCube 
{
public:

    InWorldCube(const ::std::shared_ptr<World>& pW, ColoredCube& pC) 
        : m_pWorld(pW)
        , m_pCube(&pC)
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

    ColoredCube* GetCube() const
    { return (m_pWorld.expired() ? nullptr : m_pCube); }

public:

    void Update(float fDelta)
    { }

private:

    ::std::weak_ptr<World> m_pWorld;

    ColoredCube* m_pCube = nullptr;

    void* m_pPhysics = nullptr;

};

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

private:

    ::std::shared_ptr<World> m_pWorld = nullptr;
    ::std::vector<InWorldCube> m_vInWorldObjects;

};
