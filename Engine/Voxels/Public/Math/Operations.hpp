#ifndef AB_OPERATIONS_H
#define AB_OPERATIONS_H

#include "CSystem.hpp"

namespace Voxels
{

// ---------------------------------------------------------------------------------------------------------------------
template<class Vector>
Vector Normalize(const Vector& v)
{
    Vector result = { };
    double mod = 0.;
    double invMag;

    for (size_t i = 0; i < Vector::Size; ++i) 
        mod += result[i];

    invMag = 1. / ::std::sqrt(mod);

    for (size_t i = 0; i < Vector::Size; ++i)
        result[i] = v[i] * invMag;

    return result;
}

// x  [  1  ]  ⋅  y  [  2  ]  −  y  [  1  ]  ⋅  x  [  2  ]   
// x  [  2  ]  ⋅  y  [  0  ]  −  y  [  2  ]  ⋅  x  [  0  ]   
// x  [  0  ]  ⋅  y  [  1  ]  −  y  [  0  ]  ⋅  x  [  1  ]   

} // !Voxels
  
#endif // !AB_OPERATIONS_H

