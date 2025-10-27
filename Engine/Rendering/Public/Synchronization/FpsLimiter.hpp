#ifndef AB_FPS_LIMITER_H
#define AB_FPS_LIMITER_H

namespace Voxels
{

class FpsLimiter
{
public:

    FpsLimiter() = default;

    ~FpsLimiter() = default;

public:

    void Block(float fDelta);

};

} //!Voxels

#endif //!AB_FPS_LIMITER_H
