#ifndef AB_FPS_LIMITER_H
#define AB_FPS_LIMITER_H

#include "CoreMinimal.h"

namespace Core
{

class FpsLimiter
{
public:

    FpsLimiter() = delete;

    explicit BEAST_API FpsLimiter(const float fTargetMs);

    ~FpsLimiter() = default;

public:

    FpsLimiter(const FpsLimiter&) noexcept = default;
    FpsLimiter& operator=(const FpsLimiter&) noexcept = default;

    FpsLimiter(FpsLimiter&&) noexcept = default;
    FpsLimiter& operator=(FpsLimiter&&) noexcept = default;

public:

    /**
     * @brief Blocks current thread for the amount of time needed to achive target interval (stored in ms)
     * between current and next call.
     *
     * @param fDelta Amount of time that passed since last block in ms
     */
    BEAST_API void Block(const float fDeltaMs);

private:

    float m_fTarget = -1.f;
    float m_fBalance = -1.f;

};

} //!Core
#endif //!AB_FPS_LIMITER_H
