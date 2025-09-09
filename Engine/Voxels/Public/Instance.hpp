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

private:

    VkInstance CreateInstance();
    
private:

    VkInstance m_Instance;

};

} // !Core::Voxels


#endif // !AB_INSTANCE_H
