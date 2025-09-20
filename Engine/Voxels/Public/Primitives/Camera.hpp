#ifndef AB_CAMERA_H
#define AB_CAMERA_H

#include "Primitives/Object.hpp"

namespace Voxels
{

class BEAST_VOXEL_API Camera : public WorldObject
{
public:

	Camera(float fFov = 45.f)
		: m_fFov(fFov)
	{ }

	~Camera() = default;

private:

	float m_fFov;

};

} // !Voxels

#endif // !AB_CAMERA_H
