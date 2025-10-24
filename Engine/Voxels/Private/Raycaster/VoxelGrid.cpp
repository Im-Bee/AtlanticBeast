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
    , m_Reupload(RequestStaging)
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
    size_t uCubeStart = 11;
    size_t uCubeEnd   = 22;
    for (uint32_t z = uCubeStart; z < uCubeEnd; ++z) {
        for (uint32_t y = uCubeStart; y < uCubeEnd; ++y) {
            for (uint32_t x = uCubeStart; x < uCubeEnd; ++x) {
                GenerateCube(Vec3(x, y, z));
            }
        }
    }

    for (uint32_t z = 0; z < uDim; ++z) {
        for (uint32_t y = 32; y < 33; ++y) {
            for (uint32_t x = 0; x < uDim; ++x) {
                const size_t uIndex = x + 
                                      y * uDim +
                                      z * uDim * uDim;

                if (m_VoxelGrid[uIndex].Type == 0) {
                    m_VoxelGrid[uIndex] = Voxel { };
                }
                m_VoxelGrid[uIndex].Type    = -1;
                m_VoxelGrid[uIndex].Color   = 0xFFFF00FF;
            }
        }
    }

    return voxelGrid;
}

// --------------------------------------------------------------------------------------------------------------------
void WorldGrid::GenerateCube(const Vec3& offsetPos)
{
    static uint32_t kc = 0;
    const size_t uDim = this->GetGridWidth();
    const size_t uIndex = offsetPos.x + 
                          offsetPos.y * uDim +
                          offsetPos.z * uDim * uDim;

    if (m_VoxelGrid[uIndex].Type == 0) {
        m_VoxelGrid[uIndex] = Voxel { };
    }

    m_VoxelGrid[uIndex].Id[m_VoxelGrid[uIndex].Type++] = m_uCubesCount;

    Cube c;
    c.SetPositon(offsetPos + Vec3(0.9f, 0.5f, 0.5f));
    auto cubePos = c.GetPosition();
    auto cubeSizes = c.GetHalfSize();

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                if (x == 0 && y == 0 && z == 0)
                    continue;

                size_t uCornerIndex = (offsetPos.x + x) + 
                                      (offsetPos.y + y) * uDim + 
                                      (offsetPos.z + z) * uDim * uDim;

                if (uCornerIndex < m_VoxelGrid.size() && m_VoxelGrid[uCornerIndex].Type != -1) {
                    m_VoxelGrid[uCornerIndex].Id[m_VoxelGrid[uCornerIndex].Type++] = m_uCubesCount;
                }
            }
        }
    }
    if (kc == 0) {
        c.SetColor(0xFF000000);
        ++kc;
    } 
    else if (kc == 1) {
        c.SetColor(0x00FF0000);
        ++kc;
    }
    else {
        c.SetColor(0x0000FF00);
        kc = 0;
    }
    m_Cubes[m_uCubesCount++] = c;
}

} // !Voxels

