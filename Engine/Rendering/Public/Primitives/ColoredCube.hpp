#ifndef AB_COLORED_CUBE_AB
#define AB_COLORED_CUBE_AB

#include "Primitives/Cube.hpp"
#include "Primitives/MaterialProperties/ColorProperty.hpp"
#include "Primitives/MaterialProperties/ReflectionProperty.hpp"
#include "Primitives/MaterialProperties/RoughnessProperty.hpp"

namespace Voxels
{

class alignas(16) ColoredCube : public Voxels::Cube
                              , public Voxels::ColorProperty
                              , public Voxels::ReflectionProperty
                              , public Voxels::RoughnessProperty
{ };

}

#endif //!AB_COLORED_CUBE_AB
