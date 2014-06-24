/********************************************************************
	created:	2012/11/02
	created:	2:11:2012   22:49
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_TextureD3D9.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_TextureD3D9
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_Resource.h"
#include "HippoUI_ResFactoryD3D9.h"

class HippoUI_TextureD3D9:public HippoUI_TextureResource
{
	friend class HippoUI_ResFactoryD3D9;
public:
	HippoUI_TextureD3D9();
	~HippoUI_TextureD3D9();

	virtual void* GetHardwareTexturePtr(){return (void*)(m_pTextureD3D9);}
	virtual unsigned int GetWidth(){return m_info.Width;}
	virtual unsigned int GetHeight(){return m_info.Height;}
protected:
	D3DXIMAGE_INFO m_info;
	IDirect3DTexture9* m_pTextureD3D9;
private:
};