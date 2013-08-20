#include "EngineAxisAlignedBox.h"
#include "EnginePlane.h"

namespace Engine
{
const AxisAlignedBox AxisAlignedBox::BOX_NULL;//default null box
const AxisAlignedBox AxisAlignedBox::BOX_INFINITE(AxisAlignedBox::EXTENT_INFINITE);

bool AxisAlignedBox::intersects(const Plane& p) const
{
	return (p.getSide(*this) == Plane::BOTH_SIDE);
}

}
