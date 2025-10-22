#include "Raycaster/VoxelGrid.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
WorldGrid::WorldGrid(size_t uGridWidth)
    : m_uGridDim(uGridWidth)
    , m_Cubes()
    , m_VoxelGrid(GenerateGrid(m_uGridDim, m_Cubes))
{ }

// ---------------------------------------------------------------------------------------------------------------------
vector<Voxel> WorldGrid::GenerateGrid(size_t uGridWidth, vector<Cube>& vCubes)
{
    vector<Voxel>   voxelGrid(uGridWidth * uGridWidth * uGridWidth);
    size_t          uDim = VoxelGridDim;

    for (auto& voxel : voxelGrid) {
        voxel.Type = 0; 
    }

    size_t uIndex;
    size_t uCubeStart = 33;
    size_t uCubeEnd   = 44;
    for (size_t z = uCubeStart; z < uCubeEnd; ++z) {
        for (size_t y = uCubeStart; y < uCubeEnd; ++y) {
            for (size_t x = uCubeStart; x < uCubeEnd; ++x) {
                uIndex = x + 
                         y * uDim +
                         z * uDim * uDim;

                voxelGrid[uIndex] = Voxel { };
                voxelGrid[uIndex].Type  = 1;
                voxelGrid[uIndex].Id[0]   = 0xFF0000FF;
            }
        }
    }

    for (size_t z = 0; z < uDim; ++z) {
        for (size_t y = 63; y < 64; ++y) {
            for (size_t x = 0; x < uDim; ++x) {
                uIndex = x +
                    y * uDim +
                    z * uDim * uDim;

                voxelGrid[uIndex] = Voxel { };
                voxelGrid[uIndex].Type  = 1;
                voxelGrid[uIndex].Id[0]   = 0x42DD42FF;
            }
        }
    }

    return voxelGrid;
}

} // !Voxels

