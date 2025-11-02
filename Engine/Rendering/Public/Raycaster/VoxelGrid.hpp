#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Voxels.hpp"

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

    BEAST_API explicit WorldGrid(size_t uGridWidth = VoxelGridDim);
    
    ~WorldGrid() = default;

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

public:

    void ForceUpload()
    { m_Reupload = EReupload::RequestStaging; }

public:

    template<typename U>
    void ModifyVoxel(iVec3 pos, U&& cube)
    {
        GenerateCube(Vec3::ToVec3(pos), m_VoxelGrid, m_Cubes, m_uCubesCount);
        m_Reupload = RequestStaging;
    }

private:

    ::std::vector<Voxel> GenerateGrid(const size_t uGridWidth,
                                      ::std::vector<Cube>& vCubes,
                                      size_t& uCubesState);

    BEAST_API void GenerateCube(const Vec3& offsetPos,
                                ::std::vector<Voxel>& vGrid,
                                ::std::vector<Cube>& vCubes,
                                size_t& uCubesState);

private:

    size_t m_uGridDim = -1;

    ::std::vector<Cube> m_Cubes;
    size_t m_uCubesCount = -1;

    ::std::vector<Voxel> m_VoxelGrid;

    EReupload m_Reupload = NoAction;


};

} // !Voxels
#endif // !AB_VOXEL_GRID_H
