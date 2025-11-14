#ifndef AB_ROUGHNESS_PROPERTY_H
#define AB_ROUGHNESS_PROPERTY_H

#include "Voxels.hpp"

#include "IMaterialProperties.hpp"

namespace Voxels
{

class RoughnessProperty : public IMaterialProperties
{
public:

    RoughnessProperty() 
        : m_fRoughness(0.11f)
    { }

public:

    void SetReflection(float fRef) 
    { 
        if (fRef >= 0.25f) {
            m_fRoughness = 0.25f;
            return;
        }

        m_fRoughness = fRef * 0.25f; 
    }

private:

    float m_fRoughness;  

};

} // !Voxels
#endif //!AB_ROUGHNESS_PROPERTY_H
