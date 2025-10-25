#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Core.h"
#include "Debug/Logger.hpp"
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

    enum EReupload
    {
        NoAction = 1,
        RequestStaging = NoAction << 1,
        RequestGpuUpload = RequestStaging << 1,
    };

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

    /**
     * @brief Returns status that the upload is on, shifts the value to next stage
     *
     * @return EReupload enumerator that descirbes the stage
     */
    EReupload ReuploadStatus()
    {
        switch (m_Reupload) {
            case EReupload::NoAction:
                return EReupload::NoAction;
            case EReupload::RequestStaging:
                m_Reupload = EReupload::RequestGpuUpload;
                return EReupload::RequestStaging;
            case EReupload::RequestGpuUpload:
                m_Reupload = EReupload::NoAction;
                return EReupload::RequestGpuUpload;
            default:
                return EReupload::NoAction;
        }
    }

    void ForceUpload()
    { m_Reupload = EReupload::RequestStaging; }

public:

    template<typename U>
    void ModifyVoxel(iVec3 pos, U&& cube)
    {
        GenerateCube(Vec3::ToVec3(pos));
        m_Reupload = RequestStaging;
    }

private:

    ::std::vector<Voxel> GenerateGrid(size_t uGridWidth, ::std::vector<Cube>& vCubes);

    BEAST_VOXEL_API void GenerateCube(const Vec3& offsetPos);

private:

    size_t m_uGridDim = -1;

    ::std::vector<Cube> m_Cubes;
    size_t m_uCubesCount = -1;

    ::std::vector<Voxel> m_VoxelGrid;

    EReupload m_Reupload = NoAction;


};

} // !Voxels

#endif // !AB_VOXEL_GRID_H

