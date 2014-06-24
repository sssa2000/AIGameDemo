#include "BoundingVol.h"

void sBoundingSphere::Merge(sBoundingSphere* other)
{
	D3DXVECTOR3 oldcenter = _center;
	float oldr = _radius;

	_center = other->_center + oldcenter;
	D3DXVECTOR3 tmp1 = oldcenter - _center;
	float r1 = oldr+D3DXVec3Length(&tmp1);

	D3DXVECTOR3 tmp2 = oldcenter - _center;
	float r2 = other->_radius + D3DXVec3Length(&tmp2);

	_radius = max(r2, r1);
}