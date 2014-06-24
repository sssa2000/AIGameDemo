/********************************************************************
	created:	2012/10/21
	created:	21:10:2012   11:19
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Label_Appearance.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Label_Appearance
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "IHippoUI_Appearance.h"
#include "HippoUI_ResManager.h"
#include <memory>
class HippoUI_Label;
//appearance
class HippoUI_Label_DxutAppearance:public IHippoUI_Appearance
{
public:
	HippoUI_Label_DxutAppearance(HippoUI_Label* parent);
	~HippoUI_Label_DxutAppearance();
	void Render(float escape);
private:
	void Init();

	HippoUI_Label* m_pParent;
	HippoUI_TextureProxy* m_pLabelBackGroudTexture;
};
