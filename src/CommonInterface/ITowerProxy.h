/*!
 * \file ITowerProxy.h
 * \date 2014/05/25 17:06
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief sssa2000
 *
 * TODO: long description
 *
 * \note
*/
#pragma once

#include <d3dx9math.h>
class ITowerProxy
{
public:
	virtual float GetRange() = 0;
	virtual D3DXVECTOR2 GetPos() = 0;
};