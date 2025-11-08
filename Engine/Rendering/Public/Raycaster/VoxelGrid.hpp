#ifndef AB_VOXEL_GRID_H
#define AB_VOXEL_GRID_H

#include "Voxels.hpp"

#include "Vulkan/MemoryUploadTracker.hpp"
#include "Raycaster/SingleVoxel.hpp"
#include "Primitives/ColoredCube.hpp"

namespace Voxels
{

class IWorldGrid : public MemoryUploadTracker
{
protected:

    static constexpr size_t DefaultVoxelGridDim = 96;

public:

    explicit IWorldGrid(size_t uGridWidth = DefaultVoxelGridDim)
        : m_uGridDim(uGridWidth)
        , m_VoxelGrid(uGridWidth * uGridWidth * uGridWidth)
    { }

public:

    const void* GetGridPtr() const
    { return m_VoxelGrid.data(); }

    ::std::vector<Voxel>& GetGrid()
    { return m_VoxelGrid; }

    const ::std::vector<Voxel>& GetGrid() const
    { return m_VoxelGrid; }

    size_t GetVoxelsSizeInBytes() const
    { return m_VoxelGrid.size() * sizeof(Voxel); }

    size_t GetGridWidth() const
    { return m_uGridDim; }

    size_t GetAmountOfVoxels() const
    { return m_VoxelGrid.size(); }

    virtual const void* GetObjectsPtr() const = 0;
    
    virtual size_t GetObjectsSizeInBytes() const = 0;

public:

    void SetVoxel(const iVec3& pos, uint32_t uColor)
    {
        ::std::vector<Voxel>& voxelsGrid = this->GetGrid();
        const size_t uDim = this->GetGridWidth(); 
        const size_t uIndex = pos.x + 
                              pos.y * uDim +
                              pos.z * uDim * uDim;


        if (uIndex < voxelsGrid.size())
        {
            voxelsGrid[uIndex].Type = -1;
            voxelsGrid[uIndex].Color = uColor;
        }

        this->ForceUpload();
    }

public:

    virtual bool CheckIfVoxelOccupied(const iVec3& pos) const = 0;

private:

    size_t m_uGridDim = -1;
    ::std::vector<Voxel> m_VoxelGrid;

};

template<class StoredObjectType>
class WorldGrid : public IWorldGrid
{
public:

    explicit WorldGrid(size_t uGridWidth = IWorldGrid::DefaultVoxelGridDim)
        : IWorldGrid(uGridWidth)
        , m_StoredObjects(this->GetVoxelsSizeInBytes() / sizeof(Voxel))
        , m_uObjectsCount(0)
    { }

public:

    virtual const void* GetObjectsPtr() const override 
    { return m_StoredObjects.data(); }
    
    virtual size_t GetObjectsSizeInBytes() const override 
    { return m_StoredObjects.size() * sizeof(StoredObjectType); }

    virtual bool CheckIfVoxelOccupied(const iVec3& pos) const override
    { 
        const ::std::vector<Voxel>& voxelsGrid = this->GetGrid();
        const size_t uDim = this->GetGridWidth(); 
        const size_t uIndex = pos.x + 
                              pos.y * uDim +
                              pos.z * uDim * uDim;

        if (voxelsGrid[uIndex].Type == 0) {
            return false;
        }

        for (uint32_t i = 0; i < voxelsGrid[uIndex].Type; ++i) {
            if (iVec3::ToiVec3(m_StoredObjects[voxelsGrid[uIndex].Id[i]].GetPosition()) == pos) {
                return true;
            }
        }
        
        return false;
    }

public:

    template<class U>
    StoredObjectType* GenerateAtVoxel(const iVec3& pos, U&& sot)
    {
        StoredObjectType* pR = GenerateObject(pos, this->GetGrid(), ::std::forward<U>(sot));
        this->ForceUpload();
        return pR;
    }

private:

    template<class U>
    StoredObjectType* GenerateObject(iVec3 pos, ::std::vector<Voxel>& voxelsGrid, U&& sot)
    { 
        const size_t uDim = this->GetGridWidth(); 
        const size_t uIndex = pos.x + 
                              pos.y * uDim +
                              pos.z * uDim * uDim;


        if (uIndex < voxelsGrid.size() &&
            voxelsGrid[uIndex].Type != -1 &&
            voxelsGrid[uIndex].Type < Voxel::MaxPerInstance)
        {
            voxelsGrid[uIndex].Id[voxelsGrid[uIndex].Type++] = m_uObjectsCount;
        }

        iVec3 objectSizes = iVec3::ToiVec3(sot.GetHalfSize() + (1.f * 1.5f));
        sot.SetPositon(Vec3::ToVec3(pos) + sot.GetHalfSize());
        pos = iVec3::ToiVec3(sot.GetPosition());

        for (int32_t x = -objectSizes.x; x <= objectSizes.x; ++x) {
            for (int32_t y = -objectSizes.y; y <= objectSizes.y; ++y) {
                for (int32_t z = -objectSizes.z; z <= objectSizes.z; ++z) {
                    if (x == 0 && y == 0 && z == 0)
                        continue;

                    size_t uCornerIndex = (pos.x + x) + 
                                          (pos.y + y) * uDim + 
                                          (pos.z + z) * uDim * uDim;
                    
                    if (uCornerIndex < voxelsGrid.size() &&
                        voxelsGrid[uCornerIndex].Type != -1 &&
                        voxelsGrid[uCornerIndex].Type < Voxel::MaxPerInstance)
                    {
                        voxelsGrid[uCornerIndex].Id[voxelsGrid[uCornerIndex].Type++] = m_uObjectsCount;
                    }
                }
            }
        }
        
        return &(m_StoredObjects[m_uObjectsCount++] = ::std::forward<U>(sot));
    }

private:

    ::std::vector<StoredObjectType> m_StoredObjects;
    size_t m_uObjectsCount = -1;

};

typedef ::Voxels::WorldGrid<ColoredCube> CubeWorld;

} // !Voxels
#endif // !AB_VOXEL_GRID_H
