#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Voxels.hpp"
#include "Math/Vec3.hpp"
#include "Primitives/Cube.hpp"

#include "Raycaster/SingleVoxel.hpp"

namespace Voxels
{

class WorldGrid
{

    static constexpr size_t VoxelGridDim = 96;

public:

    BEAST_VOXEL_API explicit WorldGrid(size_t uGridWidth = VoxelGridDim);
    
    BEAST_VOXEL_API ~WorldGrid() = default;

public:

    const ::std::vector<Cube>& GetCubes() const
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
    void ModifyVoxel(iVec3 pos, U&& voxel)
    {
        size_t uIndex = pos.x +
                        pos.y * m_uGridDim +
                        pos.z * m_uGridDim * m_uGridDim;

        m_VoxelGrid[uIndex] = ::std::forward<U>(voxel);
    }

    template<typename U>
    void ModifyVoxel(size_t uIndex, U&& voxel)
    {
        m_VoxelGrid[uIndex] = ::std::forward<U>(voxel);
    }

private:

    ::std::vector<Voxel> GenerateGrid(size_t uGridWidth, ::std::vector<Cube>& vCubes);

    Cube GenerateCube(const Vec3& offsetPos);

private:

    size_t m_uGridDim = -1;

    ::std::vector<Cube> m_Cubes;
    size_t m_uCubesCount = -1;

    ::std::vector<Voxel> m_VoxelGrid;


};

} // !Voxels

#endif // !AB_VOXEL_GRID_H

