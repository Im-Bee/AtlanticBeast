#include "Voxels.hpp"

#include "Synchronization/DeltaTime.hpp"

namespace Voxels 
{

using namespace ::std;
using namespace ::std::chrono;

// --------------------------------------------------------------------------------------------------------------------
DeltaTime::DeltaTime()
    : m_LastTimeStamp(Clock::now())
{ }

// --------------------------------------------------------------------------------------------------------------------
void DeltaTime::SetReferenceFrame()
{ m_LastTimeStamp = Clock::now(); }

// --------------------------------------------------------------------------------------------------------------------
float DeltaTime::FetchMs() 
{ 
    auto now = Clock::now();
    float diff = duration_cast<Duration>(now - m_LastTimeStamp).count();
    m_LastTimeStamp = now;
    return diff; 
}

} //!Voxels
