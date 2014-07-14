/********************************************************************
	created:	2014/07/11
	created:	11:7:2014   16:33
	filename: 	E:\AIGameDemo\src\AIGameDemoExe\D3dxMathExtension.h
	file path:	E:\AIGameDemo\src\AIGameDemoExe
	file base:	D3dxMathExtension
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include <d3dx9.h>
#include <utility>

void ComposeMatrix(D3DXMATRIX* m, const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const D3DXVECTOR3& scl);

D3DXQUATERNION GetRotationTo(const D3DXVECTOR3& from,const D3DXVECTOR3& to);

//射线和平面相交，返回值first表示是否相交，second表示如果相交的话 交点在哪
std::pair<bool,float> RayPlaneIntersection(const D3DXVECTOR3& start,const D3DXVECTOR3& dir,const D3DXVECTOR3& planeNormal,float d);