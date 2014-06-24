/********************************************************************
	created:	2014/06/24
	created:	24:6:2014   15:31
	filename: 	E:\AIGameDemo\src\AIGameDemoExe\Camera\CameraBase.h
	file path:	E:\AIGameDemo\src\AIGameDemoExe\Camera
	file base:	CameraBase
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <d3dx9math.h>

class CameraBase
{
public:
	virtual int FrameUpdate(unsigned int dElapsedTime)=0;
	virtual void LookAt(IDirect3DDevice9* p)=0;
	virtual const D3DXVECTOR3* GetPos() = 0;
	virtual const D3DXVECTOR3* GetDir() = 0;
	virtual const D3DXVECTOR3* GetUp() = 0;
};