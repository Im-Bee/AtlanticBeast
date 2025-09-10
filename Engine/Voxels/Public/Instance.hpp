#ifndef AB_INSTANCE_H
#define AB_INSTANCE_H

#include "Voxels.hpp"

namespace Voxels
{

class BEAST_VOXEL_API Instance
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

} // !Voxels


#endif // !AB_INSTANCE_H
