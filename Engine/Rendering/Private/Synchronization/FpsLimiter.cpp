#include "Voxels.hpp"

#include "Synchronization/FpsLimiter.hpp"

namespace Voxels
{

using namespace ::std;
using namespace ::std::chrono;

// --------------------------------------------------------------------------------------------------------------------
FpsLimiter::FpsLimiter(const float fTargetMs)
    : m_fTarget(fTargetMs)
    , m_fBalance(0.f)
{ }

// --------------------------------------------------------------------------------------------------------------------
void FpsLimiter::Block(const float fDelta)
{
    using Duration = duration<float, milli>;

    m_fBalance += m_fTarget - fDelta;
    this_thread::sleep_for(Duration(m_fTarget + m_fBalance));
}

} //!Voxels
