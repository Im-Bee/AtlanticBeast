#ifndef AB_INSTANCE_H
#define AB_INSTANCE_H

#include "Voxels.hpp"

namespace Voxels
{

class Instance
{
public:

    BEAST_API Instance();

    BEAST_API ~Instance();

public:

    VkInstance GetInstance() const
    { return m_Instance; }

private:

    VkInstance CreateInstance();
    
private:

    VkInstance m_Instance = VK_NULL_HANDLE;

};

} // !Voxels


#endif // !AB_INSTANCE_H
