#include "Raycaster/VoxelGrid.hpp"
#include "Core.h"
#include "Debug/Logger.hpp"

namespace Voxels
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
WorldGrid::WorldGrid(size_t uGridWidth)
    : m_uGridDim(uGridWidth)
    , m_Cubes(uGridWidth * uGridWidth * uGridWidth)
    , m_uCubesCount(0)
    , m_VoxelGrid(GenerateGrid(m_uGridDim, m_Cubes))
    , m_Reupload(ON_STAGE)
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
    for (uint32_t z = uCubeStart; z < uCubeEnd; ++z) {
        for (uint32_t y = uCubeStart; y < uCubeEnd; ++y) {
            for (uint32_t x = uCubeStart; x < uCubeEnd; ++x) {
                uIndex = x + 
                         y * uDim +
                         z * uDim * uDim;

                voxelGrid[uIndex] = Voxel { };
                m_Cubes[m_uCubesCount] = GenerateCube(Vec3(x, y, z));
                voxelGrid[uIndex].Type  = 1;
                voxelGrid[uIndex].Id[0] = m_uCubesCount;
                ++m_uCubesCount;
            }
        }
    }

    for (uint32_t z = 0; z < uDim; ++z) {
        for (uint32_t y = 63; y < 64; ++y) {
            for (uint32_t x = 0; x < uDim; ++x) {
                uIndex = x +
                         y * uDim +
                         z * uDim * uDim;

                voxelGrid[uIndex] = Voxel { };
                m_Cubes[m_uCubesCount] = GenerateCube(Vec3(x, y, z));
                voxelGrid[uIndex].Type  = 1;
                voxelGrid[uIndex].Id[0] = m_uCubesCount;
                ++m_uCubesCount;
            }
        }
    }

    return voxelGrid;
}

// --------------------------------------------------------------------------------------------------------------------
Cube WorldGrid::GenerateCube(const Vec3& offsetPos)
{
    Cube c;
    c.SetPositon(offsetPos + c.GetHalfSize());
    return c;
}

} // !Voxels

