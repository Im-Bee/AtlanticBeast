#ifndef AB_FPS_LIMITER_H
#define AB_FPS_LIMITER_H

namespace Voxels
{

class FpsLimiter
{
public:

    FpsLimiter(const float fTargetMs);

    ~FpsLimiter() = default;

public:

    FpsLimiter(const FpsLimiter&) noexcept = default;
    FpsLimiter& operator=(const FpsLimiter&) noexcept = default;

    FpsLimiter(FpsLimiter&&) noexcept = default;
    FpsLimiter& operator=(FpsLimiter&&) noexcept = default;

public:

    void Block(const float fDelta);

private:

    float m_fTarget = -1.f;
    float m_fBalance = -1.f;

};

} //!Voxels
#endif //!AB_FPS_LIMITER_H
