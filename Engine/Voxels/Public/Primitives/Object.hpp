#ifndef AB_OBJECT_H
#define AB_OBJECT_H

#include "Math/Vec3.hpp"
#include "Math/Rot.hpp"

namespace Voxels
{

/**
 * Holds positon of an object and rotation in radians.
 * */
class BEAST_VOXEL_API WorldObject
{
public:

	explicit WorldObject(Vec3 positon = Vec3 { 0., 0., 0. }, 
						 Vec3 rotation = Vec3 { 0., 0., 0. })
		: m_vPosition(Vec3())
		, m_vRotation(Rot3())
	{ }

	~WorldObject() = default;

public:

	void SetPositon(Vec3 pos);

	void SetRotation(Rot3 rot);

	void AddPositon(const Vec3& pos);

	void AddRotation(const Rot3& rot);

public:

	const Vec3& GetPosition() const
	{ return m_vPosition; }

	const Rot3& GetRotation() const
	{ return m_vRotation; }

private:

	Vec3 m_vPosition;
	Rot3 m_vRotation;

};

} // !Voxels

#endif // !AB_OBJECT_H
