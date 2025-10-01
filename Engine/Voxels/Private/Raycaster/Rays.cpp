#include "Raycaster/Rays.hpp"

namespace Voxels
{

HitResult MarchTheRay(const VoxelGrid* vg, const Vec3& ro, const Vec3& rd, size_t maxSteps)
{
    HitResult result;
    result.bHit = false;
    result.fDistance = 0.0f;

    Vec3 pos(std::floor(ro.x), std::floor(ro.y), std::floor(ro.z));
    iVec3 voxel(pos.x, pos.y, pos.z);

    Vec3 invDir(1.0f / rd.x, 1.0f / rd.y, 1.0f / rd.z);
    Vec3 tDelta(std::fabs(invDir.x), std::fabs(invDir.y), std::fabs(invDir.z));
    Vec3 tMax;

    for (size_t i = 0; i < 3; ++i)
    {
        float rdi = (i == 0 ? rd.x : (i == 1 ? rd.y : rd.z));
        float roi = (i == 0 ? ro.x : (i == 1 ? ro.y : ro.z));
        float offset = (rdi > 0.0f) ? (1.0f - (roi - std::floor(roi))) : (roi - std::floor(roi));

        tMax[i] = tDelta[i] * offset;
    }

    iVec3 step(std::copysign(1.0f, rd.x), std::copysign(1.0f, rd.y), std::copysign(1.0f, rd.z));

    size_t lastStepAxis = -1;
    size_t gridWidth = vg->GetGridWidth();

    for (size_t stepCount = 0; stepCount < maxSteps; ++stepCount)
    {
        if (voxel.x < 0 || voxel.x >= gridWidth ||
            voxel.y < 0 || voxel.y >= gridWidth ||
            voxel.z < 0 || voxel.z >= gridWidth)
        {
            break;
        }

        size_t index = voxel.x +
            voxel.y * gridWidth +
            voxel.z * gridWidth * gridWidth;

        if (vg->GetGrid()[index].Type == 1)
        {
            result.bHit = true;
            result.HitCoords = voxel;
            result.HitIndex = index;

            if (lastStepAxis == 0) {
                result.Normal = Vec3(-float(step.x), 0.0f, 0.0f);
                return result;
            }

            if (lastStepAxis == 1) {
                result.Normal = Vec3(0.0f, -float(step.y), 0.0f);
                return result;
            }

            result.Normal = Vec3(0.0f, 0.0f, -float(step.z));
            return result;
        }

        if (tMax.x < tMax.y)
        {
            if (tMax.x < tMax.z) {
                voxel.x += step.x;
                result.fDistance = tMax.x;
                tMax.x += tDelta.x;
                lastStepAxis = 0;
            }
            else {
                voxel.z += step.z;
                result.fDistance = tMax.z;
                tMax.z += tDelta.z;
                lastStepAxis = 2;
            }
        }
        else
        {
            if (tMax.y < tMax.z) {
                voxel.y += step.y;
                result.fDistance = tMax.y;
                tMax.y += tDelta.y;
                lastStepAxis = 1;
            }
            else {
                voxel.z += step.z;
                result.fDistance = tMax.z;
                tMax.z += tDelta.z;
                lastStepAxis = 2;
            }
        }
    }

    return result;
}

} // !Voxels