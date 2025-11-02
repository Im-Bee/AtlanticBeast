#ifndef AB_RAYS_H
#define AB_RAYS_H

#include "Math.hpp"
#include "Raycaster/VoxelGrid.hpp"

namespace Voxels
{

struct HitResult 
{
    bool    bHit;
    iVec3   iHitCoords;
    size_t  uHitIndex;
    float   fDistance;
    Vec3    Normal;
};

BEAST_API HitResult MarchTheRay(const WorldGrid* vg, const Vec3& ro, const Vec3& rd, size_t maxSteps);
     
} //!Voxels
#endif //!AB_RAYS_H
