#ifndef AB_BASIC_CUBE_H
#define AB_BASIC_CUBE_H

#include "Primitives/Cube.hpp"

namespace Voxels
{

class alignas(16) ColorCube : public Cube
{
public:

    ColorCube() 
        : m_uColor(0xFFFFFFFF)
        , Cube()
    { }

public:

    void SetColor(uint32_t uColor) 
    { m_uColor = (uColor & 0xFFFFFF00) | (m_uColor & 0x000000FF); }

    void SetAlpha(uint32_t uAlpha) 
    { m_uColor = (uAlpha & 0x000000FF) | (m_uColor & 0xFFFFFF00); }

    void SetColorAndAlpha(uint32_t uHexValue) 
    { m_uColor = uHexValue; }

private:

    uint32_t m_uColor;  

};

} // !Voxels
#endif //!AB_BASIC_CUBE_H
