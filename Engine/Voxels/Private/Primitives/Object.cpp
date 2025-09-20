#include "Primitives/Object.hpp"

namespace Voxels
{

void WorldObject::SetPositon(Vec3 pos)
{ m_vPosition = pos; }

void WorldObject::SetRotation(Vec3 rot)
{ m_vRotation = rot; }

void WorldObject::AddPositon(Vec3 pos)
{
	m_vPosition += pos;
}

void WorldObject::AddRotation(Vec3 rot)
{
	m_vRotation += rot;
}

} // !Voxels
