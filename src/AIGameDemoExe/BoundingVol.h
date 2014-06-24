/*!
 * \file BoundingVol.h
 * \date 2014/05/24 18:35
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief bounding sphere and bounding box define
 *
 * TODO: long description
 *
 * \note
*/

#pragma once
#include <d3dx9math.h>

class sBoundingSphere
{
public:
	sBoundingSphere()
	{
		_center = D3DXVECTOR3(0, 0, 0);
		_radius = 1.0f;
	}
	void Merge(sBoundingSphere* other);
	D3DXVECTOR3 _center;
	float _radius;
};

class sBoundingBox
{
public:
	sBoundingBox()
	{
		_min = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
		_max = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	}
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};
