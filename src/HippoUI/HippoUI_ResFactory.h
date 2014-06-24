/********************************************************************
	created:	2012/11/02
	created:	2:11:2012   22:08
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_ResFactory.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_ResFactory
	file ext:	h
	author:		sssa2000
	
	purpose:	封装d3d资源的创建方式
*********************************************************************/
#pragma once
#include "HippoUI_Resource.h"

class HippoUI_ResFactory
{
public:
	virtual HippoUI_TextureResource* CreateTextureFromFile(const char* filename)=0;
	virtual HippoUI_ShaderResource* CreateShaderFromFile(const char* filename)=0;

protected:
private:
};