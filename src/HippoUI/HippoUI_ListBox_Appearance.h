/********************************************************************
	created:	2012/12/10
	created:	10:12:2012   22:11
	filename: 	F:\Git\ClassicHippoAppFrameWork\HippoUI\HippoUI_ListBox_Appearance.h
	file path:	F:\Git\ClassicHippoAppFrameWork\HippoUI
	file base:	HippoUI_ListBox_Appearance
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "IHippoUI_Appearance.h"
#include "HippoUI_ResManager.h"
#include <memory>

class HippoUI_ListBox;
class HippoUI_TextureProxy;

//appearance
class HippoUI_ListBox_DxutAppearance:public IHippoUI_Appearance
{
public:
	HippoUI_ListBox_DxutAppearance(HippoUI_ListBox* parent);
	~HippoUI_ListBox_DxutAppearance();
	void Render(float escape);
private:
	void Init();
	HippoUI_Rect GetLineRect(int lineIdx);
	HippoUI_ListBox* m_pParent;
	HippoUI_TextureProxy* m_pListBoxFrameTexture;

};
