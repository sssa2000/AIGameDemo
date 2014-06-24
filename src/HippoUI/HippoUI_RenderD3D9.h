/********************************************************************
	created:	2012/11/03
	created:	3:11:2012   22:23
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_RenderD3D9.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_RenderD3D9
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_Define.h"
#include "HippoUI_ResManager.h"
#include "HippoUI_Render.h"

class HippoUI_TextureProxy;
class HippoUI_RenderD3D9:public HippoUI_Render
{
public:
	HippoUI_RenderD3D9(void* pDevice);
	~HippoUI_RenderD3D9();
	virtual void BeginRender();
	virtual void EndRender();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	virtual void DrawSprite(HippoUI_Rect& rect,HippoUI_TextureProxy* pTexProxy,HippoUi_Color& color);
	virtual void DrawString(HippoUI_Rect& rect,const char* str,HippoUi_Color& color,TEXT_AG ag);
	virtual void DrawString(HippoUI_Rect& rect,const WCHAR* str,HippoUi_Color& color,TEXT_AG ag);

	virtual void DrawRect(HippoUI_Rect&rect,HippoUi_Color& color);
	virtual HDC GetFontDC();
private:

	void Init();
	void Destroy();
	ID3DXSprite* m_pSprite;
	ID3DXFont* m_pFont;
	IDirect3DDevice9* m_pd3dDevice;
};