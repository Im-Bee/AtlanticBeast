#ifndef AB_DELTA_TIME_H
#define AB_DELTA_TIME_H

#include "CoreMinimal.h"

namespace Core
{

class DeltaTime
{
    using Clock     = ::std::chrono::steady_clock;
    using Duration  = ::std::chrono::duration<float, ::std::milli>;
    using TimeStamp = ::std::chrono::time_point<Clock>;

public:
    
    BEAST_API DeltaTime();
    
    ~DeltaTime() = default;

public:

    DeltaTime(const DeltaTime&) noexcept = default;
    DeltaTime& operator=(const DeltaTime&) noexcept = default;

    DeltaTime(DeltaTime&&) noexcept = default;
    DeltaTime& operator=(DeltaTime&&) noexcept = default;

public:

    /**
     * @brief Sets the first timestamp on the timer, will be used for `FetchMs` as the next reference.
     */
    BEAST_API void SetReferenceFrame();

    /**
     * @return Delta time between current and last reference frame or fetch
     */
    BEAST_API float FetchMs();

    /**
     * @brief This method doesn't update the last fetch time. 
     *
     * @return Delta time between current and last reference frame or fetch
     */
    BEAST_API float DeltaMs();

private:

    TimeStamp m_LastTimeStamp;      

};

} // !Core
#endif //!AB_DELTA_TIME_H
