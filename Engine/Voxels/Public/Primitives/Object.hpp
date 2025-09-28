#ifndef AB_OBJECT_H
#define AB_OBJECT_H

#include "Math/Vec3.hpp"
#include "Math/Rot.hpp"

namespace Voxels
{

/**
 * Holds positon of an object and rotation in radians.
 * */
class WorldObject
{
public:

	BEAST_VOXEL_API explicit WorldObject(Vec3 positon = Vec3 { 0.f, 0.f, 0.f },
										 Vec3 rotation = Vec3 { 0.f, 0.f, 0.f })
		: m_vPosition(Vec3())
		, m_vRotation(Rot3())
	{ }

	~WorldObject() = default;

public:

	BEAST_VOXEL_API void SetPositon(Vec3 pos);

	BEAST_VOXEL_API void SetRotation(Rot3 rot);

	BEAST_VOXEL_API void AddPositon(const Vec3& pos);

	BEAST_VOXEL_API void AddRotation(const Rot3& rot);

public:

	BEAST_VOXEL_API const Vec3& GetPosition() const
	{ return m_vPosition; }

	BEAST_VOXEL_API const Rot3& GetRotation() const
	{ return m_vRotation; }

private:

	Vec3 m_vPosition;
	Rot3 m_vRotation;

};

} // !Voxels

#endif // !AB_OBJECT_H
