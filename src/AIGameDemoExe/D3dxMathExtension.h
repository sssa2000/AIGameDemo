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


void ComposeMatrix(D3DXMATRIX* m, const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const D3DXVECTOR3& scl);

D3DXQUATERNION GetRotationTo(const D3DXVECTOR3& from,const D3DXVECTOR3& to);