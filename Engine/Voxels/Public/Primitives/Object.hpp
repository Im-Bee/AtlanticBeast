#ifndef AB_OBJECT_H
#define AB_OBJECT_H

#include "Math/Vec3.hpp"

namespace Voxels
{

class BEAST_VOXEL_API WorldObject
{
public:

	explicit WorldObject(Vec3 positon = Vec3 { 0., 0., 0. }, 
						 Vec3 rotation = Vec3 { 0., 0., 0. })
		: m_vPosition(Vec3())
		, m_vRotation(Vec3())
	{ }

	~WorldObject() = default;

public:

	void SetPositon(Vec3 pos);

	void SetRotation(Vec3 rot);

	void AddPositon(Vec3 pos);

	void AddRotation(Vec3 rot);

public:

	const Vec3& GetPosition() const
	{ return m_vPosition; }

	const Vec3& GetRotation() const
	{ return m_vPosition; }

private:

	Vec3 m_vPosition;
	Vec3 m_vRotation;

};

} // !Voxels

#endif // !AB_OBJECT_H
