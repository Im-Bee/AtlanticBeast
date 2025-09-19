#ifndef AB_IOBJECT_H
#define AB_IOBJECT_H

#include "Math/Vec3.hpp"

namespace Voxels
{

class BEAST_VOXEL_API Object
{
public:

	Object()
		: m_vPosition(Vec3())
		, m_vRotation(Vec3())
	{ }

	~Object() = default;

public:

private:

	Vec3 m_vPosition;
	Vec3 m_vRotation;

};

} // !Voxels

#endif // !AB_IOBJECT_H
