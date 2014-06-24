/********************************************************************
	created:	2012/11/02
	created:	2:11:2012   22:09
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Resource.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Resource
	file ext:	h
	author:		sssa2000
	
	purpose:	定义资源的接口，封装d3d
*********************************************************************/
#pragma once
#include <memory>
#include "d3dx9.h"
#include "d3d9.h"
#include "HippoUI_Define.h"
using namespace std;
using namespace std::tr1;

class HippoUI_Resource
{
public:
	HippoUI_Resource(){}
	virtual ~HippoUI_Resource(){}
};

class HippoUI_TextureResource:public HippoUI_Resource
{
public:
	virtual void* GetHardwareTexturePtr()=0;
	virtual unsigned int GetWidth()=0;
	virtual unsigned int GetHeight()=0;

};

class HippoUI_FontResource:public HippoUI_Resource
{
public:
protected:
private:
};

class HippoUI_ShaderResource:public HippoUI_Resource
{
public:

};

typedef shared_ptr<HippoUI_TextureResource> TextureResHandle;
typedef shared_ptr<HippoUI_ShaderResource> ShaderResHandle;

//typedef HippoUI_TextureResource* TextureResHandle;
//typedef HippoUI_ShaderResource* ShaderResHandle;
//封装texture和subtexture的变化
class HippoUI_TextureProxy
{
public:
	HippoUI_TextureProxy(TextureResHandle pTex,HippoUI_Rect& rect)
	{
		m_texture_handle=pTex;
		m_rect=rect;
	}
	void* GetHardwareTexturePtr(){return m_texture_handle->GetHardwareTexturePtr();}
	const HippoUI_Rect& GetRect(){return m_rect;}
private:
	TextureResHandle m_texture_handle;
	HippoUI_Rect m_rect;
};

