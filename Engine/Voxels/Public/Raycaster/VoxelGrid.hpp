#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Math/Vec3.hpp"
#include "Voxels.hpp"

#include "Raycaster/SingleVoxel.hpp"

namespace Voxels
{

class BEAST_VOXEL_API VoxelGrid
{

    static constexpr size_t VoxelGridDim = 256;

public:

    explicit VoxelGrid(size_t uGridWidth = VoxelGridDim);
    
    ~VoxelGrid() = default;

public:

    const ::std::vector<Voxel>& GetGrid() const
    { return m_VoxelGrid; }

    size_t GetAmountOfVoxels() const
    { return m_VoxelGrid.size(); }

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

    ::std::vector<Voxel> GenerateGrid(size_t uGridWidth);

private:

    size_t m_uGridDim;

    ::std::vector<Voxel> m_VoxelGrid;

};

} // !Voxels

#endif // !AB_VOXEL_GRID_H

