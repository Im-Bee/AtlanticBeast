#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Voxels.hpp"

#include "SingleVoxel.hpp"

namespace Voxels
{

class BEAST_VOXEL_API VoxelGrid
{

    static constexpr size_t VoxelGridDim = 64;

public:

    VoxelGrid();
    
    ~VoxelGrid() = default;

public:

    const ::std::vector<Voxel>& GetGrid() const
    { return m_VoxelGrid; }

    size_t GetAmountOfVoxels() const
    { return m_VoxelGrid.size(); }

private:

    ::std::vector<Voxel> GenerateGrid();

private:

    ::std::vector<Voxel> m_VoxelGrid;

};

} // !Voxels

#endif // !AB_VOXEL_GRID_H

