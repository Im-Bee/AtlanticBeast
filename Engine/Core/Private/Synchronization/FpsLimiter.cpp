#include "Core.h"

#include "Synchronization/FpsLimiter.hpp"

namespace Core
{

using namespace ::std;
using namespace ::std::chrono;

// --------------------------------------------------------------------------------------------------------------------
FpsLimiter::FpsLimiter(const float fTargetMs)
    : m_fTarget(fTargetMs)
    , m_fBalance(0.f)
{ }

// --------------------------------------------------------------------------------------------------------------------
void FpsLimiter::Block(const float fDeltaMs)
{
    using DurationMs = duration<float, milli>;
    const float fCompensationFactor = 0.1f;

    m_fBalance += fCompensationFactor * (m_fTarget - fDeltaMs);
    this_thread::sleep_for(DurationMs(max(0.f, m_fTarget + m_fBalance)));
}

} //!Core
