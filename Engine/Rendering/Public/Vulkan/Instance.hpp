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

    Instance(const Instance&) noexcept = delete;
    Instance& operator=(const Instance&) noexcept = delete;

    Instance(Instance&& other) noexcept 
        : m_Instance(other.m_Instance)
    { 
        other.m_Instance = VK_NULL_HANDLE;
    }

    Instance& operator=(Instance&& other) noexcept 
    { 
        this->m_Instance = other.m_Instance;
        other.m_Instance = VK_NULL_HANDLE;
        return *this;
    }

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
