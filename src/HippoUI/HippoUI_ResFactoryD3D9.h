/********************************************************************
	created:	2012/11/03
	created:	3:11:2012   0:07
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_ResFactoryD3D.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_ResFactoryD3D
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_ResFactory.h"

class HippoUI_ResFactoryD3D9:public HippoUI_ResFactory
{
public:
	virtual HippoUI_TextureResource* CreateTextureFromFile(const char* filename);
	virtual HippoUI_ShaderResource* CreateShaderFromFile(const char* filename);

protected:
private:
};