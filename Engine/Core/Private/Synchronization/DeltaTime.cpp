#include "Core.h"

#include "Synchronization/DeltaTime.hpp"

namespace Core
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

// --------------------------------------------------------------------------------------------------------------------
float DeltaTime::DeltaMs() 
{ 
    auto now = Clock::now();
    float diff = duration_cast<Duration>(now - m_LastTimeStamp).count();
    return diff; 
}

} //!Core
