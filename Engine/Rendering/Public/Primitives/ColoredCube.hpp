#ifndef AB_COLORED_CUBE_AB
#define AB_COLORED_CUBE_AB

#include "Primitives/Cube.hpp"
#include "Primitives/MaterialProperties/ColorProperty.hpp"

namespace Voxels
{

class alignas(16) ColoredCube : public Voxels::Cube
                              , public Voxels::ColorProperty
{ };

}

#endif //!AB_COLORED_CUBE_AB
