#ifndef AB_DELTA_TIME_H
#define AB_DELTA_TIME_H

#include "CPlusPlusStandard.hpp"

namespace Voxels
{

class DeltaTime
{
    using Clock     = ::std::chrono::steady_clock;
    using Duration  = ::std::chrono::duration<float, ::std::milli>;
    using TimeStamp = ::std::chrono::time_point<Clock>;

public:
    
    DeltaTime();
    
    ~DeltaTime() = default;

public:

    DeltaTime(const DeltaTime&) noexcept = default;
    DeltaTime& operator=(const DeltaTime&) noexcept = default;

    DeltaTime(DeltaTime&&) noexcept = default;
    DeltaTime& operator=(DeltaTime&&) noexcept = default;

public:

    /**
     * @brief Sets the first timestamp on the timer
     */
    void SetReferenceFrame();

    /**
     * @return Delta time between current and last fetch
     */
    float FetchMs();

private:

    TimeStamp m_LastTimeStamp;      

};

}

#endif //!AB_DELTA_TIME_H
