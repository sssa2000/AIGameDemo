/********************************************************************
	created:	2012/11/02
	created:	2:11:2012   1:05
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Render.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Render
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_Define.h"
#include "HippoUI_ResManager.h"
#include <Windows.h>
class HippoUI_Render
{
public:
	HippoUI_Render(){}
	virtual ~HippoUI_Render(){}

	virtual void BeginRender()=0;
	virtual void EndRender()=0;
	virtual void OnLostDevice()=0;
	virtual void OnResetDevice()=0;

	virtual void DrawSprite(HippoUI_Rect& rect,HippoUI_TextureProxy* pTexProxy,HippoUi_Color& color)=0;
	virtual void DrawString(HippoUI_Rect& rect,const char* str,HippoUi_Color& color,TEXT_AG ag=TEXT_AG_CENTER)=0;
	virtual void DrawString(HippoUI_Rect& rect,const WCHAR* str,HippoUi_Color& color,TEXT_AG ag=TEXT_AG_CENTER)=0;

	virtual void DrawRect(HippoUI_Rect&rect,HippoUi_Color& color)=0;
	virtual HDC GetFontDC()=0;
};