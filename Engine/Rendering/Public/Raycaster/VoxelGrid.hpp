#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Voxels.hpp"

#include "Primitives/BasicColoredCube.hpp"
#include "Vulkan/MemoryUploadTracker.hpp"
#include "Raycaster/SingleVoxel.hpp"

namespace Voxels
{

class WorldGrid : public MemoryUploadTracker
{

    using CubeType = ::Voxels::ColorCube;

    static constexpr size_t VoxelGridDim = 96;

public:

    BEAST_API explicit WorldGrid(size_t uGridWidth = VoxelGridDim);
    
    ~WorldGrid() = default;

public:

    const ::std::vector<CubeType>& GetCubes() const
    { return m_Cubes; }

    const ::std::vector<Voxel>& GetGrid() const
    { return m_VoxelGrid; }

    size_t GetAmountOfVoxels() const
    { return m_VoxelGrid.size(); }

    size_t GetAmountOfCubes() const
    { return m_Cubes.size(); }

    size_t GetGridWidth() const
    { return m_uGridDim; }

public:

    template<typename U>
    void ModifyVoxel(iVec3 pos, U&& cube)
    {
        GenerateCube(Vec3::ToVec3(pos), m_VoxelGrid, m_Cubes, m_uCubesCount);
        this->ForceUpload();
    }

private:

    ::std::vector<Voxel> GenerateGrid(const size_t uGridWidth,
                                      ::std::vector<CubeType>& vCubes,
                                      size_t& uCubesState);

    BEAST_API void GenerateCube(const Vec3& offsetPos,
                                ::std::vector<Voxel>& vGrid,
                                ::std::vector<CubeType>& vCubes,
                                size_t& uCubesState);

private:

    size_t m_uGridDim = -1;

    ::std::vector<CubeType> m_Cubes;
    size_t m_uCubesCount = -1;

    ::std::vector<Voxel> m_VoxelGrid;

};

} // !Voxels
#endif // !AB_VOXEL_GRID_H
