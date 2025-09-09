#ifndef AB_INSTANCE_H
#define AB_INSTANCE_H

#include "Voxels.h"

namespace Voxels
{

class Instance 
{
public:

    Instance();

    ~Instance();

public:

    VkInstance& GetInstance()
    { return m_Instance; }

private:

    VkInstance CreateInstance();
    
private:

    VkInstance m_Instance = VK_NULL_HANDLE;

};

} // !Core::Voxels


#endif // !AB_INSTANCE_H
