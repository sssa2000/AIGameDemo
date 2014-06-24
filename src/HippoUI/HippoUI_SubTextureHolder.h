/********************************************************************
	created:	2012/11/04
	created:	4:11:2012   23:47
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_SubTextureHolder.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_SubTextureHolder
	file ext:	h
	author:		sssa2000
	
	purpose:	HippoUI_SubTextureHolder持有一个texture的一个区域
*********************************************************************/
#pragma once

class HippoUI_SubTextureHolder
{
public:
	HippoUI_SubTextureHolder(HippoUI_TextureResource* p,HippoUI_Rect& rect);
	

protected:
	HippoUI_TextureResource* m_pRes;
	HippoUI_Rect m_subtex_rect;

};