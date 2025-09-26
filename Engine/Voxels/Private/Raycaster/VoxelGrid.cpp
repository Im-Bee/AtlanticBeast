#include "Raycaster/VoxelGrid.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
VoxelGrid::VoxelGrid(size_t uGridWidth)
    : m_uGridDim(uGridWidth)
    , m_VoxelGrid(GenerateGrid(m_uGridDim))
{ }

// ---------------------------------------------------------------------------------------------------------------------
vector<Voxel> VoxelGrid::GenerateGrid(size_t uGridWidth)
{
    vector<Voxel>   voxelGrid(uGridWidth * uGridWidth * uGridWidth);
    size_t          uDim = VoxelGridDim;

    for (auto& voxel : voxelGrid) {
        voxel.Type = 0; 
    }

    size_t uIndex;
    size_t uCubeStart   = 33;
    size_t uCubeEnd     = 44;
	voxelGrid[500].Type = 1;
    voxelGrid[500].RGBA = 0xFF0000FF;
    voxelGrid[1000].Type = 1;
    voxelGrid[1000].RGBA = 0xFF0000FF;
    voxelGrid[800].Type = 1;
    voxelGrid[800].RGBA = 0xFF0000FF;
    voxelGrid[550].Type = 1;
    voxelGrid[550].RGBA = 0xFF0000FF;
    voxelGrid[508].Type = 1;
    voxelGrid[508].RGBA = 0xFF0000FF;
    for (size_t z = uCubeStart; z < uCubeEnd; ++z) {
        for (size_t y = uCubeStart; y < uCubeEnd; ++y) {
            for (size_t x = uCubeStart; x < uCubeEnd; ++x) {
                uIndex = x + 
                         y * uDim +
                         z * uDim * uDim;

                voxelGrid[uIndex].Type  = 1;
                voxelGrid[uIndex].RGBA  = 0xFF0000FF;
            }
        }
    }

    uCubeStart = 33;
    uCubeEnd = 44;
    for (size_t z = 0; z < uDim; ++z) {
        for (size_t y = 63; y < 64; ++y) {
            for (size_t x = 0; x < uDim; ++x) {
                uIndex = x +
                    y * uDim +
                    z * uDim * uDim;

                voxelGrid[uIndex].Type = 1;
                voxelGrid[uIndex].RGBA = 0x42DD42FF;
            }
        }
    }

    return voxelGrid;
}

} // !Voxels

