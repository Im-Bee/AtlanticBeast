#include "VoxelGrid.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
VoxelGrid::VoxelGrid()
    : m_VoxelGrid(GenerateGrid())
{ }

// ---------------------------------------------------------------------------------------------------------------------
vector<Voxel> VoxelGrid::GenerateGrid()
{
    vector<Voxel>   voxelGrid(VoxelGridDim * VoxelGridDim * VoxelGridDim);
    size_t          uDim = VoxelGridDim;

    for (auto& voxel : voxelGrid) {
        voxel.Type = 0;
    }

    size_t uIndex;
    size_t uCubeStart   = 22;
    size_t uCubeEnd     = 44;
    for (size_t z = uCubeStart; z < uCubeEnd; ++z) {
        for (size_t y = uCubeStart; y < uCubeEnd; ++y) {
            for (size_t x = uCubeStart; x < uCubeEnd; ++x) {
                uIndex = x + y * uDim + z * uDim * uDim;

                voxelGrid[uIndex].Type  = 1;
                voxelGrid[uIndex].R     = 222;
                voxelGrid[uIndex].G     = 22;
                voxelGrid[uIndex].B     = 212;
                voxelGrid[uIndex].A     = 255;
            }
        }
    }

    return voxelGrid;
}

} // !Voxels

